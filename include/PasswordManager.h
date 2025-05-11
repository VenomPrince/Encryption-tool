#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>

class PasswordManager {
private:
    struct StoredPassword {
        std::string service;
        std::string username;
        std::string encryptedPassword;
        std::string algorithm;
        std::string key;
        std::string salt;  // New: Added salt for improved security
    };
    
    std::vector<StoredPassword> passwords;
    
    // Generates random salt for password security
    std::string generateSalt(size_t length = 16);
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    
    // Secure erase helper function
    void secureErase(std::string& str);

public:
    PasswordManager();
    ~PasswordManager();
    
    void addPassword(const std::string& service, const std::string& username, 
                    const std::string& encryptedPassword, const std::string& algorithm, 
                    const std::string& key);
    void listPasswords();
    bool getPassword(const std::string& service, std::string& encryptedPassword, 
                     std::string& algorithm, std::string& key, std::string& salt);
    void deletePassword(const std::string& service);
    
    // New methods:
    bool serviceExists(const std::string& service);
    void updatePassword(const std::string& service, const std::string& newEncryptedPassword, 
                      const std::string& algorithm, const std::string& key);
    void exportPasswords(const std::string& filename);
    bool importPasswords(const std::string& filename);
};

#endif // PASSWORDMANAGER_H
