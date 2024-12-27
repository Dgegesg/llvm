#include <iostream>
#include <chrono>
#include <cstdlib>  // for system("clear")
#include <vector>
#include <string>
#include <iomanip>  // For std::setw

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 10; // Height of the screen
const char EMPTY_CHAR = ' '; // Default empty space in grid
const char CURSOR_CHAR = 'X'; // Cursor character
const string TITLE = "My Custom UI"; // The title for the UI

// ANSI color codes
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background for empty space (change this to any color)
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

    // Render background layer: grid borders, title, and empty spaces
    string renderBackground() const {
        string background = ""; 

        // Add the title at the top (above the grid)
        background += "\033[1m" + TITLE + "\033[0m\n";  // Bold title
        
        // Render grid border and inside
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Draw grid corners with bold formatting
                if ((x == 0 && y == 0) || (x == width - 1 && y == 0) || 
                    (x == 0 && y == height - 1) || (x == width - 1 && y == height - 1)) {
                    background += "\033[1m+\033[0m"; // Bold corners
                }
                // Bold horizontal edges
                else if (y == 0 || y == height - 1) {
                    background += "\033[1m-\033[0m"; // Bold top and bottom edges
                }
                // Bold vertical edges
                else if (x == 0 || x == width - 1) {
                    background += "\033[1m|\033[0m"; // Bold left and right edges
                }
                // Default empty space inside the grid
                else {
                    background += EMPTY_SPACE_COLOR + " \033[0m"; // Apply color to empty space
                }
            }
            background += "\n"; // Add a new line after each row of the grid
        }
        return background; // Return the built background layer
    }

    // Render the button layer: draw the buttons on the grid
    string renderButtons(const vector<string>& buttons, const vector<pair<int, int>>& buttonPositions, int cursorX, int cursorY) const {
        string buttonLayer = "";

        // Render button layer on top of the grid
        for (size_t i = 0; i < buttonPositions.size(); ++i) {
            int y = buttonPositions[i].first;
            int x = buttonPositions[i].second;

            // Highlight button when the cursor is over it
            if (cursorX == x && cursorY == y) {
                buttonLayer += BUTTON_COLOR + " " + buttons[i] + " \033[0m"; // Active button
            } else {
                buttonLayer += BUTTON_NORMAL_COLOR + " " + buttons[i] + " \033[0m"; // Normal button
            }
        }
        return buttonLayer; // Return the button layer
    }

    // Render the cursor layer: draw the cursor on top of the grid
    string renderCursor(int cursorX, int cursorY) const {
        string cursorLayer = "";
        // Draw the cursor (on top of button or background)
        cursorLayer += "\033[31m\033[47m" + string(1, CURSOR_CHAR) + "\033[0m"; // Red cursor with white background
        return cursorLayer;
    }

    // Combine all layers into a final screen output
    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<pair<int, int>>& buttonPositions) const {
        string screen = renderBackground();  // Start with the background layer

        // Now add the button layer (buttons will be rendered over the background)
        screen += renderButtons(buttons, buttonPositions, cursorX, cursorY);

        // Add the cursor layer on top of the buttons
        screen += renderCursor(cursorX, cursorY);

        return screen;
    }

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
        // Ensure button is within the bounds of the grid (not on the border)
        if (row > 0 && row < HEIGHT - 1 && col > 0 && col < WIDTH - 1) {
            buttons.push_back(label);
            buttonPositions.push_back({row, col});
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
            if (buttonPositions[i].first == y && buttonPositions[i].second == x) {
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

    ui.run();
    return 0;
}
