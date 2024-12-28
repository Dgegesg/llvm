#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;

// Function to set terminal to raw mode
void setRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to restore terminal to default mode
void restoreMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // Enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int main() {
    cout << "Press 'q' to quit.\n";
    
    // Set terminal to raw mode
    setRawMode();

    char input;
    while (true) {
        input = getchar(); // Read a single character

        if (input == 'q') { // Quit on 'q'
            cout << "\nExiting program.\n";
            break;
        }

        cout << "You pressed: " << input << '\n';
    }

    // Restore terminal mode before exiting
    restoreMode();
    return 0;
}
