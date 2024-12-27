#include <iostream>
#include <chrono>
#include <cstdlib>  // for system("clear")
#include <vector>
#include <string>
#include <set>

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
                        int buttonWidth = buttons[i].length() + 2;

                        if (y == buttonY && x >= buttonX && x < buttonX + buttonWidth) {
                            buttonFound = true;
                            if (x == buttonX) {
                                // Render the button text only once
                                screen += BUTTON_NORMAL_COLOR + " " + buttons[i] + " \033[0m";
                            }
                            break;
                        }
                    }
                    if (!buttonFound) {
                        screen += EMPTY_SPACE_COLOR + " \033[0m";
                    }
                }
            }
            screen += "\n";
        }
        return screen;
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
        pair<int, int> buttonPos = {row, col};

        // Prevent duplicate buttons
        if (buttonLabels.find(label) == buttonLabels.end()) {
            if (row > 0 && row < HEIGHT - 1 && col > 0 && col < WIDTH - 1) {
                buttons.push_back(label);
                buttonPositions.push_back(buttonPos);
                buttonLabels.insert(label);
            } else {
                cout << "Button '" << label << "' cannot be placed on the border!" << endl;
            }
        }
    }

    void run() {
        grid.clear();
        showLoadingAnimation();

        char input;
        while (true) {
            string screen = grid.render(cursorX, cursorY, buttons, buttonPositions);
            cout << "\033[H" << screen;

            cout << "Use WASD to move, E to press, Q to quit: ";
            cin >> input;

            switch (input) {
                case 'w':
                    if (cursorY > 1) cursorY--;
                    break;
                case 's':
                    if (cursorY < HEIGHT - 2) cursorY++;
                    break;
                case 'a':
                    if (cursorX > 1) cursorX--;
                    break;
                case 'd':
                    if (cursorX < WIDTH - 2) cursorX++;
                    break;
                case 'e':
                    pressButton(cursorX, cursorY);
                    break;
                case 'q':
                    cout << "Exiting..." << endl;
                    return;
                default:
                    cout << "Invalid input!" << endl;
                    break;
            }

            customSleep(100);
        }
    }

private:
    Grid grid;
    int cursorX, cursorY;
    vector<string> buttons;
    vector<pair<int, int>> buttonPositions;
    set<string> buttonLabels;

    void customSleep(int milliseconds) {
        auto start = chrono::steady_clock::now();
        while (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() < milliseconds) {
        }
    }

    void showLoadingAnimation() {
        for (int i = 0; i < 5; ++i) {
            string dots = string(i % 4, '.');
            grid.clear();
            cout << "\033[HLoading" + dots << flush;
            customSleep(500);
        }
    }

    void pressButton(int x, int y) {
        for (size_t i = 0; i < buttonPositions.size(); ++i) {
            int buttonX = buttonPositions[i].second;
            int buttonY = buttonPositions[i].first;
            int buttonWidth = buttons[i].length() + 2;

            if (y == buttonY && x >= buttonX && x < buttonX + buttonWidth) {
                cout << "\033[HButton '" << buttons[i] << "' pressed!" << endl;
                return;
            }
        }
        cout << "\033[HNo button at the cursor position!" << endl;
    }
};

int main() {
    UI ui(WIDTH, HEIGHT);

    ui.addButton("Start", 3, 10);
    ui.addButton("Options", 4, 10);
    ui.addButton("Exit", 5, 10);

    ui.run();
    return 0;
}
