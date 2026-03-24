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

## API Examples

From code (header-based or module-based):

```cpp
#include "colorline.hpp"

int main() {
    Colored::setColor(Color::Red);
    std::cout << "Red text\n";
    Colored::setColor(Color::Reset);

    std::cout << Colored::text({Color::Green, Color::Black}, "Green text on black\n");

    Colored::print("Aqua text\n", Color::Aqua, Color::Reset);
    return 0;
}
```

`Color` values correspond to entries in `include/colorline.hpp`, and `Color::Reset` restores default terminal styling.

## Platform behavior

- On Linux/macOS, ANSI escape sequences are used.
- On Windows, both Windows API colors and ANSI fallback is handled in `Colored::setColor`.

## Notes

- `src/main.cpp` is provided as a working example showing how to use module and header interfaces.
- The project uses `C++23` and a module-friendly architecture.
