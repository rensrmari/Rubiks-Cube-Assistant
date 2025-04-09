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
     * @param scramble A string representing the original scramble of the cube.
     * @param moves A string of applied moves.
     * @param totalMoves The total number of moves.
     */
    Cube(const string& name, const string& scramble, const string& moves, int totalMoves);

    /**
     * Gets the name of the cube.
     * @return The name of the cube.
     */
    string getName() const;

    /**
     * Sets the name of the cube.
     * @param name The name of the cube.
     */
    void setName(const string& name);

    /**
     * Sets the scramble of the cube.
     * @param scramble The scramble.
     */
    void setScramble(const string& scramble);

    /**
     * Gets the move count of the cube.
     * @return The total moves.
     */
    int getTotalMoves() const;

    /**
     * Displays the state of the cube. //TODO: display commands.
     */
    void displayCube() const;

    /**
     * Performs a sequence of moves on the cube.
     * @param moves A set of moves.
     * @return A string representing the moves, each separated by spaces.
     */
    string doMoves(const string& moves);

    /**
     * Resets the Cube object.
     */
    void reset();
private:
    static const int NUM_FACES = 6;
    static const int SIZE = 3;
    static const map<char, string> COLOR_STRINGS;
    static const map<int, char> FACE_COLORS;

    /**
     * Sets the cube to a solved state.
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
     * Places a given color segment in a given row of a face.
     * @param face The index of the face to receive the new colors.
     * @param colors The color to place into the face.
     * @param row The row to place the colors in.
     */
    void placeRow(int face, vector<char> colors, int row);

    /**
     * Places a given color segment in a given column of a face.
     * @param face The index of the face to receive the new colors.
     * @param colors The color to place into the face.
     * @param col The column to place the colors in.
     */
    void placeCol(int face, vector<char> colors, int col);

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
     * Undoes a move TODO: stack
     */


    /**
     * Returns a colored representation of a face's segment.
     * @param side The face's side.
     * @param width The width of the individual segments.
     * @param row The row of the segment to print.
     * @return A string representing a segment of a face.
     */
    string printFaceSegment(int side, int width, int row) const;

    /**
     * Prints a specified number of spaces.
     * @param spaces The number of spaces.
     * @return A string of spaces.
     */
    string printSpacing(int spaces) const;

    char stickers[NUM_FACES][SIZE][SIZE];
    string name;
    string scramble;
    string moves;
    stack<string> newMoves;
    int totalMoves;
};

#endif