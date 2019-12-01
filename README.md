# nx\_homework

[![Build Status](https://travis-ci.org/yuri-kilochek/nx_homework.svg?branch=master)](https://travis-ci.org/yuri-kilochek/nx_homework)
[![Build status](https://ci.appveyor.com/api/projects/status/fwmpwinj0yy4bd07?svg=true)](https://ci.appveyor.com/project/yuri-kilochek/nx-homework)

SHA256 string hashing microservice.

Connect via TCP and send strings terminated by `'\n'`, and it will send
hex-encoded SHA256 hashes of these strings back, also terminated by newlines.

### Building

Prerequisites:

- C++17 compiler (Tested on gcc 9+ / MSVC 16+)
- cmake 3.13+
- [ninja](https://ninja-build.org/) (optional)
- [conan](https://docs.conan.io/en/latest/installation.html)

#### Commands:

To build on Windows with Ninja and MSVC you must first load the compiler
environment, typically via something like-
```cmd
call C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvars64.bat
```
-or via start menu shortcut. Then build:
```bash
mkdir build
cd build
conan install ..  # install dependencies
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -G Ninja ..
cmake --build .
cmake --install .
ctest  # run tests
```

The output executable should be installed in `install/bin`. Run it-
```
install/bin/nx_homework_hasher_microservice --help
```
-to see usage instructions.

Dynamic library encapsulating core functionlity (hashing), is also installed
in `install` along with its accompanying headers and CMake config scripts.

