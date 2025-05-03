
   VenomPrince - Encryption-tool 
<p align="center"> <img src="https://github.com/VenomPrince/Encryption-tool/blob/main/ET.png" width="300" height="120"> </p> <p align="center"> <img src="https://img.shields.io/badge/Version-2.0-brightgreen"> <a href="https://github.com/VenomPrince"> <img src="https://img.shields.io/github/followers/th3unkn0n?label=Follow&style=social"> </a> <a href="https://github.com/VenomPrince/Encryption-tool/stargazers"> <img src="https://img.shields.io/github/stars/VenomPrince/Encryption-tool?style=social"> </a> </p> <p align="center"> Encryption-tool </p>

Features

A simple and efficient command-line tool for encrypting and decrypting text using various encryption algorithms.
## Features

- **Multiple Encryption Algorithms**: Supports Caesar Cipher and Vigenère Cipher.
- **User-Friendly Interface**: Simple command-line interface for ease of use.
- **Customizable Options**: Allows users to specify keys and input text for encryption or decryption.

## Algorithms

### Caesar Cipher
A substitution cipher that shifts characters in the plaintext by a fixed number of positions in the alphabet.

### Vigenère Cipher
A polyalphabetic substitution cipher that uses a keyword to determine the shift for each character in the plaintext.

## Prerequisites

Ensure you have the following installed on your system:
- A C++ compiler (e.g., `g++`).
- A terminal or command prompt.

## Usage

### Build

To build the project, navigate to the project directory and run the following command:

```bash
g++ -o encryption-tool Encryption-tool.cpp
```

This will generate an executable file named `encryption-tool`.

### Run

To use the tool, execute the following command:

```bash
./encryption-tool
```

Follow the on-screen prompts to select an encryption algorithm, provide the necessary inputs, and perform encryption or decryption.

### Example

#### Encrypting with Caesar Cipher

1. Select the Caesar Cipher option.
2. Enter the plaintext (e.g., `HELLO`).
3. Provide the shift value (e.g., `3`).
4. The tool will output the encrypted text (e.g., `KHOOR`).

#### Decrypting with Vigenère Cipher

1. Select the Vigenère Cipher option.
2. Enter the ciphertext (e.g., `RIJVS`).
3. Provide the keyword (e.g., `KEY`).
4. The tool will output the decrypted text (e.g., `HELLO`).

## File Structure

```
Encryption-tool/
├── Encryption-tool.cpp  # Main source code
├── README.md            # Documentation
```

## Contributing

Contributions are welcome! Feel free to submit issues or pull requests to improve the tool.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments

Special thanks to the open-source community for inspiration and resources.

