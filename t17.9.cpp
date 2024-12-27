#include <iostream>
#include <cstdlib>  // for system("clear")
#include <chrono>   // for timing functions

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 10; // Height of the screen
const char EMPTY_CHAR = ' '; // Default empty space in grid
const char CURSOR_CHAR = 'X'; // Cursor character
const string TITLE = "My Custom UI"; // The title for the UI

// ANSI color codes
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background for empty space (change this to any color)


// Grid class to encapsulate the terminal grid and its drawing
class Grid {
public:
    Grid(int width, int height) : width(width), height(height) {
        grid = new char*[height];
        for (int i = 0; i < height; ++i) {
            grid[i] = new char[width];
        }
        initGrid();
    }

    ~Grid() {
        for (int i = 0; i < height; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
    }

    void initGrid() {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                grid[y][x] = EMPTY_CHAR; // Initialize grid with empty spaces
            }
        }
    }

    // Renders the grid, now building it in memory to minimize flicker
    string render(int cursorX, int cursorY) const {
        string screen = ""; // Empty string to accumulate grid content

        // Add the title at the top (above the grid)
        screen += "\033[1m" + TITLE + "\033[0m\n";  // Bold title
        
        // Render grid border and inside
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Draw grid corners with bold formatting
                if ((x == 0 && y == 0) || (x == width - 1 && y == 0) || 
                    (x == 0 && y == height - 1) || (x == width - 1 && y == height - 1)) {
                    screen += "\033[1m+\033[0m"; // Bold corners
                }
                // Bold horizontal edges
                else if (y == 0 || y == height - 1) {
                    screen += "\033[1m-\033[0m"; // Bold top and bottom edges
                }
                // Bold vertical edges
                else if (x == 0 || x == width - 1) {
                    screen += "\033[1m|\033[0m"; // Bold left and right edges
                }
                // Draw the cursor
                else if (x == cursorX && y == cursorY) {
                    screen += "\033[31m\033[47m" + string(1, CURSOR_CHAR) + "\033[0m"; // Red cursor with white background
                }
                // Draw the pixels (optional, can be used to mark user actions)
                else if (grid[y][x] == '*') {
                    screen += "\033[37m*\033[0m"; // White draw pixel
                } else {
                    // Apply color to the empty space
                    screen += EMPTY_SPACE_COLOR + " \033[0m"; // Apply color to empty space
                }
            }
            screen += "\n"; // Add a new line after each row of the grid
        }
        return screen; // Return the built string of the UI
    }

    // This method only clears the screen at the beginning (to avoid flicker during the loop)
    void clear() const {
        cout << "\033[H\033[J"; // ANSI escape sequence to clear the screen (and move cursor to top-left)
    }

private:
    int width, height;
    char** grid;
};

// UI class to handle user input and cursor movements
class UI {
public:
    UI(int width, int height) : grid(width, height), cursorX(1), cursorY(1) {}

    void run() {
        grid.clear(); // Clear screen at the start
        showLoadingAnimation(); // Show loading animation before UI starts

        char input;
        while (true) {
            // Build the UI screen in memory to minimize flickering
            string screen = grid.render(cursorX, cursorY);

            // Move the cursor to the top left of the screen, then output the content
            cout << "\033[H"; // Move cursor to top left
            cout << screen; // Output the built screen at once

            cout << "Use WASD to move, Q to quit: ";
            cin >> input;

            switch (input) {
                case 'w':  // Move cursor up
                    if (cursorY > 1) cursorY--;
                    break;
                case 's':  // Move cursor down
                    if (cursorY < HEIGHT - 2) cursorY++;
                    break;
                case 'a':  // Move cursor left
                    if (cursorX > 1) cursorX--;
                    break;
                case 'd':  // Move cursor right
                    if (cursorX < WIDTH - 2) cursorX++;
                    break;
                case 'q':  // Quit the program
                    cout << "Exiting..." << endl;
                    return;
                default:
                    cout << "Invalid input!" << endl;
                    break;
            }

            customSleep(100); // Delay for 100 milliseconds
        }
    }

private:
    Grid grid;
    int cursorX, cursorY;

    // Custom sleep to avoid overloading CPU with tight loops
    void customSleep(int milliseconds) {
        auto start = chrono::steady_clock::now();
        while (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() < milliseconds) {
            // Empty loop to simulate sleep
        }
    }

    // Show loading animation before the UI starts
    void showLoadingAnimation() {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 4; ++j) {
                string dots = "";
                for (int dotCount = 0; dotCount < j; ++dotCount) {
                    dots += ".";
                }
                // Display the loading dots without clearing the screen
                cout << "\033[HLoading" + dots << flush;
                customSleep(500); // Delay for 500 milliseconds
            }
        }
    }
};

// Main function to run the UI
int main() {
    UI ui(WIDTH, HEIGHT);
    ui.run();
    return 0;
}
