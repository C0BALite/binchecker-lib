#include "libbindiff.h"

void concatHex(unsigned char source, char * destination) {
        char tempStr[4] = "";
        snprintf(tempStr, sizeof(tempStr), "%02x ", source);
        strcat(destination, tempStr);
}

// Function to recursively get all file paths
void get_all_file_paths(const char * base_path, char ** * file_paths, int * count, int * capacity) {

        DIR * dir = opendir(base_path);
        struct dirent * entry; // Structure to store directory entry information
        char path[1024]; // Buffer to construct full paths

        // Check if directory was opened successfully
        if (dir == NULL) {
                perror("opendir");
                return;
        }

        while ((entry = readdir(dir)) != NULL) {
                // Skip the current (.) and parent (..) directory entries to avoid infinite loops
                if (strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0) {
                        continue;
                }

                // Construct the full path by combining base path with entry name
                snprintf(path, sizeof(path), "%s/%s", base_path, entry -> d_name);

                if (entry -> d_type == DT_DIR) {
                        // Recursively call this function for subdirectories
                        get_all_file_paths(path, file_paths, count, capacity);
                } else {
                        // check if we need to expand our storage array
                        if ( * count >= * capacity) {
                                // Calculate new capacity: double current size or initialize to 16 if empty
                                int new_capacity = ( * capacity == 0) ? 16 : * capacity * 2;

                                // Reallocate memory for the file paths array
                                char ** new_array = (char ** ) realloc( * file_paths, new_capacity * sizeof(char * ));

                                // Check if reallocation was successful
                                if (new_array == NULL) {
                                        perror("realloc");
                                        closedir(dir);
                                        return;
                                }

                                // Update the array pointer and capacity
                                * file_paths = new_array;
                                * capacity = new_capacity;
                        }

                        // Add the file path to our array
                        ( * file_paths)[ * count] = strdup(path);

                        // Check if string duplication was successful
                        if (( * file_paths)[ * count] == NULL) {
                                perror("strdup");
                                closedir(dir);
                                return;
                        }

                        ( * count) ++;
                }
        }

        // Close the directory handle when done
        closedir(dir);
}

struct diffChunk * compare_files(char * fp1, char * fp2, int padding) {
        FILE * file1, * file2;
        size_t bytesRead1, bytesRead2;
        struct stat sb;
        int interCheck = 0;
        int fileSize = 0;
        int state = 0;
        int currLength = 0;
        int diffCount = 0;
        int maxDiffs = 10;
        char diffBlock1[100] = "", diffBlock2[100] = "";
        unsigned char * dataBlock1, * dataBlock2;
        struct diffChunk * diffs = malloc(maxDiffs * sizeof(struct diffChunk));
        if (diffs == NULL) {
                return NULL;
        }
        //symlink discard
        lstat(fp1, & sb);
        if (S_ISLNK(sb.st_mode)) return NULL;
        file1 = fopen(fp1, "rb");
        file2 = fopen(fp2, "rb");
        //Filesize calculation
        fseek(file1, 0, SEEK_END);
        fileSize = ftell(file1);
        rewind(file1);

        dataBlock1 = (unsigned char * ) malloc(fileSize * sizeof(char));
        dataBlock2 = (unsigned char * ) malloc(fileSize * sizeof(char));
        if (file1 == NULL || file2 == NULL) {
                printf("Error opening files: %s\n", fp1);
                if (file1) fclose(file1);
                if (file2) fclose(file2);
                return NULL;
        }

        do {
                bytesRead1 = fread(dataBlock1, 1, fileSize, file1);
                bytesRead2 = fread(dataBlock2, 1, fileSize, file2);
                interCheck = 0;
                // Check if read lengths differ or content differs
                if ((bytesRead1 > 0 && memcmp(dataBlock1, dataBlock2, bytesRead1) != 0)) {
                        while (interCheck < bytesRead1) {
                                while (dataBlock1[interCheck] != dataBlock2[interCheck]) {
                                        //insert pre-padding
                                        if (state == 0) {
                                                for (int i = padding; i > 0; i--) {
                                                        if (interCheck - i >= 0) {
                                                                concatHex(dataBlock1[interCheck - i], diffBlock1);
                                                                concatHex(dataBlock2[interCheck - i], diffBlock2);
                                                        }
                                                }
                                                strcat(diffBlock1, "<b>");
                                                strcat(diffBlock2, "<b>");
                                                state = 1;
                                        }
                                        //insert non-equal bites
                                        concatHex(dataBlock1[interCheck], diffBlock1);
                                        concatHex(dataBlock2[interCheck], diffBlock2);
                                        state = 1;
                                        interCheck++;
                                        currLength++;
                                }
                                if (state == 1) {
                                        //insert post-padding
                                        strcat(diffBlock1, "</b>");
                                        strcat(diffBlock2, "</b>");
                                        for (int i = 0; i < padding; i++) {
                                                if (interCheck + i <= fileSize - 1) {
                                                        concatHex(dataBlock1[interCheck + i], diffBlock1);
                                                        concatHex(dataBlock2[interCheck + i], diffBlock2);
                                                }
                                        }
                                        //fill up a new diff chunk
                                        diffs[diffCount].pos = interCheck - currLength;
                                        diffs[diffCount].length = currLength;
                                        diffs[diffCount].diffFile1 = (unsigned char * ) malloc(sizeof(unsigned char) * 3 * (currLength + padding * 2) + 8);
                                        diffs[diffCount].diffFile2 = (unsigned char * ) malloc(sizeof(unsigned char) * 3 * (currLength + padding * 2) + 8);
                                        memcpy(diffs[diffCount].diffFile1, diffBlock1, 3 * (currLength + padding * 2) + 8);
                                        memcpy(diffs[diffCount].diffFile2, diffBlock2, 3 * (currLength + padding * 2) + 8);
                                        diffCount++;
                                        //clean up output strings
                                        memset(diffBlock1, 0, sizeof(diffBlock1));
                                        memset(diffBlock2, 0, sizeof(diffBlock2));
                                        state = 0;
                                        currLength = 0;
                                }
                                interCheck++;
                        }
                        break;
                }

        } while (bytesRead1 > 0 && bytesRead2 > 0);
        //post-check cleanup, file closure and a safety check for diffs
        fclose(file1);
        fclose(file2);
        free(dataBlock1);
        free(dataBlock2);
        dataBlock1 = dataBlock2 = NULL;
        if (diffCount == 0) {
                return NULL;
        }    // Terminate the array
    diffs[diffCount].pos = 0;
    diffs[diffCount].length = 0;
    diffs[diffCount].diffFile1 = NULL;
    diffs[diffCount].diffFile2 = NULL;
        return diffs;
}
