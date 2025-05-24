#include "PasswordStrengthAnalyzer.h"
#include <iostream>
#include <cctype>
#include <random>
#include <set>
#include <algorithm>

void PasswordStrengthAnalyzer::analyzeStrength(const std::string& password) {
    int length = password.length();
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;
    std::set<char> uniqueChars;
    int consecutiveCount = 0;
    int maxConsecutive = 0;
    
    // Check for character types and patterns
    for (size_t i = 0; i < password.length(); ++i) {
        char c = password[i];
        uniqueChars.insert(c);
        
        if (std::islower(c)) hasLower = true;
        else if (std::isupper(c)) hasUpper = true;
        else if (std::isdigit(c)) hasDigit = true;
        else hasSpecial = true;
        
        // Check for consecutive identical characters
        if (i > 0 && password[i] == password[i-1]) {
            consecutiveCount++;
            maxConsecutive = std::max(maxConsecutive, consecutiveCount + 1);
        } else {
            consecutiveCount = 0;
        }
    }
    
    // Check for common patterns
    bool hasSequentialChars = false;
    for (size_t i = 0; i < password.length() - 2; ++i) {
        if ((password[i] + 1 == password[i+1] && password[i+1] + 1 == password[i+2]) ||
            (password[i] - 1 == password[i+1] && password[i+1] - 1 == password[i+2])) {
            hasSequentialChars = true;
            break;
        }
    }
    
    // Check for common weak patterns
    std::string lowerPassword = password;
    std::transform(lowerPassword.begin(), lowerPassword.end(), lowerPassword.begin(), ::tolower);
    
    std::vector<std::string> commonPatterns = {
        "password", "123456", "qwerty", "abc123", "admin", "letmein", 
        "welcome", "monkey", "dragon", "master", "hello", "login"
    };
    
    bool hasCommonPattern = false;
    for (const auto& pattern : commonPatterns) {
        if (lowerPassword.find(pattern) != std::string::npos) {
            hasCommonPattern = true;
            break;
        }
    }
    
    // Calculate strength score
    int strength = 0;
    int maxStrength = 10;
    
    // Length scoring
    if (length >= 8) strength += 1;
    if (length >= 12) strength += 1;
    if (length >= 16) strength += 1;
    
    // Character variety scoring
    if (hasLower) strength += 1;
    if (hasUpper) strength += 1;
    if (hasDigit) strength += 1;
    if (hasSpecial) strength += 1;
    
    // Uniqueness bonus
    if (uniqueChars.size() >= password.length() * 0.8) strength += 1;
    
    // Pattern penalties
    if (maxConsecutive >= 3) strength -= 1;
    if (hasSequentialChars) strength -= 1;
    if (hasCommonPattern) strength -= 2;
    
    // Ensure strength is within bounds
    strength = std::max(0, std::min(strength, maxStrength));
    
    // Display analysis
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "\033[1;36m     PASSWORD STRENGTH ANALYSIS     \033[0m" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    std::cout << "\033[1;33mBasic Information:\033[0m" << std::endl;
    std::cout << "• Length: " << length << " characters" << std::endl;
    std::cout << "• Unique characters: " << uniqueChars.size() << "/" << length << std::endl;
    
    std::cout << "\n\033[1;33mCharacter Types:\033[0m" << std::endl;
    std::cout << "• Lowercase letters: " << (hasLower ? "\033[1;32m✓\033[0m" : "\033[1;31m✗\033[0m") << std::endl;
    std::cout << "• Uppercase letters: " << (hasUpper ? "\033[1;32m✓\033[0m" : "\033[1;31m✗\033[0m") << std::endl;
    std::cout << "• Numbers: " << (hasDigit ? "\033[1;32m✓\033[0m" : "\033[1;31m✗\033[0m") << std::endl;
    std::cout << "• Special characters: " << (hasSpecial ? "\033[1;32m✓\033[0m" : "\033[1;31m✗\033[0m") << std::endl;
    
    std::cout << "\n\033[1;33mSecurity Issues:\033[0m" << std::endl;
    if (maxConsecutive >= 3) {
        std::cout << "• \033[1;31m⚠\033[0m  Contains " << maxConsecutive << " consecutive identical characters" << std::endl;
    }
    if (hasSequentialChars) {
        std::cout << "• \033[1;31m⚠\033[0m  Contains sequential characters (abc, 123, etc.)" << std::endl;
    }
    if (hasCommonPattern) {
        std::cout << "• \033[1;31m⚠\033[0m  Contains common password patterns" << std::endl;
    }
    if (maxConsecutive < 3 && !hasSequentialChars && !hasCommonPattern) {
        std::cout << "• \033[1;32m✓\033[0m  No major security issues detected" << std::endl;
    }
    
    // Strength indicator with visual bar
    std::cout << "\n\033[1;33mOverall Strength: \033[0m";
    std::string strengthBar = "[";
    for (int i = 0; i < 10; ++i) {
        if (i < strength) {
            if (strength <= 3) strengthBar += "\033[1;31m█\033[0m";      // Red for weak
            else if (strength <= 6) strengthBar += "\033[1;33m█\033[0m"; // Yellow for moderate
            else strengthBar += "\033[1;32m█\033[0m";                     // Green for strong
        } else {
            strengthBar += "░";
        }
    }
    strengthBar += "] ";
    
    std::cout << strengthBar;
    
    // Strength label
    if (strength <= 3) {
        std::cout << "\033[1;31mWEAK\033[0m";
    } else if (strength <= 6) {
        std::cout << "\033[1;33mMODERATE\033[0m";
    } else if (strength <= 8) {
        std::cout << "\033[1;32mSTRONG\033[0m";
    } else {
        std::cout << "\033[1;32mVERY STRONG\033[0m";
    }
    
    std::cout << " (" << strength << "/10)" << std::endl;
    
    // Recommendations
    std::cout << "\n\033[1;33mRecommendations:\033[0m" << std::endl;
    if (length < 12) {
        std::cout << "• Make your password at least 12 characters long" << std::endl;
    }
    if (!hasLower || !hasUpper || !hasDigit || !hasSpecial) {
        std::cout << "• Include a mix of uppercase, lowercase, numbers, and symbols" << std::endl;
    }
    if (maxConsecutive >= 3) {
        std::cout << "• Avoid repeating the same character multiple times" << std::endl;
    }
    if (hasSequentialChars) {
        std::cout << "• Avoid sequential characters like 'abc' or '123'" << std::endl;
    }
    if (hasCommonPattern) {
        std::cout << "• Avoid common words and predictable patterns" << std::endl;
    }
    if (strength >= 8) {
        std::cout << "• \033[1;32mExcellent! Your password meets security best practices\033[0m" << std::endl;
    }
    
    std::cout << std::string(50, '=') << std::endl;
}

std::string PasswordStrengthAnalyzer::generateSecurePassword(int length) {
    // Ensure minimum length for security
    if (length < 8) length = 12;
    if (length > 50) length = 50;
    
    // Character sets for different types
    const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
    const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string numbers = "0123456789";
    const std::string symbols = "!@#$%^&*()_+-=[]{}|:;<>,.?/~";
    
    // Combine all character sets
    const std::string allChars = lowercase + uppercase + numbers + symbols;
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, allChars.size() - 1);
    
    std::string password;
    
    // Ensure at least one character from each type
    std::uniform_int_distribution<int> lowerDist(0, lowercase.size() - 1);
    std::uniform_int_distribution<int> upperDist(0, uppercase.size() - 1);
    std::uniform_int_distribution<int> numberDist(0, numbers.size() - 1);
    std::uniform_int_distribution<int> symbolDist(0, symbols.size() - 1);
    
    password += lowercase[lowerDist(generator)];
    password += uppercase[upperDist(generator)];
    password += numbers[numberDist(generator)];
    password += symbols[symbolDist(generator)];
    
    // Fill the rest randomly
    for (int i = 4; i < length; ++i) {
        char nextChar;
        do {
            nextChar = allChars[distribution(generator)];
            // Avoid consecutive identical characters
        } while (!password.empty() && nextChar == password.back());
        
        password += nextChar;
    }
    
    // Shuffle the password to avoid predictable patterns
    std::shuffle(password.begin(), password.end(), generator);
    
    return password;
}