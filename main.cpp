#include "assistant.h"
#include "cube.h"
#include "file_handler.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>
#include <limits>
#include <map>
using namespace std;

void switchMenu(bool& original, bool& updated);
void displayTitle();
void displayNewCube();
void displayLoadCube();
void displayGuide();

char getCharacterInput();
string getStringInput(string str);
bool checkFileValid();
void saveCube(const Cube& cube);

void handleNewCube(Cube& cube, bool randomized);
void handleLoadCube(Cube& cube, bool& usingCube);
void useCube(Cube& cube);

int main() {
    srand(time(0));

    // Upon startup, allow user to navigate the main menus using letters.
    const char NEW_CUBE_CHAR = 'N';
    const char LOAD_CUBE_CHAR = 'L';
    const char GUIDE_CHAR = 'G';
    const char QUIT_CHAR = 'Q';
    const char NEW_CUBE_RANDOM_CHAR = 'R';
    const char NEW_CUBE_MANUAL_CHAR = 'M';
    
    bool isTitle = true;
    bool isNewCube = false;
    bool isLoadCube = false;
    bool isGuide = false;
    bool usingCube = false;
    
    Cube currentCube;
    char userInput;

    while (true) {
        // Lets the user interact with the options on the title screen.
        if (isTitle) {
            displayTitle();
            userInput = getCharacterInput();

            if (userInput == NEW_CUBE_CHAR) {
                switchMenu(isTitle, isNewCube);
            } else if (userInput == LOAD_CUBE_CHAR) {
                switchMenu(isTitle, isLoadCube);
            } else if (userInput == GUIDE_CHAR) {
                switchMenu(isTitle, isGuide);
            } else if (userInput == QUIT_CHAR) {
                break;
            } else {
                cout << "Invalid input.\n";
            }
        } else if (isNewCube) {  // Display new cube menu, providing user with options
            displayNewCube();
            userInput = getCharacterInput();

            if (userInput == NEW_CUBE_RANDOM_CHAR) {
                handleNewCube(currentCube, true);
                usingCube = true;
            } else if (userInput == NEW_CUBE_MANUAL_CHAR) {
                handleNewCube(currentCube, false);
                usingCube = true;
            } else {
                switchMenu(isNewCube, isTitle);
            }
        } else if (isLoadCube) { // Display load cube menu, providing user with options
            displayLoadCube();
            handleLoadCube(currentCube, usingCube);
            
            if (!usingCube) {
                switchMenu(isLoadCube, isTitle);
            }
        } else if (isGuide) { // Display guide and allow user to go back
            displayGuide();
            userInput = getCharacterInput();
            
            switchMenu(isGuide, isTitle);
        } else { // The user is using the Rubik's cube
            useCube(currentCube);
            usingCube = false;
        }
    }
    
    cout << "\nThank you for using the Rubik's Cube Assistant program.\n";
}

/** Accepts character input from user and returns its uppercase version.
 * @return Uppercase character from the user.
 */
char getCharacterInput() {
    char ch;
    cin >> ch;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return toupper(ch);
}

/**
 * Sets the cube being used to either a randomized scramble or a manual one.
 * @param cube The cube in use that will be updated.
 * @param randomized Whether or not the cube will be scrambled randomly.
 */
void handleNewCube(Cube& cube, bool randomized) {
    
}

/**
 * Sets the current cube to a pre-existing one, if it exists in a given file.
 * @param cube The cube in use that will be updated.
 * @param usingCube Whether or not the cube is in use. Set to true if load was successful.
 */
void handleLoadCube(Cube& cube, bool& usingCube) {

}


void useCube(Cube& cube) {

}

/** Switches the menu that the user is seeing.
 * @param original The original menu to be switched from.
 * @param updated The new menu.
 */
void switchMenu(bool& original, bool& updated) {
    original = false;
    updated = true;
}

/**
 * Displays title menu and provides valid options.
 */
void displayTitle() {
    cout << "\nWelcome to the Rubik's Cube Assistant!\n";
    cout << "\t(N) NEW CUBE\n";
    cout << "\t(L) LOAD CUBE\n";
    cout << "\t(G) GUIDE\n";
    cout << "\t(Q) QUIT\n"
         << "Enter a letter to proceed: ";
}

/**
 * Prints new cube menu and provides valid options.
 */
void displayNewCube() {
    cout << "\nNEW CUBE\n"
         << "\t(N) Randomly generate a new Rubik's cube\n"
         << "\t(M) Manually scramble your Rubik's cube\n"
         << "What would you like to do? ";
}

/**
 * Prints load cube menu and provides valid options.
 */
void displayLoadCube() {
    cout << "\nLOAD CUBE\n"
         << "Please enter the name of a valid file: ";
}

/**
 * Prints guide and provides valid options.
 */
void displayGuide() {
    cout << "\nGUIDE\n"
         << "The Rubik's Cube Assistant program allows you to interact with a Rubik's cube with guidance of\n"
         << "a computer using the beginner's method.\n\n";

    cout << "To start interacting with your Rubik's cube, either create a new cube or load in a pre-existing one.\n"
         << "New Cube: You can scramble the new cube randomly or manually. You must provide a unique name \n"
         << "\tfor the cube to ensure that you can come back to it later.\n"
         << "Load Cube: Alternatively, you can use a cube that has already been set up. Simply provide the file\n"
         << "\tname to be processed. The data within this file must have comma-separated values containing\n"
         << "\ta unique name, correctly formatted original scramble, the moves applied, and the total moves.\n"
         << "\tAfter providing a valid file, you will be prompted to enter the name of the cube you would\n"
         << "\tlike to load.\n\n";

    cout << "Once you have started interacting with your Rubik's cube, you will be presented with a set of valid\n"
         << "commands. You will also have a flattened display of the cube. Additionally, your name, the original\n"
         << "scramble, total moves, and current moves will be displayed.\n\n";
    
    cout << "Note: The color orange (O) has been replaced with magenta.\n\n"
         << "Enter anything to go back: ";
}