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

// ANSI color codes
const string BORDER_COLOR = "\033[1m"; // Bold borders
const string EMPTY_SPACE_COLOR = "\033[44m"; // Blue background
const string RESET_COLOR = "\033[0m"; // Reset color

// Grid class to encapsulate the terminal grid and its drawing
class Grid {
public:
    Grid(int width, int height) : width(width), height(height) {}

    // Renders the UI
    string render(const vector<string>& logMessages) const {
        string screen = "";

        // Render titles
        screen += BORDER_COLOR + MAIN_TITLE + string(WIDTH - MAIN_TITLE.length(), ' ') + "  " + LOG_TITLE + RESET_COLOR + "\n";

        // Top borders
        screen += BORDER_COLOR + "+" + string(WIDTH - 2, '-') + "+  +" + string(LOG_WIDTH - 2, '-') + "+" + RESET_COLOR + "\n";

        // Content rows
        for (int y = 0; y < HEIGHT; ++y) {
            screen += BORDER_COLOR + "|" + RESET_COLOR;
            screen += EMPTY_SPACE_COLOR + string(WIDTH - 2, EMPTY_CHAR) + RESET_COLOR;
            screen += BORDER_COLOR + "|  |" + RESET_COLOR;

            if (y < (int)logMessages.size()) {
                string logMessage = logMessages[y];
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
    vector<string> logMessages = { "Welcome to the UI!", "Initializing...", "Ready." };

    while (true) {
        system("clear");
        cout << grid.render(logMessages);

        char input;
        cin >> input;

        if (input == 'q') {
            break;
        } else {
            logMessages.push_back(string("Input received: ") + input);
            if (logMessages.size() > HEIGHT) {
                logMessages.erase(logMessages.begin()); // Maintain log size
            }
        }
    }

    return 0;
}
