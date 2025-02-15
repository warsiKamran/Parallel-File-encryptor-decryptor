## Prallel File Encrytor and Decryptor

Encypt and decryot your files parallely in seconds.

## Getting Started
- Requires C++ compiler supporting c++11 or higher.
- Uses `pthread`for multithreading support.

## Installation and Setup

1. Clone the repository. 
```bash
https://github.com/warsiKamran/Parallel-File-encryptor-decryptor.git
```
2. Compile the Project.
```bash
g++ -std=c++11 -pthread -o encryptor_decryptor main.cpp
```
## Usage
To encrypt and decrypt the files, use the following commands.

1. Encrypt a File
```bash
./encryptor_decryptor encrypt <input_file> <output_file>
```
2. Decrypt a File
```bash
./encryptor_decryptor decrypt <input_file> <output_file>
```

