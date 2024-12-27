#include <iostream>
#include <cstdlib>  // for system("clear")
#include <chrono>   // for timing functions

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 10; // Height of the screen
const char EMPTY_CHAR = ' '; // Default empty space in grid
const char CURSOR_CHAR = 'X'; // Cursor character

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

    void draw(int cursorX, int cursorY) const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Draw grid borders and corners
                if ((x == 0 && y == 0) || (x == width - 1 && y == 0) || 
                    (x == 0 && y == height - 1) || (x == width - 1 && y == height - 1)) {
                    cout << "+"; // Corners
                }
                else if (y == 0 || y == height - 1) {
                    cout << "-"; // Top and bottom edges
                }
                else if (x == 0 || x == width - 1) {
                    cout << "|"; // Left and right edges
                }
                // Draw the cursor
                else if (x == cursorX && y == cursorY) {
                    cout << "\033[31m\033[47m" << CURSOR_CHAR << "\033[0m"; // Red cursor
                }
                // Draw the pixels (optional, can be used to mark user actions)
                else if (grid[y][x] == '*') {
                    cout << "\033[37m*\033[0m"; // White draw pixel
                } else {
                    cout << " "; // Empty space
                }
            }
            cout << endl;
        }
    }

    void clear() {
        system("clear"); // Linux only, clears the screen
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
        char input;
        while (true) {
            grid.clear();  // Clear the screen before each draw
            grid.draw(cursorX, cursorY);  // Draw the grid with the cursor

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

    void customSleep(int milliseconds) {
        auto start = chrono::steady_clock::now();
        while (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() < milliseconds) {
            // Busy-wait loop to create delay
        }
    }
};

int main() {
    UI ui(WIDTH, HEIGHT); // Create UI with specific grid size
    ui.run();  // Start the user interface
    return 0;
}
