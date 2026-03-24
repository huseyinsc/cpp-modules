# Terminal Utils (C++23)

A modern, production-ready C++ library for creating engaging and interactive command-line interfaces (CLI). Terminal Utils provides advanced terminal input capabilities directly inspired by contemporary CLI tools and frameworks like **Vite**, **Zsh**, **Gemini CLI**, and other modern development tools.

Whether you're building CLI applications, interactive installers, or developer tools, Terminal Utils gives you the UI components you need to create a delightful user experience in the terminal.

---

## ✨ Features

### 🎯 **Ghost Text (Zsh-style Suggestions)**

Shows inline autocomplete suggestions as the user types. The suggestion appears in a muted color and can be accepted by pressing `Tab` or the `Right Arrow` key for instant completion.

```
Command: git commit -m "initial commit"
         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ (ghost text in gray)
```

### 📋 **Interactive Single Select Menu**

A Vite-style vertical menu where users navigate with arrow keys. The selected option is highlighted, making it perfect for simple choices.

```
What is your favorite programming language?
  > C++           ← highlighted
    Rust
    Go
    Python
```

### ✅ **Interactive Multi-Select Menu**

A checkbox-style menu for selecting multiple items. Users toggle selections with the `Space` bar and see visual feedback with checkboxes (`[ ]`, `[x]`) and color coding.

```
Which tools do you use?
  > [x] Docker      ← highlighted & selected
    [ ] Git        ← available
    [x] CMake      ← selected
    [ ] Vim
```

### 🔐 **Password Input with Masking**

Securely read sensitive input with optional masking. Characters can be hidden entirely or replaced with asterisks (`*`) while typed.

```
Password: *** (input hidden, real characters stored internally)
```

### 🌍 **Full UTF-8 Support**

Seamlessly handles multi-byte Unicode characters, including:

- Turkish letters: `ç`, `ş`, `ğ`, `ı`, `ü`, `ö`
- Emoji and other special characters
- Proper backspace handling for multi-byte sequences

### 🖥️ **Cross-Platform**

Works consistently across all major platforms:

- **Linux/macOS:** Uses POSIX `termios` for terminal control
- **Windows:** Uses `conio.h` and Windows Console API for native terminal handling

---

## 📦 Dependencies

### Colorline (Header-Only Library)

Terminal Utils depends on **[Colorline](https://github.com/huseyinsc/cpp-modules/tree/main/public/colorline)** for rendering colored UI elements (such as highlighted menu options and muted suggestion text).

**Smart Dependency Resolution:**
The CMake configuration is highly optimized and automatically resolves `colorline` using the following strategy:

1. **Parent Directory:** Checks if `colorline` exists in the parent workspace (monorepo setup)
2. **Global Cache:** Looks in the global cache directory (`~/.cache/cpp-modules` on Linux/macOS, `%LOCALAPPDATA%\cpp-modules` on Windows)
3. **GitHub:** If not found locally, automatically fetches the latest version from GitHub

Since `colorline` is used as a **header-only library**, it does not add any build targets or complicate your build process.

---

## 🏗️ Project Structure

```
terminal_utils/
├── CMakeLists.txt                    # Build configuration
├── include/
│   └── terminal_utils.h              # Main header file with TerminalReader class
├── src/
│   ├── terminal_utils.cpp            # Implementation of all input methods
│   └── main.cpp                      # Example application (see below)
├── build/                            # Build output directory
│   └── bin/
│       └── terminal_utils            # Compiled example executable
└── README.md                         # This file
```

---

## 🚀 Quick Start

### Prerequisites

- **C++23 compatible compiler** (GCC 13+, Clang 17+, or MSVC 2022+)
- **CMake 3.28+**
- **Git** (for dependency resolution)

### Building the Project

```bash
# Clone or navigate to the terminal_utils directory
cd /path/to/terminal_utils

# Create and enter build directory
mkdir build
cd build

# Configure the project with CMake
cmake ..

# Build the project
cmake --build .
```

### Running the Example Application

The `main.cpp` file contains a fully functional example application demonstrating all features:

```bash
# On Linux/macOS
./bin/terminal_utils

# On Windows
.\bin\terminal_utils.exe
```

**Example Output:**

```
--- Terminal Utils Demo ---
Name: └─ [User types name] → Shows "Hüseyin" as a ghost suggestion
Hello, [Name]!

Hint: Try typing 'git commit -m'.
Command: └─ [User types] → Shows full suggestion as ghost text
Executing: git commit -m "initial commit"

Password: *** (input masked with asterisks)
Password retrieved (16 characters).

What is your favorite programming language?
  > C++
    Rust
    Go
    Python

Selected language: C++

Which tools do you use? (Space to select)
  > [x] Docker
    [ ] Git
    [x] CMake
    [ ] Vim

Selected tools count: 2
  - Docker
  - CMake
```

---

## 📚 API Reference

### Core Class: `TerminalReader`

All methods are **static** and can be called directly via `TerminalReader::methodName()`.

#### **Text Input Methods**

##### `readWithPlaceholder()`

```cpp
static std::string readWithPlaceholder(
    const std::string& prompt,
    const std::string& placeholder
);
```

Reads a line of input with a placeholder text displayed when the input is empty. The placeholder disappears as the user types.

**Parameters:**

- `prompt`: Text displayed before the input field
- `placeholder`: Hint text shown in muted color when input is empty

**Returns:** User input as `std::string`

---

##### `readWithGhostText()`

```cpp
static std::string readWithGhostText(
    const std::string& prompt,
    const std::string& suggestion,
    const std::string& placeholder = ""
);
```

Reads a line of input with inline autocompletion suggestions (Zsh-style). As the user types, the remaining part of the suggestion is displayed in a muted color. Pressing `Tab` or `Right Arrow` accepts the full suggestion.

**Parameters:**

- `prompt`: Text displayed before the input field
- `suggestion`: The autocomplete suggestion to display
- `placeholder`: Optional fallback text when input is empty (default: uses `suggestion`)

**Returns:** User input as `std::string`, or the full `suggestion` if accepted

**Example:**

```cpp
std::string cmd = TerminalReader::readWithGhostText(
    "Command: ",
    "git commit -m \"initial commit\""
);
// User types "git c" → suggestion shows "ommit -m \"initial commit\"" in gray
// User presses Tab → full suggestion accepted
```

---

##### `readPassword()`

```cpp
static std::string readPassword(
    const std::string& prompt,
    bool mask = true
);
```

Reads sensitive input (passwords, tokens, etc.) with optional character masking.

**Parameters:**

- `prompt`: Text displayed before the password field
- `mask`: If `true` (default), replaces each character with `*`; if `false`, hides input entirely

**Returns:** The actual password as `std::string`

---

#### **Menu Selection Methods**

##### `selectMenu()`

```cpp
// 1. Standard single selection (returns index)
static int selectMenu(
    const std::string& prompt,
    const std::vector<std::string>& options
);

// 2. Key-Value single selection (returns underlying value)
static std::string selectMenu(
    const std::string& prompt,
    const std::vector<std::pair<std::string, std::string>>& options
);
```

Creates an interactive single-selection menu. Users navigate with `Up` and `Down` arrow keys and confirm with `Enter`.
The overloaded version allows you to separate the "Display Member" (what the user sees) from the "Value Member" (what the program gets).

**Parameters:**

- `prompt`: Menu title/description
- `options`:
  - `vector<string>`: A simple list of options.
  - `vector<pair<string, string>>`: A list of pairs where `.first` is the display text and `.second` is the returned value.

**Returns:** - `int`: Index (0-based) of the selected option (for the standard version). - `std::string`: The underlying value of the selected option (for the pair version).

**Example:**

```cpp
/ Standard Version
std::vector<std::string> langs = {"C++", "Rust", "Go"};
int choice = TerminalReader::selectMenu("Pick a language:", langs);
// Returns: 0 for C++

// Key-Value Version
std::vector<std::pair<std::string, std::string>> tools = {
    {"Run CMake build", "cmake"},
    {"Run Make build", "make"},
    {"Standard Compile", "none"}
};
std::string toolChoice = TerminalReader::selectMenu("Action:", tools);
// User sees "Run CMake build", but function returns "cmake"
```

---

##### `multiSelectMenu()`

```cpp
// 1. Standard multi-selection (returns indices)
static std::vector<int> multiSelectMenu(
    const std::string& prompt,
    const std::vector<std::string>& options
);

// 2. Key-Value multi-selection (returns underlying values)
static std::vector<std::string> multiSelectMenu(
    const std::string& prompt,
    const std::vector<std::pair<std::string, std::string>>& options
);
```

Creates an interactive multi-selection menu with checkboxes. Users navigate with arrow keys, toggle selections with `Space`, and confirm with `Enter`.
Similar to `selectMenu`, the overloaded version allows separating display text from internal values.

**Parameters:**

- `prompt`: Menu title/description
- `options`:
  - `vector<string>`: A simple list of options.
  - `vector<pair<string, string>>`: A list of pairs where `.first` is the display text and `.second` is the returned value.

**Returns:** - `vector<int>`: Vector of indices of all selected options. - `vector<std::string>`: Vector of the underlying values of all selected options.

**Example:**

```cpp
std::vector<std::string> tools = {"Docker", "Git", "CMake", "Vim"};
auto selected = TerminalReader::multiSelectMenu("Select tools:", tools);
for (int idx : selected) {
    std::cout << "✓ " << tools[idx] << std::endl;
}
// Output:
// ✓ Docker
// ✓ CMake

// Key-Value Version
std::vector<std::pair<std::string, std::string>> flags = {
    {"Enable all warnings", "-Wall"},
    {"Optimize for speed", "-O3"},
    {"Static linking", "-static"}
};
auto selectedFlags = TerminalReader::multiSelectMenu("Select compiler flags:", flags);
// User toggles checkboxes, function returns e.g., {"-Wall", "-O3"}
```

---

#### **Utility Methods**

##### `isTerminal()`

```cpp
static bool isTerminal();
```

Checks if the current input is connected to an actual terminal (TTY) or redirected from a pipe/file.

**Returns:** `true` if connected to a terminal, `false` otherwise

---

##### `getChar()`

```cpp
static int getChar();
```

Reads a single character from input, handling special keys (arrow keys, etc.) on both Windows and Unix-like systems.

**Returns:** Character code or special key code

---

##### `getUTF8Char()`

```cpp
static std::string getUTF8Char(int firstByte);
```

Reads a complete UTF-8 character from input, handling multi-byte sequences correctly.

**Parameters:**

- `firstByte`: The first byte of the UTF-8 sequence

**Returns:** Complete UTF-8 character as `std::string`

---

##### `removeLastUTF8Char()`

```cpp
static void removeLastUTF8Char(std::string& str);
```

Safely removes the last character from a string, even if it's a multi-byte UTF-8 sequence. Useful for implementing backspace operations.

**Parameters:**

- `str`: Reference to the string to modify

---

##### `setRawMode()`

```cpp
static void setRawMode(bool enable, void* old_settings);
```

Enables or disables raw terminal mode on Unix-like systems. Raw mode allows reading individual key presses without waiting for `Enter`.

**Parameters:**

- `enable`: `true` to enable raw mode, `false` to restore normal mode
- `old_settings`: Pointer to a `termios` structure (used internally; not normally called directly)

---

## 🔧 Integration Guide

### Using Terminal Utils in Your Own Project

You can easily integrate Terminal Utils into your C++ project via CMake's `FetchContent`. It will automatically resolve its own `colorline` dependency.

#### Example CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.28)
project(MyProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    terminal_utils
    GIT_REPOSITORY https://github.com/huseyinsc/cpp-modules.git
    GIT_TAG        master
    SOURCE_SUBDIR  public/terminal_utils
)
FetchContent_MakeAvailable(terminal_utils)

# Your executable
add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE terminal_utils_lib)
```

#### Usage in Your Code

```cpp
#include <terminal_utils.h>
#include <iostream>

int main() {
    // Get user input with ghost text
    std::string name = TerminalReader::readWithGhostText(
        "Enter your name: ",
        "John Doe"
    );

    // Show a selection menu
    std::vector<std::string> options = {"Option A", "Option B", "Option C"};
    int choice = TerminalReader::selectMenu("Choose an option:", options);

    std::cout << "You selected: " << options[choice] << std::endl;

    return 0;
}
```

---

## 📋 Example Application (`main.cpp`)

The included `main.cpp` serves as a **complete, working example** demonstrating all features of the Terminal Utils library:

1. **Ghost Text Input:** Shows how to use `readWithGhostText()` with autocomplete suggestions
2. **Password Input:** Demonstrates `readPassword()` with masking
3. **Single Selection:** Uses `selectMenu()` to let users pick a programming language
4. **Multi-Selection:** Uses `multiSelectMenu()` for selecting multiple development tools

You can run this example as a great starting point to understand the library's capabilities:

```bash
./bin/terminal_utils
```

The example includes helpful comments and demonstrates best practices for using each feature.

---

## 🎨 User Experience Highlights

- **Intuitive Navigation:** Standard arrow keys and keyboard shortcuts users already know
- **Visual Feedback:** Colors and highlighting make selections clear and obvious
- **Non-Terminal Fallback:** When output is piped or redirected, the library automatically falls back to standard `std::cin` (no errors or crashes)
- **Responsive:** Immediate visual feedback as users type or navigate
- **Accessible:** Clear visual hierarchy and color contrast for readability

---

## 🐛 Troubleshooting

### Issue: Can't Build or CMake Can't Find Colorline

**Solution:** The CMake script automatically handles dependency resolution. If you encounter issues:

1. Ensure you have internet access (GitHub repository needs to be fetched)
2. Check that your CMake version is 3.28 or higher: `cmake --version`
3. Try clearing the CMake cache: `rm -rf build && mkdir build && cd build && cmake ..`

### Issue: Terminal Input Behaves Unexpectedly

**Possible Causes:**

- Input is redirected from a pipe or file (the library falls back to standard input)
- Terminal emulator doesn't support ANSI escape sequences
- Character encoding is not UTF-8

**Solution:** Ensure you're running in a proper terminal with UTF-8 encoding enabled.

---

## 📄 License

Terminal Utils is licensed under the same terms as the [cpp-modules](https://github.com/huseyinsc/cpp-modules) repository.

---

## 🤝 Contributing

Found a bug? Have a feature request? Feel free to contribute! Check the main [cpp-modules](https://github.com/huseyinsc/cpp-modules) repository for contribution guidelines.

---

## 📞 Support

For issues, questions, or feature requests, please visit the [cpp-modules GitHub repository](https://github.com/huseyinsc/cpp-modules).

---

## 🎯 What Makes Terminal Utils Different?

- **Modern C++23:** Written for today's C++ standards, taking advantage of the latest language features
- **Production-Ready:** Used in real-world CLI applications and developer tools
- **Smart Dependency Management:** The CMake script intelligently resolves dependencies without needless complexity
- **Consistent Cross-Platform Experience:** Same behavior on Windows, Linux, and macOS
- **Full UTF-8 Support:** No more worrying about Unicode handling in terminal applications
- **Inspired by Modern Tools:** Brings the UX innovations of tools like Vite, Zsh, and modern CLI tools to C++

---

**Built with ❤️ for developers who care about CLI UX**
