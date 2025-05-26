# VenomPrince - Encryption Tool 

<p align="center">
  <img src="https://github.com/VenomPrince/Encryption-tool/blob/main/ET.png" width="300" height="120">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Version-3.0-brightgreen">
  <a href="https://github.com/VenomPrince">
    <img src="https://img.shields.io/github/followers/VenomPrince?label=Follow&style=social">
  </a>
  <a href="https://github.com/VenomPrince/Encryption-tool/stargazers">
    <img src="https://img.shields.io/github/stars/VenomPrince/Encryption-tool?style=social">
  </a>
  <img src="https://img.shields.io/badge/C++-14-blue">
  <img src="https://img.shields.io/badge/License-MIT-green">
</p>

<p align="center">
  <em>Your ultimate command-line toolkit for encryption</em> 
</p>

---

## Features

###  Encryption Algorithms
| Algorithm | Type | Key | Description |
|-----------|------|-----|-------------|
|  Caesar Cipher | Substitution | Number (1-25) | Shifts letters by fixed positions |
|  Vigenère Cipher | Polyalphabetic | Text keyword | Uses keyword for variable shifts |
|  Substitution Cipher | Alphabet mapping | Custom alphabet | Replaces each letter with another |
|  Morse Code | Encoding | Separator char | Converts text to Morse code |
|  ROT13 | Special Caesar | None | Rotates letters by 13 positions |

###  Password Manager
- Securely stores encrypted passwords
- Retrieves and decrypts passwords when needed
- Lists all stored credentials
- Delete password entries

### Additional Tools
-  Password Strength Analyzer
-  Secure Password Generator
-  File encryption/decryption
-  ASCII Art Interface

---

## Quick Start

### Prerequisites
- C++14 compiler (g++ recommended)
- Linux/macOS/Windows terminal

### Installation
```bash
git clone https://github.com/VenomPrince/Encryption-tool.git
cd Encryption-tool
g++ -std=c++14 -Iinclude src/*.cpp main.cpp -o EncryptionTool
```


Running the Tool
```bash
./EncryptionTool
```
 Usage Examples
Encrypting a Message (Caesar Cipher)

    Select "Encrypt a message"

    Choose Caesar Cipher

    Enter shift value (e.g., 3)

    Input your message: "ATTACK AT DAWN"

    Get ciphertext: "DWWDFN DW GDZQ"

Storing a Password

    Select "Password Manager"

    Choose "Store new password"

    Enter service: "GitHub"

    Enter username: "VenomPrince"

    Enter password: "MySecret123!"

    Select encryption algorithm

    Password stored securely!

Generating Secure Password

    Select "Generate secure password"

    Choose length (e.g., 16)

    Get generated password: "xQ3!kP8$mL2@nZ9#"

    Optionally store it in Password Manager

 Project Structure
```
Encryption-tool/
├── include/              # Header files
│   ├── *.h              # All algorithm headers
├── src/                 # Implementation
│   ├── *.cpp            # Algorithm implementations
├── main.cpp             # Main application
├── ET.png               # Project logo
└── README.md            # You are here :)
```
🤝 Contributing

We welcome contributions! Here's how:

    Fork the project

    Create your feature branch (git checkout -b cool-new-feature)

    Commit your changes (git commit -m 'Add awesome feature')

    Push to the branch (git push origin cool-new-feature)

    Open a Pull Request

 License

MIT License - See LICENSE for details.

<p align="center"> Made with ❤️ by <a href="https://github.com/VenomPrince">VenomPrince</a> and contributors </p> 
