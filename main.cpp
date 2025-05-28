#include "include/EncryptionApp.h"
#include "include/ROT13Cipher.h"

int main() {
    EncryptionApp app;
    app.run();

    ROT13Cipher* cipher = new ROT13Cipher();
    std::string result = cipher->processText("Hello", true);

    delete cipher;

    return 0;
}