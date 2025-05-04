#ifndef MORSECODECIPHER_H
#define MORSECODECIPHER_H

#include "CipherAlgorithm.h"
#include <map>
#include <string>

class MorseCodeCipher : public CipherAlgorithm {
private:
    std::map<char, std::string> charToMorse;
    std::map<std::string, char> morseToChar;
    std::string separator = " ";

    void initializeReverseMap();

protected:
    std::string processText(const std::string& text, bool isEncryption) override;

public:
    MorseCodeCipher();
    void setKey(const std::string& key) override;
    std::string getDescription() const override;
    std::string getKeyInstructions() const override;
};

#endif // MORSECODECIPHER_H