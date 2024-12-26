#include <iostream>
#include <cstdio>
#include <memory>
#include <array>

int main() {
    // Command to execute
    const char* cmd = "ifconfig";

    // Open a pipe to the process
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    
    if (!pipe) {
        std::cerr << "Error: Could not open pipe!" << std::endl;
        return 1;
    }

    // Buffer to hold data from the command output
    std::array<char, 128> buffer;
    
    // Read the output and print it to the console
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        std::cout << buffer.data();
    }

    return 0;
}
