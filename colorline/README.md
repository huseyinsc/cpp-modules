# ColorLine

A small C++ module-based terminal color helper library with a CLI example.

## Overview

This repo includes:

- `include/colorline.hpp` : `Color` enum and `Colored` class for setting terminal colors and formatting text.
- `src/colorline.cppm` : C++20 module interface file that exports `Color` and `Colored`.
- `src/main.cpp` : working CLI example for the colorline module.
- `CMakeLists.txt` : build setup for the library and executable.

## Build (CMake)

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

The executable is produced as `build/bin/colorline`.

## Usage

```bash
./build/bin/colorline all
./build/bin/colorline 1
```

- `all` prints all defined colors.
- numeric argument selects a specific color code (0..15).

## Notes

- `main.cpp` is provided as a working example showing how to use module and header interfaces.
- The project uses `C++23` and a module-friendly architecture.
