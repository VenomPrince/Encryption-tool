#ifndef ASCIIARTGENERATOR_H
#define ASCIIARTGENERATOR_H

#include <string>
#include <chrono>
#include <thread>

class ASCIIArtGenerator {
public:
    static void displayBanner();
    static void displayLoadingAnimation(const std::string& message, int milliseconds);
};

#endif // ASCIIARTGENERATOR_H