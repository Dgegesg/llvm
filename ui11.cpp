#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <limits>
#include <chrono>
#include <unistd.h>  // For usleep

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 10;
const int LOG_WIDTH = 35;
const char EMPTY_CHAR = ' ';
const string MAIN_TITLE = "Artoriasphere";
const string LOG_TITLE = "Output Echo";
const char CURSOR_CHAR = 'X';

const string BORDER_COLOR = "\033[1m";
const string EMPTY_SPACE_COLOR = "\033[44m";
const string BUTTON_COLOR = "\033[44;97m";
const string CURSOR_COLOR = "\033[41;97m";
const string LABEL_COLOR = "\033[44;97m";
const string RESET_COLOR = "\033[0m";

string currentColor = EMPTY_SPACE_COLOR;

class Grid {
public:
    Grid(int width, int height) : width(width), height(height) {}

    string render(int cursorX, int cursorY, const vector<string>& buttons, const vector<pair<int, string>>& labels, const vector<string>& logMessages, bool inputMode) const {
        string screen;

        // Render Titles
        string title = inputMode ? "\033[KWe need your input data to process: \n\033[J" : "\033[KCursor control (wasd) input: \n\033[J";
        screen += "\033[K" + BORDER_COLOR + MAIN_TITLE + string(WIDTH - MAIN_TITLE.length(), ' ') + "  " + LOG_TITLE + RESET_COLOR + "\n";
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
                        screen += CURSOR_COLOR + string(1, CURSOR_CHAR) + RESET_COLOR;
                    } else if (x >= buttonStartX && x < buttonStartX + button.length()) {
                        screen += BUTTON_COLOR + string(1, button[x - buttonStartX]) + RESET_COLOR;
                    } else {
                        screen += currentColor + string(1, EMPTY_CHAR) + RESET_COLOR;
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
                                if (x == cursorX && y == cursorY) {
                                    screen += CURSOR_COLOR + string(1, CURSOR_CHAR) + RESET_COLOR;
                                } else {
                                    screen += LABEL_COLOR + string(1, labelText[x - labelStartX]) + RESET_COLOR;
                                }
                                isLabel = true;
                                break;
                            }
                        }
                    }
                    if (isLabel) continue;

                    if (x == cursorX && y == cursorY) {
                        screen += CURSOR_COLOR + string(1, CURSOR_CHAR) + RESET_COLOR;
                    } else {
                        screen += currentColor + string(1, EMPTY_CHAR) + RESET_COLOR;
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

void changeColor(const string& color) {
    if (color == "red") {
        currentColor = "\033[41m";
    } else if (color == "blue") {
        currentColor = "\033[44m";
    } else if (color == "green") {
        currentColor = "\033[42m";
    } else if (color == "yellow") {
        currentColor = "\033[43m";
    }
}

int main() {
    Grid grid(WIDTH, HEIGHT);

    int cursorX = WIDTH / 2 - 1, cursorY = 3;
    vector<string> buttons = { "Start", "Options", "Input", "Help", "Change Color", "Exit" };
    vector<pair<int, string>> labels = { {1, "Label: Custom Console UI"}, {2, "Label: Use Arrow Keys"} };
    vector<string> logMessages = { "Welcome to the UI!", "Initializing...", "Ready." };
    bool inputMode = false;

    while (true) {
        cout << "\033[H";
        cout << grid.render(cursorX, cursorY, buttons, labels, logMessages, inputMode);

        if (inputMode) {
            string userInput;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            try {
                getline(cin, userInput);
                string processedInput = processInput(userInput);
                logMessages.push_back(processedInput);
            } catch (...) {
                // Skip processing if an error occurs
                cout << "Error processing input. Skipping...\n";
            }
            inputMode = false;
            continue;
        }

        char input;
        try {
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
                    } else if (buttons[buttonIndex] == "Help") {
                        logMessages.push_back("Help: Use W/A/S/D to navigate and E to select.");
                    } else if (buttons[buttonIndex] == "Change Color") {
                        logMessages.push_back("Enter color (red, blue, green, yellow):");
                        string color;
                        cin >> color;
                        changeColor(color);
                    } else if (buttons[buttonIndex] == "Exit") {
                        cout << "\033[H";
                        break;
                    }
                }
            }
        } catch (...) {
            cout << "Invalid input. Please enter a valid character.\n";
        }

        if ((int)logMessages.size() > HEIGHT) {
            logMessages.erase(logMessages.begin());
        }

        // Throttle to avoid excessive screen redraws
        usleep(50000);  // Using usleep instead of sleep for older platforms
    }
    cout << "\033[2J";
    return 0;
}
