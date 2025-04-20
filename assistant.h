#ifndef ASSISTANT_H
#define ASSISTANT_H

#include "cube.h"
#include "sticker_data.h"
#include <string>
#include <vector>
#include <map>
#include <utility>
using namespace std;

class Assistant {
public:
	enum Stages { WHITE_CROSS, WHITE_CORNERS, SECOND_LAYER, YELLOW_CROSS, YELLOW_EDGES, YELLOW_CORNERS_POSITION, YELLOW_CORNERS_ORIENTATION, ADJUST_UPPER_FACE };

	/**
	 * Constructor for Assistant object.
	 * @param cube The Cube to access.
	 */
	Assistant(Cube& cube);

	/**
	 * Solves the Cube, displaying steps for each stage.
	 */
	void solve();

	/**
	 * Displays a hint based on the current stage and the next move.
	 */
	void printHint() const;
private:
	/**
	 * Processes a sequence and displays the result.
	 * @param sequence The sequence to process.
	 * @param message The message to output if there was a sequence performed.
	 */
	void processSequence(const string& sequence, const string& message);

	/**
	 * Simplfies clockwise sequences (triple moves -> prime, two moves have 2 appended).
	 * @param sequence The sequence to simplify.
	 * @return A string containing the simplified sequence.
	 */
	string simplifySequence(const string& sequence) const;

	/**
	 * Returns a sequence that will result in a face in a given position.
	 * @param face The original face to move, which will be updated.
	 * @param newFace Where the original face should end up.
	 * @param onlyYMoves Whether or not the rotation should only include y rotations, which is required for some processes.
	 * @return A string containing rotations that result in the desired position.
	 */
	string rotateToFace(int& face, int newFace, bool onlyYMoves) const;

	/**
	 * Picks an outward facing sticker to use, meaning it must not be facing the top or bottom.
	 * @param stickers The stickers to consider.
	 * @return The StickerData whose face is not top or bottom.
	 */
	StickerData pickOutward(const vector<StickerData>& stickers) const;

	/**
	 * Turns an edge to the bottom and updates its coordinates.
	 * @param row The row of the edge.
	 * @param col The column of the edge.
	 * @return A string containing a sequence that will result in an edge in the bottom of the front face.
	 */
	string turnEdgeToBottom(int& row, int& col) const;

	/**
	 * Turns an edge to the top and updates its coordinates during the second layer solution.
	 * @param row The row of the edge.
	 * @param col The column of the edge.
	 * @return A string containing a sequence that will result in an edge on top.
	 */
	string turnEdgetoTop(int& row, int& col) const;

	/**
	 * Returns a sequence that will result in a edge in a given face, assuming it is the top or bottom on the front face.
	 * @param face The face the sticker is on, which will be updated.
	 * @param newFace The face to position the sticker on.
	 * @param row The row of the sticker.
	 * @param col The column of the sticker.
	 * @param isOnTop Whether or not the sticker is on the top.
	 * @return A string containing a sequence that will result in the edge being in the given face.
	 */
	string turnEdgeToFace(int& face, int newFace, int row, int col, bool isOnTop) const;

	/**
	 * Turns a corner to the bottom and updates its coordinates.
	 * @param row The row of the corner.
	 * @param col The column of the corner.
	 * @return A string containing a sequence that will reuslt in the corner in the bottom.
	 */
	string turnCornerToBottom(int& row, int& col) const;

	/**
	 * Turns a corner to the given pair of faces.
	 * @param baseColor The base color.
	 * @param colors The colors adjacent to the base color.
	 * @param face The original face of the corner, which will be updated.
	 * @param isOnTop Whether or not the corner is on top.
	 * @return A string containing the sequence that will result in the corner being correctly positioned.
	 */
	string turnCornerToFaces(char baseColor, const pair<char, char>& colors, int& face, bool isOnTop) const;

	/**
	 * Gets a side color of a corner.
	 * @param color The color to use and disregard later.
	 * @param adjColors The adjacent colors of the corner.
	 * @return A color of the corner facing the side that is not the specified color.
	 */
	char getSideColor(char color, const pair<char, char>& adjColors) const;

	/**
	 * Prints a message indicating that a process has completed.
	 * @param message The message to print.
	 */
	void printComplete(const string& message) const;

	/**
	 * Checks the current stage of the solve.
	 * @return A number representing the current stage.
	 */
	int checkStage() const;

	/**
	 * Checks if the white cross has been completed.
	 * @return True if it has been completed.
	 */
	bool checkWhiteCross() const;

	/**
	 * Checks if an edge of the white cross is in its correct spot.
	 * @param face The index of the face of the white cross.
	 * @param row The row of the edge to check.
	 * @param col The column of the edge to check.
	 * @return Whether or not the edge is in its correct spot in the white cross.
	 */
	bool checkWhiteCrossEdge(int face, int row, int col) const;

	/**
	 * Attempts to achieve a white cross on the Cube.
	 */
	void getWhiteCross();

	/**
	 * Checks if the white corners have been completed.
	 * @return True if they have been completed.
	 */
	bool checkWhiteCorners() const;

	/**
	 * Checks if a corner of the white corners is in its correct spot and correctly oriented.
	 * @param face The index of the face to check.
	 * @param row The row to check.
	 * @param col The column to check.
	 * @return Whether or not the corner is in its correct spot.
	 */
	bool checkWhiteCorner(int face, int row, int col) const;

	/**
	 * Attempts to achieve white corners on the Cube.
	 */
	void getWhiteCorners();

	/**
	 * Checks if the second layer has been completed.
	 * @return True if it has been completed.
	 */
	bool checkSecondLayer() const;

	/**
	 * Checks if an edge of the second layer is correctly positioned and oriented.
	 * @param faces The faces of the edge.
	 * @param cols The columns of the edge.
	 * @return True if the edge is correct.
	 */
	bool checkSecondLayerEdge(const pair<int, int>& faces, const pair<int, int>& cols) const;

	/**
	 * Checks if an edge of the second layer is correctly positioned,
	 * which would be achieved by having its front-facing sticker matching the center
	 * of the front face, or by having being already situated in its correct spot,
	 * but needing to be oriented correctly.
	 * @param frontSticker The data associated with the front-facing sticker.
	 * @param adjSticker The data of the adjacent sticker.
	 * @return Whether or not the edge is correctly positioned.
	 */
	bool checkSecondLayerPosition(const StickerData& frontEdge, const StickerData& adjEdge) const;

	/**
	 * Attempts to complete the second layer.
	 */
	void getSecondLayer();

	/**
	 * Checks if the yellow cross has been completed.
	 * @return True if it has been completed.
	 */
	bool checkYellowCross() const;

	/**
	 * Attempts to achieve a yellow cross.
	 */
	void getYellowCross();
	
	/**
	 * Checks for an L-shape during the yellow cross.
	 * @return A pair containing whether or not the shape was found,
	 * as well as the sequence to position the shape for its algorithm to work.
	 */
	pair<bool, string> checkLShape() const;

	/**
	 * Checks for a line shape during the yellow cross.
	 * @return A pair containing whether or not the shape was found,
	 * as well as the sequence to position the shape for its algorithm to work.
	 */
	pair<bool, string> checkLineShape() const;

	/**
	 * Checks for a dot shape during the yellow cross.
	 * @return Whether or not the shape was found.
	 */
	bool checkDotShape() const;

	/**
	 * Checks if the yellow edges have been oriented correctly (matching the center colors of their faces).
	 * @return True if they have been correctly oriented.
	 */
	bool checkYellowEdges() const;

	/**
	 * Returns a sequence that results in two yellow edges matching their face's center colors.
	 * @param face The face of the yellow edges.
	 * @return A sequence of moves, and the data of the matching edges.
	 */
	pair<string, vector<StickerData>> matchYellowEdges() const;

	/**
	 * Returns a string that would rotate the Cube so that algorithms will properly swap yellow edges.
	 * @param matchInfo The way the matching yellow edges are set up.
	 * @param matchCoords The colors of the incorrect edges.
	 * @return A sequence that will result in properly positioned yellow edges.
	 */
	string prepareYellowEdges(const string& matchInfo, const pair<char, char>& matchCoords) const;

	/**
	 * Attemps to achieve correctly oriented yellow edges.
	 */
	void getYellowEdges();

	/**
	 * Checks if the yellow corners have been positioned correctly.
	 * @return True if they have been correctly positioned.
	 */
	bool checkYellowCornersPosition() const;

	/**
	 * Checks if a yellow corner is in its correct position.
	 * @param face The face of the corner.
	 * @param row The row to check.
	 * @param col The column to check.
	 * @return True if it is in its correct position.
	 */
	bool checkYellowCornerPosition(int face, int row, int col) const;

	/**
	 * Attempts to correct the yellow corner positioning.
	 */
	void getYellowCornersPosition();

	/**
	 * Returns a string that would rotate the Cube, positioning a correct yellow corner in the top, right, front spot.
	 * @param corner A vector of stickers that represent the corner.
	 * @return A sequence that will result in this corner being positioned in this spot.
	 */
	string positionRightCorner(const vector<StickerData>& corner) const;

	/**
	 * Checks if the yellow corners have been oriented correctly.
	 * @return True if they have been correctly oriented.
	 */
	bool checkYellowCornersOrientation() const;

	/**
	 * Attempts to orient yellow corners correctly.
	 */
	void getYellowCornersOrientation();

	/**
	 * Finds an incorrectly oriented yellow corner.
	 * @param useUMoves Whether or not U moves should be used to position this corner in the top, right, front spot.
	 * @return Whether or not the corner could be found, the sequence that will position the corner correctly,
	 * and the colors of the corner.
	 */
	pair<bool, pair<string, vector<char>>> findNotOriented(bool useUMoves) const;

	/**
	 * Corrects the orientation of a yellow corner in the top, right, front position.
	 * @return A sequence that will result in the yellow corner being oriented up.
	 */
	string correctOrientation() const;

	Cube* cube;
};

#endif