#pragma once
#include <iostream>
#include <array>
#include <utility>
#include <string>
#include <string_view>

#if defined(_WIN32) || defined(_WIN64)
    #ifndef PLATFORM_WINDOWS
        #define PLATFORM_WINDOWS
    #endif
    #include <windows.h>
#else
    #ifndef PLATFORM_LINUX
        #define PLATFORM_LINUX
    #endif
#endif

enum class Color {
    Black = 0, Blue, Green, Aqua, Red, Purple, Yellow, White,
    Gray, LightBlue, LightGreen, LightAqua, LightRed, LightPurple, 
    LightYellow, BrightWhite, Reset
};

class Colored {
private:
    // Derleme hatasını önlemek için const char* kullanıyoruz
    static constexpr std::array<std::pair<const char*, const char*>, 17> colorArray = {{
        {"\033[38;5;0m", "\033[48;5;0m"},     // Black
        {"\033[38;5;21m", "\033[48;5;21m"},   // Blue
        {"\033[38;5;34m", "\033[48;5;34m"},   // Green
        {"\033[38;5;45m", "\033[48;5;45m"},   // Aqua
        {"\033[38;5;196m", "\033[48;5;196m"}, // Red
        {"\033[38;5;57m", "\033[48;5;57m"},   // Purple
        {"\033[38;5;226m", "\033[48;5;226m"}, // Yellow
        {"\033[38;5;15m", "\033[48;5;15m"},   // White
        {"\033[38;5;59m", "\033[48;5;59m"},   // Gray
        {"\033[38;5;39m", "\033[48;5;39m"},   // LightBlue
        {"\033[38;5;77m", "\033[48;5;77m"},   // LightGreen
        {"\033[38;5;87m", "\033[48;5;87m"},   // LightAqua
        {"\033[38;5;203m", "\033[48;5;203m"}, // LightRed
        {"\033[38;5;141m", "\033[48;5;141m"}, // LightPurple
        {"\033[38;5;227m", "\033[48;5;227m"}, // LightYellow
        {"\033[38;5;231m", "\033[48;5;231m"}, // BrightWhite
        {"\033[39m", "\033[49m"}              // Reset
    }};

    static constexpr std::array<const char*, 17> colorNames = {
        "Black", "Blue", "Green", "Aqua", "Red", "Purple", "Yellow", "White",
        "Gray", "Light Blue", "Light Green", "Light Aqua", "Light Red", "Light Purple", 
        "Light Yellow", "Bright White", "Reset"
    };

    static constexpr bool shouldUseAnsi(Color c) noexcept {
        return c == Color::Red || c == Color::Purple || c == Color::Yellow;
    }

public:
    static inline const char* Reset = "\033[0m";
    
    // std::string_view ve [[nodiscard]] entegrasyonu
    [[nodiscard]] static std::string text(
        std::pair<Color, Color> startPair, 
        std::string_view text = "", 
        std::pair<Color, Color> endPair = {Color::Reset, Color::Reset}
    ) { 
        std::string result = fg(startPair.first) + bg(startPair.second) + std::string(text);
        result += fg(endPair.first) + bg(endPair.second);
        return result;
    }

    [[nodiscard]] static std::string fg(Color start) { 
        return colorArray[static_cast<size_t>(start)].first; 
    }
    
    [[nodiscard]] static std::string fg(Color start, std::string_view text, Color end = Color::BrightWhite) { 
        return fg(start) + std::string(text) + fg(end);
    }
    
    [[nodiscard]] static std::string bg(Color c) { 
        return colorArray[static_cast<size_t>(c)].second; 
    }
    
    [[nodiscard]] static std::string bg(Color start, std::string_view text, Color end = Color::BrightWhite) { 
        return bg(start) + std::string(text) + bg(end);
    }

    static void setColor(Color c) {
#ifdef PLATFORM_WINDOWS
        if (shouldUseAnsi(c)) {
            std::cout << fg(c) << std::flush;
        } else {
            // Windows'ta mevcut arka planı koruyan DeepSeek düzeltmesi
            HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(handle, &csbi);
            WORD attributes = (csbi.wAttributes & 0xFFF0) | static_cast<WORD>(c);
            SetConsoleTextAttribute(handle, attributes);
        }
#else
        std::cout << fg(c) << std::flush;
#endif
    }

    static void setColor(unsigned int hexColor) {
        if(hexColor > 15) hexColor = 7;
        setColor(static_cast<Color>(hexColor));
    }

    [[nodiscard]] static std::string_view getName(Color c) noexcept {
        return colorNames[static_cast<size_t>(c)];
    }
    
    static void print(std::string_view text, Color start, Color end = Color::BrightWhite) {
        setColor(start);
        std::cout << text;
        setColor(end);
    }

    static void print(std::string_view text, unsigned int start, unsigned int end = 7) {
        setColor(start);
        std::cout << text;
        setColor(end);
    }
};
