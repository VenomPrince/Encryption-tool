#ifndef VIGENERECIPHER_H
#define VIGENERECIPHER_H

#include "CipherAlgorithm.h"

class VigenereCipher : public CipherAlgorithm {
private:
    std::string key = "KEY";  // Default key

protected:
    std::string processText(const std::string& text, bool isEncryption) override;

public:
    void setKey(const std::string& newKey) override;
    std::string getDescription() const override;
    std::string getKeyInstructions() const override;
};

#endif // VIGENERECIPHER_H