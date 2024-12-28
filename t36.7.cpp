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
const string LABEL_COLOR = "\033[42;97m";
const string RESET_COLOR = "\033[0m";

class Grid {
public:
    Grid(int width, int height) : width(width), height(height) {}

    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<pair<int, string>>& labels, const vector<string>& logMessages, bool inputMode) const {
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
                string button = buttons[buttonIndex];
                int buttonStartX = (WIDTH - 2 - button.length()) / 2;

                for (int x = 0; x < WIDTH - 2; ++x) {
                    if (x == cursorX && y == cursorY) {
                        if (x >= buttonStartX && x < buttonStartX + button.length()) {
                            // Cursor over button text; hide button
                            screen += CURSOR_COLOR + string(1, CURSOR_CHAR) + RESET_COLOR;
                        } else {
                            // Cursor not on button text
                            screen += CURSOR_COLOR + string(1, CURSOR_CHAR) + RESET_COLOR;
                        }
                    } else if (x >= buttonStartX && x < buttonStartX + button.length() && !(cursorX >= buttonStartX && cursorX < buttonStartX + button.length() && cursorY == y)) {
                        // Button text (only if not hidden by cursor)
                        screen += BUTTON_COLOR + string(1, button[x - buttonStartX]) + RESET_COLOR;
                    } else {
                        // Empty space
                        screen += EMPTY_SPACE_COLOR + string(1, EMPTY_CHAR) + RESET_COLOR;
                    }
                }
            } else {
                for (int x = 0; x < WIDTH - 2; ++x) {
                    bool isLabel = false;
                    for (const auto& label : labels) {
                        if (label.first == y) {
                            string labelText = label.second;
                            int labelStartX = (WIDTH - 2 - labelText.length()) / 2;
                            if (x >= labelStartX && x < labelStartX + labelText.length()) {
                                screen += LABEL_COLOR + string(1, labelText[x - labelStartX]) + RESET_COLOR;
                                isLabel = true;
                                break;
                            }
                        }
                    }
                    if (isLabel) continue;

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
    vector<pair<int, string>> labels = { {1, "Label: Custom Console UI"}, {2, "Label: Use Arrow Keys"} };
    vector<string> logMessages = { "Welcome to the UI!", "Initializing...", "Ready." };
    bool inputMode = false;

    while (true) {
        system("clear");
        cout << grid.render(cursorX, cursorY, buttons, labels, logMessages, inputMode);

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
            int buttonStartX = (WIDTH - 2 - buttons[buttonIndex].length()) / 2;
            if (buttonIndex >= 0 && buttonIndex < (int)buttons.size() && cursorX >= buttonStartX && cursorX < buttonStartX + (int)buttons[buttonIndex].length()) {
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