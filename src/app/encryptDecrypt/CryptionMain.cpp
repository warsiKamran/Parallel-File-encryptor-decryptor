#include <iostream>
#include "Cryption.hpp"

//argc: This parameter holds the count of command-line arguments passed to the program. argc will be 2 if the user provides one argument.

// argv: This is an array of C-strings (character pointers) that holds each command-line argument. argv[0] is the program name ("./cryption"), and argv[1] is the task_data argument.

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: ./cryption <task_data>" << std::endl;
        return 1;
    }

    executeCryption(argv[1]);

    return 0;
}

