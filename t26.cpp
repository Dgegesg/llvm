#include <iostream>
#include <cstdlib>  // for system("clear")
#include <vector>
#include <string>

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 10; // Height of the screen
const int LOG_WIDTH = 40; // Adjusted log width to match the first UI
const char EMPTY_CHAR = ' '; // Default empty space in grid
const char CURSOR_CHAR = 'X'; // Cursor character
const string TITLE = "Custom Console UI"; // The title for the UI
const string LOG_TITLE = "Echo output"; // Title for the log area

// ANSI color codes
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background for empty space
const string BUTTON_NORMAL_COLOR = "\033[34;1m\033[44m"; // Darker blue button with white text
const string CURSOR_COLOR = "\033[31m\033[44m"; // Red cursor with blue background
const string CONSOLE_LOG_COLOR = "\033[44m"; // Blue background for console log grid

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

    // Renders the grid
    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<pair<int, int>>& buttonPositions, const vector<string>& consoleLog, const vector<pair<string, pair<int, int>>>& labels) const {
        string screen = ""; // Empty string to accumulate grid content

        // Add the title at the top (above the grid)
        screen += "\033[1m" + TITLE + "\033[0m\n";  // Bold title

        // Render the grid
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if ((x == 0 && y == 0) || (x == width - 1 && y == 0) || 
                    (x == 0 && y == height - 1) || (x == width - 1 && y == height - 1)) {
                    screen += "\033[1m+\033[0m"; // Bold corners
                }
                else if (y == 0 || y == height - 1) {
                    screen += "\033[1m-\033[0m"; // Bold top and bottom edges
                }
                else if (x == 0 || x == width - 1) {
                    screen += "\033[1m|\033[0m"; // Bold left and right edges
                }
                else if (x == cursorX && y == cursorY) {
                    screen += CURSOR_COLOR + string(1, CURSOR_CHAR) + "\033[0m"; // Cursor color
                } else {
                    bool buttonFound = false;

                    // Check if we are on a button
                    for (size_t i = 0; i < buttonPositions.size(); ++i) {
                        int buttonX = buttonPositions[i].second;
                        int buttonY = buttonPositions[i].first;
                        int buttonWidth = buttons[i].length(); // Button width

                        if (y == buttonY && x >= buttonX && x < buttonX + buttonWidth) {
                            buttonFound = true;
                            screen += BUTTON_NORMAL_COLOR + string(1, buttons[i][x - buttonX]) + "\033[0m"; // Dark blue button with white text
                            break;
                        }
                    }

                    if (!buttonFound) {
                        // Default empty space
                        screen += EMPTY_SPACE_COLOR + " " + "\033[0m"; // Apply color to empty space
                    }
                }
            }

            screen += "\n"; // Move to the next line
        }

        // Render labels below the grid
        for (const auto& label : labels) {
            int row = label.second.first;
            int col = label.second.second;
            screen += "\033[" + to_string(row) + ";" + to_string(col) + "H" + label.first;
        }

        // Render "Choose an option:" label below the grid
        screen += "\033[" + to_string(HEIGHT + 2) + ";2HChoose an option:";

        // Render the log area
        screen += "\n\033[1m+" + string(LOG_WIDTH - 2, '-') + "+\033[0m\n"; // Single top border for log
        for (int i = 0; i < 5; ++i) {
            screen += "\033[1m|\033[0m";

            // Render log messages inside
            if (i < (int)consoleLog.size()) {
                string message = consoleLog[i];
                if (message.length() > LOG_WIDTH - 2) { // Cut long messages
                    message = message.substr(0, LOG_WIDTH - 2);
                }
                screen += CONSOLE_LOG_COLOR + message + string(LOG_WIDTH - 2 - message.length(), ' ') + "\033[0m";
            } else {
                screen += string(LOG_WIDTH - 2, ' ');
            }

            screen += "\033[1m|\033[0m\n";
        }

        // Render log bottom border
        screen += "\033[1m+" + string(LOG_WIDTH - 2, '-') + "+\033[0m\n";

        return screen;
    }

private:
    int width, height;
    char** grid;
};

int main() {
    Grid grid(WIDTH, HEIGHT);

    int cursorX = 1, cursorY = 1;
    vector<string> buttons = { "Start", "Options", "Exit" };
    vector<pair<int, int>> buttonPositions = { {3, 4}, {4, 4}, {5, 4} };
    vector<string> consoleLog = { "Welcome to the UI!", "Initializing...", "Ready." };
    vector<pair<string, pair<int, int>>> labels = {};

    while (true) {
        system("clear");
        cout << grid.render(cursorX, cursorY, buttons, buttonPositions, consoleLog, labels);

        char input;
        cin >> input;

        if (input == 'w' && cursorY > 1) {
            cursorY--;
        } else if (input == 's' && cursorY < HEIGHT - 2) {
            cursorY++;
        } else if (input == 'a' && cursorX > 1) {
            cursorX--;
        } else if (input == 'd' && cursorX < WIDTH - 2) {
            cursorX++;
        } else if (input == 'e') {
            // Check if cursor is on a button and interact
            for (size_t i = 0; i < buttonPositions.size(); ++i) {
                int buttonX = buttonPositions[i].second;
                int buttonY = buttonPositions[i].first;
                int buttonWidth = buttons[i].length();

                if (cursorY == buttonY && cursorX >= buttonX && cursorX < buttonX + buttonWidth) {
                    consoleLog.push_back("Selected: " + buttons[i]);
                    break;
                }
            }
        } else if (input == 'q') {
            break;
        }
    }

    return 0;
}
