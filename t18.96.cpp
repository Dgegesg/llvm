#include <iostream>
#include <cstdlib>  // for system("clear")
#include <vector>
#include <string>

using namespace std;

const int WIDTH = 40;  // Width of the screen
const int HEIGHT = 10; // Height of the screen
const char EMPTY_CHAR = ' '; // Default empty space in grid
const char CURSOR_CHAR = 'X'; // Cursor character
const string TITLE = "Interactive UI"; // Title for interactive UI
const string CONSOLE_TITLE = "Console Log UI"; // Title for console log UI

// ANSI color codes
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background for empty space
const string BUTTON_NORMAL_COLOR = "\033[47m"; // Normal button color when cursor is not over it

// Grid class for interactive UI
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

    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<pair<int, int>>& buttonPositions) const {
        string screen = ""; // Empty string to accumulate grid content

        // Add the title at the top (above the grid)
        screen += "\033[1m" + TITLE + "\033[0m\n";  // Bold title

        // Render grid border and inside
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if ((x == 0 && y == 0) || (x == width - 1 && y == 0) ||
                    (x == 0 && y == height - 1) || (x == width - 1 && y == height - 1)) {
                    screen += "\033[1m+\033[0m"; // Bold corners
                } else if (y == 0 || y == height - 1) {
                    screen += "\033[1m-\033[0m"; // Bold top and bottom edges
                } else if (x == 0 || x == width - 1) {
                    screen += "\033[1m|\033[0m"; // Bold left and right edges
                } else if (x == cursorX && y == cursorY) {
                    screen += "\033[31m\033[47m" + string(1, CURSOR_CHAR) + "\033[0m"; // Red cursor with white background
                } else {
                    bool buttonFound = false;

                    for (size_t i = 0; i < buttonPositions.size(); ++i) {
                        int buttonX = buttonPositions[i].second;
                        int buttonY = buttonPositions[i].first;
                        int buttonWidth = buttons[i].length(); // Button width

                        if (y == buttonY && x >= buttonX && x < buttonX + buttonWidth) {
                            buttonFound = true;
                            screen += BUTTON_NORMAL_COLOR + string(1, buttons[i][x - buttonX]) + "\033[0m"; // Normal button
                            break;
                        }
                    }

                    if (!buttonFound) {
                        screen += EMPTY_SPACE_COLOR + " \033[0m"; // Default empty space
                    }
                }
            }
            screen += "\n"; // Add a new line after each row of the grid
        }
        return screen;
    }

    void clear() const {
        cout << "\033[H\033[J"; // ANSI escape sequence to clear the screen
    }

private:
    int width, height;
    char** grid;
};

// Grid class for console logging UI
class ConsoleLogGrid {
public:
    ConsoleLogGrid(int width, int height) : width(width), height(height) {
        initGrid();
    }

    void initGrid() {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                consoleGrid[y][x] = ' '; // Initialize grid with empty spaces
            }
        }
    }

    void logMessage(const string& message) {
        for (int i = height - 1; i > 0; --i) {
            for (int j = 0; j < width; ++j) {
                consoleGrid[i][j] = consoleGrid[i - 1][j]; // Shift the previous row up
            }
        }

        // Insert the new message at the bottom row
        for (int i = 0; i < message.length(); ++i) {
            if (i < width) {
                consoleGrid[0][i] = message[i];
            }
        }
    }

    string render() const {
        string screen = "\033[1m" + CONSOLE_TITLE + "\033[0m\n";  // Bold title
        for (int y = 0; y < height; ++y) {
            screen += string(consoleGrid[y], 0, width) + "\n";
        }
        return screen;
    }

    void clear() const {
        cout << "\033[H\033[J"; // ANSI escape sequence to clear the screen
    }

private:
    int width, height;
    char consoleGrid[10][40]; // Fixed size console grid for log
};

class UI {
public:
    UI(int width, int height) : interactiveGrid(width, height), cursorX(1), cursorY(1) {}

    void addButton(string label, int row, int col) {
        if (row > 0 && row < HEIGHT - 1 && col > 0 && col < WIDTH - 1) {
            buttons.push_back(label);
            buttonPositions.push_back({row, col});  // Track added position
        } else {
            cout << "Button '" << label << "' cannot be placed on the border!" << endl;
        }
    }

    void run() {
        char input;
        while (true) {
            interactiveGrid.clear();
            string screen = interactiveGrid.render(cursorX, cursorY, buttons, buttonPositions);
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
    Grid interactiveGrid;
    int cursorX, cursorY;
    vector<string> buttons;  // Button labels
    vector<pair<int, int>> buttonPositions;  // Button positions (row, col)

    void pressButton(int x, int y) {
        for (size_t i = 0; i < buttonPositions.size(); ++i) {
            int buttonX = buttonPositions[i].second;
            int buttonY = buttonPositions[i].first;
            int buttonWidth = buttons[i].length(); // Button label width

            if (y == buttonY && x >= buttonX && x < buttonX + buttonWidth) {
                consoleEcho("Button '" + buttons[i] + "' pressed!");
                return;
            }
        }
        consoleEcho("No button at the cursor position!");
    }

    void consoleEcho(const string& message) {
        consoleLogGrid.logMessage(message);
    }

    ConsoleLogGrid consoleLogGrid{WIDTH, HEIGHT}; // Console log grid
};

int main() {
    UI ui1(WIDTH, HEIGHT);
    UI ui2(WIDTH, HEIGHT);

    ui1.addButton("Start", 3, 10);
    ui1.addButton("Options", 4, 10);
    ui1.addButton("Exit", 5, 10);

    ui2.run();

    ui1.run(); // Interactive UI

    return 0;
}
