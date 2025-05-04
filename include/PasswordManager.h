#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <iomanip>

class PasswordManager {
private:
    struct StoredPassword {
        std::string service;
        std::string username;
        std::string encryptedPassword;
        std::string algorithm;
        std::string key;
    };
    
    std::vector<StoredPassword> passwords;
    
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

public:
    PasswordManager();
    ~PasswordManager();
    
    void addPassword(const std::string& service, const std::string& username, 
                    const std::string& encryptedPassword, const std::string& algorithm, 
                    const std::string& key);
    void listPasswords();
    bool getPassword(const std::string& service, std::string& encryptedPassword, 
                     std::string& algorithm, std::string& key);
    void deletePassword(const std::string& service);
};

#endif // PASSWORDMANAGER_H