#include "../include/terminal_utils.h"

void TerminalReader::setRawMode(bool enable, void* old_settings) {
#ifndef _WIN32
    struct termios* oldt = (struct termios*)old_settings;
    if (enable) {
        tcgetattr(STDIN_FILENO, oldt);
        struct termios newt = *oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, oldt);
    }
#endif
}

int TerminalReader::getChar() {
#ifdef _WIN32
    int c = _getch();
    if (c == 0 || c == 224) return _getch(); // Özel tuşların (oklar vb.) kodunu döndür
    return c;
#else
    int c = getchar();
    if (c == 27) { // Escape dizisi (Ok tuşları için)
        int next = getchar();
        if (next == '[') return getchar(); 
        return next;
    }
    return c;
#endif
}

std::string TerminalReader::getUTF8Char(int firstByte) {
    std::string result;
    result += (char)firstByte;
    
    // Check if it's a multi-byte UTF-8 character
    if ((firstByte & 0xE0) == 0xC0) { // 2-byte character
        result += (char)getChar();
    } 
    else if ((firstByte & 0xF0) == 0xE0) { // 3-byte character
        result += (char)getChar();
        result += (char)getChar();
    }
    else if ((firstByte & 0xF8) == 0xF0) { // 4-byte character
        result += (char)getChar();
        result += (char)getChar();
        result += (char)getChar();
    }
    
    return result;
}

bool TerminalReader::isTerminal() {
    return ISATTY(0);
}

void TerminalReader::removeLastUTF8Char(std::string& str) {
    if (str.empty()) return;
    
    // Find the start of the last character
    // UTF-8 characters: first byte determines length
    size_t pos = str.length() - 1;
    
    // Look backwards for the start of the character
    // If we find a byte that's not a continuation byte (not starting with 10xxxxxx)
    while (pos > 0 && (str[pos] & 0xC0) == 0x80) {
        pos--;
    }
    
    // Remove from that position to the end
    str.erase(pos);
}

std::string TerminalReader::readWithPlaceholder(const std::string& prompt, const std::string& placeholder) {
    std::string input = "";
    if (!isTerminal()) { std::cout << prompt; std::getline(std::cin, input); return input; }

#ifndef _WIN32
    struct termios oldt;
    setRawMode(true, &oldt);
#endif

    auto render = [&]() {
        std::cout << "\r\033[2K" << prompt << input;
        if (input.empty()) {
            std::cout << "\033[38;5;59m" << placeholder << "\033[0m" 
                      << "\033[" << placeholder.length() << "D";
        }
        std::cout << std::flush;
    };

    render();
    while (true) {
        int c = getChar();
        if (c == 13 || c == 10) { std::cout << "\n"; break; }
        if (c == 9 || c == 77 || c == 67) { // Tab veya Sağ Ok
            if (input.empty()) { input = placeholder; render(); }
            continue;
        }
        if (c == 8 || c == 127) {
            if (!input.empty()) { 
                removeLastUTF8Char(input); 
                render(); 
            }
            continue;
        }
        if (c >= 32) {
            std::string utf8Char = getUTF8Char(c);
            input += utf8Char;
            render();
        }
    }

#ifndef _WIN32
    setRawMode(false, &oldt);
#endif
    return input;
}

std::string TerminalReader::readWithGhostText(
    const std::string& prompt, 
    const std::string& suggestion,
    const std::string& placeholder
) {
    std::string _placeholder = placeholder.empty() ? suggestion : placeholder;
    std::string input = "";
    if (!isTerminal()) { std::cout << prompt; std::getline(std::cin, input); return input; }

#ifndef _WIN32
    struct termios oldt;
    setRawMode(true, &oldt);
#endif

    auto render = [&]() {
        std::cout << "\r\033[2K" << prompt << input;
        if(input.empty()) {
            std::cout << "\033[38;5;59m" << _placeholder << "\033[0m" 
                      << "\033[" << _placeholder.length() << "D";

        }
        else if (suggestion.find(input) == 0 && input != suggestion) {
            std::string ghost = suggestion.substr(input.length());
            std::cout << "\033[38;5;59m" << ghost << "\033[0m" 
                      << "\033[" << ghost.length() << "D";
        }
        std::cout << std::flush;
    };

    render();
    while (true) {
        int c = getChar();
        if (c == 13 || c == 10) { std::cout << "\n"; break; }
        if (c == 9 || c == 77 || c == 67) { // Tab veya Sağ Ok
            if (suggestion.find(input) == 0) { input = suggestion; render(); }
            continue;
        }
        if (c == 8 || c == 127) {
            if (!input.empty()) { 
                removeLastUTF8Char(input); 
                render(); 
            }
            continue;
        }
        if (c >= 32) {
            std::string utf8Char = getUTF8Char(c);
            input += utf8Char;
            render();
        }
    }

#ifndef _WIN32
    setRawMode(false, &oldt);
#endif
    return input;
}

std::string TerminalReader::readPassword(const std::string& prompt, bool mask) {
    std::string password = "";
    std::cout << prompt << std::flush;
#ifndef _WIN32
    struct termios oldt;
    setRawMode(true, &oldt);
#endif
    while (true) {
        int c = getChar();
        if (c == 13 || c == 10) { std::cout << "\n"; break; }
        if (c == 8 || c == 127) {
            if (!password.empty()) {
                removeLastUTF8Char(password); 
                if (mask) std::cout << "\b \b" << std::flush;
            }
        } 
        else if (c >= 32) {
            std::string utf8Char = getUTF8Char(c);
            password += utf8Char;
            if (mask) std::cout << "*" << std::flush;
        }
    }
#ifndef _WIN32
    setRawMode(false, &oldt);
#endif
    return password;
}