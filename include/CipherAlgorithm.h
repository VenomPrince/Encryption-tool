#ifndef CIPHERALGORITHM_H
#define CIPHERALGORITHM_H

#include <string>
#include <fstream>

class CipherAlgorithm {
protected:
    virtual std::string processText(const std::string& text, bool isEncryption) = 0;
public:
    virtual ~CipherAlgorithm() = default;
    
    std::string encrypt(const std::string& plaintext);
    std::string decrypt(const std::string& ciphertext);
    bool processFile(const std::string& inputFilename, const std::string& outputFilename, bool isEncryption);
    
    virtual void setKey(const std::string& key) = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getKeyInstructions() const = 0;
};

#endif // CIPHERALGORITHM_H