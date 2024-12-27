#include <iostream>
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
    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<pair<int, int>>& buttonPositions, const vector<string>& labels, const vector<pair<int, int>>& labelPositions) const {
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
                    bool labelFound = false;

                    // Check if we are on a button
                    for (size_t i = 0; i < buttonPositions.size(); ++i) {
                        int buttonX = buttonPositions[i].second;
                        int buttonY = buttonPositions[i].first;
                        int buttonWidth = buttons[i].length(); // Button width

                        if (y == buttonY && x >= buttonX && x < buttonX + buttonWidth) {
                            buttonFound = true;

                            // Hide the button when the cursor is over it
                            if (cursorX >= buttonX && cursorX < buttonX + buttonWidth && cursorY == buttonY) {
                                screen += EMPTY_SPACE_COLOR + " \033[0m"; // Show empty space instead of button
                            } else {
                                screen += BUTTON_NORMAL_COLOR + string(1, buttons[i][x - buttonX]) + "\033[0m"; // Normal button
                            }
                            break;
                        }
                    }

                    // Check if we are on a label
                    for (size_t i = 0; i < labelPositions.size(); ++i) {
                        int labelX = labelPositions[i].second;
                        int labelY = labelPositions[i].first;
                        int labelWidth = labels[i].length(); // Label width

                        if (y == labelY && x >= labelX && x < labelX + labelWidth) {
                            labelFound = true;
                            screen += "\033[37m" + string(1, labels[i][x - labelX]) + "\033[0m"; // Display label in white
                            break;
                        }
                    }

                    if (!buttonFound && !labelFound) {
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

class UI {
public:
    UI(int width, int height) : grid(width, height), cursorX(1), cursorY(1) {}

    void addButton(string label, int row, int col) {
        // Ensure button is within the bounds of the grid (not on the border)
        if (row > 0 && row < HEIGHT - 1 && col > 0 && col < WIDTH - 1) {
            buttons.push_back(label);
            buttonPositions.push_back({row, col});  // Track added position
        } else {
            cout << "Button '" << label << "' cannot be placed on the border!" << endl;
        }
    }

    void addLabel(string text, int row, int col) {
        // Ensure label is within the bounds of the grid
        if (row > 0 && row < HEIGHT - 1 && col > 0 && col < WIDTH - 1) {
            labels.push_back(text);
            labelPositions.push_back({row, col});  // Track added position
        } else {
            cout << "Label '" << text << "' cannot be placed on the border!" << endl;
        }
    }

    void run() {
        grid.clear(); // Clear screen at the start

        char input;
        while (true) {
            // Build the UI screen in memory to minimize flickering
            string screen = grid.render(cursorX, cursorY, buttons, buttonPositions, labels, labelPositions);

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
        }
    }

private:
    Grid grid;
    int cursorX, cursorY;
    vector<string> buttons;  // Button labels
    vector<pair<int, int>> buttonPositions;  // Button positions (row, col)
    vector<string> labels;  // Label texts
    vector<pair<int, int>> labelPositions;  // Label positions (row, col)

    // Press a button under the cursor
    void pressButton(int x, int y) {
        for (size_t i = 0; i < buttonPositions.size(); ++i) {
            int buttonX = buttonPositions[i].second;
            int buttonY = buttonPositions[i].first;
            int buttonWidth = buttons[i].length(); // Button label width

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

    // Add some labels
    ui.addLabel("Label1", 6, 5);  // Ensure unique column for each label
    ui.addLabel("Label2", 7, 5);  // New line automatically
    ui.addLabel("Label3", 8, 5);  // Continuing in the next row

    ui.run();
    return 0;
}
