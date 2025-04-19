#include "file_handler.h"
#include "cube.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
using namespace std;

FileHandler::FileHandler() {
    file = "";
}

FileHandler::FileHandler(const string& file) {
    this->file = file;
}

string FileHandler::getFileName() const { return file; }

void FileHandler::setFileName(const string& file) { this->file = file; }

int FileHandler::checkValidFile(bool acceptEmpty) const {
    ifstream ifs(file);
    
    // First check if unable to open file.
    if (!ifs.is_open()) {
        return CANT_OPEN;
    }

    // Check if the file is empty, if it should be considered valid.
    char ch;
    ifs >> ch;

    if (ifs.eof() && acceptEmpty) {
        return VALID;
    } else if (ifs.eof() && !acceptEmpty) { // File is not supposed to be empty
        return NO_DATA;
    } else {
        ifs.unget(); // Prepare for checking valid CSV
    }

    // Checks for valid CSVs (name, scramble, moves, and total moves).
    set<string> uniqueNames;
    string line;

    while (getline(ifs, line)) {
        istringstream iss(line);
        
        // Get name.
        string name = "";
        processLine(iss, name);

        // Get scramble.
        string scramble = "";
        processLine(iss, scramble);

        // Get applied moves.
        string moves = "";
        processLine(iss, moves);

        // Get number of total moves (must match the count of applied moves).
        int totalMoves;
        iss >> totalMoves;

        // If stream fails or the other conditions are invalid, the file is invalid.
        if (!iss) {
            return BAD_FORMAT;
        } else if (uniqueNames.count(name) == 1) {
            return DUPLICATE_NAMES;
        } else if (!Cube::checkMoves(scramble) || !Cube::checkMoves(moves)) {
            return INVALID_MOVES;
        } else { // Check if the applied moves (now valid) and given total moves match
            if (Cube::countMoves(moves) != totalMoves) {
                return MISMATCHED_MOVES;
            }
        }

        uniqueNames.insert(name);
    }
    
    return VALID;
}

void FileHandler::processLine(istringstream& iss, string& str) const {
    char ch;

    while (iss >> ch) {
        if (ch == ',') { // Don't process comma
            break;
        }

        str += ch;
    }
}

void FileHandler::displayError(int status) const {
    switch (status) {
        case CANT_OPEN:
            cout << "\nError: Could not open \"" << file << "\".\n";
            break;
        case NO_DATA:
            cout << "\nError: No data found in \"" << file << "\".\n";
            break;
        case BAD_FORMAT:
            cout << "\nError: Could not process \"" << file << "\".\n";
            break;
        case DUPLICATE_NAMES:
            cout << "\nError: There were duplicate names found in \"" << file << "\".\n";
            break;
        case INVALID_MOVES:
            cout << "\nError: There were invalid moves found in \"" << file << "\".\n";
            break;
        case MISMATCHED_MOVES:
            cout << "\nError: The total move count and applied moves in \"" << file << "\" are conflicting.\n";
            break;
        default:
            cout << "\nFile is invalid.\n";
    }
}

bool FileHandler::processValidFile() {
    if (checkValidFile(false) != VALID) {
        return false;
    }

    vector<string> cubeNames;
    vector<string> cubeScrambles;
    vector<string> cubeMoves;
    vector<int> cubeTotalMoves;
    ifstream ifs(file);
    string line;

    while (getline(ifs,line)) {
        istringstream iss(line);
        
        // Get name.
        string name = "";
        processLine(iss, name);
        cubeNames.push_back(name);

        // Get scramble.
        string scramble = "";
        processLine(iss, scramble);
        cubeScrambles.push_back(scramble);

        // Get applied moves.
        string moves = "";
        processLine(iss, moves);
        cubeMoves.push_back(moves);

        // Get number of total moves.
        int totalMoves;
        iss >> totalMoves;
        cubeTotalMoves.push_back(totalMoves);
    }

    for (int i = 0; i < cubeNames.size(); i++) {
        string name = cubeNames[i];
        Cube cube(name, cubeScrambles[i], cubeMoves[i], cubeTotalMoves[i]);
        savedCubes[name] = cube;
    }

    return true;
}

void FileHandler::displaySavedCubes() const {
    const int COLUMN_WIDTH = 26;
    
    cout << "\nSaved Data:\n" << string(2 * COLUMN_WIDTH + 1, '-') << endl;
    displayTableRow("Name", "Move Count", COLUMN_WIDTH);
    cout << string(2 * COLUMN_WIDTH + 1, '-') << endl;

    for (const auto& pair : savedCubes) {
        displayTableRow(pair.first, to_string(pair.second.getTotalMoves()), COLUMN_WIDTH);
    }
    
    cout << string(2 * COLUMN_WIDTH + 1, '-') << endl << endl;
}

void FileHandler::displayTableRow(const string& first, const string& second, int width) const {
    cout << left << "| " << setw(width - 2) << first << "| " << setw(width - 2) << second << "|" << endl;
}

bool FileHandler::loadCube(Cube& cube, const string& name) const {
    if (savedCubes.count(name) == 1) {
        cube = savedCubes.at(name);
        return true;
    } else {
        return false;
    }
}

bool FileHandler::checkTaken(const string& name) const {
    if (savedCubes.count(name) == 1) {
        return true;
    }

    return false;
}

void FileHandler::saveCubeToFile(const Cube& cube) {
    ofstream ofs(file);

    if (!ofs.is_open()) {
        cout << "\nCould not open \"" << file << "\" for saving.\n";
        return;
    } else {
        cout << "\nSaving cube data...\n";
    }

    // Update Cube data if it exists or add a new entry.
    savedCubes[cube.getName()] = cube;

    for (const auto& pair : savedCubes) {
        convertCubeData(ofs, pair.second);
    }

    cout << "Saved " << cube.getName() << "'s cube to \"" << file << "\".\n";
}

void FileHandler::convertCubeData(ofstream& ofs, const Cube& cube) const {
    ofs << cube.getName() << ", " + cube.getScramble() << ", " << cube.getMoves() << cube.getCurrentMoves() << ", " << cube.getTotalMoves() << endl;
}

void FileHandler::reset() {
    file = "";
    savedCubes.clear();
}