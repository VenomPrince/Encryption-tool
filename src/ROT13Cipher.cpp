#include "ROT13Cipher.h"
#include <cctype>
#include <iostream>

ROT13Cipher::ROT13Cipher() {
    // Default initialization
}

ROT13Cipher::ROT13Cipher(const std::string& info) {
    // Use info for custom initialization
}

std::string ROT13Cipher::processText(const std::string& text, bool isEncryption) {
    std::string result = text;
    
    for (char& c : result) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            c = static_cast<char>(base + (c - base + 13) % 26);
        }
    }
    
    return result;
}

void ROT13Cipher::setKey(const std::string& key) {
    // ROT13 doesn't use a key
}

std::string ROT13Cipher::getDescription() const {
    return "\033[1;34mROT13:\033[0m A simple letter substitution cipher that replaces each letter with the letter 13 positions after it.";
}

std::string ROT13Cipher::getKeyInstructions() const {
    return "\033[1;34mROT13 doesn't require a key. Press Enter to continue.\033[0m ";
}

std::ostream& operator<<(std::ostream& os, const ROT13Cipher& cipher) {
    os << cipher.getDescription();
    return os;
}