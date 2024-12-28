#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // for system("clear")

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 10;
const int LOG_WIDTH = 30;
const char EMPTY_CHAR = ' ';
const string MAIN_TITLE = "Custom Console UI";
const string LOG_TITLE = "Output Echo";
const char CURSOR_CHAR = 'X';

const string BORDER_COLOR = "\033[1m";
const string EMPTY_SPACE_COLOR = "\033[44m";
const string BUTTON_COLOR = "\033[44;97m";
const string CURSOR_COLOR = "\033[41;97m";
const string RESET_COLOR = "\033[0m";

class Grid {
public:
    Grid(int width, int height) : width(width), height(height) {}

    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<string>& logMessages, const vector<bool>& buttonVisibility, bool inputMode) const {
        string screen;

        // Render Titles
        string title = inputMode ? "Input:" : "Choose an option:";
        screen += BORDER_COLOR + MAIN_TITLE + string(WIDTH - MAIN_TITLE.length(), ' ') + "  " + LOG_TITLE + RESET_COLOR + "\n";
        screen += BORDER_COLOR + "+" + string(WIDTH - 2, '-') + "+  +" + string(LOG_WIDTH - 2, '-') + "+" + RESET_COLOR + "\n";

        int logStartIndex = max(0, (int)logMessages.size() - HEIGHT);
        vector<string> visibleLogs(logMessages.begin() + logStartIndex, logMessages.end());

        for (int y = 0; y < HEIGHT; ++y) {
            screen += BORDER_COLOR + "|" + RESET_COLOR;

            if (y >= 3 && y < 3 + (int)buttons.size()) {
                int buttonIndex = y - 3;
                if (buttonVisibility[buttonIndex] && !(cursorX >= (WIDTH - 2 - buttons[buttonIndex].length()) / 2 && cursorX < (WIDTH + buttons[buttonIndex].length()) / 2 && cursorY == y)) {
                    string button = buttons[buttonIndex];
                    string row = string((WIDTH - 2 - button.length()) / 2, EMPTY_CHAR) + button;
                    row += string(WIDTH - 2 - row.length(), EMPTY_CHAR);

                    for (int x = 0; x < WIDTH - 2; ++x) {
                        if (x == cursorX && y == cursorY) {
                            screen += CURSOR_COLOR + string(1, CURSOR_CHAR) + RESET_COLOR;
                        } else {
                            screen += BUTTON_COLOR + string(1, row[x]) + RESET_COLOR;
                        }
                    }
                } else {
                    for (int x = 0; x < WIDTH - 2; ++x) {
                        if (x == cursorX && y == cursorY) {
                            screen += CURSOR_COLOR + string(1, CURSOR_CHAR) + RESET_COLOR;
                        } else {
                            screen += EMPTY_SPACE_COLOR + string(1, EMPTY_CHAR) + RESET_COLOR;
                        }
                    }
                }
            } else {
                for (int x = 0; x < WIDTH - 2; ++x) {
                    if (x == cursorX && y == cursorY) {
                        screen += CURSOR_COLOR + string(1, CURSOR_CHAR) + RESET_COLOR;
                    } else {
                        screen += EMPTY_SPACE_COLOR + string(1, EMPTY_CHAR) + RESET_COLOR;
                    }
                }
            }

            screen += BORDER_COLOR + "|  |" + RESET_COLOR;

            if (y < (int)visibleLogs.size()) {
                string logMessage = visibleLogs[y];
                if (logMessage.length() > LOG_WIDTH - 2) {
                    logMessage = logMessage.substr(0, LOG_WIDTH - 2);
                }
                screen += EMPTY_SPACE_COLOR + logMessage + string(LOG_WIDTH - 2 - logMessage.length(), ' ') + RESET_COLOR;
            } else {
                screen += EMPTY_SPACE_COLOR + string(LOG_WIDTH - 2, EMPTY_CHAR) + RESET_COLOR;
            }

            screen += BORDER_COLOR + "|" + RESET_COLOR + "\n";
        }

        screen += BORDER_COLOR + "+" + string(WIDTH - 2, '-') + "+  +" + string(LOG_WIDTH - 2, '-') + "+" + RESET_COLOR + "\n";
        screen += title;

        return screen;
    }

private:
    int width, height;
};

// Function to process user input
string processInput(const string& input) {
    return "Processed: " + input;
}

int main() {
    Grid grid(WIDTH, HEIGHT);

    int cursorX = WIDTH / 2 - 1, cursorY = 3;
    vector<string> buttons = { "Start", "Options", "Input", "Exit" };
    vector<bool> buttonVisibility(buttons.size(), true);
    vector<string> logMessages = { "Welcome to the UI!", "Initializing...", "Ready." };
    bool inputMode = false;

    while (true) {
        system("clear");
        cout << grid.render(cursorX, cursorY, buttons, logMessages, buttonVisibility, inputMode);

        if (inputMode) {
            string userInput;
            cin.ignore(); // Clear newline
            getline(cin, userInput);
            string processedInput = processInput(userInput);
            logMessages.push_back(processedInput);
            inputMode = false;
            continue;
        }

        char input;
        cin >> input;

        if (input == 'w' && cursorY > 0) {
            cursorY--;
        } else if (input == 's' && cursorY < HEIGHT - 1) {
            cursorY++;
        } else if (input == 'a' && cursorX > 0) {
            cursorX--;
        } else if (input == 'd' && cursorX < WIDTH - 3) {
            cursorX++;
        } else if (input == 'e') {
            int buttonIndex = cursorY - 3;
            if (buttonIndex >= 0 && buttonIndex < (int)buttons.size() && buttonVisibility[buttonIndex]) {
                buttonVisibility[buttonIndex] = false;
                logMessages.push_back("Selected: " + buttons[buttonIndex]);
                if (buttons[buttonIndex] == "Input") {
                    inputMode = true;
                } else if (buttons[buttonIndex] == "Exit") {
                    break;
                }
            }
        }

        if ((int)logMessages.size() > HEIGHT) {
            logMessages.erase(logMessages.begin());
        }
    }

    return 0;
}
