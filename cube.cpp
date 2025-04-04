#include "cube.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

const string Cube::VALID_MOVES = "ULFRBD";
const map<char, string> Cube::COLOR_STRINGS = { // for hint message, annotation of step
	{ 'W', "White" },
	{ 'G', "Green" },
	{ 'R', "Red" },
	{ 'B', "Blue" },
	{ 'O', "Orange" },
	{ 'Y', "Yellow" },
};
const map<int, char> Cube::FACE_COLORS = {
	{ Faces::TOP, 'W' },
	{ Faces::LEFT, 'G' },
	{ Faces::FRONT, 'R' },
	{ Faces::RIGHT, 'B' },
	{ Faces::BACK, 'O' },
	{ Faces::BOTTOM, 'Y' }
};

Cube::Cube() {
	name = "";
	scramble = "";
	moves = "";
	totalMoves = 0;
}

Cube::Cube(const string& name, const string& scramble, const string& moves, const int totalMoves) {
	this->name = name;
	this->scramble = scramble;
	this->moves = moves;
	this->totalMoves = totalMoves;
	doMoves(scramble); // check valid first? TODO
	doMoves(moves);
}

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

void Cube::displayCube() const {
	const int LEFT_PADDING = 8;
	const int FACE_WIDTH = 5;
	const int FACE_SPACING = 4;
	const int MIDDLE_PADDING = LEFT_PADDING + FACE_WIDTH + FACE_SPACING;

	// Print the top segments of the cube.
	cout << printSpacing(MIDDLE_PADDING) << "TOP\n"
	     << printSpacing(MIDDLE_PADDING) << printFaceSegment(Faces::TOP, 0) << endl
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(Faces::TOP, 1) << endl
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(Faces::TOP, 2) << endl << endl;

	// Print the left, front, right, and back segments of the cube.
	cout << printSpacing(LEFT_PADDING) << "LEFT"                           // Headers
		 << printSpacing(FACE_WIDTH - 4 + FACE_SPACING) << "FRONT"
		 << printSpacing(FACE_WIDTH - 5 + FACE_SPACING) << "RIGHT"
		 << printSpacing(FACE_WIDTH - 5 + FACE_SPACING) << "BACK" << endl;

	cout << printSpacing(LEFT_PADDING) << printFaceSegment(Faces::LEFT, 0) // 1st rows
		<< printSpacing(FACE_SPACING) << printFaceSegment(Faces::FRONT, 0)
		<< printSpacing(FACE_SPACING) << printFaceSegment(Faces::RIGHT, 0)
		<< printSpacing(FACE_SPACING) << printFaceSegment(Faces::BACK, 0) << endl;

	cout << printSpacing(LEFT_PADDING) << printFaceSegment(Faces::LEFT, 1) // 2nd rows
		<< printSpacing(FACE_SPACING) << printFaceSegment(Faces::FRONT, 1)
		<< printSpacing(FACE_SPACING) << printFaceSegment(Faces::RIGHT, 1)
		<< printSpacing(FACE_SPACING) << printFaceSegment(Faces::BACK, 1) << endl;

	cout << printSpacing(LEFT_PADDING) << printFaceSegment(Faces::LEFT, 2) // 3rd rows
		<< printSpacing(FACE_SPACING) << printFaceSegment(Faces::FRONT, 2)
		<< printSpacing(FACE_SPACING) << printFaceSegment(Faces::RIGHT, 2)
		<< printSpacing(FACE_SPACING) << printFaceSegment(Faces::BACK, 2) << endl << endl;

	// Print the bottom segments of the cube.
	cout << printSpacing(MIDDLE_PADDING) << "BOTTOM\n"
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(Faces::BOTTOM, 0) << endl
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(Faces::BOTTOM, 1) << endl
		 << printSpacing(MIDDLE_PADDING) << printFaceSegment(Faces::BOTTOM, 2) << endl << endl;
}

string Cube::printFaceSegment(const int side, const int row) const {
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
	
		oss << color << elem << RESET << (i < SIZE - 1 ? " " : "");
	}

	return oss.str();
}

string Cube::printSpacing(const int spacing) const {
	ostringstream oss;
	oss << left << setw(spacing) << " ";

	return oss.str();
}

void Cube::doMoves(const string& moves) {

}

bool checkMoves(const string& moves) {

}