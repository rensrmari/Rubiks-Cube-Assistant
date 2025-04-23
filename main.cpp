#include "assistant.h"
#include "cube.h"
#include "file_handler.h"
#include "assistant_tests.h"
#include "cube_tests.h"
#include "file_handler_tests.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstring>
using namespace std;

void handleNewCube(Cube& cube, FileHandler& handler, bool randomized, bool& original, bool& usingCube);
void applyRandomScramble(Cube& cube, string& scramble);
void applyManualScramble(Cube& cube, string& scramble);
void handleLoadCube(Cube& cube, FileHandler& handler, bool& original, bool& usingCube);
void useCube(Cube& cube, FileHandler& handler, bool newCube);
void saveCube(Cube& cube, FileHandler& handler, bool& successful);

char getCharacterInput();
void switchMenu(bool& original, bool& updated);
void displayTitle();
void displayNewCube();
void displayLoadCube();
void displayGuide();

int main(int argc, char* argv[]) {
    // Check for command line arguments for unit tests.
    if (argc > 1) {
        char* arg = argv[1];
        char assistantStr[] = "assistant";
        char cubeStr[] = "cube";
        char fileHandlerStr[] = "file_handler";

        if (strcmp(arg, assistantStr) == 0) {
            cout << "\nTesting the Assistant...\n";
            AssistantTests test;
            return test.runTests();
        } else if (strcmp(arg, cubeStr) == 0) {
            cout << "\nTesting the Cube...\n";
            CubeTests test;
            return test.runTests();
        } else if (strcmp(arg, fileHandlerStr) == 0) {
            cout << "\nTesting the File Handler...\n";
            FileHandlerTests test;
            return test.runTests();
        } else {
            cout << "\nInvalid argument. The valid arguments are:\n";
            cout << "\t   assistant - Test the Assistant class\n";
            cout << "\t        cube - Test the Cube class\n";
            cout << "\tfile_handler - Test the File Handler class\n";
            return 1;
        }
    }

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
    
    FileHandler handler;
    Cube currentCube;
    bool createdNewCube = false;
    char userInput;

    srand(time(0));
    
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
                handleNewCube(currentCube, handler, true, isNewCube, usingCube);
            } else if (userInput == NEW_CUBE_MANUAL_CHAR) {
                handleNewCube(currentCube, handler, false, isNewCube, usingCube);
            }

            createdNewCube = true;
            
            if (!usingCube) {
                switchMenu(isNewCube, isTitle);
                createdNewCube = false;
            }
        } else if (isLoadCube) { // Display load cube menu, providing user with options
            displayLoadCube();
            handleLoadCube(currentCube, handler, isLoadCube, usingCube);
            
            if (!usingCube) {
                switchMenu(isLoadCube, isTitle);
            }
        } else if (isGuide) { // Display guide and allow user to go back with character input
            displayGuide();
            userInput = getCharacterInput();
            switchMenu(isGuide, isTitle);
        } else { // The user is using the Rubik's cube
            useCube(currentCube, handler, createdNewCube);
            switchMenu(usingCube, isTitle);
            createdNewCube = false;
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
 * @param cube The Cube in use that will be updated.
 * @param handler The FileHandler to update.
 * @param randomized Whether or not the Cube will be scrambled randomly.
 * @param original The original menu.
 * @param usingCube Whether or not the Cube is in use.
 */
void handleNewCube(Cube& cube, FileHandler& handler, bool randomized, bool& original, bool& usingCube) {
    // Prompt user for the name of a valid file (empty or CSV-compliant).
    cout << "\nPlease enter the name of a valid or empty file to use: ";
    string fileName;
    getline(cin, fileName);
    handler.setFileName(fileName);

    // First check if provided file is empty or valid.
    int isEmpty = handler.checkValidFile(true);
    int isValid = handler.checkValidFile(false);

    if (isEmpty == FileHandler::VALID || isValid == FileHandler::VALID) {
        cout << "\nYou are now using \"" << fileName << "\".\n";
    } else {
        handler.displayError(isValid);
        handler.reset();
        return;
    }
    
    // Apply scrambles to the new cube.
    string scramble = "";
    if (randomized) {
        applyRandomScramble(cube, scramble);   
    } else {
        applyManualScramble(cube, scramble);
    }
    
    cout << endl;

    // Add a name to the new cube.
    // If the file is a valid non-empty file, duplicate names must be checked.
    handler.processValidFile();
    string name;
    bool validName = false;

    do {
        cout << "Please enter a unique name for this cube: ";
        getline(cin, name);

        if (!handler.checkTaken(name)) {
            validName = true;
        }

        if (!validName) {
            cout << "\nThis name is taken. ";
        }
    } while (!validName);

    // Update cube information and begin using the cube.
    cube.setName(name);
    cube.setScramble(scramble);
    switchMenu(original, usingCube);
}

/**
 * Randomly scrambles a given Cube.
 * @param cube The cube to scramble.
 * @param scramble The scramble sequence to update.
 */
void applyRandomScramble(Cube& cube, string& scramble) {
    const int MAX_MOVES = 75000;
    const int DEFAULT_MOVES = 25;
    string validTurns = "ULFRBD";
    string validModifiers = " '2";

    // Prompt user moves.
    int numMoves;
    cout << "\nPlease enter the number of scrambling moves (max " << MAX_MOVES << "): ";
    cin >> numMoves;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // If invalid input, use default moves.
    if (cin.fail() || numMoves < 0 || numMoves > MAX_MOVES) {
        cin.clear();
        numMoves = DEFAULT_MOVES;
        cout << "\nCannot process that request, scrambling with " << numMoves << " moves instead.\n";
    }

    // Apply random moves.
    int count = 0;
    while (count < numMoves) {
        // Disable double turn modifier at a count of one less to ensure the exact number of moves.
        if (count == numMoves - 1) {
            validModifiers = " \'";
        }

        int validTurnsIdx = rand() % validTurns.length();
        int validModifiersIdx = rand() % validModifiers.length();

        string add = string(1, validTurns[validTurnsIdx]) + string(1, validModifiers[validModifiersIdx]);
        cube.doMoves(add, false);
        scramble += add;

        // Increment counter.
        if (validModifiersIdx == 2) {
            count += 2;
        } else {
            count++;
        }
    }
}

/**
 * Allows the user to manually input moves to scramble a Cube.
 * @param cube The Cube to scramble.
 * @param scramble The scramble sequence to update.
 */
void applyManualScramble(Cube& cube, string& scramble) {
    const string STOP_STR_1 = "Q";
    const string STOP_STR_2 = "q";
    bool stop = false;

    while (!stop) {
        cout << "\nEnter your scramble (or \"Q\" to finish): ";
        string moves;
        getline(cin, moves);
        istringstream iss(moves);
        
        string move;
        while (iss >> move) {
            if (Cube::checkMoves(move)) { // See if move line was valid
                cube.doMoves(move, false);
                scramble += move;
            } else if (move == STOP_STR_1 || move == STOP_STR_2) { // Check for sentinel to terminate
                stop = true;
                break;
            } else {
                cout << "Invalid input. Valid moves include U, L, F, R, B, D, and modifiers (\"'\" or \"2\").\n";
                break;
            }
        }
    }
}

/**
 * Sets the current Cube to a pre-existing one, if it exists in a given valid file.
 * @param cube The Cube in use that will be updated.
 * @param handler The FileHandler to update.
 * @param original The original menu.
 * @param usingCube Whether or not the Cube is in use.
 */
void handleLoadCube(Cube& cube, FileHandler& handler, bool& original, bool& usingCube) {
    // Prompt user for the name of a valid file of CSV.
    // The process will stop if the file invalid, outputting why is it not valid.
    // Otherwise, the user will be prompted for the name of a Cube to load.
    cout << "\nPlease enter a valid file: ";
    string fileName;
    getline(cin, fileName);
    handler.setFileName(fileName);

    int fileStatus = handler.checkValidFile(false);
    if (fileStatus == FileHandler::VALID && handler.processValidFile()) {
        cout << "\nSuccessfully loaded \"" << fileName << "\".\n";
        handler.displaySavedCubes();

        // Prompt a name from the file.
        string requestedName;
        bool validName = false;
        do {
            cout << "Please enter the name of your cube: ";
            getline(cin, requestedName);

            if (handler.loadCube(cube, requestedName)) {
                validName = true;
            }

            if (!validName) {
                cout << "\nInvalid name. ";
            }
        } while (!validName);

        // Update cube information and begin using the cube.
        switchMenu(original, usingCube);
    } else {
        handler.displayError(fileStatus);
        handler.reset();
    }
}

/**
 * Handles the logic of using the Cube.
 * Allows the user to interact with the Cube and assistant as well as save and exit.
 * @param cube The Cube to interact with.
 * @param handler The FileHandler to handle the saving of the Cube.
 * @param newCube Whether or not the cube is new.
 */
void useCube(Cube& cube, FileHandler& handler, bool newCube) {
    const string EXIT_COMMAND = "EXIT";
    const string SAVE_COMMAND = "SAVE";
    const string UNDO_COMMAND = "UNDO";
    const string SOLVE_COMMAND = "SOLVE";

    Assistant assistant(cube);
    Cube originalCube = cube;  // Used to check if saving a loaded Cube is needed
    bool recentlySaved = false;
    bool invalidInput = false;
    string userInput;

    while (true) {
        // Display the state of the Cube and valid commands, but avoid repeating when provided invalid input.
        if (!invalidInput) {
            cout << "\n[" << cube.getName() << "'S CUBE]\n";
            cube.displayState(true);
        }

        // Prompt for a command.
        cout << "Please enter a command or sequence of moves (EX: U2 D2 F2 B2 L2 R2): ";
        getline(cin, userInput);

        // Process the user's command.
        if (Cube::checkMoves(userInput)) { // First check if it is a valid sequence
            cube.doMoves(userInput, true);
            recentlySaved = false;
            invalidInput = false;
        } else if (userInput == EXIT_COMMAND) { // Prompt user to save new or loaded Cube with changes
            if (!recentlySaved && (!(originalCube == cube) || newCube)) {
                cout << "\nYou haven't saved your data, would you like to save it now (Y/N)? ";
                char userResponse = getCharacterInput();

                if (userResponse == 'Y') {
                    saveCube(cube, handler, recentlySaved);
                }
            }

            cout << "\nYou have finished using " << cube.getName() << "'s cube.\n";
            break;
        } else if (userInput == SAVE_COMMAND) {
            saveCube(cube, handler, recentlySaved);
            invalidInput = false;
        } else if (userInput == UNDO_COMMAND) {
            cout << cube.undo();
            invalidInput = false;
        } else if (userInput == SOLVE_COMMAND) {
            assistant.solve();
            recentlySaved = false;
            invalidInput = false;
        } else {
            cout << "Invalid input. ";
            invalidInput = true;
        }
    }

    cube.reset();
    handler.reset();
}

/**
 * Handles the saving of a cube, displaying the status of the save.
 * @param cube The cube to save.
 * @param handler The FileHandler to use.
 * @param successful Whether or not the save was successful.
 */
void saveCube(Cube& cube, FileHandler& handler, bool& successful) {
    successful = handler.saveCubeToFile(cube);
    char userInput;

    // Allow retrial of saving if an error occurred.
    while (true) {
        if (successful) {
            cout << "\nSaved " << cube.getName() << "'s cube to \"" << handler.getFileName() << "\".\n";
            break;
        } else {
            handler.displayError(handler.checkValidFile(false)); // There must be invalid data in the file
            cout << "\nSave was unsuccessful, try again (Y/N)? ";
            userInput = getCharacterInput();
            
            if (userInput == 'Y') {
                successful = handler.saveCubeToFile(cube);
            } else {
                break;
            }
        }
    }
}

/**
 * Switches the menu that the user is seeing.
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
    // https://www.asciiart.eu/text-to-ascii-art
    cout << "\n\n ____        _     _ _    _        ____      _          \n"
         << "|  _ \\ _   _| |__ (_) | _( )___   / ___|   _| |__   ___ \n"
         << "| |_) | | | | '_ \\| | |/ /// __| | |  | | | | '_ \\ / _ \\\n"
         << "|  _ <| |_| | |_) | |   <  \\__ \\ | |__| |_| | |_) |  __/\n"
         << "|_| \\_\\\\__,_|_.__/|_|_|\\_\\ |___/  \\____\\__,_|_.__/ \\___|\n"
         << "   / \\   ___ ___(_)___| |_ __ _ _ __ | |_\n"
         << "  / _ \\ / __/ __| / __| __/ _` | '_ \\| __|\n"
         << " / ___ \\\\__ \\__ \\ \\__ \\ || (_| | | | | |_\n"
         << "/_/   \\_\\___/___/_|___/\\__\\__,_|_| |_|\\__|\n\n";
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
    cout << "\n\n[NEW CUBE]\n"
         << "\t(R) Randomly generate a new Rubik's cube\n"
         << "\t(M) Manually scramble your Rubik's cube\n"
         << "What would you like to do? ";
}

/**
 * Prints load cube menu and provides valid options.
 */
void displayLoadCube() {
    cout << "\n\n[LOAD CUBE]";
}

/**
 * Prints guide and provides valid options.
 */
void displayGuide() {
    cout << "\n\n[GUIDE]\n"
         << "The Rubik's Cube Assistant program allows you to interact with a Rubik's cube and receive guidance from\n"
         << "an assistant that uses beginner's method: a layer-by-layer approach involving (1) solving the white\n"
         << "cross, (2) solving the white corners, (3) solving the second layer, (4) solving the yellow cross, (5)\n"
         << "aligning the yellow edges, (6) positioning the yellow corners, and (7) orienting the yellow corners.\n\n";

    cout << "To start interacting with your Rubik's cube, either create a new cube or load in a pre-existing one.\n"
         << "New Cube: You can scramble the new cube randomly or manually. You must provide a unique name for the\n"
         << "          cube to ensure that you can come back to it later. The provided file may be empty.\n\n"
         << "Load Cube: Alternatively, you can use a cube that has already been set up. Simply provide the file\n"
         << "           name to be processed. The data within this file must have comma-separated values containing\n"
         << "           a unique name, correctly formatted original scramble, the moves applied, and the total moves.\n"
         << "           After providing a valid file, you will be prompted to enter the name of the cube you would\n"
         << "           like to load.\n\n";

    cout << "Once you have started interacting with your Rubik's cube, you will be shown with a flattened display\n"
         << "of your cube, as well as its scramble, total moves, and current moves. You will also be presented with\n"
         << "a set of valid commands, which include performing moves, undoing a move, saving your cube, and exiting.\n\n";
    
    cout << "One of the commands is called \"SOLVE\", which is point of access to the assistant. From there, you may\n"
         << "step through the stages of solving the cube, skip each stage, or exit the assistant.\n\n";
    
    cout << "Notes:\n"
         << " * As the assistant uses the beginner's method of solving, the last step (yellow corner orientation)\n"
         << "   temporarily messes up the solved portions of the cube. Therefore, exiting is disabled for this step.\n"
         << " * The color orange (O) has been replaced with magenta for increased color compatibility.\n\n"
         << "Enter anything to go back: ";
}