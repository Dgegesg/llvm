#include <iostream>
#include <unistd.h> // For sleep function

using namespace std;

const int WIDTH = 20;  // Width of the screen (number of "pixels")
const int HEIGHT = 10; // Height of the screen (number of "pixels")

// Create a 2D grid to hold the pixel data
char grid[HEIGHT][WIDTH];

// Initialize the grid with dots (representing empty pixels)
void initGrid() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            grid[y][x] = '.';  // Set all pixels to be empty (represented by '.')
        }
    }
}

// Function to draw a pixel on the grid
void drawPixel(int x, int y, char symbol = '*') {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        grid[y][x] = symbol;  // Update the grid with the symbol at the position
    }
}

// Function to draw the grid to the console
void drawGrid() {
    // Move the cursor to the top-left of the terminal
    cout << "\033[H";  // ANSI escape sequence to move cursor to (1, 1)

    // Output the grid to the console
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            cout << grid[y][x];  // Output the current pixel (either '.' or the symbol)
        }
        cout << endl;
    }
}

int main() {
    // Initialize the grid
    initGrid();

    // Clear the screen at the start
    cout << "\033[2J";  // ANSI escape sequence to clear screen

    // Draw the initial grid
    drawGrid();

    // Simulate drawing pixels by replacing a few dots with a character
    drawPixel(5, 3);  // Draw a pixel at (5, 3)
    drawPixel(10, 4); // Draw a pixel at (10, 4)
    drawPixel(15, 2); // Draw a pixel at (15, 2)

    // Redraw the grid with the updated pixels
    drawGrid();

    // Pause to view the output
    sleep(3);

    // Clear the screen after the pause
    cout << "\033[2J";  // ANSI escape sequence to clear screen

    return 0;
}
