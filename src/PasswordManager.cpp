#include "PasswordManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <sstream>

PasswordManager::PasswordManager() {
    loadFromFile("password_database.txt");
}

PasswordManager::~PasswordManager() {
    saveToFile("password_database.txt");
    
    // Securely erase passwords from memory
    for (auto& entry : passwords) {
        secureErase(entry.encryptedPassword);
        secureErase(entry.key);
        secureErase(entry.salt);
    }
}

std::string PasswordManager::generateSalt(size_t length) {
    const std::string chars = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%^&*()_+-=";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, chars.size() - 1);
    
    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt += chars[distribution(generator)];
    }
    
    return salt;
}

void PasswordManager::secureErase(std::string& str) {
    // Overwrite memory before freeing
    std::fill(str.begin(), str.end(), 0);
    str.clear();
}

void PasswordManager::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open password file for writing." << std::endl;
        return;
    }
    
    for (const auto& entry : passwords) {
        file << entry.service << "\n"
             << entry.username << "\n"
             << entry.encryptedPassword << "\n"
             << entry.algorithm << "\n"
             << entry.key << "\n"
             << entry.salt << "\n";  // Added salt to file format
    }
    
    file.close();
}

void PasswordManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No existing password file found. Creating new database." << std::endl;
        return;
    }
    
    passwords.clear();
    
    std::string service, username, encryptedPassword, algorithm, key, salt;
    
    // Try to read with salt (new format)
    while (std::getline(file, service) && 
           std::getline(file, username) && 
           std::getline(file, encryptedPassword) && 
           std::getline(file, algorithm) && 
           std::getline(file, key)) {
        
        // Check if we have a salt in the file (backward compatibility)
        if (!std::getline(file, salt)) {
            salt = generateSalt(); // Generate salt for old entries
            file.clear(); // Clear any error flags
        }
        
        passwords.push_back({service, username, encryptedPassword, algorithm, key, salt});
    }
    
    file.close();
}

void PasswordManager::addPassword(const std::string& service, const std::string& username, 
                                const std::string& encryptedPassword, const std::string& algorithm, 
                                const std::string& key) {
    // Generate a unique salt for this password
    std::string salt = generateSalt();
    
    // Check if service already exists
    auto it = std::find_if(passwords.begin(), passwords.end(), 
                         [&service](const StoredPassword& entry) {
                             return entry.service == service;
                         });
    
    if (it != passwords.end()) {
        // Update existing entry
        it->username = username;
        it->encryptedPassword = encryptedPassword;
        it->algorithm = algorithm;
        it->key = key;
        it->salt = salt;
        std::cout << "Password for " << service << " updated." << std::endl;
    } else {
        // Add new entry
        passwords.push_back({service, username, encryptedPassword, algorithm, key, salt});
        std::cout << "Password for " << service << " stored successfully." << std::endl;
    }
    
    saveToFile("password_database.txt");
}

void PasswordManager::listPasswords() {
    if (passwords.empty()) {
        std::cout << "No saved passwords found." << std::endl;
        return;
    }
    
    std::cout << "\n===== Saved Passwords =====\n";
    std::cout << std::left << std::setw(20) << "Service" << std::setw(20) << "Username" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    for (const auto& entry : passwords) {
        std::cout << std::left << std::setw(20) << entry.service << std::setw(20) << entry.username << std::endl;
    }
}

bool PasswordManager::getPassword(const std::string& service, std::string& encryptedPassword, 
                                 std::string& algorithm, std::string& key, std::string& salt) {
    for (const auto& entry : passwords) {
        if (entry.service == service) {
            encryptedPassword = entry.encryptedPassword;
            algorithm = entry.algorithm;
            key = entry.key;
            salt = entry.salt;
            return true;
        }
    }
    return false;
}

bool PasswordManager::serviceExists(const std::string& service) {
    return std::any_of(passwords.begin(), passwords.end(),
                     [&service](const StoredPassword& entry) {
                         return entry.service == service;
                     });
}

void PasswordManager::updatePassword(const std::string& service, const std::string& newEncryptedPassword, 
                                  const std::string& algorithm, const std::string& key) {
    for (auto& entry : passwords) {
        if (entry.service == service) {
            entry.encryptedPassword = newEncryptedPassword;
            entry.algorithm = algorithm;
            entry.key = key;
            entry.salt = generateSalt(); // Generate new salt for updated password
            saveToFile("password_database.txt");
            std::cout << "Password for " << service << " updated successfully." << std::endl;
            return;
        }
    }
    
    std::cout << "Service not found. Use addPassword to create a new entry." << std::endl;
}

void PasswordManager::deletePassword(const std::string& service) {
    auto it = std::find_if(passwords.begin(), passwords.end(), 
                         [&service](const StoredPassword& entry) {
                             return entry.service == service;
                         });
    
    if (it != passwords.end()) {
        // Securely erase sensitive data
        secureErase(it->encryptedPassword);
        secureErase(it->key);
        secureErase(it->salt);
        
        passwords.erase(it);
        saveToFile("password_database.txt");
        std::cout << "Password for " << service << " deleted successfully." << std::endl;
    } else {
        std::cout << "No password found for " << service << "." << std::endl;
    }
}

void PasswordManager::exportPasswords(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for export: " << filename << std::endl;
        return;
    }
    
    // Export in a simple CSV format
    file << "Service,Username,Algorithm\n";
    for (const auto& entry : passwords) {
        file << entry.service << "," << entry.username << "," << entry.algorithm << "\n";
    }
    
    file.close();
    std::cout << "Passwords exported to " << filename << " successfully!" << std::endl;
    std::cout << "Note: For security, actual passwords are not exported." << std::endl;
}

bool PasswordManager::importPasswords(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for import: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    std::getline(file, line); // Skip header line
    
    int importCount = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string service, username, algorithm;
        
        std::getline(ss, service, ',');
        std::getline(ss, username, ',');
        std::getline(ss, algorithm, ',');
        
        if (!service.empty() && !username.empty()) {
            // Prompt user to enter the password for this service
            std::cout << "Enter password for " << service << " (" << username << "): ";
            std::string password;
            std::getline(std::cin, password);
            
            // Generate a dummy encrypted password (user will need to encrypt it properly)
            std::string encryptedPassword = "IMPORT_" + password;
            std::string key = "default";
            
            addPassword(service, username, encryptedPassword, algorithm, key);
            importCount++;
        }
    }
    
    std::cout << "Successfully imported " << importCount << " passwords." << std::endl;
    return true;
}