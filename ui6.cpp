#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

char getKeyPress() {
    char ch;
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    // Wait for input with a timeout of 0 seconds
    int ret = select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout);

    if (ret > 0) {
        read(STDIN_FILENO, &ch, 1);
    } else {
        ch = 0;  // No input detected
    }

    return ch;
}

int main() {
    std::cout << "Press 'q' or 'ESC' to exit..." << std::endl;

    while (true) {
        char ch = getKeyPress();

        if (ch) {
            if (ch == 'q') {
                std::cout << "You pressed 'q'. Exiting..." << std::endl;
                break;
            } else if (ch == 27) {  // Escape key
                std::cout << "You pressed 'Escape'. Exiting..." << std::endl;
                break;
            }
        }
    }

    return 0;
}
