#ifndef CUBE_H
#define CUBE_H

#define WHITE_BG "\033[107m"
#define RED_BG "\033[101m"
#define GREEN_BG "\033[102m"
#define BLUE_BG "\033[104m"
#define ORANGE_BG "\033[105m"
#define YELLOW_BG "\033[103m"
#define WHITE_FG "\033[97m"
#define RED_FG "\033[91m"
#define GREEN_FG "\033[92m"
#define BLUE_FG "\033[94m"
#define ORANGE_FG "\033[95m"
#define YELLOW_FG "\033[93m"
#define BLACK_FG "\033[30m"
#define RESET "\033[0m"

#include <string>
#include <stack>
#include <map>
#include <set>
#include <vector>
using namespace std;

class Cube {
public:
    enum Faces { TOP, LEFT, FRONT, RIGHT, BACK, BOTTOM };
    static const set<char> VALID_MOVES;

    /**
      * Checks whether the set of moves is valid (i.e. no unpaired numbers or apostrophes, only valid letters)
      * @param moves A set of moves.
      * @return Whether or not the moves are valid.
      */
    static bool checkMoves(const string& moves);

    /**
     * Checks the number of moves in a valid sequence.
     * @param moves The moves to be checked.
     * @return A number of moves, -1 if the sequence is invalid.
     */
    static int countMoves(const string& moves);

    /**
     * Constructor for Cube.
     */
    Cube();

    /**
     * Constructor for cube that applies the original scramble, subsequent moves, and total moves.
     * @param name A unique name.
     * @param scramble A string representing the original scramble of the Cube.
     * @param moves A string of applied moves.
     * @param totalMoves The total number of moves.
     */
    Cube(const string& name, const string& scramble, const string& moves, int totalMoves);

    /**
     * Gets the name of the Cube.
     * @return The name of the Cube.
     */
    string getName() const;

    /**
     * Sets the name of the Cube.
     * @param name The name of the Cube.
     */
    void setName(const string& name);

    /**
     * Gets the scramble of the Cube.
     * @return The scramble.
     */
    string getScramble() const;

    /**
     * Sets the scramble of the Cube.
     * @param scramble The scramble.
     */
    void setScramble(const string& scramble);

    /**
     * Gets the original moves of the Cube.
     * @return The original moves.
     */
    string getMoves() const;

    /**
     * Gets the current moves of the Cube.
     */
    string getCurrentMoves() const;

    /**
     * Gets the move count of the Cube.
     * @return The total moves.
     */
    int getTotalMoves() const;

    /**
     * Displays the state of the Cube.
     * @param printCommands Whether or not commands should be displayed.
     */
    void displayState(bool printCommands) const;

    /**
     * Performs a sequence of moves on the cube.
     * @param moves A set of moves.
     * @param update Whether or not the moves should update the current moves and counter.
     * @return A string representing the moves, each separated by spaces.
     */
    string doMoves(const string& moves, bool update);

    /**
     * Undoes a move, outputting the result of the undo.
     */
    void undo();

    /**
     * Resets the Cube object.
     */
    void reset();

    /**
     * Checks if Cubes are equal.
     * @param rhs The Cube to check against.
     * @return Whether the Cubes are equal or not.
     */
    bool operator==(const Cube& rhs) const;
private:
    static const int NUM_FACES = 6;
    static const int SIZE = 3;
    static const map<char, string> COLOR_STRINGS;
    static const map<int, char> FACE_COLORS;

    /**
     * Sets the Cube to a solved state.
     */
    void createSolved();

    /**
     * Separates a sequence with spaces.
     * @param moves The sequence to tokenize.
     * @return The tokenizes sequence.
     */
    string tokenizeMoves(const string& moves) const;

    /**
     * Processes a valid move.
     * @param letter A letter representing a valid move on the Rubik's cube.
     */
    void processMove(char letter);

    /**
     * Gets a face of the Cube.
     * @param face The index of the face to get.
     * @return The colors of a specified face.
     */
    vector<vector<char>> getFace(int face) const;

    /**
     * Places colors on a specified face of the Cube.
     * @param colors The colors to use.
     * @param face The index of the face to give the colors to.
     */
    void placeFace(const vector<vector<char>>& colors, int face);

    /**
     * Places a given color segment in a given row of a face.
     * @param colors The color to place into the face.
     * @param face The index of the face to receive the new colors.
     * @param row The row to place the colors in.
     */
    void placeRow(const vector<char>& colors, int face, int row);

    /**
     * Places a given color segment in a given column of a face.
     * @param colors The color to place into the face.
     * @param face The index of the face to receive the new colors.
     * @param col The column to place the colors in.
     */
    void placeCol(const vector<char>& colors, int face, int col);

    /**
     * Gets a row of a face.
     * @param face The index of the face to check.
     * @param row The row of the face.
     * @return The colors of a segment in a face.
     */
    vector<char> getRow(int face, int row) const;

    /**
     * Gets a column of a face.
     * @param face The index of the face to check.
     * @param col The column of the face.
     * @return The colors of a segment in a face.
     */
    vector<char> getCol(int face, int col) const;

    /**
     * Rotates the colors of a given face in a clockwise fashion.
     * @param face The index of the face to rotate.
     */
    void rotateFace(int face);

    /**
     * Performs a clockwise turn on the top side, rotating it.
     * Switches the topmost rows of the left, front, right, and back faces.
     */
    void turnU();

    /**
     * Performs a clockwise turn on the left side, rotating it.
     * Switches the columns of the top (leftmost), front (leftmost), bottom (leftmost), and back (rightmost) faces.
     */
    void turnL();

    /**
     * Performs a clockwise turn on the front side, rotating it.
     * Switches the top row (bottom), right column (leftmost), bottom row (topmost), and left column (rightmost) faces.
     */
    void turnF();

    /**
     * Performs a clockwise turn on the right side, rotating it.
     * Switches the columns of the top (rightmost), back (leftmost), bottom (rightmost), and front (rightmost) faces.
     */
    void turnR();

    /**
     * Performs a clockwise turn on the back side (looks counterclockwise from the front), rotating it.
     * Switches the top row (topmost), left column (leftmost), bottom row (bottom), and right (rightmost) faces.
     */
    void turnB();

    /**
     * Performs a clockwise turn on the bottom side.
     * Switching the bottom rows on left, front, right, and back faces.
     */
    void turnD();

    /**
     * Rotates the Cube clockwise along the x-axis.
     * Rotates the left face counterclockwise and the right face clockwise.
     * Switches the middle faces in a clockwise fashion (front -> top, bottom -> front, back -> bottom, top -> back).
     */
    void rotX();

    /**
     * Rotates the Cube clockwise along the y-axis.
     * Rotates the top face clockwise and the bottom face counterclockwise.
     * Switches the middle faces in a clockwise fashion (front -> left, right -> front, back -> right, left -> back).
     */
    void rotY();

    /**
     * Returns a colored representation of a face's segment.
     * @param side The face's side.
     * @param width The width of the individual segments.
     * @param row The row of the segment to print.
     * @return A string representing a segment of a face.
     */
    string printFaceSegment(int side, int width, int row) const;

    /**
     * Returns a specified number of spaces.
     * @param spaces The number of spaces.
     * @return A string of spaces.
     */
    string printSpacing(int spaces) const;

    /**
     * Returns a segment of a command display with spacing and advances a counter to the next line.
     * @param spacing The number of spaces to include.
     * @param counter The current line.
     * @param print Whether or not the segment should be returned.
     */
    string printCommandWithSpacing(int spacing, int& counter, bool print) const;

    /**
     * Returns a segment of the command display.
     * @param line The line to return.
     * return A line of the command display.
     */
    string printCommandSegment(int line) const;

    /**
     * Prints the current moves.
     */
    void printCurrentMoves() const;

    char stickers[NUM_FACES][SIZE][SIZE];
    string name;
    string scramble;
    string moves;
    stack<string> currentMoves;
    int totalMoves;
};

#endif