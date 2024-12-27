#include <iostream>
#include <cstdlib> // for system("clear") or system("cls")

using namespace std;

const int WIDTH = 20;  // Width of the screen (number of "pixels")
const int HEIGHT = 10; // Height of the screen (number of "pixels")
char grid[HEIGHT][WIDTH]; // The pixel grid

// Function to initialize the grid with empty spaces
void initGrid() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            grid[y][x] = ' ';  // Initialize all pixels as empty (using space)
        }
    }
}

// Function to draw the grid in the terminal
void drawGrid(int cursorX, int cursorY) {
    // Clear the terminal screen (platform-specific)
    #ifdef _WIN32
        system("cls"); // Windows command to clear the screen
    #else
        system("clear"); // Unix/Linux/Mac command to clear the screen
    #endif
    
    // Draw the grid
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == cursorX && y == cursorY) {
                cout << "\033[37mX\033[0m";  // Display the cursor at the current position in white
            } else {
                if (grid[y][x] == '*') {
                    cout << "\033[37m*\033[0m";  // Display the drawn pixel in white
                } else {
                    cout << "█";  // Display the empty space as a filled square
                }
            }
        }
        cout << endl;
    }
}

// Function to draw a pixel at the current cursor position
void drawPixel(int cursorX, int cursorY) {
    if (cursorX >= 0 && cursorX < WIDTH && cursorY >= 0 && cursorY < HEIGHT) {
        grid[cursorY][cursorX] = '*';  // Set the current grid position to a drawn pixel
    }
}

// Main function
int main() {
    // Initialize the grid with empty pixels
    initGrid();
    
    int cursorX = 0, cursorY = 0;  // Start cursor position at the top-left corner
    char input;  // To store user input

    while (true) {
        // Draw the grid with the cursor in the correct position
        drawGrid(cursorX, cursorY);
        
        cout << "Use WASD to move, Space to draw, Q to quit: ";
        cin >> input;

        switch (input) {
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
            case ' ':  // Draw pixel at current cursor position
                drawPixel(cursorX, cursorY);
                break;
            case 'q':  // Quit the program
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid input!" << endl;
                break;
        }
    }

    return 0;
}
