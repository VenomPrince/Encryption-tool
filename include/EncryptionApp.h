#ifndef ENCRYPTIONAPP_H
#define ENCRYPTIONAPP_H

#include <vector>
#include <memory>
#include "CipherAlgorithm.h"
#include "PasswordManager.h"

class EncryptionApp {
private:
    std::vector<std::unique_ptr<CipherAlgorithm>> algorithms;
    PasswordManager passwordManager;
    
    void displayMenu();
    int selectAlgorithm();
    void processMessage(bool isEncryption);
    void processFile(bool isEncryption);
    void displayPasswordMenu();

public:
    EncryptionApp();
    void passwordManagerMenu();
    void analyzePasswordStrength();
    void generateSecurePassword();
    void run();
};

#endif // ENCRYPTIONAPP_H