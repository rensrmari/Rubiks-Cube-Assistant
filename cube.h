#ifndef CUBE_H
#define CUBE_H

#define WHITE_BG "\033[107m"
#define RED_BG "\033[101m"
#define GREEN_BG "\033[102m"
#define BLUE_BG "\033[94m"
#define ORANGE_BG "\033[105m"
#define YELLOW_BG "\033[103m"
#define WHITE_FG "\033[97m"
#define RED_FG "\033[91m"
#define GREEN_FG "\033[92m"
#define BLUE_FG "\033[94m"
#define ORANGE_FG "\033[96m"
#define YELLOW_FG "\033[93m"
#define RESET "\033[0m"

#include <string>
#include <stack>
#include <map>
using namespace std;

enum Faces { TOP, LEFT, FRONT, RIGHT, BACK, BOTTOM };

class Cube {
public:
    static const string VALID_MOVES;

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
    Cube(const string& name, const string& scramble, const string& moves, const int totalMoves);

    /**
     * Displays the state of the cube. //TODO: display commands.
     */
    void displayCube() const;

    /**
     * Perform a sequence of moves on the cube.
     * @param moves A set of moves.
     */
    void doMoves(const string& moves);
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
     * Returns a colored representation of a face's segment.
     * @param side The face's side.
     * @param row The row of the segment to print.
     * @return A string representing a segment of a face.
     */
    string printFaceSegment(const int side, const int row) const;

    /**
     * Prints a specified number of spaces.
     * @param spaces The number of spaces.
     * @return A string of spaces.
     */
    string printSpacing(const int spaces) const;

    // TODO: printfront?

    char stickers[NUM_FACES][SIZE][SIZE];

    string name;
    string scramble;
    string moves;
    stack<string> newMoves;
    int totalMoves;
};

/**
  * Checks whether the set of moves is valid.
  * @param A set of moves.
  * @return Whether or not the moves are valid.
  */
bool checkMoves(const string& moves);

#endif