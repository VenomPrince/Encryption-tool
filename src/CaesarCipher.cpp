#include "include/CaesarCipher.h"
#include <iostream>
#include <string>
#include <cctype>

std::string CaesarCipher::processText(const std::string& text, bool isEncryption) {
    std::string result = text;
    int actualShift = isEncryption ? shift : -shift;
    
    for (char& c : result) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            c = static_cast<char>(base + (c - base + actualShift + 26) % 26);
        }
    }
    
    return result;
}

void CaesarCipher::setKey(const std::string& key) {
    try {
        shift = std::stoi(key) % 26;
        if (shift < 0) shift += 26;  // Ensure positive shift
    } catch (const std::exception& e) {
        std::cerr << "Invalid key. Using default shift (3)." << std::endl;
        shift = 3;
    }
}

std::string CaesarCipher::getDescription() const {
    return "Caesar Cipher: A simple substitution cipher where each letter is shifted by a fixed number of positions in the alphabet.";
}

std::string CaesarCipher::getKeyInstructions() const {
    return "Enter a number between 1 and 25 to specify the shift value.";
}