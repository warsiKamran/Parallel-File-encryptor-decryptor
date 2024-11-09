# Parallel File Encryptor and Decryptor

This project is a high-performance file encryptor and decryptor implemented in C++. It leverages multithreading to perform encryption and decryption operations in parallel, allowing for significantly faster processing on large files. 

## Features
- **Parallel Processing**: Utilizes multithreading to handle file encryption and decryption in multiple threads simultaneously, improving performance on large files.
- **Configurable Encryption Method**: Easily extendable to incorporate various encryption algorithms.
- **File Integrity**: Ensures encrypted files can be decrypted back to their original state without data loss.

## Getting Started

### Prerequisites
- **Operating System**: Compatible with Linux, macOS, and Windows.
- **Compiler**: Requires a C++ compiler supporting C++11 or higher (e.g., GCC or Clang).
- **Multithreading Libraries**: Uses `pthread` for multithreading support (included by default in most environments).

### Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/warsiKamran/Parallel-File-encryptor-decryptor.git
   ```

2. **Compile the Project**
   ```bash
   cd ParallelFileEncryptorDecryptor
   make clean
   make
   ./encrypt_decrypt
   Note: Give the path as test
   ```
   
## How It Works

The program splits the input file into chunks, each processed by a separate thread. Using multithreading, each chunk is encrypted or decrypted concurrently, making the program efficient and suitable for large files.

### Threading Model
- **Thread Creation**: The program spawns a user-defined number of threads based on system resources and file size.
- **Synchronization**: The threads are synchronized to ensure data consistency during encryption and decryption.
- **Error Handling**: If any thread encounters an error, the program safely terminates, ensuring file integrity.

### Encryption Algorithm
The project can be adapted to use various encryption algorithms (e.g., AES, XOR).

## Limitations
- The encryption and decryption speed may vary depending on the file size and the number of threads.
- Designed primarily for text files; binary files may require adjustments.

## Contributing
Feel free to contribute by forking the repository and submitting a pull request. Suggestions and bug reports are also welcome!
