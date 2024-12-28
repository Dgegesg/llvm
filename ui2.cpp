#include <iostream>
#include <unistd.h>
#include <sys/select.h>

using namespace std;

const char CURSOR_CHAR = 'X';
const char EMPTY_CHAR = ' ';

const int WIDTH = 40;
const int HEIGHT = 10;

void clearScreen() {
    cout << "\033[H\033[J"; // Clear screen
}

void renderGrid(int cursorX, int cursorY) {
    clearScreen();
    cout << "Artoriasphere\n";
    cout << string(WIDTH, '-') << "\n";

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == cursorX && y == cursorY) {
                cout << CURSOR_CHAR;
            } else {
                cout << EMPTY_CHAR;
            }
        }
        cout << '\n';
    }

    cout << string(WIDTH, '-') << "\n";
    cout << "Use Arrow Keys to move and 'q' to quit.\n";
}

int main() {
    int cursorX = WIDTH / 2;
    int cursorY = HEIGHT / 2;

    while (true) {
        renderGrid(cursorX, cursorY);

        // Set non-blocking input
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000; // 10ms

        int ready = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);

        if (ready == -1) {
            cerr << "Error in select().\n";
            break;
        } else if (ready > 0) {
            char input;
            read(STDIN_FILENO, &input, 1);

            if (input == 'q') {
                cout << "\nExiting...\n";
                break;
            } else if (input == 'w' && cursorY > 0) {
                cursorY--;
            } else if (input == 's' && cursorY < HEIGHT - 1) {
                cursorY++;
            } else if (input == 'a' && cursorX > 0) {
                cursorX--;
            } else if (input == 'd' && cursorX < WIDTH - 1) {
                cursorX++;
            }
        }
    }

    clearScreen();
    return 0;
}
