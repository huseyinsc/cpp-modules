#include <iostream>
#include "../include/terminal_utils.h"

int main() {
    std::cout << "--- Terminal Utils Demo ---" << std::endl;

    // 1. Placeholder Örneği
    std::string name = TerminalReader::readWithPlaceholder("Adınız: ", "Hüseyin");
    std::cout << "Merhaba, " << (name.empty() ? "Misafir" : name) << "!\n" << std::endl;

    // 2. Ghost Text Örneği
    std::cout << "İpucu: 'git commit -m' yazmayı deneyin." << std::endl;
    std::string cmd = TerminalReader::readWithGhostText("Komut: ", "git commit -m \"initial commit\"");
    std::cout << "Çalıştırılacak: " << cmd << "\n" << std::endl;

    // 3. Şifre Örneği
    std::string pass = TerminalReader::readPassword("Şifre: ", true);
    std::cout << "Şifre alındı (" << pass.length() << " karakter)." << std::endl;

    return 0;
}