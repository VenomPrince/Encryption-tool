#ifndef ROT13CIPHER_H
#define ROT13CIPHER_H

#include "CipherAlgorithm.h"

class ROT13Cipher : public CipherAlgorithm {
protected:
    std::string processText(const std::string& text, bool isEncryption) override;

public:
    void setKey(const std::string& key) override;
    std::string getDescription() const override;
    std::string getKeyInstructions() const override;
};

#endif // ROT13CIPHER_H