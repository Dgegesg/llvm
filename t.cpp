#include <iostream>
#include <unistd.h> // For sleep function

using namespace std;

const int WIDTH = 20;  // Width of the screen (number of "pixels")
const int HEIGHT = 10; // Height of the screen (number of "pixels")

void drawPixel(int x, int y, char symbol = '*') {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        // Move the cursor to position (x, y)
        cout << "\033[" << y + 1 << ";" << x + 1 << "H" << symbol;
    }
}

void clearScreen() {
    cout << "\033[2J\033[1;1H"; // ANSI escape sequence to clear screen
}

void drawGrid() {
    // Draw a simple grid of "pixels" (represented by dots)
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            cout << ".";
        }
        cout << endl;
    }
}

int main() {
    // Clear the screen at the start
    clearScreen();
    
    // Draw an initial grid
    drawGrid();

    // Simulate drawing pixels by replacing a few dots with a character
    drawPixel(5, 3);  // Draw a pixel at (5, 3)
    drawPixel(10, 4); // Draw a pixel at (10, 4)
    drawPixel(15, 2); // Draw a pixel at (15, 2)

    // Pause to view the output
    sleep(3);
    
    // Clear the screen after the pause
    clearScreen();

    return 0;
}
