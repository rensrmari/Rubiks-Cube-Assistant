#include "assistant.h"
#include "cube.h"
#include <utility>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;

Assistant::Assistant(Cube& cube) {
    this->cube = &cube;
}

void Assistant::solve() { // bool for hint? marker for each step, then return? pass down to all functions? then, take out strings. !("")
    switch (checkStage()) {
        case WHITE_CROSS:
            getWhiteCross();
            break;
        case WHITE_CORNERS:
            getWhiteCorners();
            break;
        case SECOND_LAYER:
            getSecondLayer();
            break;
        case YELLOW_CROSS:
            getYellowCross();
            break;
        case YELLOW_CORNERS_POSITION:
            getYellowCornersPosition();
            break;
        case YELLOW_CORNERS_ORIENTATION:
            getYellowCornersOrientation();
            break;
    }
}

string Assistant::getNextMove() {
return "";
}


void Assistant::processSequence(const string& sequence, const string& message) {
    cube->doMoves(sequence, true);

    if (sequence.length() != 0) {
        cout << "\n\n" << message << endl << "Moves: " << Cube::tokenizeMoves(sequence);
        cube->displayState(false);
    }
}

string Assistant::simplifySequence(const string& sequence) const {
    if (sequence.length() == 3) { // Convert triple moves to a single prime 
        return string(1, sequence[0]) + "'";
    } else if (sequence.length() == 2) { // Make two moves into a double move
        return string(1, sequence[0]) + "2";
    } else {
        return sequence;
    }
}

string Assistant::rotateToFace(int face, int newFace, bool onlyYMoves) const {
    string sequence = "";
    string tempSequence = "";
    Cube temp = *cube;
    int rotationsToCheck = 3;
    char originalCenter = cube->stickers[face][1][1]; // Keep track of original face's center, as it won't change

    // All rotations will be checked to attain the desired center color in a given face and therefore the original face.
    while (rotationsToCheck > 0) {
        if (temp.stickers[newFace][1][1] == originalCenter) {
            sequence = simplifySequence(tempSequence);
            break;
        }

        if (tempSequence.length() == 3) { // Stop at 3 moves and perform the 4th move to reset the state
            temp.doMoves(tempSequence.substr(0, 1), false);
            tempSequence = "";
            rotationsToCheck--;
        }

        string toAdd;
        if (rotationsToCheck == 3) {
            toAdd = onlyYMoves ? " " : "x";
        } else if (rotationsToCheck == 2) {
            toAdd = "y";
        } else {
            toAdd = onlyYMoves ? " " : "z";
        }

        tempSequence += toAdd;
        temp.doMoves(toAdd, false); // Apply the move and check if it results in the color being on the face
    }

    return sequence;
}

string Assistant::turnEdgeToFace(int face, int newFace, int row, int col, bool isOnTop) const {
    string sequence = "";
    string tempSequence = "";
    string moveToUse = isOnTop ? "U" : "D";
    Cube temp = *cube;
    char originalColor1 = cube->stickers[face][row][col];
    char originalColor2 = getAdjEdge(*cube, face, row, col).color;
    int turnsToCheck = 3;

    while (turnsToCheck > 0) {
        turnsToCheck--;

        // Check if the edge is in the correct spot.
        char color1 = temp.stickers[newFace][row][col];
        char color2 = getAdjEdge(temp, newFace, row, col).color;

        if (originalColor1 == color1 && originalColor2 == color2
        || originalColor1 == color2 && originalColor2 == color1) {
            sequence = simplifySequence(tempSequence);
            break;
        }

        tempSequence += moveToUse;
        temp.doMoves(moveToUse, false);
    }

    return sequence;
}

StickerData Assistant::getAdjEdge(const Cube& cube, int face, int row, int col) const {
    StickerData res;

    // Determine the face.
    if (face == Cube::TOP) {
        res.row = 0;
        res.col = 1;

        if (row == 0 && col == 1) { // Top edge -> top of back face
            res.face = Cube::BACK;
        } else if (row == 1 && col == 0) { // Left edge -> top of left face
            res.face = Cube::LEFT;
        } else if (row == 1 && col == 2) { // Right edge -> top of right face
            res.face = Cube::RIGHT;
        } else { // Bottom edge -> top of front face
            res.face = Cube::FRONT;
        }
    } else if (face == Cube::LEFT) {
        if (row == 0 && col == 1) { // Top edge -> left of top face
            res.face = Cube::TOP;
            res.row = 1;
            res.col = 0;
        } else if (row == 1 && col == 0) { // Left edge -> right of back face
            res.face = Cube::BACK;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 2) { // Right edge -> left of front face
            res.face = Cube::FRONT;
            res.row = 1;
            res.col = 0;
        } else { // Bottom edge -> left of bottom face
            res.face = Cube::BOTTOM;
            res.row = 1;
            res.col = 0;
        }
    } else if (face == Cube::FRONT) {
        if (row == 0 && col == 1) { // Top edge -> bottom of the top face
            res.face = Cube::TOP;
            res.row = 2;
            res.col = 1;
        } else if (row == 1 && col == 0) { // Left edge -> right of left face
            res.face = Cube::LEFT;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 2) { // Right edge -> left of right face
            res.face = Cube::RIGHT;
            res.row = 1;
            res.col = 0;
        } else { // Bottom edge -> top of bottom face
            res.face = Cube::BOTTOM;
            res.row = 0;
            res.col = 1;
        }
    } else if (face == Cube::RIGHT) {
        if (row == 0 && col == 1) { // Top edge -> right of top face
            res.face = Cube::TOP;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 0) { // Left edge -> right of front face
            res.face = Cube::FRONT;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 2) { // Right edge -> left of back face
            res.face = Cube::BACK;
            res.row = 1;
            res.col = 0;
        } else { // Bottom edge -> right of bottom face
            res.face = Cube::BOTTOM;
            res.row = 1;
            res.col = 2;
        }
    } else if (face == Cube::BACK) {
        if (row == 0 && col == 1) { // Top edge -> top of top face
            res.face = Cube::TOP;
            res.row = 0;
            res.col = 1;
        } else if (row == 1 && col == 0) { // Left edge -> right of right face
            res.face = Cube::RIGHT;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 2) { // Right edge -> left of left face
            res.face = Cube::LEFT;
            res.row = 1;
            res.col = 0;
        } else { // Bottom edge -> bottom of bottom face
            res.face = Cube::BOTTOM;
            res.row = 2;
            res.col = 1;
        }
    } else {
        res.row = 2;
        res.col = 1;

        if (row == 0 && col == 1) { // Top edge -> bottom of front face
            res.face = Cube::FRONT;
        } else if (row == 1 && col == 0) { // Left edge -> bottom of left face
            res.face = Cube::LEFT;
        } else if (row == 1 && col == 2) { // Right edge -> bottom of right face
            res.face = Cube::RIGHT;
        } else { // Bottom edge -> bottom of back face
            res.face = Cube::BACK;
        }
    }

    res.color = cube.stickers[res.face][res.row][res.col];
    return res;
}

int Assistant::findCenter(char color) const {
	for (int i = 0; i < Cube::NUM_FACES; i++) {
		if (cube->stickers[i][1][1] == color) {
			return i;
		}
	}

	return -1;
}

StickerData Assistant::findEdge(char color1, char color2) const {
    StickerData res;

    vector<pair<int, int>> coords = { // Store row and columns that need to be checked
        make_pair(0, 1),
        make_pair(1, 0),
        make_pair(1, 2),
        make_pair(2, 1)
    };
    
    // Check each face.
    for (int i = 0; i < Cube::NUM_FACES; i++) {
        for (int j = 0; j < coords.size(); j++) { // Check each edge
            pair<int, int> coord = coords[j];
            char edge = cube->stickers[i][coord.first][coord.second];
            char adjEdge = getAdjEdge(*cube, i, coord.first, coord.second).color;
            
            if (edge == color1 && adjEdge == color2 || edge == color2 && adjEdge == color2) {
                res.face = i;
                res.color = edge;
                res.row = coord.first;
                res.col = coord.second;
                break;
            }
        }
    }

    return res;
}


string Assistant::turnEdgeToBottom(int row, int col) const {
    string sequence = "";

    if (row == 0 && col == 1) { // Edge is on top and needs a double move
        sequence = "F2";
    } else if (row == 1 && col == 0) { // Edge is on the left and only needs a counterclockwise move
        sequence = "F'";
    } else if (row == 1 && col == 2) { // Edge is on the right and only needs a clockwise move
        sequence = "F";
    } else { // Edge is already on the bottom
        sequence = "";
    }

    return sequence;
}

void Assistant::fixFace(int& face, const string& rotations) const {
    // The faces affected by y rotations are 1-4.
    if (rotations == "y") {
        face = 1 + abs(face - 1) % 5;
    } else if (rotations == "y'") {
        face = 1 + (face + 1) % 5;
    } else {
        face = 1 + (face + 2) % 5;
    }
}

int Assistant::checkStage() const {
    if (!checkWhiteCross()) { return Stages::WHITE_CROSS; }
    if (!checkWhiteCorners()) { return Stages::WHITE_CORNERS; }
    if (!checkSecondLayer()) { return Stages::SECOND_LAYER; }
    if (!checkYellowCross()) { return Stages::YELLOW_CROSS; }
    if (!checkYellowCornersPosition()) { return Stages::YELLOW_CORNERS_POSITION; }
    if (!checkYellowCornersOrientation()) { return Stages::YELLOW_CORNERS_ORIENTATION; }

    return Stages::ADJUST_UPPER_FACE; // Only the upper face needs to be turned to solve
}

bool Assistant::checkWhiteCross() const {
    int face = findCenter('W');
    
    if (checkWhiteCrossEdge(face, 0, 1) && checkWhiteCrossEdge(face, 1, 0)
    && checkWhiteCrossEdge(face, 1, 2) && checkWhiteCrossEdge(face, 2, 1)) {
        return true;
    }
    
    return false;
}

bool Assistant::checkWhiteCrossEdge(int face, int row, int col) const {
    if (cube->stickers[face][row][col] == 'W') { // First check if the edge is white
        StickerData adjEdge = getAdjEdge(*cube, face, row, col);

        if (adjEdge.color == cube->stickers[adjEdge.face][1][1]) { // Check if the adjacent color matches its face's center
            return true;
        }
    }

    return false;
}

void Assistant::getWhiteCross() {
    // Make the white side face up.
    int face = findCenter('W');
    processSequence(rotateToFace(face, Cube::TOP, false), "[WHITE CROSS] Get the white center on the top.");

    // Position the four white edges next to the white center, with the adjacent colors matching the centers of the sides.
    vector<StickerData> whiteCrossEdges = {
        { Cube::TOP, cube->stickers[Cube::TOP][0][1], 0, 1 }, // Top edge
        { Cube::TOP, cube->stickers[Cube::TOP][1][2], 1, 2 }, // Right edge
        { Cube::TOP, cube->stickers[Cube::TOP][2][1], 2, 1 }, // Front edge
        { Cube::TOP, cube->stickers[Cube::TOP][1][0], 1, 0 }  // Left edge
    };
    vector<int> adjFaces = { Cube::BACK, Cube::RIGHT, Cube::FRONT, Cube::LEFT };
    int i = 0;

    while (!checkWhiteCross()) {
        StickerData edge = whiteCrossEdges[i]; // An edge slot to possibly correct
        int adjFace = adjFaces[i]; // The face the edge slot should be on
        string rotations;

        // Check if the edge is in its correct position.
        if (!checkWhiteCrossEdge(edge.face, edge.row, edge.col)) {
            // If the edge is not correct, locate the correct edge (white and a color that matches an adjacent center color).
            StickerData correctEdge = findEdge('W', cube->stickers[adjFace][1][1]);
            StickerData adjEdge = getAdjEdge(*cube, correctEdge.face, correctEdge.row, correctEdge.col);
            StickerData& useEdge = (correctEdge.face == Cube::BOTTOM || correctEdge.face == Cube::TOP) ? adjEdge : correctEdge; // Only use the edge that appears in front

            // Rotate the Cube so that the edge is in front.
            string edgeColors = Cube::COLOR_STRINGS.at(correctEdge.color) + "-" + Cube::COLOR_STRINGS.at(adjEdge.color);
            rotations = rotateToFace(useEdge.face, Cube::FRONT, true);
            processSequence(rotations, "[WHITE CROSS] Rotate the cube so that the " + edgeColors + " edge is in front.");
            useEdge.face = Cube::FRONT;
            fixFace(adjFace, rotations);

            // Position the edge on the bottom so it can be turned to the correct face.
            if (useEdge.face != adjFace) {
                processSequence(turnEdgeToBottom(useEdge.row, useEdge.col), "[WHITE CROSS] Position the " + edgeColors + " edge on the bottom.");
                useEdge.row = 2;
                useEdge.col = 1;

                // Turn the bottom face to position it on the correct face later.
                processSequence(turnEdgeToFace(useEdge.face, adjFace, useEdge.row, useEdge.col, false), "[WHITE CROSS] Turn the bottom face to position the " + edgeColors + " edge on the " + Cube::FACE_STRINGS.at(adjFace) + " face.");
                useEdge.face = adjFace;

                // Rotate the Cube so that the edge is once again in front.
                rotations = rotateToFace(useEdge.face, Cube::FRONT, true);
                processSequence(rotations, "[WHITE CROSS] Rotate the cube so that the " + edgeColors + " edge is in front again.");
                useEdge.face = Cube::FRONT;
                fixFace(adjFace, rotations);
            }
            
            // Perform a certain algorithm to get the edge, which is on the right face, in the correct slot.
            // "Facing" refers to the direction of the white sticker.
            string algorithm = "";
            string message = "";
            if (useEdge.color == 'W' && useEdge.row == 0 && useEdge.col == 1) { // Edge is in correct spot but needs to be oriented
                algorithm = "FU'RU";
                message = "[WHITE CROSS] Perform an algorithm to orient the " + edgeColors + " edge correctly.";
            } else if (useEdge.color == 'W' && useEdge.row == 1 && useEdge.col == 0) { // Edge is on the left, facing front.
                algorithm = "UL'U'";
                message = "[WHITE CROSS] Perform an algorithm to orient the " + edgeColors + " edge from the left.";
            } else if (useEdge.color == 'W' && useEdge.row == 1 && useEdge.col == 2) { // Edge is on the right, facing front
                algorithm = "U'RU";
                message = "[WHITE CROSS] Perform an algorithm to orient the " + edgeColors + " edge from the right.";
            } else if (useEdge.color == 'W' && useEdge.row == 2 && useEdge.col == 1) { // Edge is on the bottom, facing front
                algorithm = "F'U'RU";
                message = "[WHITE CROSS] Perform an algorithm to bring up the " + edgeColors + " edge from the bottom.";
            } else if (useEdge.color != 'W' && useEdge.row == 1 && useEdge.col == 0) { // Edge is on the left, facing left
                algorithm = "F";
                message = "[WHITE CROSS] Use a move to bring up the " + edgeColors + " edge from the left.";
            } else if (useEdge.color != 'W' && useEdge.row == 1 && useEdge.col == 2) { // Edge is on the right, facing right
                algorithm = "F'";
                message = "[WHITE CROSS] Use a move to bring up the " + edgeColors + " edge from the right.";
            } else { // Edge is on the bottom, facing down
                algorithm = "F2";
                message = "[WHITE CROSS] Use two moves to bring up the " + edgeColors + " edge from the bottom.";
            }

            processSequence(algorithm, message);
        }

        i = (i + 1) % 4; // Iterate through 4 adjacent faces
    }
}

bool Assistant::checkWhiteCorners() const {
    if (checkWhiteCross()) {

    }return false;
}

void Assistant::getWhiteCorners() {

}

bool Assistant::checkSecondLayer() const {
    return false;
}

void Assistant::getSecondLayer() {

}

bool Assistant::checkYellowCross() const {
    return false;
}

void Assistant::getYellowCross() {

}

bool Assistant::checkYellowCornersPosition() const {
    return false;
}

void Assistant::getYellowCornersPosition() {

}

bool Assistant::checkYellowCornersOrientation() const {
    return false;
}

void Assistant::getYellowCornersOrientation() {

}