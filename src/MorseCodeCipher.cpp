#include "include/MorseCodeCipher.h"

MorseCodeCipher::MorseCodeCipher() {
    charToMorse = {
        {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."}, {'F', "..-."}, 
        {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"}, {'K', "-.-"}, {'L', ".-.."},
        {'M', "--"}, {'N', "-."}, {'O', "---"}, {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."},
        {'S', "..."}, {'T', "-"}, {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"},
        {'Y', "-.--"}, {'Z', "--.."}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
        {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, 
        {'9', "----."}, {'0', "-----"}, {' ', "/"}
    };
    initializeReverseMap();
}

void MorseCodeCipher::initializeReverseMap() {
    for (const auto& pair : charToMorse) {
        morseToChar[pair.second] = pair.first;
    }
}

std::string MorseCodeCipher::processText(const std::string& text, bool isEncryption) {
    if (isEncryption) {
        std::string result;
        for (char c : text) {
            char upperC = std::toupper(c);
            if (charToMorse.find(upperC) != charToMorse.end()) {
                result += charToMorse[upperC] + separator;
            }
        }
        if (!result.empty() && result.substr(result.length() - separator.length()) == separator) {
            result = result.substr(0, result.length() - separator.length());
        }
        return result;
    } else {
        std::string result;
        std::string currentMorse;
        
        std::string textWithSpace = text + " ";
        
        for (char c : textWithSpace) {
            if (c == ' ' || c == '/') {
                if (!currentMorse.empty()) {
                    if (morseToChar.find(currentMorse) != morseToChar.end()) {
                        result += morseToChar[currentMorse];
                    }
                    currentMorse.clear();
                }
                if (c == '/') {
                    result += ' ';
                }
            } else {
                currentMorse += c;
            }
        }
        
        return result;
    }
}

void MorseCodeCipher::setKey(const std::string& key) {
    if (!key.empty()) {
        separator = key;
    }
}

std::string MorseCodeCipher::getDescription() const {
    return "Morse Code: Converts text to dots and dashes according to international Morse code.";
}

std::string MorseCodeCipher::getKeyInstructions() const {
    return "Enter a separator character/string for Morse code symbols (default is space).";
}