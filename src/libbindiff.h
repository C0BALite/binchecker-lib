#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

struct diffChunk
{
    int pos;
    int length;
    unsigned char *diffFile1;
    unsigned char *diffFile2;
};

struct diffChunk * compare_files(char *ptr1, char *ptr2, int padding);
void get_all_file_paths(const char *base_path, char ***file_paths, int *count, int *capacity);
void concatHex(unsigned char source, char *destination);
