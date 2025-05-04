#ifndef PASSWORDSTRENGTHANALYZER_H
#define PASSWORDSTRENGTHANALYZER_H

#include <string>
#include <random>

class PasswordStrengthAnalyzer {
public:
    static void analyzeStrength(const std::string& password);
    static std::string generateSecurePassword(int length = 12);
};

#endif // PASSWORDSTRENGTHANALYZER_H