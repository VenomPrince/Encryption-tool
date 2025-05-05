#include "SubstitutionCipher.h"
#include <algorithm>
#include <cctype>

SubstitutionCipher::SubstitutionCipher() {
    setKey("QWERTYUIOPASDFGHJKLZXCVBNM");
}

void SubstitutionCipher::generateMaps(const std::string& key) {
    encryptionMap.clear();
    decryptionMap.clear();
    
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string processedKey = key;
    
    std::transform(processedKey.begin(), processedKey.end(), processedKey.begin(), ::toupper);
    
    processedKey.erase(
        std::remove_if(processedKey.begin(), processedKey.end(), [](char c) { return !std::isalpha(c); }),
        processedKey.end()
    );
    
    std::string uniqueKey = "";
    for (char c : processedKey) {
        if (uniqueKey.find(c) == std::string::npos) {
            uniqueKey += c;
        }
    }
    
    for (char c : alphabet) {
        if (uniqueKey.find(c) == std::string::npos) {
            uniqueKey += c;
        }
    }
    
    for (size_t i = 0; i < alphabet.size(); ++i) {
        encryptionMap[alphabet[i]] = uniqueKey[i];
        decryptionMap[uniqueKey[i]] = alphabet[i];
        
        encryptionMap[std::tolower(alphabet[i])] = std::tolower(uniqueKey[i]);
        decryptionMap[std::tolower(uniqueKey[i])] = std::tolower(alphabet[i]);
    }
}

std::string SubstitutionCipher::processText(const std::string& text, bool isEncryption) {
    std::string result = text;
    
    for (char& c : result) {
        if (std::isalpha(c)) {
            auto& map = isEncryption ? encryptionMap : decryptionMap;
            auto it = map.find(c);
            if (it != map.end()) {
                c = it->second;
            }
        }
    }
    
    return result;
}

void SubstitutionCipher::setKey(const std::string& key) {
    std::string processedKey = key;
    if (processedKey.empty()) {
        processedKey = "QWERTYUIOPASDFGHJKLZXCVBNM";
    }
    generateMaps(processedKey);
}

std::string SubstitutionCipher::getDescription() const {
    return "\033[1;34mSubstitution Cipher:\033[0m Each letter is replaced with another letter according to a fixed mapping.";
}

std::string SubstitutionCipher::getKeyInstructions() const {
    return "Enter a key to determine the substitution alphabet. The key will be processed to create a unique alphabet.";
}