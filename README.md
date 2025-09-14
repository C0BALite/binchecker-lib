<h1 align="center" id="title">BinDiff-Lib</h1>

<p id="description">This shared library is written in C language using GNOME Builder. It compares files in two separate directories, finds differences in binary data and displays them. Symbolic links, nonexistent files and files requiring permission are skipped.</p> 

The application has been tested and runs on Alt Regular GNOME. App tested using file_scrambler.py.

<h2> Installation Steps:</h2>

<p>1. Install the required dependencies</p>

```
sudo apt-get install meson gcc
```

<p>2. Open project directory</p>

```
cd binchecker-lib
```

<p>3. Configure the build directory</p>

```
meson setup build
```

<p>4. Install the library</p>

```
meson install -C build
```

<p>5. Update dynamic linker cache</p>

```
sudo ldconfig
```
