#include "EncryptionApp.h"
#include "CaesarCipher.h"
#include "VigenereCipher.h"
#include "SubstitutionCipher.h"
#include "MorseCodeCipher.h"
#include "ROT13Cipher.h"
#include "ASCIIArtGenerator.h"
#include "PasswordStrengthAnalyzer.h"
#include <iostream>
#include <ctime>
#include <string>

EncryptionApp::EncryptionApp() {
    algorithms.push_back(std::make_unique<CaesarCipher>());
    algorithms.push_back(std::make_unique<VigenereCipher>());
    algorithms.push_back(std::make_unique<SubstitutionCipher>());
    algorithms.push_back(std::make_unique<MorseCodeCipher>());
    algorithms.push_back(std::make_unique<ROT13Cipher>());
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void EncryptionApp::displayMenu() {
    ASCIIArtGenerator::displayBanner();
    std::cout << "\n==== Encryption/Decryption Tool ====\n";
    std::cout << "1. Encrypt a message\n";
    std::cout << "2. Decrypt a message\n";
    std::cout << "3. Encrypt a file\n";
    std::cout << "4. Decrypt a file\n";
    std::cout << "5. Password Manager\n";
    std::cout << "6. Analyze password strength\n";
    std::cout << "7. Generate secure password\n";
    std::cout << "8. Exit\n";
    std::cout << "Enter your choice: ";
}

int EncryptionApp::selectAlgorithm() {
    std::cout << "\nSelect encryption algorithm:\n";
    for (size_t i = 0; i < algorithms.size(); ++i) {
        std::cout << i + 1 << ". " << algorithms[i]->getDescription() << "\n";
    }
    std::cout << algorithms.size() + 1 << ". Go back to previous menu\n";
    std::cout << "Enter your choice (1-" << algorithms.size() + 1 << "): ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == static_cast<int>(algorithms.size()) + 1) {
        return -1;  // Special value to indicate going back
    }
    
    if (choice < 1 || choice > static_cast<int>(algorithms.size())) {
        std::cout << "Invalid choice. Using algorithm 1.\n";
        return 0;
    }
    
    return choice - 1;
}

void EncryptionApp::processMessage(bool isEncryption) {
    int algorithmIndex = selectAlgorithm();
    
    // Check if user wants to go back
    if (algorithmIndex == -1) {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    std::string key;
    std::cout << algorithms[algorithmIndex]->getKeyInstructions() << "\nEnter key (or type 'back' to go back): ";
    std::getline(std::cin, key);
    
    if (key == "back") {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    algorithms[algorithmIndex]->setKey(key);
    
    std::string message;
    std::cout << "Enter message (or type 'back' to go back): ";
    std::getline(std::cin, message);
    
    if (message == "back") {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    std::string result;
    if (isEncryption) {
        result = algorithms[algorithmIndex]->encrypt(message);
        std::cout << "\nEncrypted message: " << result << std::endl;
    } else {
        result = algorithms[algorithmIndex]->decrypt(message);
        std::cout << "\nDecrypted message: " << result << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void EncryptionApp::processFile(bool isEncryption) {
    int algorithmIndex = selectAlgorithm();
    
    // Check if user wants to go back
    if (algorithmIndex == -1) {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    std::string key;
    std::cout << algorithms[algorithmIndex]->getKeyInstructions() << "\nEnter key (or type 'back' to go back): ";
    std::getline(std::cin, key);
    
    if (key == "back") {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    algorithms[algorithmIndex]->setKey(key);
    
    std::string inputFile, outputFile;
    std::cout << "Enter input file path (or type 'back' to go back): ";
    std::getline(std::cin, inputFile);
    
    if (inputFile == "back") {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    std::cout << "Enter output file path (or type 'back' to go back): ";
    std::getline(std::cin, outputFile);
    
    if (outputFile == "back") {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    bool success = algorithms[algorithmIndex]->processFile(inputFile, outputFile, isEncryption);
    if (success) {
        std::cout << "\nFile " << (isEncryption ? "encrypted" : "decrypted") << " successfully." << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void EncryptionApp::displayPasswordMenu() {
    std::cout << "\n==== Password Manager ====\n";
    std::cout << "1. Store a new password\n";
    std::cout << "2. Retrieve a password\n";
    std::cout << "3. List all stored passwords\n";
    std::cout << "4. Delete a password\n";
    std::cout << "5. Back to main menu\n";
    std::cout << "Enter your choice: ";
}

void EncryptionApp::passwordManagerMenu() {
    int choice;
    do {
        displayPasswordMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: {
                std::string service, username, password;
                std::cout << "Enter service name (or type 'back' to go back): ";
                std::getline(std::cin, service);
                
                if (service == "back") {
                    break;
                }
                
                std::cout << "Enter username (or type 'back' to go back): ";
                std::getline(std::cin, username);
                
                if (username == "back") {
                    break;
                }
                
                std::cout << "Enter password (or type 'back' to go back): ";
                std::getline(std::cin, password);
                
                if (password == "back") {
                    break;
                }
                
                int algorithmIndex = selectAlgorithm();
                
                // Check if user wants to go back
                if (algorithmIndex == -1) {
                    break;
                }
                
                std::string key;
                std::cout << algorithms[algorithmIndex]->getKeyInstructions() << "\nEnter key (or type 'back' to go back): ";
                std::getline(std::cin, key);
                
                if (key == "back") {
                    break;
                }
                
                algorithms[algorithmIndex]->setKey(key);
                
                std::string encryptedPassword = algorithms[algorithmIndex]->encrypt(password);
                passwordManager.addPassword(service, username, encryptedPassword, 
                                          std::to_string(algorithmIndex), key);
                
                std::cout << "Password stored successfully!" << std::endl;
                
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
            }
            case 2: {
                std::string service;
                std::cout << "Enter service name (or type 'back' to go back): ";
                std::getline(std::cin, service);
                
                if (service == "back") {
                    break;
                }
                
                std::string encryptedPassword, algorithmStr, key;
                if (passwordManager.getPassword(service, encryptedPassword, algorithmStr, key)) {
                    int algorithmIndex = std::stoi(algorithmStr);
                    algorithms[algorithmIndex]->setKey(key);
                    std::string decryptedPassword = algorithms[algorithmIndex]->decrypt(encryptedPassword);
                    
                    std::cout << "\nPassword for " << service << " retrieved:\n";
                    std::cout << "Decrypted password: " << decryptedPassword << std::endl;
                } else {
                    std::cout << "No password found for " << service << "." << std::endl;
                }
                
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
            }
            case 3:
                passwordManager.listPasswords();
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
            case 4: {
                std::string service;
                std::cout << "Enter service name to delete (or type 'back' to go back): ";
                std::getline(std::cin, service);
                
                if (service == "back") {
                    break;
                }
                
                passwordManager.deletePassword(service);
                
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
            }
            case 5:
                std::cout << "Returning to main menu.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

void EncryptionApp::analyzePasswordStrength() {
    std::string password;
    std::cout << "Enter password to analyze (or type 'back' to go back): ";
    std::getline(std::cin, password);
    
    if (password == "back") {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    PasswordStrengthAnalyzer::analyzeStrength(password);
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void EncryptionApp::generateSecurePassword() {
    int length;
    std::cout << "Enter desired password length (8-30 recommended) or 0 to go back: ";
    std::cin >> length;
    std::cin.ignore();
    
    if (length == 0) {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    if (length < 1) length = 12;
    
    std::string password = PasswordStrengthAnalyzer::generateSecurePassword(length);
    
    std::cout << "\nGenerated secure password: " << password << std::endl;
    PasswordStrengthAnalyzer::analyzeStrength(password);
    
    std::cout << "\nWould you like to store this password? (y/n/b for back): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == 'b' || choice == 'B') {
        std::cout << "Returning to main menu.\n";
        return;
    }
    
    if (choice == 'y' || choice == 'Y') {
        std::string service, username;
        std::cout << "Enter service name (or type 'back' to go back): ";
        std::getline(std::cin, service);
        
        if (service == "back") {
            std::cout << "Returning to main menu.\n";
            return;
        }
        
        std::cout << "Enter username (or type 'back' to go back): ";
        std::getline(std::cin, username);
        
        if (username == "back") {
            std::cout << "Returning to main menu.\n";
            return;
        }
        
        int algorithmIndex = selectAlgorithm();
        
        // Check if user wants to go back
        if (algorithmIndex == -1) {
            std::cout << "Returning to main menu.\n";
            return;
        }
        
        std::string key;
        std::cout << algorithms[algorithmIndex]->getKeyInstructions() << "\nEnter key (or type 'back' to go back): ";
        std::getline(std::cin, key);
        
        if (key == "back") {
            std::cout << "Returning to main menu.\n";
            return;
        }
        
        algorithms[algorithmIndex]->setKey(key);
        
        std::string encryptedPassword = algorithms[algorithmIndex]->encrypt(password);
        passwordManager.addPassword(service, username, encryptedPassword, 
                                  std::to_string(algorithmIndex), key);
        
        std::cout << "Password stored successfully!" << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void EncryptionApp::run() {
    ASCIIArtGenerator::displayLoadingAnimation("Initializing encryption tools", 500);
    
    int choice;
    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                processMessage(true);
                break;
            case 2:
                processMessage(false);
                break;
            case 3:
                processFile(true);
                break;
            case 4:
                processFile(false);
                break;
            case 5:
                passwordManagerMenu();
                break;
            case 6:
                analyzePasswordStrength();
                break;
            case 7:
                generateSecurePassword();
                break;
            case 8:
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
        }
    } while (choice != 8);
}