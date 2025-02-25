## RMM Sample

This is a sample project for performing Remote Monitoring and Managment. 

* Written in C++, using Qt 6.8
* Fully tested on Windows 11. OSX is partially complete, and Linux hasn't started.

### Running the Project

* You can download the latest release from the [Releases](https://github.com/7Towers/RMM/releases) page.

### Windows Build
To build on Windows, set the following CMAKE variables:

```bash
-DCMAKE_TOOLCHAIN_FILE=C:/Qt/6.8.x/mingw_64/lib/cmake/Qt6/qt.toolchain.cmake
-DCMAKE_PREFIX_PATH=C:/Qt/6.8.x/mingw_64
```

Also, make sure your PATH includes the Qt bin directory. For example:

```bash
C:\Qt\6.8.x\mingw_64\bin
```