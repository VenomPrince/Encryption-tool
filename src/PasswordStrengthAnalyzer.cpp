#include "include/PasswordStrengthAnalyzer.h"
#include <iostream>
#include <cctype>
#include <random>

void PasswordStrengthAnalyzer::analyzeStrength(const std::string& password) {
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

std::string PasswordStrengthAnalyzer::generateSecurePassword(int length) {
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