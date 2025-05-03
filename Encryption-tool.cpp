#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <map>
#include <algorithm>
#include <cctype>

// Base class for all encryption algorithms
class CipherAlgorithm {
protected:
    virtual std::string processText(const std::string& text, bool isEncryption) = 0;
public:
    virtual ~CipherAlgorithm() = default;
    
    // Common interface for encryption and decryption
    std::string encrypt(const std::string& plaintext) {
        return processText(plaintext, true);
    }
    
    std::string decrypt(const std::string& ciphertext) {
        return processText(ciphertext, false);
    }
    
    // Method to handle file encryption/decryption
    bool processFile(const std::string& inputFilename, const std::string& outputFilename, bool isEncryption) {
        std::ifstream inputFile(inputFilename);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open input file: " << inputFilename << std::endl;
            return false;
        }
        
        std::string fileContent((std::istreambuf_iterator<char>(inputFile)),
                               std::istreambuf_iterator<char>());
        inputFile.close();
        
        std::string result = isEncryption ? encrypt(fileContent) : decrypt(fileContent);
        
        std::ofstream outputFile(outputFilename);
        if (!outputFile.is_open()) {
            std::cerr << "Error: Unable to open output file: " << outputFilename << std::endl;
            return false;
        }
        
        outputFile << result;
        outputFile.close();
        
        return true;
    }
    
    virtual void setKey(const std::string& key) = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getKeyInstructions() const = 0;
};

// Caesar Cipher implementation
class CaesarCipher : public CipherAlgorithm {
private:
    int shift = 3;  // Default shift value

protected:
    std::string processText(const std::string& text, bool isEncryption) override {
        std::string result = text;
        int actualShift = isEncryption ? shift : -shift;
        
        for (char& c : result) {
            if (std::isalpha(c)) {
                char base = std::isupper(c) ? 'A' : 'a';
                c = static_cast<char>(base + (c - base + actualShift + 26) % 26);
            }
        }
        
        return result;
    }

public:
    void setKey(const std::string& key) override {
        try {
            shift = std::stoi(key) % 26;
            if (shift < 0) shift += 26;  // Ensure positive shift
        } catch (const std::exception& e) {
            std::cerr << "Invalid key. Using default shift (3)." << std::endl;
            shift = 3;
        }
    }
    
    std::string getDescription() const override {
        return "Caesar Cipher: A simple substitution cipher where each letter is shifted by a fixed number of positions in the alphabet.";
    }
    
    std::string getKeyInstructions() const override {
        return "Enter a number between 1 and 25 to specify the shift value.";
    }
};

// Vigenère Cipher implementation
class VigenereCipher : public CipherAlgorithm {
private:
    std::string key = "KEY";  // Default key

protected:
    std::string processText(const std::string& text, bool isEncryption) override {
        std::string result = text;
        size_t keyIndex = 0;
        
        for (char& c : result) {
            if (std::isalpha(c)) {
                char base = std::isupper(c) ? 'A' : 'a';
                char keyChar = std::toupper(key[keyIndex % key.length()]) - 'A';
                
                if (isEncryption) {
                    c = static_cast<char>(base + (c - base + keyChar) % 26);
                } else {
                    c = static_cast<char>(base + (c - base - keyChar + 26) % 26);
                }
                
                keyIndex++;
            }
        }
        
        return result;
    }

public:
    void setKey(const std::string& newKey) override {
        if (newKey.empty()) {
            std::cerr << "Empty key not allowed. Using default key." << std::endl;
            return;
        }
        
        // Check if key contains at least one letter
        bool hasLetter = false;
        for (char c : newKey) {
            if (std::isalpha(c)) {
                hasLetter = true;
                break;
            }
        }
        
        if (!hasLetter) {
            std::cerr << "Key must contain at least one letter. Using default key." << std::endl;
            return;
        }
        
        key = newKey;
    }
    
    std::string getDescription() const override {
        return "Vigenère Cipher: A method of encrypting text using a series of different Caesar ciphers based on the letters of a keyword.";
    }
    
    std::string getKeyInstructions() const override {
        return "Enter a keyword made up of letters (e.g., 'SECRET').";
    }
};

// Substitution Cipher implementation
class SubstitutionCipher : public CipherAlgorithm {
private:
    std::map<char, char> encryptionMap;
    std::map<char, char> decryptionMap;
    
    void generateMaps(const std::string& key) {
        encryptionMap.clear();
        decryptionMap.clear();
        
        std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::string processedKey = key;
        
        // Convert to uppercase
        std::transform(processedKey.begin(), processedKey.end(), processedKey.begin(), ::toupper);
        
        // Remove non-alphabetic characters
        processedKey.erase(
            std::remove_if(processedKey.begin(), processedKey.end(), [](char c) { return !std::isalpha(c); }),
            processedKey.end()
        );
        
        // Remove duplicate characters
        std::string uniqueKey = "";
        for (char c : processedKey) {
            if (uniqueKey.find(c) == std::string::npos) {
                uniqueKey += c;
            }
        }
        
        // Add remaining alphabet characters
        for (char c : alphabet) {
            if (uniqueKey.find(c) == std::string::npos) {
                uniqueKey += c;
            }
        }
        
        // Create encryption and decryption maps
        for (size_t i = 0; i < alphabet.size(); ++i) {
            encryptionMap[alphabet[i]] = uniqueKey[i];
            decryptionMap[uniqueKey[i]] = alphabet[i];
            
            // Add lowercase mappings as well
            encryptionMap[std::tolower(alphabet[i])] = std::tolower(uniqueKey[i]);
            decryptionMap[std::tolower(uniqueKey[i])] = std::tolower(alphabet[i]);
        }
    }

protected:
    std::string processText(const std::string& text, bool isEncryption) override {
        std::string result = text;
        
        for (char& c : result) {
            if (std::isalpha(c)) {
                auto& map = isEncryption ? encryptionMap : decryptionMap;
                auto it = map.find(c);
                if (it != map.end()) {
                    c = it->second;
                }
            }
        }
        
        return result;
    }

public:
    SubstitutionCipher() {
        // Initialize with a default key
        setKey("QWERTYUIOPASDFGHJKLZXCVBNM");
    }
    
    void setKey(const std::string& key) override {
        std::string processedKey = key;
        if (processedKey.empty()) {
            processedKey = "QWERTYUIOPASDFGHJKLZXCVBNM";  // Default substitution
        }
        generateMaps(processedKey);
    }
    
    std::string getDescription() const override {
        return "Substitution Cipher: Each letter is replaced with another letter according to a fixed mapping.";
    }
    
    std::string getKeyInstructions() const override {
        return "Enter a key to determine the substitution alphabet. The key will be processed to create a unique alphabet.";
    }
};

// Main application class
class EncryptionApp {
private:
    std::vector<std::unique_ptr<CipherAlgorithm>> algorithms;
    
    void displayMenu() {
        std::cout << "\n==== Encryption/Decryption Tool ====\n";
        std::cout << "1. Encrypt a message\n";
        std::cout << "2. Decrypt a message\n";
        std::cout << "3. Encrypt a file\n";
        std::cout << "4. Decrypt a file\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
    }
    
    int selectAlgorithm() {
        std::cout << "\nSelect encryption algorithm:\n";
        for (size_t i = 0; i < algorithms.size(); ++i) {
            std::cout << i + 1 << ". " << algorithms[i]->getDescription() << "\n";
        }
        std::cout << "Enter your choice (1-" << algorithms.size() << "): ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear the input buffer
        
        if (choice < 1 || choice > static_cast<int>(algorithms.size())) {
            std::cout << "Invalid choice. Using algorithm 1.\n";
            return 0;
        }
        
        return choice - 1;
    }
    
    void processMessage(bool isEncryption) {
        int algorithmIndex = selectAlgorithm();
        
        std::string key;
        std::cout << algorithms[algorithmIndex]->getKeyInstructions() << "\nEnter key: ";
        std::getline(std::cin, key);
        algorithms[algorithmIndex]->setKey(key);
        
        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        
        std::string result;
        if (isEncryption) {
            result = algorithms[algorithmIndex]->encrypt(message);
            std::cout << "\nEncrypted message: " << result << std::endl;
        } else {
            result = algorithms[algorithmIndex]->decrypt(message);
            std::cout << "\nDecrypted message: " << result << std::endl;
        }
    }
    
    void processFile(bool isEncryption) {
        int algorithmIndex = selectAlgorithm();
        
        std::string key;
        std::cout << algorithms[algorithmIndex]->getKeyInstructions() << "\nEnter key: ";
        std::getline(std::cin, key);
        algorithms[algorithmIndex]->setKey(key);
        
        std::string inputFile, outputFile;
        std::cout << "Enter input file path: ";
        std::getline(std::cin, inputFile);
        std::cout << "Enter output file path: ";
        std::getline(std::cin, outputFile);
        
        bool success = algorithms[algorithmIndex]->processFile(inputFile, outputFile, isEncryption);
        if (success) {
            std::cout << "\nFile " << (isEncryption ? "encrypted" : "decrypted") << " successfully." << std::endl;
        }
    }

public:
    EncryptionApp() {
        // Initialize with available cipher algorithms
        algorithms.push_back(std::make_unique<CaesarCipher>());
        algorithms.push_back(std::make_unique<VigenereCipher>());
        algorithms.push_back(std::make_unique<SubstitutionCipher>());
        // You can add more algorithms here as you implement them
    }
    
    void run() {
        int choice;
        do {
            displayMenu();
            std::cin >> choice;
            std::cin.ignore(); // Clear the input buffer
            
            switch (choice) {
                case 1:
                    processMessage(true); // Encrypt message
                    break;
                case 2:
                    processMessage(false); // Decrypt message
                    break;
                case 3:
                    processFile(true); // Encrypt file
                    break;
                case 4:
                    processFile(false); // Decrypt file
                    break;
                case 5:
                    std::cout << "Exiting program. Goodbye!\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 5);
    }
};

// Main function
int main() {
    EncryptionApp app;
    app.run();
    return 0;
}