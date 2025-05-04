#include "include/CipherAlgorithm.h"
#include <iostream>

std::string CipherAlgorithm::encrypt(const std::string& plaintext) {
    return processText(plaintext, true);
}

std::string CipherAlgorithm::decrypt(const std::string& ciphertext) {
    return processText(ciphertext, false);
}

bool CipherAlgorithm::processFile(const std::string& inputFilename, const std::string& outputFilename, bool isEncryption) {
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