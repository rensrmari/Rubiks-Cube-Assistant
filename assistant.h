#ifndef ASSISTANT_H
#define ASSISTANT_H

#include "cube.h"
#include "sticker_data.h"
#include <string>
using namespace std;

class Assistant {
public:
	enum Stages { WHITE_CROSS, WHITE_CORNERS, SECOND_LAYER, YELLOW_CROSS, YELLOW_CORNERS_POSITION, YELLOW_CORNERS_ORIENTATION, ADJUST_UPPER_FACE };

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
	 * Returns the next move.
	 */
	string getNextMove();

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
	 * @param face The original face to move.
	 * @param newFace Where the original face should end up.
	 * @param onlyYMoves Whether or not the rotation should only include y rotations, which is required for some processes.
	 * @return A string containing rotations that result in the desired position.
	 */
	string rotateToFace(int face, int newFace, bool onlyYMoves) const;

	/**
	 * Returns a sequence that will result in a edge in a given face, assuming it is the top or bottom on the front face.
	 * @param face The face the sticker is on.
	 * @param newFace The face to position the sticker on.
	 * @param row The row of the sticker.
	 * @param col The column of the sticker.
	 * @param isOnTop Whether or not the sticker is on the top.
	 * @return A string containing a sequence that will result in the edge being in the given face.
	 */
	string turnEdgeToFace(int face, int newFace, int row, int col, bool isOnTop) const;

	/**
	 * Gets the data of the adjacent edge of a given edge.
	 * @param cube The cube to check.
	 * @param face The index of the face of the given edge.
	 * @param row The row of the given edge.
	 * @param col The column of the given edge.
	 * @return StickerData containing the data of the adjacent edge.
	 */
	StickerData getAdjEdge(const Cube& cube, int face, int row, int col) const;

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
	 * Returns a sequence that will result in an edge on the bottom during the white cross process, assuming it is in front.
	 * @param row The row of the edge.
	 * @param col The column of the edge.
	 * @return A string containing a sequence that will result in an edge in the bottom of the front face.
	 */
	string turnEdgeToBottom(int row, int col) const;

	/**
	 * Corrects the face if there were y rotations.
	 * @param face The original face.
	 * @param rotations The rotations that were applied.
	 */
	void fixFace(int& face, const string& rotations) const;

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
	 * Attempts to achieve white corners on the Cube.
	 */
	void getWhiteCorners();

	/**
	 * Checks if the second layer has been completed.
	 * @return True if it has been completed.
	 */
	bool checkSecondLayer() const;

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
	 * Checks if the yellow corners have been positioned correctly.
	 * @return True if they have been correctly positioned.
	 */
	bool checkYellowCornersPosition() const;

	/**
	 * Attempts to correct the yellow corner positioning.
	 */
	void getYellowCornersPosition();

	/**
	 * Checks if the yellow corners have been oriented correctly.
	 * @return True if they have been correctly oriented.
	 */
	bool checkYellowCornersOrientation() const;

	/**
	 * Attempts to orient yellow corners correctly.
	 */
	void getYellowCornersOrientation();

	Cube* cube;
};

#endif