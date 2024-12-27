#include <iostream>
#include <chrono>
#include <cstdlib>  // For clearing the screen manually
#include <vector>
#include <string>

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 20; // Height of the screen
const char EMPTY_CHAR = ' '; // Default empty space in grid
const char CURSOR_CHAR = 'X'; // Cursor character
const string TITLE = "V18 August UI"; // The title for the UI

// ANSI color codes
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background for empty space (can be customized)
const string BUTTON_BG_COLOR = "\033[47m"; // White background for buttons
const string BUTTON_TEXT_COLOR = "\033[30m"; // Black text color for buttons

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

    // Function to render the grid with a cursor and menu
    string render(int cursorX, int cursorY, const vector<string>& menuItems, int menuIndex) const {
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
                // Draw the menu buttons
                else if (x >= (width / 2) - 10 && x < (width / 2) + 10 &&
                         y >= (height / 2) - menuItems.size() / 2 && y < (height / 2) + menuItems.size() / 2) {
                    int index = y - (height / 2) + menuItems.size() / 2;
                    if (index >= 0 && index < menuItems.size()) {
                        if (x == (width / 2) - 10) {
                            // Left border of the button
                            screen += BUTTON_BG_COLOR + " " + "\033[0m";
                        } else if (x == (width / 2) + 9) {
                            // Right border of the button
                            screen += BUTTON_BG_COLOR + " " + "\033[0m";
                        } else if (index == menuIndex) {
                            // Highlight the selected menu option
                            screen += "\033[31m" + CURSOR_CHAR + "\033[0m"; // Red cursor ('X') on selected menu item
                        } else {
                            // Regular menu item
                            screen += BUTTON_BG_COLOR + " " + "\033[0m";
                        }
                    }
                }
                else {
                    screen += EMPTY_SPACE_COLOR + " \033[0m"; // Empty space with background color
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
    UI(int width, int height) : grid(width, height), cursorX(1), cursorY(1), menuIndex(0) {}

    void run() {
        grid.clear(); // Clear screen at the start
        showLoadingAnimation(); // Show loading animation before UI starts

        vector<string> menuItems = {"Option 1", "Option 2", "Option 3", "Exit"};
        char input;
        
        while (true) {
            // Build the UI screen in memory to minimize flickering
            string screen = grid.render(cursorX, cursorY, menuItems, menuIndex);

            // Move the cursor to the top left of the screen, then output the content
            cout << "\033[H"; // Move cursor to top left
            cout << screen; // Output the built screen at once

            cout << "Use arrow keys to move cursor, Enter to select: ";
            cin >> input;

            switch (input) {
                case 'w':  // Move cursor up
                    if (menuIndex > 0) menuIndex--;
                    break;
                case 's':  // Move cursor down
                    if (menuIndex < menuItems.size() - 1) menuIndex++;
                    break;
                case 13:  // Enter key pressed (to simulate menu selection)
                    if (menuIndex == menuItems.size() - 1) {
                        cout << "Exiting..." << endl;
                        return;
                    } else {
                        cout << "You selected: " << menuItems[menuIndex] << endl;
                    }
                    break;
                case 'q':  // Quit the program
                    cout << "Exiting..." << endl;
                    return;
                default:
                    break;
            }

            customSleep(100); // Delay for 100 milliseconds to simulate smooth UI
        }
    }

private:
    Grid grid;
    int cursorX, cursorY;
    int menuIndex;

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
                customSleep(100); // Delay for 500 milliseconds
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
