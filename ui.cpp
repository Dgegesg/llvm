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

// Function to draw the grid in the terminal with rounded corners, edges, and cursor
void drawGrid(int cursorX, int cursorY) {
    // Clear the terminal screen (platform-specific)
    #ifdef _WIN32
        system("cls"); // Windows command to clear the screen
    #else
        system("clear"); // Unix/Linux/Mac command to clear the screen
    #endif
    
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // Draw rounded corners
            if ((x == 0 && y == 0)) {
                cout << "\033[47m◜\033[0m";  // Top-left rounded corner
            } else if ((x == WIDTH - 1 && y == 0)) {
                cout << "\033[47m◝\033[0m";  // Top-right rounded corner
            } else if ((x == 0 && y == HEIGHT - 1)) {
                cout << "\033[47m◞\033[0m";  // Bottom-left rounded corner
            } else if ((x == WIDTH - 1 && y == HEIGHT - 1)) {
                cout << "\033[47m◟\033[0m";  // Bottom-right rounded corner
            }
            // Draw horizontal edges (excluding corners)
            else if (y == 0 || y == HEIGHT - 1) {
                cout << "\033[47m─\033[0m";  // Top and bottom edges (horizontal)
            }
            // Draw vertical edges (excluding corners)
            else if (x == 0 || x == WIDTH - 1) {
                cout << "\033[47m│\033[0m";  // Left and right edges (vertical)
            }
            // Draw the cursor at the current position
            else if (x == cursorX && y == cursorY) {
                cout << "\033[31m\033[47mX\033[0m";  // Red text with white background for the cursor
            }
            // Draw the drawn pixels in the grid
            else if (grid[y][x] == '*') {
                cout << "\033[37m*\033[0m";  // Draw pixel in white
            } else {
                // Make the grid "invisible" but keep the white background
                cout << "\033[47m \033[0m";  // Invisible space with white background
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
    
    int cursorX = 1, cursorY = 1;  // Start cursor position at the top-left corner (inside the border)
    char input;  // To store user input

    while (true) {
        // Draw the grid with the cursor in the correct position
        drawGrid(cursorX, cursorY);
        
        cout << "Use WASD to move, Space to draw, Q to quit: ";
        cin >> input;

        switch (input) {
            case 'w':  // Move cursor up
                if (cursorY > 1) cursorY--;  // Prevent cursor from going outside the top border
                break;
            case 's':  // Move cursor down
                if (cursorY < HEIGHT - 2) cursorY++;  // Prevent cursor from going outside the bottom border
                break;
            case 'a':  // Move cursor left
                if (cursorX > 1) cursorX--;  // Prevent cursor from going outside the left border
                break;
            case 'd':  // Move cursor right
                if (cursorX < WIDTH - 2) cursorX++;  // Prevent cursor from going outside the right border
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
