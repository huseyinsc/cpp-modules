#include <iostream>
#include "../include/terminal_utils.h"

int main() {
    std::cout << "--- Terminal Utils Demo ---" << std::endl;

    // 1. Placeholder Örneği
    std::string name = TerminalReader::readWithGhostText("Name: ", "Hüseyin", "<your name>");
    std::cout << "Hello, " << (name.empty() ? "Guest" : name) << "!\n" << std::endl;

    // 2. Ghost Text Örneği
    std::cout << "Hint: Try typing 'git commit -m'." << std::endl;
    std::string cmd = TerminalReader::readWithGhostText("Command: ", "git commit -m \"initial commit\"");
    std::cout << "Executing: " << cmd << "\n" << std::endl;

    // 3. Şifre Örneği
    std::string pass = TerminalReader::readPassword("Password: ", true);
    std::cout << "Password retrieved (" << pass.length() << " characters)." << std::endl;

    return 0;
}