#include "include/PasswordManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

PasswordManager::PasswordManager() {
    loadFromFile("password_database.txt");
}

PasswordManager::~PasswordManager() {
    saveToFile("password_database.txt");
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
             << entry.key << "\n";
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
    
    std::string service, username, encryptedPassword, algorithm, key;
    while (std::getline(file, service) && 
           std::getline(file, username) && 
           std::getline(file, encryptedPassword) && 
           std::getline(file, algorithm) && 
           std::getline(file, key)) {
        
        passwords.push_back({service, username, encryptedPassword, algorithm, key});
    }
    
    file.close();
}

void PasswordManager::addPassword(const std::string& service, const std::string& username, 
                                const std::string& encryptedPassword, const std::string& algorithm, 
                                const std::string& key) {
    passwords.push_back({service, username, encryptedPassword, algorithm, key});
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
                                 std::string& algorithm, std::string& key) {
    for (const auto& entry : passwords) {
        if (entry.service == service) {
            encryptedPassword = entry.encryptedPassword;
            algorithm = entry.algorithm;
            key = entry.key;
            return true;
        }
    }
    return false;
}

void PasswordManager::deletePassword(const std::string& service) {
    auto it = std::find_if(passwords.begin(), passwords.end(), 
                         [&service](const StoredPassword& entry) {
                             return entry.service == service;
                         });
    
    if (it != passwords.end()) {
        passwords.erase(it);
        saveToFile("password_database.txt");
        std::cout << "Password for " << service << " deleted successfully." << std::endl;
    } else {
        std::cout << "No password found for " << service << "." << std::endl;
    }
}