#include "cube.h"
#include "sticker_data.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <string>
#include <stack>
#include <map>
#include <set>
#include <vector>
#include <utility>
using namespace std;

const set<char> Cube::VALID_MOVES = { 'U', 'L', 'F', 'R', 'B', 'D', 'x', 'y', 'z' };
const map<char, string> Cube::COLOR_STRINGS = {
	{ 'W', "White" },
	{ 'O', "Orange" },
	{ 'G', "Green" },
	{ 'R', "Red" },
	{ 'B', "Blue" },
	{ 'Y', "Yellow" },
};
const map<int, string> Cube::FACE_STRINGS = {
	{ Cube::TOP, "Top" },
	{ Cube::LEFT, "Left" },
	{ Cube::FRONT, "Front" },
	{ Cube::RIGHT, "Right" },
	{ Cube::BACK, "Back" },
	{ Cube::BOTTOM, "Bottom" }
};
const map<int, char> Cube::FACE_COLORS = {
	{ TOP, 'W' },
	{ LEFT, 'O' },
	{ FRONT, 'G' },
	{ RIGHT, 'R' },
	{ BACK, 'B' },
	{ BOTTOM, 'Y' }
};

string Cube::getColors(const vector<char>& colors) {
    return Cube::getData(Cube::COLOR_STRINGS, colors);
}

string Cube::getFaces(const vector<int>& faces) {
    return Cube::getData(Cube::FACE_STRINGS, faces);
}

bool Cube::checkColors(const pair<char, char>& colors1, const pair<char, char>& colors2) {
    int numMatches = 0;

    vector<char> testColors1 = { colors1.first, colors1.second };
    vector<char> testColors2 = { colors2.first, colors2.second };

    for (char color : testColors1) {
        for (char sticker : testColors2) {
            if (color == sticker) {
                numMatches++;
            }
        }
    }

    if (numMatches < testColors1.size()) {
        return false;
    }
    
    return true;
}

bool Cube::checkMoves(const string& moves) {
	istringstream iss(moves);
	char ch;
	bool prevMove = false;
	bool prevApostrophe = false;

	while (iss >> ch) {
		if (Cube::VALID_MOVES.count(ch) == 1) {                                 // Characters in the set are valid
			prevMove = true;
		} else {
			if (ch != ' ') {                                                    // Spaces are valid
				if (isdigit(ch) && prevMove || isdigit(ch) && prevApostrophe) { // If there was a valid move or apostrophe before, check if number is 2
					int testNum = ch - '0';

					if (testNum == 2) {
						prevMove = false;
						prevApostrophe = false;
					} else {
						return false;
					}
				} else if (ch == '\'' && prevMove) {                            // Apostrophes are valid if there was a previous move
					prevMove = false;
					prevApostrophe = true;
				} else {
					return false;
				}
			}
		}

		if (iss.fail()) { 
			return false;
		}
	}

	return true;
}

string Cube::tokenizeMoves(const string& moves) {
	string res = "";
	istringstream iss(moves);
	char ch;

	while (iss >> ch) {
		char next = iss.peek();
		
		// Check for numbers and apostrophes (ex: U, U2, U', and U'2 are all valid moves).
		if (next == '2') {                // The number will always be the last in a move
			res += string(1, ch) + "2 ";
			iss.get();
		} else if (next == '\'') {        // Apostrophes will always follow the letter
			res += string(1, ch);
		} else {                          // Otherwise, the next letter will be another letter or EOF
			res += string(1, ch) + " ";
		}
	}

	return res;
}

int Cube::countMoves(const string& moves) {
	if (!checkMoves(moves)) {
		return -1;
	}

	istringstream iss(moves);
	int numMoves = 0;
	char ch;

	while (iss >> ch) {
		if (ch != '\'' && ch != '2') { // Counting letters
			numMoves++;
		}

		if (ch == '2') { // Double the count if there is a double move
			numMoves++;
		}
	}

	return numMoves;
}

Cube::Cube() {
	name = "";
	scramble = "";
	moves = "";
	totalMoves = 0;
	createSolved();
}

Cube::Cube(const string& name, const string& scramble, const string& moves, int totalMoves) {
	this->name = name;
	this->scramble = scramble;
	this->moves = moves;
	this->totalMoves = totalMoves;
	createSolved();
	doMoves(scramble, false);
	doMoves(moves, false);
}

char Cube::getAt(int face, int row, int col) const {
	if ((face >= 0 && face < NUM_FACES)
	&& (row >= 0 && row < SIZE)
	&& (col >= 0 && col < SIZE)) {
		return stickers[face][row][col];
	}

	return '\0';
}

int Cube::findCenter(char color) const {
	for (int i = 0; i < NUM_FACES; i++) {
		if (stickers[i][1][1] == color) {
			return i;
		}
	}

	return -1;
}

StickerData Cube::findEdge(char color1, char color2) const {
    StickerData res = { -1, '\0', -1, -1};

    vector<pair<int, int>> coords = { // Store row and columns that need to be checked
        {0, 1},
        {1, 0},
        {1, 2},
        {2, 1}
    };
    
    // Check each face and each edge.
    for (int i = 0; i < Cube::NUM_FACES; i++) {
        for (int j = 0; j < coords.size(); j++) {
            pair<int, int> coord = coords[j];
            char edge = stickers[i][coord.first][coord.second];
            char adjEdge = getAdjEdge(i, coord.first, coord.second).color;
            
            if (checkColors({ edge, adjEdge }, { color1, color2 })) {
                res.face = i;
                res.color = edge;
                res.row = coord.first;
                res.col = coord.second;
                return res;
            }
        }
    }

    return res;
}

StickerData Cube::getAdjEdge(int face, int row, int col) const {
    StickerData res;

    if (face == TOP) {
        res.row = 0;
        res.col = 1;

        if (row == 0 && col == 1) { // Top edge -> top of back face
            res.face = BACK;
        } else if (row == 1 && col == 0) { // Left edge -> top of left face
            res.face = LEFT;
        } else if (row == 1 && col == 2) { // Right edge -> top of right face
            res.face = RIGHT;
        } else { // Bottom edge -> top of front face
            res.face = FRONT;
        }
    } else if (face == LEFT) {
        if (row == 0 && col == 1) { // Top edge -> left of top face
            res.face = TOP;
            res.row = 1;
            res.col = 0;
        } else if (row == 1 && col == 0) { // Left edge -> right of back face
            res.face = BACK;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 2) { // Right edge -> left of front face
            res.face = FRONT;
            res.row = 1;
            res.col = 0;
        } else { // Bottom edge -> left of bottom face
            res.face = BOTTOM;
            res.row = 1;
            res.col = 0;
        }
    } else if (face == FRONT) {
        if (row == 0 && col == 1) { // Top edge -> bottom of the top face
            res.face = TOP;
            res.row = 2;
            res.col = 1;
        } else if (row == 1 && col == 0) { // Left edge -> right of left face
            res.face = LEFT;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 2) { // Right edge -> left of right face
            res.face = RIGHT;
            res.row = 1;
            res.col = 0;
        } else { // Bottom edge -> top of bottom face
            res.face = BOTTOM;
            res.row = 0;
            res.col = 1;
        }
    } else if (face == RIGHT) {
        if (row == 0 && col == 1) { // Top edge -> right of top face
            res.face = TOP;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 0) { // Left edge -> right of front face
            res.face = FRONT;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 2) { // Right edge -> left of back face
            res.face = BACK;
            res.row = 1;
            res.col = 0;
        } else { // Bottom edge -> right of bottom face
            res.face = BOTTOM;
            res.row = 1;
            res.col = 2;
        }
    } else if (face == BACK) {
        if (row == 0 && col == 1) { // Top edge -> top of top face
            res.face = TOP;
            res.row = 0;
            res.col = 1;
        } else if (row == 1 && col == 0) { // Left edge -> right of right face
            res.face = RIGHT;
            res.row = 1;
            res.col = 2;
        } else if (row == 1 && col == 2) { // Right edge -> left of left face
            res.face = LEFT;
            res.row = 1;
            res.col = 0;
        } else { // Bottom edge -> bottom of bottom face
            res.face = BOTTOM;
            res.row = 2;
            res.col = 1;
        }
    } else if (face == BOTTOM) {
        res.row = 2;
        res.col = 1;

        if (row == 0 && col == 1) { // Top edge -> bottom of front face
            res.face = FRONT;
        } else if (row == 1 && col == 0) { // Left edge -> bottom of left face
            res.face = LEFT;
        } else if (row == 1 && col == 2) { // Right edge -> bottom of right face
            res.face = RIGHT;
        } else { // Bottom edge -> bottom of back face
            res.face = BACK;
        }
    } else {
		res.face = -1;
		res.row = -1;
		res.col = -1;
	}

    res.color = stickers[res.face][res.row][res.col];
    return res;
}

bool Cube::checkEdgeMatch(int face, char color, const pair<int, int>& coord) const {
	StickerData adjEdge = getAdjEdge(face, coord.first, coord.second);
    char centerColor = stickers[adjEdge.face][1][1];

    // The top of the edge has to be yellow, and the adjacent edge's color
    // must match its faces center color.
    if (stickers[face][coord.first][coord.second] == color
    && adjEdge.color == centerColor) {
        return true;
    }

    return false;
}

pair<StickerData, pair<StickerData, StickerData>> Cube::findCorner(char baseColor, const pair<char, char>& adjColors) const {
    pair<StickerData, pair<StickerData, StickerData>> res = {
		{-1, '\0', -1, -1},
		{ {-1, '\0', -1, -1 }, { -1, '\0', -1, -1 } }
	};
    
    // Store row and columns that need to be checked.
    vector<pair<int, int>> coords = {
        {0, 0},
        {0, 2},
        {2, 0},
        {2, 2}
    };

    // Check each face and its corners.
    for (int i = 0; i < Cube::NUM_FACES; i++) {
        for (int j = 0; j < coords.size(); j++) {
            pair<int, int> coord = coords[j];
            char color = stickers[i][coord.first][coord.second];
            pair<StickerData, StickerData> adjCorners = getAdjCorners(i, coord.first, coord.second);
            char color1 = adjCorners.first.color;
            char color2 = adjCorners.second.color;

            if (color == baseColor && checkColors(adjColors, { color1, color2 })) {
                res.first.color = color;
                res.first.face = i;
                res.first.row = coord.first;
                res.first.col = coord.second;
				res.second = adjCorners;
                return res;
            }
        }
    }

    return res;
}

pair<StickerData, StickerData> Cube::getAdjCorners(int face, int row, int col) const {
    pair<StickerData, StickerData> res;

    if (face == TOP) {
        if (row == 0 && col == 0) { // Top left corner
            res.first = { LEFT, stickers[LEFT][0][0], 0, 0 };
            res.second = { BACK, stickers[BACK][0][2], 0, 2 };
        } else if (row == 0 && col == 2) { // Top right corner
            res.first = { RIGHT, stickers[RIGHT][0][2], 0, 2 };
            res.second = { BACK, stickers[BACK][0][0], 0, 0 };
        } else if (row == 2 && col == 0) { // Bottom left corner
            res.first = { LEFT, stickers[LEFT][0][2], 0, 2 };
            res.second = { FRONT, stickers[FRONT][0][0], 0, 0 };
        } else { // Bottom right corner
            res.first = { RIGHT, stickers[RIGHT][0][0], 0, 0 };
            res.second = { FRONT, stickers[FRONT][0][2], 0, 2 };
        }
    } else if (face == LEFT) {
        if (row == 0 && col == 0) { // Top left corner
            res.first = { TOP, stickers[TOP][0][0], 0, 0 };
            res.second = { BACK, stickers[BACK][0][2], 0, 2 };
        } else if (row == 0 && col == 2) { // Top right corner
            res.first = { TOP, stickers[TOP][2][0], 2, 0 };
            res.second = { FRONT, stickers[FRONT][0][0], 0, 0 };
        } else if (row == 2 && col == 0) { // Bottom left corner
            res.first = { BOTTOM, stickers[BOTTOM][2][0], 2, 0 };
            res.second = { BACK, stickers[BACK][2][2], 2, 2 };
        } else { // Bottom right corner
            res.first = { BOTTOM, stickers[BOTTOM][0][0], 0, 0 };
            res.second = { FRONT, stickers[FRONT][2][0], 2, 0 };
        }
    } else if (face == FRONT) {
        if (row == 0 && col == 0) { // Top left corner
            res.first = { TOP, stickers[TOP][2][0], 2, 0 };
            res.second = { LEFT, stickers[LEFT][0][2], 0, 2 };
        } else if (row == 0 && col == 2) { // Top right corner
            res.first = { TOP, stickers[TOP][2][2], 2, 2 };
            res.second = { RIGHT, stickers[RIGHT][0][0], 0, 0 };
        } else if (row == 2 && col == 0) { // Bottom left corner
            res.first = { BOTTOM, stickers[BOTTOM][0][0], 0, 0 };
            res.second = { LEFT, stickers[LEFT][2][2], 2, 2 };
        } else { // Bottom right corner
            res.first = { BOTTOM, stickers[BOTTOM][0][2], 0, 2 };
            res.second = { RIGHT, stickers[RIGHT][2][0], 2, 0 };
        }
    } else if (face == RIGHT) {
        if (row == 0 && col == 0) { // Top left corner
            res.first = { TOP, stickers[TOP][2][2], 2, 2 };
            res.second = { FRONT, stickers[FRONT][0][2], 0, 2 };
        } else if (row == 0 && col == 2) { // Top right corner
            res.first = { TOP, stickers[TOP][0][2], 0, 2 };
            res.second = { BACK, stickers[BACK][0][0], 0, 0 };
        } else if (row == 2 && col == 0) { // Bottom left corner
            res.first = { BOTTOM, stickers[BOTTOM][0][2], 0, 2 };
            res.second = { FRONT, stickers[FRONT][2][2], 2, 2 };
        } else { // Bottom right corner
            res.first = { BOTTOM, stickers[BOTTOM][2][2], 2, 2 };
            res.second = { BACK, stickers[BACK][2][0], 2, 0 };
        }
    } else if (face == BACK) {
        if (row == 0 && col == 0) { // Top left corner
            res.first = { TOP, stickers[TOP][0][2], 0, 2 };
            res.second = { RIGHT, stickers[RIGHT][0][2], 0, 2 };
        } else if (row == 0 && col == 2) { // Top right corner
            res.first = { TOP, stickers[TOP][0][0], 0, 0 };
            res.second = { LEFT, stickers[LEFT][0][0], 0, 0 };
        } else if (row == 2 && col == 0) { // Bottom left corner
            res.first = { BOTTOM, stickers[BOTTOM][2][2], 2, 2 };
            res.second = { RIGHT, stickers[RIGHT][2][2], 2, 2 };
        } else { // Bottom right corner
            res.first = { BOTTOM, stickers[BOTTOM][2][0], 2, 0 };
            res.second = { LEFT, stickers[LEFT][2][0], 2, 0 };
        }
    } else if (face == BOTTOM) {
        if (row == 0 && col == 0) { // Top left corner
            res.first = { FRONT, stickers[FRONT][2][0], 2, 0 };
            res.second = { LEFT, stickers[LEFT][2][2], 2, 2 };
        } else if (row == 0 && col == 2) { // Top right corner
            res.first = { FRONT, stickers[FRONT][2][2], 2, 2 };
            res.second = { RIGHT, stickers[RIGHT][2][0], 2, 0 };
        } else if (row == 2 && col == 0) { // Bottom left corner
            res.first = { BACK, stickers[BACK][2][2], 2, 2 };
            res.second = { LEFT, stickers[LEFT][2][0], 2, 0 };
        } else { // Bottom right corner
            res.first = { BACK, stickers[BACK][2][0], 2, 0 };
            res.second = { RIGHT, stickers[RIGHT][2][2], 2, 2 };
        }
    } else {
		res.first = { -1, '\0', -1, -1 };
		res.second = { -1, '\0', -1, -1 };
	}

    return res;
}

pair<int, int> Cube::getSideCorners(const vector<StickerData>& corner) const {
	pair<int, int> indices = { -1, -1 };

	for (int i = 0; i < corner.size(); i++) {
        if (corner[i].face == TOP || corner[i].face == BOTTOM) { // Once a non-side-facing index is found, assign the other indices
            indices.first = (i + 1) % corner.size();
            indices.second = (i + 2) % corner.size();
        }
    }

	return indices;
}

bool Cube::checkCornerPosition(char baseColor, const pair<char, char>& colors) const {
    // Check if the corner is in the correct spot.
    // First, get the corner colors of the given row and column.
    // Then, check the side-facing stickers and see if their faces contain the center colors of the correct faces.
    pair<StickerData, pair<StickerData, StickerData>> corner = findCorner(baseColor, colors);
    vector<StickerData> cornerStickers = { corner.first, corner.second.first, corner.second.second };
    bool foundBase = false; 
    int side1Idx, side2Idx;

    // Determine if the base color is found, since different corners can have the same adjacent colors.
    for (int i = 0; i < cornerStickers.size(); i++) {
        if (cornerStickers[i].color == baseColor) {
            foundBase = true;
        }
    }

    // Proceed with checking the side-facing stickers if the base color was found.
    if (foundBase) {
		pair<int, int> indices = getSideCorners(cornerStickers);
        char side1Center = stickers[cornerStickers[indices.first].face][1][1];
        char side2Center = stickers[cornerStickers[indices.second].face][1][1];
		
        if (checkColors(colors, { side1Center, side2Center })) {
            return true;
        }
    }

    return false;
}

string Cube::getName() const { return name; }

void Cube::setName(const string& name) { this->name = name; }

void Cube::setScramble(const string& scramble) { this->scramble = scramble; }

string Cube::getScramble() const { return scramble; }

string Cube::getMoves() const { return moves; }

string Cube::getCurrentMoves() const {
	string res = "";
	stack<string> temp = currentMoves;
	vector<string> reversed;

	while (temp.size() > 0) {
		reversed.push_back(temp.top());
		temp.pop();
	}

	for (string move : reversed) {
		res += move;
	}

	return res;
}

int Cube::getTotalMoves() const { return totalMoves; }

void Cube::createSolved() {
	char color;

	for (int i = 0; i < NUM_FACES; i++) { // For each face, create 3x3 array of its color
		color = FACE_COLORS.at(i);

		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				stickers[i][j][k] = color;
			}
		}
	}
}

void Cube::displayState(bool printCommands) const {
	const int INDIV_WIDTH = 2;
	const int FACE_SPACING = 5;
	const int FACE_WIDTH = INDIV_WIDTH * SIZE + (SIZE - 1);
	
	const int LEFT_PADDING = 3;
	const int MIDDLE_PADDING = LEFT_PADDING + FACE_WIDTH + FACE_SPACING;
	const int RIGHT_PADDING = 8;
	const int RIGHT_PADDING_FROM_MIDDLE = (FACE_WIDTH * 2) + (FACE_SPACING * 2) + RIGHT_PADDING;
	const int RIGHT_PADDING_FROM_LEFT = RIGHT_PADDING_FROM_MIDDLE + (FACE_WIDTH * 2) + FACE_SPACING + LEFT_PADDING;

	const int COMMAND_LENGTH = 25;
	int count = 1;

	// Print the top segments of the cube.
	cout << endl << endl << printSpacing(MIDDLE_PADDING) << "TOP" << printCommandWithSpacing(RIGHT_PADDING_FROM_MIDDLE + (FACE_WIDTH - 3), count, printCommands) << endl
	     << printSpacing(MIDDLE_PADDING) << printFaceSegment(TOP, INDIV_WIDTH, 0) << printCommandWithSpacing(RIGHT_PADDING_FROM_MIDDLE, count, printCommands) << endl
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(TOP, INDIV_WIDTH, 1) << printCommandWithSpacing(RIGHT_PADDING_FROM_MIDDLE, count, printCommands) << endl
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(TOP, INDIV_WIDTH, 2) << printCommandWithSpacing(RIGHT_PADDING_FROM_MIDDLE, count, printCommands) << endl
		 << printCommandWithSpacing(RIGHT_PADDING_FROM_LEFT, count, printCommands) << endl;

	// Print the left, front, right, and back segments of the cube.
	cout << printSpacing(LEFT_PADDING) << "LEFT"
		 << printSpacing(FACE_WIDTH - 4 + FACE_SPACING) << "FRONT"
		 << printSpacing(FACE_WIDTH - 5 + FACE_SPACING) << "RIGHT"
		 << printSpacing(FACE_WIDTH - 5 + FACE_SPACING) << "BACK" << printCommandWithSpacing(RIGHT_PADDING + (FACE_WIDTH - 4), count, printCommands) << endl;

	cout << printSpacing(LEFT_PADDING) << printFaceSegment(LEFT, INDIV_WIDTH, 0)
		 << printSpacing(FACE_SPACING) << printFaceSegment(FRONT, INDIV_WIDTH, 0)
		 << printSpacing(FACE_SPACING) << printFaceSegment(RIGHT, INDIV_WIDTH, 0)
		 << printSpacing(FACE_SPACING) << printFaceSegment(BACK, INDIV_WIDTH, 0) << printCommandWithSpacing(RIGHT_PADDING, count, printCommands) << endl;

	cout << printSpacing(LEFT_PADDING) << printFaceSegment(LEFT, INDIV_WIDTH, 1)
	 	 << printSpacing(FACE_SPACING) << printFaceSegment(FRONT, INDIV_WIDTH, 1)
	 	 << printSpacing(FACE_SPACING) << printFaceSegment(RIGHT, INDIV_WIDTH, 1)
		 << printSpacing(FACE_SPACING) << printFaceSegment(BACK, INDIV_WIDTH, 1) << printCommandWithSpacing(RIGHT_PADDING, count, printCommands) << endl;

	cout << printSpacing(LEFT_PADDING) << printFaceSegment(LEFT, INDIV_WIDTH, 2)
		 << printSpacing(FACE_SPACING) << printFaceSegment(FRONT, INDIV_WIDTH, 2)
		 << printSpacing(FACE_SPACING) << printFaceSegment(RIGHT, INDIV_WIDTH, 2)
		 << printSpacing(FACE_SPACING) << printFaceSegment(BACK, INDIV_WIDTH, 2) << printCommandWithSpacing(RIGHT_PADDING, count, printCommands) << endl
		 << printCommandWithSpacing(RIGHT_PADDING_FROM_LEFT, count, printCommands) << endl;

	// Print the bottom segments of the cube.
	cout << printSpacing(MIDDLE_PADDING) << "BOTTOM" << printCommandWithSpacing(RIGHT_PADDING_FROM_MIDDLE + (FACE_WIDTH - 6), count, printCommands) << endl
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(BOTTOM, INDIV_WIDTH, 0) << printCommandWithSpacing(RIGHT_PADDING_FROM_MIDDLE, count, printCommands) << endl
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(BOTTOM, INDIV_WIDTH, 1) << printCommandWithSpacing(RIGHT_PADDING_FROM_MIDDLE, count, printCommands) << endl
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(BOTTOM, INDIV_WIDTH, 2) << printCommandWithSpacing(RIGHT_PADDING, count, printCommands) << endl << endl;
		
	// Print cube data.
	string scrambleString = showMoves(tokenizeMoves(scramble));
	cout << "Original scramble: " << (scrambleString.length() > 0 ? scrambleString : "None") << endl;
	cout << "Total moves: " << totalMoves << endl;
	cout << "Current moves: " << printCurrentMoves();
	cout << endl;
}

string Cube::printFaceSegment(int side, int width, int row) const {
	ostringstream oss;
	
	for (int i = 0; i < SIZE; i++) {
		// Assign the right background color
		char elem = stickers[side][row][i];
		string color;

		switch (elem) {
			case 'W':
				color = WHITE_BG;
				break;
			case 'G':
				color = GREEN_BG;
				break;
			case 'R':
				color = RED_BG;
				break;
			case 'B':
				color = BLUE_BG;
				break;
			case 'O':
				color = ORANGE_BG;
				break;
			case 'Y':
				color = YELLOW_BG;
				break;
		}
	
		oss << color << BLACK_FG << elem << printSpacing(width - 1) << RESET << (i < SIZE - 1 ? " " : ""); // Decrement (additional width)
	}

	return oss.str();
}

string Cube::printSpacing(int spacing) const {
	ostringstream oss;
	oss << left << setw(spacing) << " ";

	return oss.str();
}

string Cube::printCommandWithSpacing(int spacing, int& counter, bool print) const {
	string res = "";

	if (print) {
		res = printSpacing(spacing) + printCommandSegment(counter);
		counter++;
	} 

	return res;
}

string Cube::printCommandSegment(int line) const {
	switch (line) {
		case 1:
			return "-------------------------------------------------------------------------------";
		case 2:
			return "| COMMANDS                                                                    |";
		case 3:
			return "| --------                                                                    |";
		case 4:
			return "|  U: Clockwise turn on the top side.          Note: You can append a \"'\"     |";
		case 5:
			return "|  L: Clockwise turn on the left side.               to a letter for          |";
		case 6:
			return "|  F: Clockwise turn on the front side.              counterclockwise moves,  |";
		case 7:
			return "|  R: Clockwise turn on the right side.              or \"2\" for two moves.    |";
		case 8: 
			return "|  B: Clockwise turn on the back side.                                        |";
		case 9:
			return "|  D: Clockwise turn on the bottom side.                                      |";
		case 10:
			return "|  x: Clockwise rotation on the x-axis.                                       |";
		case 11:
			return "|  y: Clockwise rotation on the y-axis.                       SOLVE     UNDO  |";
		case 12:
			return "|  z: Clockwise rotation on the z-axis.                       SAVE      EXIT  |";
		case 13:
			return "-------------------------------------------------------------------------------";
		default:
			return "";
	}

	return "";
}

string Cube::printCurrentMoves() const {
	stack<string> temp = currentMoves;
	vector<string> reversed;
	string res = "";
	bool moves = false;

	while (temp.size() > 0) {
		reversed.push_back(temp.top());
		temp.pop();
		moves = true;
	}

	if (moves) {
		for (int i = reversed.size() - 1; i >= 0; i--) {
			res += reversed[i] + " ";
		}
	} else {
		res = "None";
	}

	return showMoves(res);
}

string Cube::showMoves(const string& moves) const {
	int count = countMoves(moves);

	if (count >= MAX_DISPLAY) {
		return "(" + to_string(count) + " moves)";
	} else {
		return moves;
	}
}

string Cube::doMoves(const string& moves, bool update) {
	string res = "";

	if (checkMoves(moves)) {
		// Process the moves if valid.
		res = tokenizeMoves(moves);
		istringstream iss(res);
		string move;

		while (iss >> move) {
			char letter = move[0];
			bool prime = false;
			bool twice = false;

			for (int i = 0; i < move.length(); i++) { // Check for move modifiers
				if (move[i] == '\'') {
					prime = true;
				}

				if (move[i] == '2') {
					twice = true;
				}
			}

			if (!prime && !twice) { // No modifier
				processMove(letter);
			} else if (twice) { // Turning a face twice in either direction ends up the same
				processMove(letter);
				processMove(letter);
			} else { // Prime moves are counterclockwise, which is the same as three times clockwise
				processMove(letter);
				processMove(letter);
				processMove(letter);
			}

			// Add the move to the current moves.
			if (update) {
				currentMoves.push(move);
			}
		}

		// Update the counter.
		if (update) {
			totalMoves += countMoves(res);
		}
	}

	return res;
}

void Cube::processMove(char letter) {
	switch (letter) {
		case 'U':
			turnU();
			break;
		case 'L':
			turnL();
			break;
		case 'F':
			turnF();
			break;
		case 'R':
			turnR();
			break;
		case 'B':
			turnB();
			break;
		case 'D':
			turnD();
			break;
		case 'x':
			rotX();
			break;
		case 'y':
			rotY();
			break;
		case 'z':
			rotZ();
			break;
	}
}

void Cube::undo() {
	if (currentMoves.size() > 0) {
		string undoneMove = currentMoves.top();
		currentMoves.pop();
		bool prime = false;
		bool twice = false;

		for (int i = 0; i < undoneMove.length(); i++) {
			if (undoneMove[i] == '\'') {
				prime = true;
			}

			if (undoneMove[i] == '2') {
				twice = true;
			}
		}

		char letter = undoneMove[0];
		if (!prime && !twice) { // A prime (counterclockwise) move is the opposite of a normal (clockwise) move
			doMoves(string(1, letter) + "'", false);
		} else if (twice) {
			// Remove the double modifier, leaving the rest of the move intact, and perform the opposite move.
			if (prime) {
				processMove(letter);
				undoneMove.erase(2);
			} else {
				doMoves(string(1, letter) + "'", false);
				undoneMove.erase(1);
			}

			currentMoves.push(undoneMove);
		} else { // A prime move is canceled out with a normal move
			processMove(letter);
		}

		totalMoves--;
		cout << "\nUndid " << undoneMove << ".\n";
	} else {
		cout << "\nNo current moves to undo.\n";
	}
}

bool Cube::operator==(const Cube& rhs) const {
	return name == rhs.name && scramble == rhs.scramble && moves == rhs.moves &&
				   currentMoves == rhs.currentMoves && totalMoves == rhs.totalMoves;
}

void Cube::reset() {
	createSolved();
	scramble = "";
	moves = "";
	totalMoves = 0;
}

void Cube::placeFace(const vector<vector<char>>& colors, int face) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			stickers[face][i][j] = colors[i][j];
		}
	}
}

void Cube::placeRow(const vector<char>& colors, int face, int row) {
	for (int i = 0; i < SIZE; i++) {
		stickers[face][row][i] = colors[i];
	}
}

void Cube::placeCol(const vector<char>& colors, int face, int col) {
	for (int i = 0; i < SIZE; i++) {
		stickers[face][i][col] = colors[i];
	}
}

vector<char> Cube::getRow(int face, int row) const {
	vector<char> res;

	for (int i = 0; i < SIZE; i++) {
		res.push_back(stickers[face][row][i]);
	}
	
	return res;
}

vector<char> Cube::getCol(int face, int col) const {
	vector<char> res;

	for (int i = 0; i < SIZE; i++) {
		res.push_back(stickers[face][i][col]);
	}
	
	return res;
}

void Cube::rotateFace(int face) {
	// Get original color values.
	vector<char> topRow = getRow(face, 0);
	vector<char> rightCol = getCol(face, 2);
	vector<char> bottomRow = getRow(face, 2);
	vector<char> leftCol = getCol(face, 0);

	// Reverse the order of the right and left segments to properly translate them to bottom and top respectively.
	reverse(rightCol.begin(), rightCol.end());
	reverse(leftCol.begin(), leftCol.end());

	// Replace each segment of the face.
	placeCol(topRow, face, 2);    // Top to right
	placeRow(rightCol, face, 2);  // Right to bottom
	placeCol(bottomRow, face, 0); // Bottom to left
	placeRow(leftCol, face, 0);   // Left to top
}

void Cube::turnU() {
	// Rotate the top face.
	rotateFace(TOP);

	// Switch sides of the top face.
	vector<char> frontSide = getRow(FRONT, 0);
	vector<char> rightSide = getRow(RIGHT, 0);
	vector<char> backSide = getRow(BACK, 0);
	vector<char> leftSide = getRow(LEFT, 0);

	placeRow(rightSide, FRONT, 0);
	placeRow(backSide, RIGHT, 0);
	placeRow(leftSide, BACK, 0);
	placeRow(frontSide, LEFT, 0);
}

void Cube::turnL() {
	// Rotate the left face.
	rotateFace(LEFT);

	// Switch sides of the left face.
	vector<char> topSide = getCol(TOP, 0);
	vector<char> frontSide = getCol(FRONT, 0);
	vector<char> bottomSide = getCol(BOTTOM, 0);
	vector<char> backSide = getCol(BACK, 2);

	// Reverse back segment so it can be properly translated to the top.
	// Reverse bottom segment so it can be properly translated to the back.
	reverse(backSide.begin(), backSide.end());
	reverse(bottomSide.begin(), bottomSide.end());

	placeCol(backSide, TOP, 0);
	placeCol(topSide, FRONT, 0);
	placeCol(frontSide, BOTTOM, 0);
	placeCol(bottomSide, BACK, 2);
}

void Cube::turnF() {
	// Rotate the front face.
	rotateFace(FRONT);

	// Switch sides of the front face.
	vector<char> topSide = getRow(TOP, 2);
	vector<char> rightSide = getCol(RIGHT, 0);
	vector<char> bottomSide = getRow(BOTTOM, 0);
	vector<char> leftSide = getCol(LEFT, 2);

	// Reverse left segment so it can be properly translated to the top.
	// Reverse right segment so it can be properly translated to the bottom.
	reverse(leftSide.begin(), leftSide.end());
	reverse(rightSide.begin(), rightSide.end());

	placeRow(leftSide, TOP, 2);
	placeCol(topSide, RIGHT, 0);
	placeRow(rightSide, BOTTOM, 0);
	placeCol(bottomSide, LEFT, 2);
}

void Cube::turnR() {
	// Rotate the right face.
	rotateFace(RIGHT);

	// Switch sides of the right face.
	vector<char> topSide = getCol(TOP, 2);
	vector<char> frontSide = getCol(FRONT, 2);
	vector<char> bottomSide = getCol(BOTTOM, 2);
	vector<char> backSide = getCol(BACK, 0);

	// Reverse top segment so it can be properly translated to the back.
	// Reverse back segment so it can be properly translated to the bottom.
	reverse(topSide.begin(), topSide.end());
	reverse(backSide.begin(), backSide.end());

	placeCol(frontSide, TOP, 2);
	placeCol(bottomSide, FRONT, 2);
	placeCol(backSide, BOTTOM, 2);
	placeCol(topSide, BACK, 0);
}

void Cube::turnB() {
	// Rotate the back face.
	rotateFace(BACK);

	// Switch sides of the back face.
	vector<char> topSide = getRow(TOP, 0);
	vector<char> rightSide = getCol(RIGHT, 2);
	vector<char> bottomSide = getRow(BOTTOM, 2);
	vector<char> leftSide = getCol(LEFT, 0);

	// Reverse top segment so it can be properly translated to the left.
	// Reverse bottom segment so it can be properly translated to the right.
	reverse(topSide.begin(), topSide.end());
	reverse(bottomSide.begin(), bottomSide.end());

	placeRow(rightSide, TOP, 0);
	placeCol(bottomSide, RIGHT, 2);
	placeRow(leftSide, BOTTOM, 2);
	placeCol(topSide, LEFT, 0);
}

void Cube::turnD() {
	// Rotate the bottom face.
	rotateFace(BOTTOM);

	// Switch sides of the bottom face.
	vector<char> frontSide = getRow(FRONT, 2);
	vector<char> rightSide = getRow(RIGHT, 2);
	vector<char> backSide = getRow(BACK, 2);
	vector<char> leftSide = getRow(LEFT, 2);

	placeRow(leftSide, FRONT, 2);
	placeRow(frontSide, RIGHT, 2);
	placeRow(rightSide, BACK, 2);
	placeRow(backSide, LEFT, 2);
}

vector<vector<char>> Cube::getFace(int face) const {
	vector<vector<char>> colors;

	for (int i = 0; i < SIZE; i++) {
		vector<char> row;
		
		for (int j = 0; j < SIZE; j++) {
			row.push_back(stickers[face][i][j]);
		}

		colors.push_back(row);
	}

	return colors;
}

vector<vector<char>> Cube::getZFace(int face) const {
	vector<vector<char>> colors;

	for (int i = 0; i < SIZE; i++) { // Start at the left
		vector<char> col;

		for (int j = SIZE - 1; j >= 0; j--) { // Start at the bottom
			col.push_back(stickers[face][j][i]);
		}

		colors.push_back(col);
	}

	return colors;
}

void Cube::rotX() {
	// Rotate the left face counterclockwise.
	rotateFace(LEFT);
	rotateFace(LEFT);
	rotateFace(LEFT);

	// Rotate the right face clockwise.
	rotateFace(RIGHT);

	// Switch the middle faces.
	vector<vector<char>> frontFace = getFace(FRONT);
	vector<vector<char>> bottomFace = getFace(BOTTOM);
	vector<vector<char>> backFace = getFace(BACK);
	vector<vector<char>> topFace = getFace(TOP);

	// Reverse the back and top faces to properly translate them
	// from the back and to the top respectively (reverse their rows and vertical orders).
	for (int i = 0; i < SIZE; i++) {
		reverse(backFace[i].begin(), backFace[i].end());
		reverse(topFace[i].begin(), topFace[i].end());
	}

	vector<char> backFaceBottom = backFace[2];
	backFace[2] = backFace[0];
	backFace[0] = backFaceBottom;

	vector<char> topFaceBottom = topFace[2];
	topFace[2] = topFace[0];
	topFace[0] = topFaceBottom;

	placeFace(frontFace, TOP);
	placeFace(bottomFace, FRONT);
	placeFace(backFace, BOTTOM);
	placeFace(topFace, BACK);
}

void Cube::rotY() {
	// Rotate the top face clockwise.
	rotateFace(TOP);

	// Rotate the bottom face counterclockwise.
	rotateFace(BOTTOM);
	rotateFace(BOTTOM);
	rotateFace(BOTTOM);

	// Switch the middle faces.
	vector<vector<char>> frontFace = getFace(FRONT);
	vector<vector<char>> rightFace = getFace(RIGHT);
	vector<vector<char>> backFace = getFace(BACK);
	vector<vector<char>> leftFace = getFace(LEFT);

	placeFace(frontFace, LEFT);
	placeFace(rightFace, FRONT);
	placeFace(backFace, RIGHT);
	placeFace(leftFace, BACK);
}

void Cube::rotZ() {
	// Rotate the front face clockwise.
	rotateFace(FRONT);

	// Rotate the back face counterclockwise.
	rotateFace(BACK);
	rotateFace(BACK);
	rotateFace(BACK);

	// Switch the middle faces.
	vector<vector<char>> topFace = getZFace(TOP);
	vector<vector<char>> leftFace = getZFace(LEFT);
	vector<vector<char>> bottomFace = getZFace(BOTTOM);
	vector<vector<char>> rightFace = getZFace(RIGHT);

	placeFace(topFace, RIGHT);
	placeFace(leftFace, TOP);
	placeFace(bottomFace, LEFT);
	placeFace(rightFace, BOTTOM);
}