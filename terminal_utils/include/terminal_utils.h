#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#include <iostream>
#include <string>

#ifdef _WIN32
    #include <conio.h>
    #include <io.h>
    #define ISATTY _isatty
#else
    #include <termios.h>
    #include <unistd.h>
    #define ISATTY isatty
#endif

class TerminalReader {
public:
    static void setRawMode(bool enable, void* old_settings);
    static bool isTerminal();
    static int getChar();
    static std::string getUTF8Char(int firstByte);
    static void removeLastUTF8Char(std::string& str);
    // Standart Placeholder Okuma
    static std::string readWithPlaceholder(const std::string& prompt, const std::string& placeholder);
    
    // Ghost Text (Zsh stili) Okuma
    static std::string readWithGhostText(
        const std::string& prompt, 
        const std::string& suggestion, 
        const std::string& placeholder = ""
    );
    
    // Şifre Okuma (Yazılanı gizler veya yıldız koyar)
    static std::string readPassword(const std::string& prompt, bool mask = true);
};

#endif