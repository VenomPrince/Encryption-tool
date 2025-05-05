#include "VigenereCipher.h"
#include <iostream>
#include <string>
#include <cctype>

std::string VigenereCipher::processText(const std::string& text, bool isEncryption) {
    std::string result = text;
    size_t keyIndex = 0;
    
    for (char& c : result) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            char keyChar = std::toupper(key[keyIndex % key.length()]) - 'A';
            
            if (isEncryption) {
                c = static_cast<char>(base + (c - base + keyChar) % 26);
            } else {
                c = static_cast<char>(base + (c - base - keyChar + 26) % 26);
            }
            
            keyIndex++;
        }
    }
    
    return result;
}

void VigenereCipher::setKey(const std::string& newKey) {
    if (newKey.empty()) {
        std::cerr << "Empty key not allowed. Using default key." << std::endl;
        return;
    }
    
    bool hasLetter = false;
    for (char c : newKey) {
        if (std::isalpha(c)) {
            hasLetter = true;
            break;
        }
    }
    
    if (!hasLetter) {
        std::cerr << "Key must contain at least one letter. Using default key." << std::endl;
        return;
    }
    
    key = newKey;
}

std::string VigenereCipher::getDescription() const {
    return "\033[1;34mVigenère Cipher:\033[0m A method of encrypting text using a series of different Caesar ciphers based on the letters of a keyword.";
}

std::string VigenereCipher::getKeyInstructions() const {
    return "Enter a keyword made up of letters (e.g., 'SECRET').";
}