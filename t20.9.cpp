#include <iostream>
#include <cstdlib>  // for system("clear")
#include <vector>
#include <string>

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 10; // Height of the screen
const int LOG_WIDTH = 20; // Width of the console log area
const char EMPTY_CHAR = ' '; // Default empty space in grid
const char CURSOR_CHAR = 'X'; // Cursor character
const string TITLE = "Custom Console UI"; // The title for the UI
const string LOG_TITLE = "Echo output"; // Title for the log area

// ANSI color codes
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background for empty space
const string BUTTON_NORMAL_COLOR = "\033[47m"; // Normal button color when cursor is not over it
const string CURSOR_COLOR = "\033[31m\033[47m"; // Red cursor with white background
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

        // Render grid and log side by side
        for (int y = 0; y < height; ++y) {
            // Render interactive UI
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

                            // Show the button normally if not under cursor
                            if (cursorX >= buttonX && cursorX < buttonX + buttonWidth && cursorY == buttonY) {
                                screen += EMPTY_SPACE_COLOR + " \033[0m"; // Show empty space instead of button
                            } else {
                                screen += BUTTON_NORMAL_COLOR + string(1, buttons[i][x - buttonX]) + "\033[0m"; // Normal button
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

            // Render log UI side by side with the grid
            if (y == 0) {
                screen += "  \033[1m+" + string(LOG_WIDTH - 2, '-') + "+\033[0m";
            } else if (y == 1) {
                screen += "  \033[1m|" + LOG_TITLE + string(LOG_WIDTH - 2 - LOG_TITLE.length(), ' ') + "|\033[0m";
            } else if (y == height - 1) {
                screen += "  \033[1m+" + string(LOG_WIDTH - 2, '-') + "+\033[0m";
            } else {
                screen += "  \033[1m|\033[0m";

                // Render log messages inside
                if (y - 2 < (int)consoleLog.size() && y - 2 >= 0) {
                    string message = consoleLog[y - 2];
                    if (message.length() > LOG_WIDTH - 2) { // Cut long messages
                        message = message.substr(0, LOG_WIDTH - 2);
                    }
                    screen += CONSOLE_LOG_COLOR + message + string(LOG_WIDTH - 2 - message.length(), ' ') + "\033[0m";
                } else {
                    screen += string(LOG_WIDTH - 2, ' ');
                }

                screen += "\033[1m|\033[0m";
            }

            screen += "\n"; // Move to the next line
        }

        // Render labels
        for (const auto& label : labels) {
            int row = label.second.first;
            int col = label.second.second;
            if (row > 0 && row < HEIGHT - 1 && col > 0 && col < WIDTH - 1) {
                // Hide the character if the cursor is on it
                if (cursorY == row && cursorX >= col && cursorX < col + (int)label.first.length()) {
                    string hiddenLabel = label.first;
                    hiddenLabel[cursorX - col] = ' '; // Replace character under cursor with space
                    screen += "\033[" + to_string(row + 2) + ";" + to_string(col + 1) + "H" + EMPTY_SPACE_COLOR + hiddenLabel + "\033[0m";
                } else {
                    screen += "\033[" + to_string(row + 2) + ";" + to_string(col + 1) + "H" + EMPTY_SPACE_COLOR + label.first + "\033[0m"; // Render label with background color
                }
            }
        }

        return screen; // Return the built string of the UI
    }

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

    void addLabel(string label, int row, int col) {
        // Add a label at a specific position
        if (row > 0 && row < HEIGHT - 1 && col > 0 && col < WIDTH - 1) {
            labels.push_back({label, {row, col}});
        } else {
            cout << "Label '" << label << "' cannot be placed on the border!" << endl;
        }
    }

    void consoleEcho(string message) {
        // Add message to the log, trimming if necessary
        if (consoleLog.size() >= HEIGHT - 3) {
            consoleLog.erase(consoleLog.begin()); // Remove oldest line if limit exceeded
        }
        consoleLog.push_back(message);  // Add new message
    }

    void run() {
        grid.clear(); // Clear screen at the start

        char input;
        while (true) {
            renderInteractivePage();

            cout << "\033[" << HEIGHT + 3 << ";1HUse WASD to move, E to press:\n"; // Show control text below UI
            cin >> input;
            handleInput(input);
        }
    }

private:
    Grid grid;
    int cursorX, cursorY;
    vector<string> buttons;  // Button labels
    vector<pair<int, int>> buttonPositions;  // Button positions (row, col)
    vector<pair<string, pair<int, int>>> labels; // Labels with positions
    vector<string> consoleLog;  // Console log messages

    void renderInteractivePage() {
        string screen = grid.render(cursorX, cursorY, buttons, buttonPositions, consoleLog, labels);
        cout << "\033[H" << screen;
    }

    void handleInput(char input) {
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
            default:
                consoleEcho("Invalid input!");
                break;
        }
    }

    void pressButton(int x, int y) {
        for (size_t i = 0; i < buttonPositions.size(); ++i) {
            int buttonX = buttonPositions[i].second;
            int buttonY = buttonPositions[i].first;
            int buttonWidth = buttons[i].length(); // Button label width

            // Check if the cursor is inside the button's bounds
            if (y == buttonY && x >= buttonX && x < buttonX + buttonWidth) {
                executeButtonAction(i); // Execute corresponding action
                return;
            }
        }
        consoleEcho("No button at the cursor position!");
    }

    void executeButtonAction(int index) {
        switch (index) {
            case 0:  // "Start"
                consoleEcho("Starting game...");
                break;
            case 1:  // "Options"
                consoleEcho("Opening options...");
                break;
            case 2:  // "Exit"
                consoleEcho("Exiting...");
                break;
            default:
                consoleEcho("Unknown button action!");
                break;
        }
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
    ui.addLabel("Welcome!", 1, 5);
    ui.addLabel("Choose an option:", 2, 5);

    ui.run();
    return 0;
}
