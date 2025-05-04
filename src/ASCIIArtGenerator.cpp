#include "ASCIIArtGenerator.h"
#include <iostream>

void ASCIIArtGenerator::displayBanner() {
    std::cout << "\033[1;32m";
    std::cout << "  _____                             _   _               _____           _ \n";
    std::cout << " | ____|_ __   ___ _ __ _   _ _ __ | |_(_) ___  _ __   |_   _|__   ___ | |\n";
    std::cout << " |  _| | '_ \\ / __| '__| | | | '_ \\| __| |/ _ \\| '_ \\    | |/ _ \\ / _ \\| |\n";
    std::cout << " | |___| | | | (__| |  | |_| | |_) | |_| | (_) | | | |   | | (_) | (_) | |\n";
    std::cout << " |_____|_  |_|\\___|_|   \\__, | .__/ \\__|_|\\___/|_| |_|   |_|\\___/ \\___/|_|\n";
    std::cout << "                        |___/|_|                                                \n";
    std::cout << "\033[0m";
    std::cout << " \n                                                          Encryption Tool v3.0\n";
    std::cout << "\033[1;34m";
    std::cout << " \n                                         Created By Prince, Ashes, and Nishant\n";
    std::cout << "\033[0m";
}

void ASCIIArtGenerator::displayLoadingAnimation(const std::string& message, int milliseconds) {
    std::cout << message;
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
    std::cout << std::endl;
}
