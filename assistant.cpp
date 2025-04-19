#include "assistant.h"
#include "cube.h"
#include "sticker_data.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
using namespace std;

Assistant::Assistant(Cube& cube) {
    this->cube = &cube;
}

void Assistant::solve() { // bool for hint? marker for each step, then return? pass down to all functions? then, take out strings. !("")
    switch (checkStage()) {
        case WHITE_CROSS:
            getWhiteCross();
        case WHITE_CORNERS:
            getWhiteCorners();
        case SECOND_LAYER:
            getSecondLayer(); // todo: improve the checking here
        case YELLOW_CROSS:
            getYellowCross();
        case YELLOW_EDGES:
            getYellowEdges();
        case YELLOW_CORNERS_POSITION:
            getYellowCornersPosition();
        case YELLOW_CORNERS_ORIENTATION:
            getYellowCornersOrientation(); // auf?
    }
}


void Assistant::printHint() const { // const? bypass

}

void Assistant::processSequence(const string& sequence, const string& message) {
    cube->doMoves(sequence, true);

    if (sequence.length() != 0) {
        cout << "\n" << message << endl << "Moves: " << Cube::tokenizeMoves(sequence);
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

string Assistant::rotateToFace(int& face, int newFace, bool onlyYMoves) const {
    string sequence = "";
    string tempSequence = "";
    Cube temp = *cube;
    int rotationsToCheck = 3;
    char originalCenter = cube->getAt(face, 1, 1); // Keep track of original face's center, as it won't change

    // All rotations will be checked to attain the desired center color in a given face and therefore the original face.
    while (rotationsToCheck > 0) {
        if (temp.getAt(newFace, 1, 1) == originalCenter) {
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

    face = newFace;
    return sequence;
}

StickerData Assistant::pickOutward(const vector<StickerData>& stickers) const {
    StickerData useSticker;

    for (const StickerData& data : stickers) {
        if (data.face != Cube::TOP && data.face != Cube::BOTTOM) {
            useSticker = data;
            break;
        }
    }

    return useSticker;
}

string Assistant::turnEdgeToBottom(int& row, int& col) const {
    string sequence = "";

    if (row == 0 && col == 1) { // Edge is on top and needs a double move
        sequence = "F2";
    } else if (row == 1 && col == 0) { // Edge is on the left, so the top edge might be already solved
        sequence = "F'D'FD";
    } else if (row == 1 && col == 2) { // Edge is on the right, so the top edge might be already solved
        sequence = "FDF'D'";
    }

    row = 2;
    col = 1;

    return sequence;
}

string Assistant::turnEdgeToFace(int& face, int newFace, int row, int col, bool isOnTop) const {
    string sequence = "";
    string tempSequence = "";
    string moveToUse = isOnTop ? "U" : "D";
    Cube temp = *cube;
    char originalColor1 = cube->getAt(face, row, col);
    char originalColor2 = cube->getAdjEdge(face, row, col).color;
    int turnsToCheck = 3;

    while (turnsToCheck >= 0) {
        turnsToCheck--;

        // Check if the edge is in the correct spot.
        char color1 = temp.getAt(newFace, row, col);
        char color2 = temp.getAdjEdge(newFace, row, col).color;

        if (Cube::checkColors({ color1, color2 }, { originalColor1, originalColor2 })) {
            sequence = simplifySequence(tempSequence);
            break;
        }

        tempSequence += moveToUse;
        temp.doMoves(moveToUse, false);
    }

    face = newFace;
    return sequence;
}

string Assistant::turnCornerToBottom(int& row, int& col) const {
    string sequence = "";

    if (row == 0 && col == 0) { // Top left corner - drop the corner and preserve the top sticker's position
        sequence = "F'D'FD";
        row = 2;
        col = 0;
    } else if (row == 0 && col == 2) { // Top right corner - drop the corner and preseve the top sticker's position
        sequence = "FDF'D'";
        row = 2;
        col = 2;
    }

    return sequence;
}

string Assistant::turnEdgetoTop(int& row, int& col) const {
    string sequence = "";

    if (row == 1 && col == 0) { // Edge is in the middle left - take out the edge and replace white corner
        sequence = "FU'F'LF'L'FU";
    } else if (row == 1 && col == 2) { // Edge is in the middle right - take out the edge and replace white corner
        sequence = "F'UFR'FRF'U'";
    }

    row = 0;
    col = 1;

    return sequence;
}

string Assistant::turnCornerToFaces(char baseColor, const pair<char, char>& colors, int& face, bool isOnTop) const {
    string sequence = "";
    string tempSequence = "";
    string moveToUse = isOnTop ? "U" : "D";
    Cube temp = *cube;
    int turnsToCheck = 3;

    while (turnsToCheck >= 0) {
        turnsToCheck--;

        // Proceed with checking if the base color was found.
        if (temp.checkCornerPosition(baseColor, colors)) {
            sequence = simplifySequence(tempSequence);
            break;
        }

        // Otherwise, keep adding to the sequence.
        tempSequence += moveToUse;
        temp.doMoves(moveToUse, false);

        // Update the face with each turn.
        // The faces wrap around to 1 after the 4th index (1-4).
        face = (face + 1) % 5;

        if (face == 0) {
            face++;
        }
    }

    return sequence;
}

char Assistant::getSideColor(char color, const pair<char, char>& adjColors) const {
    // Locate the specified corner.
    pair<StickerData, pair<StickerData, StickerData>> corner = cube->findCorner(color, adjColors);
    vector<StickerData> cornerStickers = { corner.first, corner.second.first, corner.second.second };

    // Get the indices of the side-facing colors.
    pair<int, int> indices = cube->getSideCorners(cornerStickers);
    StickerData sideFacing1 = cornerStickers[indices.first];
    StickerData sideFacing2 = cornerStickers[indices.second];

    // Do not reuse the used color if it happens to be side-facing.
    return (sideFacing1.color == color) ? sideFacing2.color : sideFacing1.color;
}

void Assistant::printComplete(const string& message) const {
    const int WIDTH = 30;
    cout << "\n\n" << string(WIDTH, '-') << " " << message << " " << string(WIDTH, '-') << "\n\n";
}

int Assistant::checkStage() const {
    if (!checkWhiteCross()) { return WHITE_CROSS; }
    if (!checkWhiteCorners()) { return WHITE_CORNERS; }
    if (!checkSecondLayer()) { return SECOND_LAYER; }
    if (!checkYellowCross()) { return YELLOW_CROSS; }
    if (!checkYellowEdges()) { return YELLOW_EDGES; }
    if (!checkYellowCornersPosition()) { return YELLOW_CORNERS_POSITION; }
    if (!checkYellowCornersOrientation()) { return YELLOW_CORNERS_ORIENTATION; }

    return ADJUST_UPPER_FACE; // Only the upper face needs to be turned to solve
}

bool Assistant::checkWhiteCross() const {
    int face = cube->findCenter('W'); // Get the white face so its edges can be checked
    
    if (checkWhiteCrossEdge(face, 0, 1) && checkWhiteCrossEdge(face, 1, 0)
    && checkWhiteCrossEdge(face, 1, 2) && checkWhiteCrossEdge(face, 2, 1)) {
        return true;
    }
    
    return false;
}

bool Assistant::checkWhiteCrossEdge(int face, int row, int col) const {
    if (cube->getAt(face, row, col) == 'W') { // First check if the edge is white
        StickerData adjEdge = cube->getAdjEdge(face, row, col);

        if (adjEdge.color == cube->getAt(adjEdge.face, 1, 1)) { // Check if the adjacent color matches its face's center
            return true;
        }
    }

    return false;
}

void Assistant::getWhiteCross() {
    // Get the white center face up.
    int face = cube->findCenter('W');
    processSequence(rotateToFace(face, Cube::TOP, false), "[WHITE CROSS] Rotate the cube so that the white center is on the top.");

    // Position the four white edges next to the white center, with the adjacent colors matching the centers of the sides.
    vector<pair<int, int>> whiteCrossEdges = {
        {0, 1}, // Top edge
        {1, 2}, // Right edge
        {2, 1}, // Front edge
        {1, 0}  // Left edge
    };
    vector<int> adjFaces = { Cube::BACK, Cube::RIGHT, Cube::FRONT, Cube::LEFT };
    int i = 0;

    while (!checkWhiteCross()) {
        pair<int, int> edge = whiteCrossEdges[i]; // An edge slot to possibly correct
        char correctFaceColor = cube->getAt(adjFaces[i], 1, 1); // The color whose face the edge slot should be on

        // Check if the edge needs to be corrected.
        if (!checkWhiteCrossEdge(Cube::TOP, edge.first, edge.second)) {
            // Locate the correct edge, which has white and a color that matches an adjacent center color.
            StickerData correctEdge = cube->findEdge('W', correctFaceColor);
            StickerData adjEdge = cube->getAdjEdge(correctEdge.face, correctEdge.row, correctEdge.col);
            StickerData useEdge = pickOutward({ correctEdge, adjEdge }); // Only use the edge that appears in front
            string edgeColors = Cube::getColors({ correctEdge.color, adjEdge.color });
            
            // Rotate the Cube so that the edge is in front.
            processSequence(rotateToFace(useEdge.face, Cube::FRONT, true), "[WHITE CROSS] Rotate the cube so that the " + edgeColors + " edge is in front.");

            // Check if the edge is in the correct position.
            int correctFace = cube->findCenter(correctFaceColor);
            if (useEdge.face != correctFace) {
                processSequence(turnEdgeToBottom(useEdge.row, useEdge.col), "[WHITE CROSS] Position the " + edgeColors + " edge on the bottom.");

                // Turn the bottom face to position it on the correct face.
                processSequence(turnEdgeToFace(useEdge.face, correctFace, useEdge.row, useEdge.col, false),
                                "[WHITE CROSS] Turn the bottom face to position the " + edgeColors + " edge on the " + Cube::FACE_STRINGS.at(correctFace) + " face.");
                useEdge.face = correctFace;

                // Rotate the Cube so that the edge is once again in front.
                processSequence(rotateToFace(useEdge.face, Cube::FRONT, true), "[WHITE CROSS] Rotate the cube so that the " + edgeColors + " edge is in front again.");
            }
            
            // Perform a certain algorithm to get the edge in the correct slot.
            // "Facing" refers to the direction of the white sticker.
            char color = useEdge.color;
            int row = useEdge.row;
            int col = useEdge.col;
            string algorithm = "";
            string message = "";

            if (color == 'W' && row == 0 && col == 1) { // Edge is in correct spot but needs to be oriented
                algorithm = "FU'RU";
                message = "[WHITE CROSS] Perform an algorithm to orient the " + edgeColors + " edge correctly.";
            } else if (color == 'W' && row == 1 && col == 0) { // Edge is on the left, facing front
                algorithm = "UL'U'";
                message = "[WHITE CROSS] Perform an algorithm to bring up the " + edgeColors + " edge from the left.";
            } else if (color == 'W' && row == 1 && col == 2) { // Edge is on the right, facing front
                algorithm = "U'RU";
                message = "[WHITE CROSS] Perform an algorithm to bring up the " + edgeColors + " edge from the right.";
            } else if (color == 'W' && row == 2 && col == 1) { // Edge is on the bottom, facing front
                algorithm = "F'U'RU";
                message = "[WHITE CROSS] Perform an algorithm to bring up the " + edgeColors + " edge from the bottom.";
            } else if (color != 'W' && row == 1 && col == 0) { // Edge is on the left, facing left
                algorithm = "F";
                message = "[WHITE CROSS] Use a move to bring up the " + edgeColors + " edge from the left.";
            } else if (color != 'W' && row == 1 && col == 2) { // Edge is on the right, facing right
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

    printComplete("COMPLETED WHITE CROSS");
}

bool Assistant::checkWhiteCorners() const {
    if (checkWhiteCross()) {
        int face = cube->findCenter('W'); // Get the white face to check its corners
        
        if (checkWhiteCorner(face, 0, 0) && checkWhiteCorner(face, 0, 2)
        && checkWhiteCorner(face, 2, 0) && checkWhiteCorner(face, 2, 2)) {
            return true;
        }
    }
    
    return false;
}

bool Assistant::checkWhiteCorner(int face, int row, int col) const {
    // First check if the sticker is white.
    if (cube->getAt(face, row, col) == 'W') {
        pair<StickerData, StickerData> adjCorners = cube->getAdjCorners(face, row, col);
        vector<StickerData> stickers = { adjCorners.first, adjCorners.second };

        // Check if the adjacent colors match the center colors of their faces.
        for (const StickerData& data : stickers) {
            char centerColor = cube->getAt(data.face, 1, 1);
            char adjCornerColor = data.color;

            if (centerColor != data.color) {
                return false;
            }
        }
        
        return true;
    }
    
    return false;
}

void Assistant::getWhiteCorners() {
    // Get the white center face up.
    int face = cube->findCenter('W');
    processSequence(rotateToFace(face, Cube::TOP, false), "[WHITE CORNERS] Rotate the cube so that the white center is on the top.");

    // Iterate over four corners until they are properly positioned.
    vector<pair<int, int>> whiteCorners = {
        {0, 0}, // Top left corner
        {0, 2}, // Top right corner
        {2, 0}, // Bottom left corner
        {2, 2}  // Bottom right corner
    };
    vector<pair<int, int>> adjPairs = {
        {Cube::LEFT, Cube::BACK},  // Top and left faces
        {Cube::BACK, Cube::RIGHT}, // Top and right faces
        {Cube::FRONT, Cube::LEFT}, // Bottom and left faces
        {Cube::RIGHT, Cube::FRONT} // Bottom and right faces
    };

    int i = 0;
    while (!checkWhiteCorners()) {
        pair<int, int> corner = whiteCorners[i]; // A corner to possibly correct
        pair<int, int> adjFaces = adjPairs[i]; // The two faces that the corner must be in between
        pair<char, char> faceColors = { cube->getAt(adjFaces.first, 1, 1), cube->getAt(adjFaces.second, 1, 1) }; // Face colors the corner must be between

        // Check if the edge needs to be corrected.
        if (!checkWhiteCorner(Cube::TOP, corner.first, corner.second)) {
            // Find the correct corner, which has white and colors of two adjacent faces.
            pair<StickerData, pair<StickerData, StickerData>> correctCorner = cube->findCorner('W', faceColors);
            pair<char, char> adjCornerColors = { correctCorner.second.first.color, correctCorner.second.second.color };
            StickerData useCorner = pickOutward({ correctCorner.first, correctCorner.second.first, correctCorner.second.second });
            string cornerColors = Cube::getColors({ correctCorner.first.color, adjCornerColors.first, adjCornerColors.second });

            // Rotate the Cube to the corner's position.
            processSequence(rotateToFace(useCorner.face, Cube::FRONT, true), "[WHITE CORNERS] Rotate the cube so that the " + cornerColors + " corner is in front.");

            // Check if the corner is between its proper faces.
            if (!cube->checkCornerPosition('W', adjCornerColors)) {
                // Position the corner to the bottom.
                processSequence(turnCornerToBottom(useCorner.row, useCorner.col), "[WHITE CORNERS] Position the " + cornerColors + " corner on the bottom.");

                // Turn the corner to the correct faces.
                int face1 = cube->findCenter(faceColors.first); // Locate the correct faces as they may have changed due to rotation
                int face2 = cube->findCenter(faceColors.second);
                string cornerFaces = Cube::getFaces({ face1, face2 });
                processSequence(turnCornerToFaces('W', adjCornerColors, useCorner.face, false),
                                "[WHITE CORNERS] Turn the bottom face to position the " + cornerColors + " corner on the " + cornerFaces + " faces.");

                // Rotate the Cube so that the corner is in front again.
                processSequence(rotateToFace(useCorner.face, Cube::FRONT, true), "[WHITE CORNERS] Rotate the cube so that the " + cornerColors + " corner is in front again.");
            }

            // Apply algorithms to the corner to correct it.
            char color = useCorner.color;
            char sideColor = getSideColor(color, adjCornerColors);
            int row = useCorner.row;
            int col = useCorner.col;
            string algorithm = "";
            string message = "";
            
            if (color == 'W' && row == 0 && col == 0) { // Corner in top left, facing front
                algorithm = "LD'L'F'D'F";
                message = "[WHITE CORNERS] Use an algorithm to orient the " + cornerColors + " corner on the top left.";
            } else if (color == 'W' && row == 0 && col == 2) { // Corner in top right, facing front
                algorithm = "R'DRFDF'";
                message = "[WHITE CORNERS] Use an algorithm to orient the " + cornerColors + " corner on the top right.";
            } else if (color == 'W' && row == 2 && col == 0) { // Corner in bottom left, facing front
                algorithm = "F'D'F";
                message = "[WHITE CORNERS] Use an algorithm to bring up the " + cornerColors + " corner from the bottom left.";
            } else if (color == 'W' && row == 2 && col == 2) { // Corner in bottom right, facing front
                algorithm = "FDF'";
                message = "[WHITE CORNERS] Use an algorithm to bring up the " + cornerColors + " corner from the bottom right.";
            } else if (color != 'W' && sideColor == 'W' && row == 0 && col == 0) { // Corner in top left, facing left
                algorithm = "LDL'D'LDL'";
                message = "[WHITE CORNERS] Use an algorithm to orient the " + cornerColors + " corner on the top left.";
            } else if (color != 'W' && sideColor == 'W' && row == 0 && col == 2) { // Corner in top right, facing right
                algorithm = "R'D'RDR'D'R";
                message = "[WHITE CORNERS] Use an algorithm to orient the " + cornerColors + " corner on the top right.";
            } else if (color != 'W' && sideColor == 'W' && row == 2 && col == 0) { // Corner in bottom left, facing left
                algorithm = "LDL'";
                message = "[WHITE CORNERS] Use an algorithm to bring up the " + cornerColors + " corner from the bottom left.";
            } else if (color != 'W' && sideColor == 'W' && row == 2 && col == 2) { // Corner in bottom right, facing right
                algorithm = "R'D'R";
                message = "[WHITE CORNERS] Use an algorithm to bring up the " + cornerColors + " corner from the bottom right.";
            } else if (color != 'W' && sideColor != 'W' && row == 2 && col == 0) { // Corner in bottom left, facing down
                algorithm = "LD'L'F'D2F";
                message = "[WHITE CORNERS] Use an algorithm to bring up the " + cornerColors + " corner from the bottom left.";
            } else { // Corner in bottom right, facing down
                algorithm = "R'DRFD2F'";
                message = "[WHITE CORNERS] Use an algorithm to bring up the " + cornerColors + " corner from the bottom right.";
            }

            processSequence(algorithm, message);
        }

        i = (i + 1) % 4; // Iterate through the four corners
    }

    printComplete("COMPLETED WHITE CORNERS");
}

bool Assistant::checkSecondLayer() const {
    // Check the pairs of faces to see if the edges are in their right positions.
    if (checkWhiteCorners()) {
        if (checkSecondLayerEdge({ Cube::BACK, Cube::RIGHT }, { 0, 2 })
        && checkSecondLayerEdge({ Cube::RIGHT, Cube::FRONT }, { 0, 2 })
        && checkSecondLayerEdge({ Cube::FRONT, Cube::LEFT }, { 0, 2 })
        && checkSecondLayerEdge({ Cube::LEFT, Cube::BACK }, { 0, 2 })) {
            return true;
        }
    }
    
    return false;
}

bool Assistant::checkSecondLayerEdge(const pair<int, int>& faces, const pair<int, int>& cols) const {
    // Check the first face.
    char firstColor = cube->getAt(faces.first, 1, cols.first);
    char correctFirstColor = cube->getAt(faces.first, 1, 1);

    // Check the second face.
    char secondColor = cube->getAt(faces.second, 1, cols.second);
    char correctSecondColor = cube->getAt(faces.second, 1, 1);
    
    // To be valid, the edge colors must match the center colors of adjacent faces.
    if (firstColor != correctFirstColor || secondColor != correctSecondColor) {
        return false;
    }

    return true;
}

bool Assistant::checkSecondLayerPosition(StickerData frontEdge, StickerData adjEdge) const {
    // First check if the edge is already in the middle and between the correct faces.
    char frontSideColor = cube->getAt(frontEdge.face, 1, 1);
    char adjSideColor = cube->getAt(adjEdge.face, 1, 1);

    if (Cube::checkColors({ frontEdge.color, adjEdge.color }, { frontSideColor, adjSideColor })) { // The edge only needs to be oriented
        return true;
    }

    if (frontEdge.color == cube->getAt(frontEdge.face, 1, 1)) { // The front side of the edge matches its face's center color
        return true;
    }

    return false;
}

void Assistant::getSecondLayer() {
    // Get the yellow center up.
    int face = cube->findCenter('Y');
    processSequence(rotateToFace(face, Cube::TOP, false), "[SECOND LAYER] Rotate the cube so that the yellow center is on the top.");
    
    // Iterate over the 4 edges until they are properly positioned.
    pair<int, int> secondLayerCoord = { 0, 2 }; // Leftmost and rightmost stickers of respective faces
    vector<pair<int, int>> adjPairs = {
        { Cube::BACK, Cube::RIGHT },  // Back and right faces
        { Cube::RIGHT, Cube::FRONT }, // Right and front faecs
        { Cube::FRONT, Cube::LEFT },  // Front and left faces
        { Cube::LEFT, Cube::BACK }    // Left and back faces
    };

    int i = 0;
    while (!checkSecondLayer()) {
        pair<int, int> adjFaces = adjPairs[i];
        pair<char, char> faceColors = { cube->getAt(adjFaces.first, 1, 1), cube->getAt(adjFaces.second, 1, 1) };

        // Check if the edge needs to be corrected.
        if (!checkSecondLayerEdge(adjFaces, secondLayerCoord)) {
            // Locate the correct edge containing the colors of the adjacent faces.
            StickerData correctEdge = cube->findEdge(faceColors.first, faceColors.second);
            StickerData adjEdge = cube->getAdjEdge(correctEdge.face, correctEdge.row, correctEdge.col);
            StickerData useEdge = pickOutward({ correctEdge, adjEdge });
            string edgeColors = Cube::getColors({ correctEdge.color, adjEdge.color });

            // Rotate to the edge's position.
            processSequence(rotateToFace(useEdge.face, Cube::FRONT, true), "[SECOND LAYER] Rotate the cube so that the " + edgeColors + " edge is in front.");

            // Check if the edge is in the right position.
            if (!checkSecondLayerPosition(useEdge, cube->getAdjEdge(useEdge.face, useEdge.row, useEdge.col))) {
                // Position the edge up to move it to the proper face.
                processSequence(turnEdgetoTop(useEdge.row, useEdge.col), "[SECOND LAYER] Position the " + edgeColors + " edge on top.");

                // Move the edge to the proper face.
                int face1 = cube->findCenter(correctEdge.color);
                int face2 = cube->findCenter(adjEdge.color);

                // The new face is calculated using the front-facing edge, as its center color
                // needs to match the center color of its new face.
                int useFace = (useEdge.color == correctEdge.color) ? face1 : face2;
                string edgeFaces = cube->getFaces({ face1, face2 });
                processSequence(turnEdgeToFace(useEdge.face, useFace, useEdge.row, useEdge.col, true), "[SECOND LAYER] Turn the upper face to move the " + edgeColors + " edge to the " + edgeFaces + " faces.");

                // Rotate to the new face.
                processSequence(rotateToFace(useEdge.face, Cube::FRONT, true), "[SECOND LAYER] Rotate the cube so that the " + edgeColors + " is in front again.");
            }

            // Apply algorithms to the edge.
            adjEdge = cube->getAdjEdge(useEdge.face, useEdge.row, useEdge.col);
            int faceSlot = cube->findCenter(adjEdge.color);
            int row = useEdge.row;
            int col = useEdge.col;
            string algorithm = "";
            string message = "";

            if (faceSlot < Cube::FRONT && row == 0 && col == 1) { // Edge is on the top and needs to be moved to the left
                algorithm = "U'L'ULUFU'F'";
                message = "[SECOND LAYER] Use an algorithm to bring down the " + edgeColors + " edge from the top to the left.";
            } else if (faceSlot > Cube::FRONT && row == 0 && col == 1) { // Edge is on the top and needs to be moved to the right
                algorithm = "URU'R'U'F'UF";
                message = "[SECOND LAYER] Use an algorithm to bring down the " + edgeColors + " edge from the top to the right.";
            } else if (row == 1 && col == 0) { // Edge is in the correct left slot and needs to be oriented in place
                algorithm = "L'ULU'FU2F'U'FU2F'";
                message = "[SECOND LAYER] Use an algorithm to orient the " + edgeColors + " edge correctly on the left.";
            } else { // Edge is in the correct right slot and needs to be oriented in place.
                algorithm = "RU'R'UF'U2FUF'U2F";
                message = "[SECOND LAYER] Use an algorithm to orient the " + edgeColors + " edge correctly on the right.";
            }

            processSequence(algorithm, message);
        }

        i = (i + 1) % 4; // Iterate over four edges
    }

    printComplete("COMPLETED SECOND LAYER");
}

bool Assistant::checkYellowCross() const {
    if (checkSecondLayer()) {
        int face = cube->findCenter('Y'); // Get the yellow face so its edges can be checked

        if (cube->getAt(face, 0, 1) == 'Y' && cube->getAt(face, 1, 0) == 'Y'
        && cube->getAt(face, 1, 2) == 'Y' && cube->getAt(face, 2, 1)) {
            return true;
        }
    }
    
    return false;
}

void Assistant::getYellowCross() {
    // Get the yellow center facing up.
    int face = cube->findCenter('Y');
    processSequence(rotateToFace(face, Cube::TOP, false), "[YELLOW CROSS] Rotate the cube so that the yellow center is on top.");

    // Check for various cases and solve them.
    string algorithm = "";
    string message = "";

    while (!checkYellowCross()) {
        // Check for line shapes first.
        pair<bool, string> shape = checkLineShape();
        if (shape.first) {
            processSequence(shape.second, "[YELLOW CROSS] Turn the upper face to correctly position the line shape.");
            algorithm = "FRUR'U'F'";
            message = "[YELLOW CROSS] Perform an algorithm to get the line shape into a cross shape.";
            break;
        }

        // Then, check for L shapes.
        shape = checkLShape();
        if (shape.first) {
            processSequence(shape.second, "[YELLOW CROSS] Turn the upper face to correctly position the L shape.");
            algorithm = "FURU'R'F'";
            message = "[YELLOW CROSS] Perform an algorithm to get the L shape into a cross shape.";
            break;
        }

        // Then, check for dot shapes.
        if (checkDotShape()) {
            algorithm = "FRUR'U'F'y2FRUR'U'F'y2FRUR'U'F'";
            message = "[YELLOW CROSS] Perform two familiar algorithms to get the dot shape into a cross shape.";
            break;
        }
    }

    processSequence(algorithm, message);
    printComplete("COMPLETED YELLOW CROSS");
}

pair<bool, string> Assistant::checkLShape() const {
    pair<bool, string> res = { false, "" };

    if (cube->getAt(Cube::TOP, 1, 0) == 'Y' && cube->getAt(Cube::TOP, 0, 1) == 'Y') { // L in top left - no additional positioning needed
        res.first = true;
    } else if (cube->getAt(Cube::TOP, 0, 1) == 'Y' && cube->getAt(Cube::TOP, 1, 2) == 'Y') { // L in top right - need one counterclockwise upper move
        res.first = true;
        res.second = "U'";
    } else if (cube->getAt(Cube::TOP, 1, 2) == 'Y' && cube->getAt(Cube::TOP, 2, 1) == 'Y') { // L in bottom right - need two upper moves
        res.first = true;
        res.second = "U2";
    } else if (cube->getAt(Cube::TOP, 2, 1) == 'Y' && cube->getAt(Cube::TOP, 1, 0) == 'Y') { // L in bottom left - need one clockwise upper move
        res.first = true;
        res.second = "U";
    }

    return res;
}

pair<bool, string> Assistant::checkLineShape() const {
    pair<bool, string> res = { false, "" };

    if (cube->getAt(Cube::TOP, 1, 0) == 'Y' && cube->getAt(Cube::TOP, 1, 2) == 'Y') { // Line is horizontal - no additional positioning needed
        res.first = true;
    } else if (cube->getAt(Cube::TOP, 0, 1) == 'Y' && cube->getAt(Cube::TOP, 2, 1) == 'Y') { // Line is vertical - needs a single upper move
        res.first = true;
        res.second = "U";
    }
    return res;
}

bool Assistant::checkDotShape() const {
    // The dot shape must have a singular yellow color in the center
    // and not have any yellow on the top edges (but corners are OK).
    if (cube->getAt(Cube::TOP, 1, 1) == 'Y'
    && cube->getAt(Cube::TOP, 0, 1) != 'Y'
    && cube->getAt(Cube::TOP, 1, 0) != 'Y'
    && cube->getAt(Cube::TOP, 1, 2) != 'Y'
    && cube->getAt(Cube::TOP, 2, 1)) {
        return true;
    }

    return false;
}

bool Assistant::checkYellowEdges() const {
    if (checkYellowCross()) {
        int face = cube->findCenter('Y'); // Get the yellow face so its edges can be checked

        if (checkYellowEdge(face, { 0, 1 }) && checkYellowEdge(face, { 1, 0 })
        && checkYellowEdge(face, { 1, 2 }) && checkYellowEdge(face, { 2, 1 })) {
            return true;
        }
    }
    
    return false;
}

bool Assistant::checkYellowEdge(int face, const pair<int, int>& coord) const {
    StickerData adjEdge = cube->getAdjEdge(face, coord.first, coord.second);
    char centerColor = cube->getAt(adjEdge.face, 1, 1);

    // The top of the edge has to be yellow, and the adjacent edge's color
    // must match its faces center color.
    if (cube->getAt(face, coord.first, coord.second) == 'Y'
    && adjEdge.color == centerColor) {
        return true;
    }

    return false;
}

void Assistant::getYellowEdges() {
    // Get the yellow center facing up.
    int face = cube->findCenter('Y');
    processSequence(rotateToFace(face, Cube::FRONT, false), "[YELLOW EDGES] Rotate the cube so that the yellow center is on top.");
    
    // 

    while (!checkYellowEdges()) {

    }

    printComplete("COMPLETED YELLOW EDGES");
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