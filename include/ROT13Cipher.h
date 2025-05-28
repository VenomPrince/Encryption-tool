#ifndef ROT13CIPHER_H
#define ROT13CIPHER_H

#include "CipherAlgorithm.h"

class ROT13Cipher : public CipherAlgorithm {
public:
    ROT13Cipher(); // Default constructor
    ROT13Cipher(const std::string& info); // Overloaded constructor
    std::string processText(const std::string& text, bool isEncryption) override;
    void setKey(const std::string& key) override;
    std::string getDescription() const override;
    std::string getKeyInstructions() const override;
    friend std::ostream& operator<<(std::ostream& os, const ROT13Cipher& cipher);
};

#endif // ROT13CIPHER_H