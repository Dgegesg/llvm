#include <iostream>
#include <limits>
#include <cstdlib>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

// Function to clear the terminal screen based on the operating system
void clearScreen() {
#ifdef _WIN32
    system("cls");  // For Windows
#else
    system("clear");  // For Linux/MacOS/Unix
#endif
}

// Function to pause the program and wait for user input
void pause() {
    cout << "Press Enter to continue..." << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();  // Wait for user to press enter
}

// Function to handle keypresses (up, down, enter)
enum Key {
    UP = 65,
    DOWN = 66,
    ENTER = 10
};

// Function to read keypresses (without waiting for Enter)
int getKeyPress() {
    int ch = getchar();
    return ch;
}

// Function to display the menu with highlighting
void displayMenu(const vector<string>& options, int selectedIndex) {
    clearScreen();  // Clear the screen each time the menu is shown

    cout << "====================================\n";
    cout << "     Interactive Terminal UI\n";
    cout << "====================================\n";

    for (int i = 0; i < options.size(); ++i) {
        if (i == selectedIndex) {
            // Highlight the selected option
            cout << "> ";
        } else {
            cout << "  ";
        }
        cout << options[i] << endl;
    }

    cout << "====================================\n";
    cout << "Use arrow keys to navigate, Enter to select\n";
}

// Option 1: Show a simple message
void option1() {
    clearScreen();
    cout << "You chose Option 1 - Here's your message:\n";
    cout << "This is an interactive terminal UI in C++!\n";
    pause();
}

// Option 2: Show more info
void option2() {
    clearScreen();
    cout << "You chose Option 2 - Here's some more information:\n";
    cout << "This program demonstrates navigation, dynamic menus, and keypress detection!\n";
    pause();
}

// Option 3: Exit program
void exitProgram() {
    clearScreen();
    cout << "Exiting program... Goodbye!\n";
    exit(0);  // Terminate the program
}

int main() {
    vector<string> menuOptions = {
        "Option 1 - Show Message",
        "Option 2 - Show More Info",
        "Option 3 - Exit"
    };

    int selectedIndex = 0;
    while (true) {
        displayMenu(menuOptions, selectedIndex);  // Display the menu with current selection

        int key = getKeyPress();  // Wait for user keypress

        if (key == 27) {  // If the user presses the ESC key
            // Skip the next character (arrow key code)
            getchar();
            continue;
        }

        if (key == UP) {
            // Move up in the menu
            selectedIndex = (selectedIndex > 0) ? selectedIndex - 1 : menuOptions.size() - 1;
        } else if (key == DOWN) {
            // Move down in the menu
            selectedIndex = (selectedIndex < menuOptions.size() - 1) ? selectedIndex + 1 : 0;
        } else if (key == ENTER) {
            // Enter key pressed - perform the corresponding action
            switch (selectedIndex) {
                case 0:
                    option1();
                    break;
                case 1:
                    option2();
                    break;
                case 2:
                    exitProgram();
                    break;
            }
        }
    }

    return 0;
}
