#ifndef CUBE_H
#define CUBE_H

#include "sticker_data.h"
#include <string>
#include <stack>
#include <map>
#include <set>
#include <vector>
#include <utility>
using namespace std;

class Cube {
public:
    enum Faces { TOP, LEFT, FRONT, RIGHT, BACK, BOTTOM };
    static const set<char> VALID_MOVES;
    static const map<char, string> COLOR_STRINGS;
    static const map<int, string> FACE_STRINGS;
    static const int MAX_DISPLAY = 100;
    
    /**
     * Creates a string representing the given data.
     * @param map The map to use.
     * @param keys The keys to use.
     * @return A string with the given data.
     */
    template <typename T1, typename T2>
    static string getData(const map<T1, T2>& map, const vector<T1>& keys) {
        string res = "";

        for (int i = 0; i < keys.size(); i++) {
            T1 key = keys.at(i);

            if (map.count(key) != 0) {
                res += map.at(key) + (i < keys.size() - 1 ? "-" : ""); // Use hyphens to separate words
            } else {
                break;
            }
        }

        return res;
    }

	/**
	 * Returns a string representing the given colors.
	 * @param colors The colors to represent.
	 * @return A string with the given colors.
	 */
	static string getColors(const vector<char>& colors);

	/**
	 * Returns a string representing the given faces.
	 * @param faces The faces to represent.
	 * @return A string with the given faces.
	 */
	static string getFaces(const vector<int>& faces);

	/**
	 * Checks if a pair of colors are equal to another pair of colors.
	 * @param colors1 One pair of colors.
	 * @param colors2 Another pair of colors.
	 * @return Whether or not the stickers match.
	 */
	static bool checkColors(const pair<char, char>& colors1, const pair<char, char>& colors2);

    /**
      * Checks whether the set of moves is valid (i.e. no unpaired numbers or apostrophes, only valid letters)
      * @param moves A set of moves.
      * @return Whether or not the moves are valid.
      */
    static bool checkMoves(const string& moves);
    
    /**
     * Separates a sequence with spaces.
     * @param moves The sequence to tokenize.
     * @return The tokenizes sequence.
     */
    static string tokenizeMoves(const string& moves);

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
     * Gets the colors at a specified location.
     * @param face The index of the face to check.
     * @param row The row to check.
     * @param col The column to check.
     * @return A color at the location.
     */
    char getAt(int face, int row, int col) const;
    
    /**
     * Locates a face that contains a specified center color.
     * @param color The character representing the desired color.
     * @return The index of the face containing the color.
     */
    int findCenter(char color) const;
    
	/**
	 * Locates an edge based on two colors, returning its StickerData.
	 * @param color1 The first color.
	 * @param color2 The second color.
	 * @return StickerData of the found edge.
	 */
	StickerData findEdge(char color1, char color2) const;

    /**
     * Gets the data of the adjacent edge of a given edge.
     * @param face The index of the face of the given edge.
     * @param row The row of the given edge.
     * @param col The column of the given edge.
     * @return StickerData containing the data of the adjacent edge.
     */
    StickerData getAdjEdge(int face, int row, int col) const;

	/**
	 * Checks if an edge has a certain color, and if its adjacent color matches their face's center color.
	 * @param face The face of the edge.
     * @param color The color that the edge must have.
	 * @param coord A coordinate on the face, whose adjacent sticker needs to be checked.
	 * @return True if the edge has the given color and if its adjacent color matches.
	 */
	bool checkEdgeMatch(int face, char color, const pair<int, int>& coord) const;

	/**
	 * Locates a corner with the given criteria.
	 * @param baseColor The base color of the sticker.
	 * @param adjColors The adjacent colors of the sticker.
	 * @return A pair containing StickerData and a pair of StickerData, representing the base sticker and adjacent stickers respectively.
	 */
	pair<StickerData, pair<StickerData, StickerData>> findCorner(char baseColor, const pair<char, char>& adjColors) const;
    
    /**
     * Gets the data of stickers adjacent to a given corner sticker.
     * @param face The index of the face to check.
     * @param row The row of the sticker.
     * @param col The column of the sticker.
     * @return A pair containing the data of two adjacent edges to a corner.
     */
    pair<StickerData, StickerData> getAdjCorners(int face, int row, int col) const;

    /**
     * Gets the two side-facing (not top or bottom) sides of a corner.
     * @param corner A vector of StickerData of a corner.
     * @return A pair of indices whose elements face to the sides.
     */
    pair<int, int> getSideCorners(const vector<StickerData>& corner) const;

	/**
	 * Checks if a corner is in its correct spot.
	 * @param baseColor The base color.
	 * @param colors The colors adjacent to the base color.
	 * @return Whether or not the corner is in its correct spot.
	 */
	bool checkCornerPosition(char baseColor, const pair<char, char>& colors) const;

    /**
     * Checks if the Cube is solved (all faces contain one color).
     * @return Whether or not the Cube is solved.
     */
    bool checkSolved() const;

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
     * Undoes a move, returning the result of the undo.
     * @return A string that displays the result of the action.
     */
    string undo();

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
    static const map<int, char> FACE_COLORS;
    static const int NUM_FACES = 6;
    static const int SIZE = 3;

    /**
     * Sets the Cube to a solved state.
     */
    void createSolved();

    /**
     * Processes a valid move.
     * @param letter A letter representing a valid move on the Rubik's cube.
     */
    void processMove(char letter);

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
     * Gets a face of the Cube.
     * @param face The index of the face to get.
     * @return The colors of a specified face.
     */
    vector<vector<char>> getFace(int face) const;

    /**
     * Gets a face of the Cube, read bottom-top, left-right to help with translation of faces during z rotations.
     * @param face The index of the face to get.
     * @return The colors of a specified face.
     */
    vector<vector<char>> getZFace(int face) const;

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
     * Rotates the Cube clockwise along the z-axis.
     * Rotates the front face clockwise and the back face counterclockwise.
     * Switches the middle faces in a clockwise fashion (top -> right, left -> top, bottom -> left, right -> bottom).
     */
    void rotZ();

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
     * @return A string with the current moves.
     */
    string printCurrentMoves() const;

    /**
     * Creates a string dependent on the number of moves to prevent intrusive display.
     * @param moves The moves to be checked.
     * @return A string that either contains the moves or indicates the count.
     */
    string showMoves(const string& moves) const;

    char stickers[NUM_FACES][SIZE][SIZE];
    string name;
    string scramble;
    string moves;
    stack<string> currentMoves;
    int totalMoves;
};

#endif