#include <iostream>
#include <chrono>
#include <cstdlib>  // for system("clear")
#include <string>
#include <vector>

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 10; // Height of the screen
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

    // Renders the grid, now building it in memory to minimize flicker
    string render(int cursorX, int cursorY, int selectedMenuOption, const vector<string>& menuOptions) const {
        string screen = ""; // Empty string to accumulate grid content

        // Add the title at the top (above the grid)
        screen += "\033[1m" + TITLE + "\033[0m\n";  // Bold title
        
        // Render grid border and inside
        for (int y = 0; y < height - 3; ++y) {  // Leave space for the menu at the bottom
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
                // Draw the cursor inside the grid
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

        // Render the menu at the bottom with buttons
        for (int i = 0; i < menuOptions.size(); ++i) {
            // Highlight the selected option (the button where the cursor is)
            if (i == selectedMenuOption) {
                screen += BUTTON_BG_COLOR + BUTTON_TEXT_COLOR + "  " + menuOptions[i] + "  \033[0m\n"; // White button with black text
            } else {
                screen += "  " + menuOptions[i] + "  \n"; // Regular menu options
            }
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
    UI(int width, int height) : grid(width, height), cursorX(1), cursorY(1), selectedMenuOption(0) {
        // Initialize the menu options
        menuOptions.push_back("Start Game");
        menuOptions.push_back("Settings");
        menuOptions.push_back("Exit");
    }

    void run() {
        grid.clear(); // Clear screen at the start
        showLoadingAnimation(); // Show loading animation before UI starts

        while (true) {
            // Build the UI screen in memory to minimize flickering
            string screen = grid.render(cursorX, cursorY, selectedMenuOption, menuOptions);

            // Move the cursor to the top left of the screen, then output the content
            cout << "\033[H"; // Move cursor to top left
            cout << screen; // Output the built screen at once

            char input;
            cin >> input;

            switch (input) {
                case 'w':  // Move cursor up
                    if (selectedMenuOption > 0) selectedMenuOption--;
                    break;
                case 's':  // Move cursor down
                    if (selectedMenuOption < menuOptions.size() - 1) selectedMenuOption++;
                    break;
                case 'q':  // Quit the program
                    quitAnimation();  // Start the quit animation
                    return;
                case '\n':  // Enter key to select menu option (when the cursor is over a button)
                    handleMenuSelection();
                    break;
                default:
                    break;
            }

            customSleep(100); // Delay for 100 milliseconds
        }
    }

private:
    Grid grid;
    int cursorX, cursorY;
    int selectedMenuOption;
    vector<string> menuOptions;

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

    // Handle menu selection based on the selected option
    void handleMenuSelection() {
        switch (selectedMenuOption) {
            case 0:  // Start Game
                cout << "\033[HStarting game...\n"; // Add more functionality here for starting the game
                customSleep(500);
                break;
            case 1:  // Settings
                cout << "\033[HOpening settings...\n"; // Add more functionality here for settings
                customSleep(500);
                break;
            case 2:  // Exit
                quitAnimation();
                return;
            default:
                break;
        }
    }

    // Quit animation to simulate smooth quitting
    void quitAnimation() {
        for (int i = 0; i < 5; ++i) {
            cout << "\033[HQuitting" << flush; // Display quit message
            customSleep(500);  // Delay for half a second
            cout << "\033[H" << string(10, ' ') << "\033[H"; // Clear the message
            customSleep(500);  // Delay for half a second
        }
        cout << "\033[HExiting... \033[0m\n"; // Final exit message
    }
};

// Main function to run the UI
int main() {
    UI ui(WIDTH, HEIGHT);
    ui.run();
    return 0;
}
