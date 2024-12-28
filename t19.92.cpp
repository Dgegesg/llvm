#include <iostream>
#include <cstdlib>  // for system("clear")
#include <vector>
#include <string>

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 15; // Height of the screen (increased for log display)
const char EMPTY_CHAR = ' '; // Default empty space in grid
const char CURSOR_CHAR = 'X'; // Cursor character
const string TITLE = "Custom Console UI"; // The title for the UI

// ANSI color codes
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background for empty space
const string BUTTON_NORMAL_COLOR = "\033[47m"; // Normal button color when cursor is not over it
const string CURSOR_COLOR = "\033[31m\033[47m"; // Red cursor with white background
const string CONSOLE_LOG_COLOR = "\033[42m"; // Green background for console log

const int LOG_HEIGHT = 5; // Height of the console log section

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
    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<pair<int, int>>& buttonPositions, const vector<string>& consoleLog) const {
        string screen = ""; // Empty string to accumulate grid content

        // Add the title at the top (above the grid)
        screen += "\033[1m" + TITLE + "\033[0m\n";  // Bold title

        // Render grid border and inside
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

            // Render console log at the bottom rows of the grid
            if (y >= height - LOG_HEIGHT - 1 && y < height - 1) {
                int logIndex = y - (height - LOG_HEIGHT - 1); // Map grid row to log index
                if (logIndex < consoleLog.size()) {
                    string logMessage = consoleLog[logIndex];
                    if (logMessage.length() > width - 2) { // Truncate long messages
                        logMessage = logMessage.substr(0, width - 2);
                    }
                    screen += " " + CONSOLE_LOG_COLOR + logMessage + "\033[0m";
                }
            }

            screen += "\n"; // Add a new line after each row of the grid
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
    UI(int width, int height) : grid(width, height), cursorX(1), cursorY(1), activePage(1) {}

    void addButton(string label, int row, int col) {
        // Ensure button is within the bounds of the grid (not on the border)
        if (row > 0 && row < HEIGHT - LOG_HEIGHT - 1 && col > 0 && col < WIDTH - 1) {
            buttons.push_back(label);
            buttonPositions.push_back({row, col});  // Track added position
        } else {
            cout << "Button '" << label << "' cannot be placed on the border!" << endl;
        }
    }

    void consoleEcho(string message) {
        // Add message to the log, trimming if necessary
        if (consoleLog.size() >= LOG_HEIGHT) {
            consoleLog.erase(consoleLog.begin()); // Remove oldest line if limit exceeded
        }
        consoleLog.push_back(message);  // Add new message
    }

    void run() {
        grid.clear(); // Clear screen at the start

        char input;
        while (true) {
            render();
            cin >> input;
            handleInput(input);
        }
    }

private:
    Grid grid;
    int cursorX, cursorY, activePage;
    vector<string> buttons;  // Button labels
    vector<pair<int, int>> buttonPositions;  // Button positions (row, col)
    vector<string> consoleLog;  // Console log messages

    void render() {
        string screen = grid.render(cursorX, cursorY, buttons, buttonPositions, consoleLog);
        cout << "\033[H" << screen << "Use WASD to move, E to press, Q to quit\n";
    }

    void handleInput(char input) {
        switch (input) {
            case 'w':  // Move cursor up
                if (cursorY > 1) cursorY--;
                break;
            case 's':  // Move cursor down
                if (cursorY < HEIGHT - LOG_HEIGHT - 2) cursorY++;
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
                exit(0);
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
                exit(0);
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

    ui.run();
    return 0;
}
