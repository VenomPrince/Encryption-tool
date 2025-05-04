#ifndef SUBSTITUTIONCIPHER_H
#define SUBSTITUTIONCIPHER_H

#include "CipherAlgorithm.h"
#include <map>
#include <string>

class SubstitutionCipher : public CipherAlgorithm {
private:
    std::map<char, char> encryptionMap;
    std::map<char, char> decryptionMap;
    
    void generateMaps(const std::string& key);

protected:
    std::string processText(const std::string& text, bool isEncryption) override;

public:
    SubstitutionCipher();
    void setKey(const std::string& key) override;
    std::string getDescription() const override;
    std::string getKeyInstructions() const override;
};

#endif // SUBSTITUTIONCIPHER_H