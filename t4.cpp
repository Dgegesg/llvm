#include <iostream>

using namespace std;

const int WIDTH = 20;  // Width of the screen (number of "pixels")
const int HEIGHT = 10; // Height of the screen (number of "pixels")

// Create a 2D grid to hold the pixel data
char grid[HEIGHT][WIDTH];

// Initialize the grid with dots (representing empty pixels)
void initGrid() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            grid[y][x] = '-';  // Set all pixels to be empty (represented by '.')
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

    // Main loop to handle keypresses
    bool running = true;
    int cursorX = 0, cursorY = 0;  // Initial cursor position
    char key;  // To store the user input
    while (running) {
        // Ask the user for input (with "Enter" required)
        cout << "Enter command (WASD to move, Space to draw, Q to quit): ";
        cin >> key;

        switch (key) {
            case 'w':  // Move cursor up
                if (cursorY > 0) cursorY--;
                break;
            case 's':  // Move cursor down
                if (cursorY < HEIGHT - 1) cursorY++;
                break;
            case 'a':  // Move cursor left
                if (cursorX > 0) cursorX--;
                break;
            case 'd':  // Move cursor right
                if (cursorX < WIDTH - 1) cursorX++;
                break;
            case 'q':  // Quit the program
                running = false;
                break;
            case ' ':  // Draw pixel at current cursor position
                drawPixel(cursorX, cursorY);
                break;
            default:
                cout << "Invalid command!" << endl;
                continue;
        }

        // Clear the screen and redraw the grid with updated pixels
        cout << "\033[2J";  // Clear the screen
        drawGrid();

        // Show the cursor position on the grid
        drawPixel(cursorX, cursorY, 'X');  // Mark the cursor with 'X'
        drawGrid();
    }

    return 0;
}
