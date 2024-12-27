#include <iostream>
#include <chrono>
#include <cstdlib>  // for system("clear")
#include <vector>
#include <string>

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 10; // Height of the screen
const char EMPTY_CHAR = ' '; // Default empty space in grid
const char CURSOR_CHAR = 'X'; // Cursor character
const string TITLE = "My Custom UI"; // The title for the UI

// ANSI color codes
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background for empty space
const string BUTTON_COLOR = "\033[42m"; // Green background for the button (when cursor is over it)
const string BUTTON_NORMAL_COLOR = "\033[47m"; // Normal button color when cursor is not over it

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
    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<pair<int, int>>& buttonPositions) const {
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
                } else {
                    bool buttonFound = false;
                    // Check if we are on a button
                    for (size_t i = 0; i < buttonPositions.size(); ++i) {
                        int buttonX = buttonPositions[i].second;
                        int buttonY = buttonPositions[i].first;
                        int buttonWidth = buttons[i].length() + 2; // Button width (padding around it)

                        // Check if cursor is within the bounds of the button (button area)
                        if (y == buttonY && x >= buttonX && x < buttonX + buttonWidth) {
                            buttonFound = true;

                            // Highlight button when the cursor is over it
                            if (cursorX == x && cursorY == y) {
                                screen += BUTTON_COLOR + " " + buttons[i] + " \033[0m"; // Active button
                            } else {
                                screen += BUTTON_NORMAL_COLOR + " " + buttons[i] + " \033[0m"; // Normal button
                            }
                            break;
                        }
                    }
                    if (!buttonFound) {
                        // Default empty space
                        screen += EMPTY_SPACE_COLOR + " \033[0m"; // Apply color to empty space
                    }
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

    void addButton(string label, int row, int col) {
        // Check for existing button at the given position (to avoid duplicates)
        pair<int, int> buttonPos = {row, col};
        
        // Ensure button is within the bounds of the grid (not on the border)
        if (row > 0 && row < HEIGHT - 1 && col > 0 && col < WIDTH - 1) {
            buttons.push_back(label);
            buttonPositions.push_back(buttonPos);  // Track added position
        } else {
            cout << "Button '" << label << "' cannot be placed on the border!" << endl;
        }
    }

    void run() {
        grid.clear(); // Clear screen at the start
        showLoadingAnimation(); // Show loading animation before UI starts

        char input;
        while (true) {
            // Build the UI screen in memory to minimize flickering
            string screen = grid.render(cursorX, cursorY, buttons, buttonPositions);

            // Move the cursor to the top left of the screen, then output the content
            cout << "\033[H"; // Move cursor to top left
            cout << screen; // Output the built screen at once

            cout << "Use WASD to move, E to press, Q to quit: ";
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
                case 'e':  // Press button under cursor
                    pressButton(cursorX, cursorY);
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
    vector<string> buttons;  // Button labels
    vector<pair<int, int>> buttonPositions;  // Button positions (row, col)

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
                // Clear the screen before displaying the loading dots
                grid.clear();
                cout << "\033[HLoading" + dots << flush;
                customSleep(500); // Delay for 500 milliseconds
            }
        }
    }

    // Press a button under the cursor
    void pressButton(int x, int y) {
        for (size_t i = 0; i < buttonPositions.size(); ++i) {
            int buttonX = buttonPositions[i].second;
            int buttonY = buttonPositions[i].first;
            int buttonWidth = buttons[i].length() + 2; // Button label width + padding around it

            // Check if the cursor is inside the button's bounds
            if (y == buttonY && x >= buttonX && x < buttonX + buttonWidth) {
                cout << "\033[HButton '" << buttons[i] << "' pressed!" << endl;
                return;
            }
        }
        cout << "\033[HNo button at the cursor position!" << endl;
    }
};

// Main function to run the UI
int main() {
    UI ui(WIDTH, HEIGHT);
    
    // Add some buttons
    ui.addButton("Start", 3, 10);
    ui.addButton("Options", 4, 10);
    ui.addButton("Exit", 5, 10);
    ui.addButton("Start", 3, 10);  // This should be ignored as a duplicate

    ui.run();
    return 0;
}
