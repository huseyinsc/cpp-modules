#include <iostream>
#include <string>
#include <sstream>

/* * ---------------------------------------------------------------------------
 * CHOOSE YOUR METHOD
 * ---------------------------------------------------------------------------
 * Currently, as of March 2026, C++20 Modules still face limited support 
 * from various IDE IntelliSense engines and static analysis tools. 
 * Therefore, the traditional header-based approach is enabled by default 
 * to ensure a better developer experience (autocompletion, error highlighting).
 */

// METHOD 1: Traditional Header (Active for IntelliSense support)
#include "../include/colorline.hpp"

// METHOD 2: Modern C++20 Module (Commented out due to limited IDE tooling support)
//import colorline; 

/* * ---------------------------------------------------------------------------
 * COMPILATION COMMANDS (GCC/G++)
 * ---------------------------------------------------------------------------
 *
 * FOR HEADER-BASED (Current):
 * g++ -std=c++20 main.cpp -I../include -o colorline
 *
 * FOR MODULE-BASED:
 * 1. Precompile the module interface:
 * g++ -std=c++20 -fmodules-ts -c colorline.cppm -o colorline.o
 * 2. Compile main with the module:
 * g++ -std=c++20 -fmodules-ts main.cpp colorline.o -o colorline
 */

unsigned int parseColorCode(const std::string& arg) {
    std::istringstream iss(arg);
    unsigned int value;
    if (!(iss >> std::hex >> value) || value > 0xF) {
        return 7; // Default to White if invalid
    }
    return value;
}

int main(int argc, char* argv[]) {
    std::string prefix = "";
    std::string program = "colorline";
    std::string osName = "Linux";

#ifdef _WIN32
    osName = "Windows";
    program += ".exe";
    Colored::setColor(Color::White);
#else
    std::cout << Colored::Reset;
    prefix = "./";
#endif

    if (argc >= 2) {
        std::string cmd = argv[1];
        
        if (argc == 3) {
            osName = argv[2]; 
        }

        if (cmd == "all") {
            for (unsigned int i = 0; i < 16; ++i) {
                Colored::setColor(static_cast<Color>(i));
                std::cout << prefix << program << " " << std::hex << i 
                          << " " << osName << " (" << Colored::getName(static_cast<Color>(i)) << ")\n";
            }
            std::cout << Colored::Reset << std::endl;
        } else {
            unsigned int color = parseColorCode(cmd);
            Colored::setColor(static_cast<Color>(color));
        }
        return 0;
    }

    std::cout << "Usage: " << prefix << program << " [attr] [os_name](optional)\n";
    std::cout << "Example: " << prefix << program << " all " << osName << "\n";
    return 1;
}