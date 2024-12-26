#include <iostream>
#include <cstdlib>  // For system()

int main() {
    // Run the ls command to list directory contents
    int result = system("ls");

    // Check if the command was successful
    if (result != 0) {
        std::cerr << "Error executing ls command." << std::endl;
    }

    return 0;
}
