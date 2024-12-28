#include <iostream>
#include <cstdlib>  // for system("clear")
#include <vector>
#include <string>

using namespace std;

const int WIDTH = 30;  // Width of the main UI
const int HEIGHT = 10; // Height of the main UI
const int LOG_WIDTH = 30; // Width of the log UI
const char EMPTY_CHAR = ' '; // Default empty space in grid
const string MAIN_TITLE = "Custom Console UI"; // Title for the main UI
const string LOG_TITLE = "Output Echo"; // Title for the log UI
const char CURSOR_CHAR = 'X'; // Cursor character

// ANSI color codes
const string BORDER_COLOR = "\033[1m"; // Bold borders
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background
const string BUTTON_COLOR = "\033[44;97m"; // Dark blue button with white text
const string CURSOR_COLOR = "\033[41;97m"; // Red background with white text for cursor
const string RESET_COLOR = "\033[0m"; // Reset color

// Grid class to encapsulate the terminal grid and its drawing
class Grid {
public:
    Grid(int width, int height) : width(width), height(height) {}

    // Renders the UI
    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<string>& logMessages) const {
        string screen = "";

        // Render titles
        screen += BORDER_COLOR + MAIN_TITLE + string(WIDTH - MAIN_TITLE.length(), ' ') + "  " + LOG_TITLE + RESET_COLOR + "\n";

        // Top borders
        screen += BORDER_COLOR + "+" + string(WIDTH - 2, '-') + "+  +" + string(LOG_WIDTH - 2, '-') + "+" + RESET_COLOR + "\n";

        // Content rows
        int logStartIndex = max(0, (int)logMessages.size() - HEIGHT);
        vector<string> visibleLogs(logMessages.begin() + logStartIndex, logMessages.end());

        for (int y = 0; y < HEIGHT; ++y) {
            screen += BORDER_COLOR + "|" + RESET_COLOR;

            // Render buttons and cursor in the main UI
            if (y >= 3 && y < 3 + (int)buttons.size()) {
                int buttonIndex = y - 3;
                string button = buttons[buttonIndex];
                string row = string((WIDTH - 2 - button.length()) / 2, EMPTY_CHAR) + button;
                row += string(WIDTH - 2 - row.length(), EMPTY_CHAR);

                if (cursorY == y) {
                    screen += CURSOR_COLOR + row + RESET_COLOR;
                } else {
                    screen += BUTTON_COLOR + row + RESET_COLOR;
                }
            } else {
                screen += EMPTY_SPACE_COLOR + string(WIDTH - 2, EMPTY_CHAR) + RESET_COLOR;
            }

            screen += BORDER_COLOR + "|  |" + RESET_COLOR;

            // Render log messages
            if (y < (int)visibleLogs.size()) {
                string logMessage = visibleLogs[y];
                if (logMessage.length() > LOG_WIDTH - 2) {
                    logMessage = logMessage.substr(0, LOG_WIDTH - 2);
                }
                screen += logMessage + string(LOG_WIDTH - 2 - logMessage.length(), EMPTY_CHAR);
            } else {
                screen += string(LOG_WIDTH - 2, EMPTY_CHAR);
            }

            screen += BORDER_COLOR + "|" + RESET_COLOR + "\n";
        }

        // Bottom borders
        screen += BORDER_COLOR + "+" + string(WIDTH - 2, '-') + "+  +" + string(LOG_WIDTH - 2, '-') + "+" + RESET_COLOR + "\n";

        // Render "Choose an option:" prompt
        screen += "Choose an option:";

        return screen;
    }

private:
    int width, height;
};

int main() {
    Grid grid(WIDTH, HEIGHT);

    int cursorX = 0, cursorY = 3; // Initial cursor position
    vector<string> buttons = { "Start", "Options", "Exit" };
    vector<string> logMessages = { "Welcome to the UI!", "Initializing...", "Ready." };

    while (true) {
        system("clear");
        cout << grid.render(cursorX, cursorY, buttons, logMessages);

        char input;
        cin >> input;

        if (input == 'w' && cursorY > 3) {
            cursorY--; // Move cursor up
        } else if (input == 's' && cursorY < 3 + (int)buttons.size() - 1) {
            cursorY++; // Move cursor down
        } else if (input == 'e') {
            // Perform action based on the button selected
            int buttonIndex = cursorY - 3;
            if (buttonIndex >= 0 && buttonIndex < (int)buttons.size()) {
                logMessages.push_back("Selected: " + buttons[buttonIndex]);
                if (buttons[buttonIndex] == "Exit") {
                    break; // Exit the program
                }
            }
        }

        // Ensure the log doesn't exceed the height
        if ((int)logMessages.size() > HEIGHT) {
            logMessages.erase(logMessages.begin());
        }
    }

    return 0;
}
