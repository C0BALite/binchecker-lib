BinDiff-Lib
===========

This shared library is written in C language using GNOME Builder. It compares files in two separate directories, finds differences in binary data and displays them. Symbolic links, nonexistent files and files requiring permission are skipped.

The application has been tested and runs on Alt Regular GNOME. App tested using file\_scrambler.py.

Installation Steps:
-------------------

1\. Install the required dependencies

``` sudo apt-get install meson gcc ```

2\. Open project directory

``` cd binchecker-lib ```

3\. Configure the build directory

``` meson setup build ```

4\. Install the library

``` meson install -C build ```
