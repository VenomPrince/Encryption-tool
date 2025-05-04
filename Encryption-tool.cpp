#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <map>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>

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
// Morse Code implementation
class MorseCodeCipher : public CipherAlgorithm {
private:
    std::map<char, std::string> charToMorse = {
        {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."}, {'F', "..-."}, 
        {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"}, {'K', "-.-"}, {'L', ".-.."},
        {'M', "--"}, {'N', "-."}, {'O', "---"}, {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."},
        {'S', "..."}, {'T', "-"}, {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"},
        {'Y', "-.--"}, {'Z', "--.."}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
        {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, 
        {'9', "----."}, {'0', "-----"}, {' ', "/"}
    };
    
    std::map<std::string, char> morseToChar;
    std::string separator = " ";

    void initializeReverseMap() {
        for (const auto& pair : charToMorse) {
            morseToChar[pair.second] = pair.first;
        }
    }

protected:
    std::string processText(const std::string& text, bool isEncryption) override {
        if (isEncryption) {
            std::string result;
            for (char c : text) {
                char upperC = std::toupper(c);
                if (charToMorse.find(upperC) != charToMorse.end()) {
                    result += charToMorse[upperC] + separator;
                }
            }
            // Remove trailing separator if exists
            if (!result.empty() && result.substr(result.length() - separator.length()) == separator) {
                result = result.substr(0, result.length() - separator.length());
            }
            return result;
        } else {
            std::string result;
            std::string currentMorse;
            
            // Add a space at the end to process the last morse code
            std::string textWithSpace = text + " ";
            
            for (char c : textWithSpace) {
                if (c == ' ' || c == '/') {
                    if (!currentMorse.empty()) {
                        if (morseToChar.find(currentMorse) != morseToChar.end()) {
                            result += morseToChar[currentMorse];
                        }
                        currentMorse.clear();
                    }
                    if (c == '/') {
                        result += ' ';  // Space between words
                    }
                } else {
                    currentMorse += c;
                }
            }
            
            return result;
        }
    }

public:
    MorseCodeCipher() {
        initializeReverseMap();
    }
    
    void setKey(const std::string& key) override {
        if (!key.empty()) {
            separator = key;
        }
    }
    
    std::string getDescription() const override {
        return "Morse Code: Converts text to dots and dashes according to international Morse code.";
    }
    
    std::string getKeyInstructions() const override {
        return "Enter a separator character/string for Morse code symbols (default is space).";
    }
};

// ROT13 Cipher implementation
class ROT13Cipher : public CipherAlgorithm {
protected:
    std::string processText(const std::string& text, bool isEncryption) override {
        // ROT13 is its own inverse, so encryption and decryption are the same
        std::string result = text;
        
        for (char& c : result) {
            if (std::isalpha(c)) {
                char base = std::isupper(c) ? 'A' : 'a';
                c = static_cast<char>(base + (c - base + 13) % 26);
            }
        }
        
        return result;
    }

public:
    void setKey(const std::string& key) override {
        // ROT13 doesn't use a key
    }
    
    std::string getDescription() const override {
        return "ROT13: A simple letter substitution cipher that replaces each letter with the letter 13 positions after it.";
    }
    
    std::string getKeyInstructions() const override {
        return "ROT13 doesn't require a key. Press Enter to continue.";
    }
};

// ASCII Art Banner generator
class ASCIIArtGenerator {
public:
    static void displayBanner() {
        std::cout << "\033[1;32m";  // Green color
        std::cout << "  _____                             _   _               _____           _ \n";
        std::cout << " | ____|_ __   ___ _ __ _   _ _ __ | |_(_) ___  _ __   |_   _|__   ___ | |\n";
        std::cout << " |  _| | '_ \\ / __| '__| | | | '_ \\| __| |/ _ \\| '_ \\    | |/ _ \\ / _ \\| |\n";
        std::cout << " | |___| | | | (__| |  | |_| | |_) | |_| | (_) | | | |   | | (_) | (_) | |\n";
        std::cout << " |_____|_| |_|\\___|_|   \\__, | .__/ \\__|_|\\___/|_| |_|   |_|\\___/ \\___/|_|\n";
        std::cout << "                        |___/|_|                                                \n";
        std::cout << "\033[0m";  // Reset color
        std::cout << " \n                                                          Encryption Tool v2.0\n";
        std::cout << "\033[1;34m";  // Blue color
        std::cout << " \n                                         Created By Prince, Ashes, and Nishant\n";
        std::cout << "\033[0m";  // Reset color
        
    }
    
    static void displayLoadingAnimation(const std::string& message, int milliseconds) {
        std::cout << message;
        for (int i = 0; i < 3; i++) {
            std::cout << ".";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }
        std::cout << std::endl;
    }
};

// Password Manager add-on
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
    
    void saveToFile(const std::string& filename) {
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
    
    void loadFromFile(const std::string& filename) {
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

public:
    PasswordManager() {
        loadFromFile("password_database.txt");
    }
    
    ~PasswordManager() {
        saveToFile("password_database.txt");
    }
    
    void addPassword(const std::string& service, const std::string& username, 
                    const std::string& encryptedPassword, const std::string& algorithm, 
                    const std::string& key) {
        passwords.push_back({service, username, encryptedPassword, algorithm, key});
        saveToFile("password_database.txt");
    }
    
    void listPasswords() {
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
    
    bool getPassword(const std::string& service, std::string& encryptedPassword, 
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
    
    void deletePassword(const std::string& service) {
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
};

// Strength Analyzer
class PasswordStrengthAnalyzer {
public:
    static void analyzeStrength(const std::string& password) {
        int length = password.length();
        bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;
        
        for (char c : password) {
            if (std::islower(c)) hasLower = true;
            else if (std::isupper(c)) hasUpper = true;
            else if (std::isdigit(c)) hasDigit = true;
            else hasSpecial = true;
        }
        
        int strength = 0;
        if (length >= 8) strength++;
        if (length >= 12) strength++;
        if (hasLower) strength++;
        if (hasUpper) strength++;
        if (hasDigit) strength++;
        if (hasSpecial) strength++;
        
        std::cout << "\nPassword Strength Analysis:\n";
        std::cout << "Length: " << length << " characters\n";
        std::cout << "Contains lowercase letters: " << (hasLower ? "Yes" : "No") << "\n";
        std::cout << "Contains uppercase letters: " << (hasUpper ? "Yes" : "No") << "\n";
        std::cout << "Contains digits: " << (hasDigit ? "Yes" : "No") << "\n";
        std::cout << "Contains special characters: " << (hasSpecial ? "Yes" : "No") << "\n";
        
        std::cout << "Strength: ";
        switch (strength) {
            case 0:
            case 1:
                std::cout << "\033[1;31mVery Weak\033[0m";
                break;
            case 2:
                std::cout << "\033[1;31mWeak\033[0m";
                break;
            case 3:
                std::cout << "\033[1;33mModerate\033[0m";
                break;
            case 4:
                std::cout << "\033[1;32mStrong\033[0m";
                break;
            case 5:
            case 6:
                std::cout << "\033[1;32mVery Strong\033[0m";
                break;
        }
        std::cout << std::endl;
    }
    
    static std::string generateSecurePassword(int length = 12) {
        const std::string chars = 
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789"
            "!@#$%^&*()_+-=[]{}|:;<>,.?/";
        
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(0, chars.size() - 1);
        
        std::string password;
        for (int i = 0; i < length; ++i) {
            password += chars[distribution(generator)];
        }
        
        return password;
    }
};

class EncryptionApp {
private:
    std::vector<std::unique_ptr<CipherAlgorithm>> algorithms;
    PasswordManager passwordManager;
    
    void displayMenu() {
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
        algorithms.push_back(std::make_unique<MorseCodeCipher>());
        algorithms.push_back(std::make_unique<ROT13Cipher>());
        
        // Seed random number generator
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }
    
    void displayPasswordMenu() {
        std::cout << "\n==== Password Manager ====\n";
        std::cout << "1. Store a new password\n";
        std::cout << "2. Retrieve a password\n";
        std::cout << "3. List all stored passwords\n";
        std::cout << "4. Delete a password\n";
        std::cout << "5. Back to main menu\n";
        std::cout << "Enter your choice: ";
    }
    
    void passwordManagerMenu() {
        int choice;
        do {
            displayPasswordMenu();
            std::cin >> choice;
            std::cin.ignore(); // Clear the input buffer
            
            switch (choice) {
                case 1: {
                    std::string service, username, password;
                    std::cout << "Enter service name: ";
                    std::getline(std::cin, service);
                    std::cout << "Enter username: ";
                    std::getline(std::cin, username);
                    std::cout << "Enter password: ";
                    std::getline(std::cin, password);
                    
                    int algorithmIndex = selectAlgorithm();
                    
                    std::string key;
                    std::cout << algorithms[algorithmIndex]->getKeyInstructions() << "\nEnter key: ";
                    std::getline(std::cin, key);
                    algorithms[algorithmIndex]->setKey(key);
                    
                    std::string encryptedPassword = algorithms[algorithmIndex]->encrypt(password);
                    passwordManager.addPassword(service, username, encryptedPassword, 
                                              std::to_string(algorithmIndex), key);
                    
                    std::cout << "Password stored successfully!" << std::endl;
                    break;
                }
                case 2: {
                    std::string service;
                    std::cout << "Enter service name: ";
                    std::getline(std::cin, service);
                    
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
                    break;
                }
                case 3:
                    passwordManager.listPasswords();
                    break;
                case 4: {
                    std::string service;
                    std::cout << "Enter service name to delete: ";
                    std::getline(std::cin, service);
                    passwordManager.deletePassword(service);
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
    
    void analyzePasswordStrength() {
        std::string password;
        std::cout << "Enter password to analyze: ";
        std::getline(std::cin, password);
        
        PasswordStrengthAnalyzer::analyzeStrength(password);
    }
    
    void generateSecurePassword() {
        int length;
        std::cout << "Enter desired password length (8-30 recommended): ";
        std::cin >> length;
        std::cin.ignore(); // Clear the input buffer
        
        if (length < 1) length = 12; // Default length
        
        std::string password = PasswordStrengthAnalyzer::generateSecurePassword(length);
        
        std::cout << "\nGenerated secure password: " << password << std::endl;
        PasswordStrengthAnalyzer::analyzeStrength(password);
        
        std::cout << "\nWould you like to store this password? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear the input buffer
        
        if (choice == 'y' || choice == 'Y') {
            std::string service, username;
            std::cout << "Enter service name: ";
            std::getline(std::cin, service);
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            
            int algorithmIndex = selectAlgorithm();
            
            std::string key;
            std::cout << algorithms[algorithmIndex]->getKeyInstructions() << "\nEnter key: ";
            std::getline(std::cin, key);
            algorithms[algorithmIndex]->setKey(key);
            
            std::string encryptedPassword = algorithms[algorithmIndex]->encrypt(password);
            passwordManager.addPassword(service, username, encryptedPassword, 
                                      std::to_string(algorithmIndex), key);
            
            std::cout << "Password stored successfully!" << std::endl;
        }
    }
    
    void run() {
        ASCIIArtGenerator::displayLoadingAnimation("Initializing encryption tools", 500);
        
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
                    passwordManagerMenu(); // Password manager
                    break;
                case 6:
                    analyzePasswordStrength(); // Analyze password strength
                    break;
                case 7:
                    generateSecurePassword(); // Generate secure password
                    break;
                case 8:
                    std::cout << "Exiting program. Goodbye!\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 8);
    }
};

// Main function
int main() {
    EncryptionApp app;
    app.run();
    return 0;
}