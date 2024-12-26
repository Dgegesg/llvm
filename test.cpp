#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    // Define the path to list the contents from
    fs::path p = fs::current_path();  // or specify any other path
    
    // Try to list the directory contents
    try {
        // Check if the path is a directory
        if (fs::is_directory(p)) {
            std::cout << "Listing contents of directory: " << p << std::endl;
            
            for (const auto& entry : fs::directory_iterator(p)) {
                std::cout << entry.path().filename() << std::endl;
            }
        } else {
            std::cerr << p << " is not a valid directory." << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error accessing the filesystem: " << e.what() << std::endl;
    }
    
    return 0;
}
