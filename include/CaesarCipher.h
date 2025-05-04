#ifndef CAESARCIPHER_H
#define CAESARCIPHER_H

#include "CipherAlgorithm.h"

class CaesarCipher : public CipherAlgorithm {
private:
    int shift = 3;  // Default shift value

protected:
    std::string processText(const std::string& text, bool isEncryption) override;

public:
    void setKey(const std::string& key) override;
    std::string getDescription() const override;
    std::string getKeyInstructions() const override;
};

#endif // CAESARCIPHER_H