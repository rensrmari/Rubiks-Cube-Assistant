#include "cube_tests.h"
#include "tester.h"
#include "cube.h"
#include "sticker_data.h"
#include <string>
#include <utility>
using namespace std;

int CubeTests::runTests() const {
    Tester tester;

    // Test templated string construction for valid colors
    {
        bool passed = Cube::getData(Cube::COLOR_STRINGS, { 'W', 'O', 'G', 'R', 'B', 'Y' }) == "White-Orange-Green-Red-Blue-Yellow";
        tester.test("String constructed from valid colors results in hyphens between each color", passed);
    }

    // Test templated string construction for invalid colors
    {
        bool passed = Cube::getData(Cube::COLOR_STRINGS, { 'x', 'a', 'b' }) == "";
        tester.test("Templated string constructed from invalid colors results in empty string", passed);
    }

    // Test templated string construction for valid faces
    {
        bool passed = Cube::getData(Cube::FACE_STRINGS, { 0, 1, 2, 3, 4, 5 }) == "Top-Left-Front-Right-Back-Bottom";
        tester.test("Templated string constructed from valid faces results in hyphens between each face", passed);
    }

    // Test templated string construction for invalid faces
    {
        bool passed = Cube::getData(Cube::FACE_STRINGS, { -1, -2, -3 }) == "";
        tester.test("Templated string constructed from invalid faces results in empty string", passed);
    }

    // Test string construction for valid colors
    {
        bool passed = Cube::getColors({ 'W', 'O', 'G', 'R', 'B', 'Y' }) == "White-Orange-Green-Red-Blue-Yellow";
        tester.test("String constructed from valid colors results in hyphens between each color", passed);
    }

    // Test string construction for invalid colors
    {
        bool passed = Cube::getColors({ 'x', 'a', 'b' }) == "";
        tester.test("String constructed from invalid colors results in empty string", passed);
    }

    // Test string construction for valid faces
    {
        bool passed = Cube::getFaces({ 0, 1, 2, 3, 4, 5 }) == "Top-Left-Front-Right-Back-Bottom";
        tester.test("String constructed from valid faces results in hyphens between each face", passed);
    }

    // Test string construction for invalid faces
    {
        bool passed = Cube::getFaces({ -1, -2, -3 }) == "";
        tester.test("String constructed from invalid faces results in empty string", passed);
    }

    // Test color comparison for equal color pairs
    {
        bool passed = Cube::checkColors({ 'W', 'O' }, { 'O', 'W' });
        tester.test("Comparing pairs of identical colors results in true", passed);
    }

    // Test color comparison for non-equal color pairs
    {
        bool passed = !Cube::checkColors({ 'W', 'O' }, { 'W', 'G' });
        tester.test("Comparing pairs of non-equal colors results in false", passed);
    }

    // Test move checking for a valid move sequence
    {
        bool passed = Cube::checkMoves("RUR'URU2R'");
        tester.test("A valid set of moves is considered valid", passed);
    }

    // Test move checking for an invalid move sequence
    {
        bool passed = !Cube::checkMoves("abcdRUR'URU2R'");
        tester.test("An invalid set of moves is not considered valid", passed);
    }

    // Test move tokenization
    {
        bool passed = Cube::tokenizeMoves("RUR'URU2R'") == "R U R' U R U2 R'";
        tester.test("Each move is properly separated by a space, with no leading or trailing whitespace", passed);
    }

    // Test move counting
    {
        bool passed = Cube::countMoves("RUR'URU2R'") == 8;
        tester.test("Moves are properly counted (clockwise/counterclockwise = 1, double moves = 2)", passed);
    }

    // Test default constructor
    {
        Cube cube;
        bool passed = (cube.getName() == "") && (cube.getScramble() == "") && (cube.getMoves() == "") && (cube.getTotalMoves() == 0) && (cube.checkSolved());
        tester.test("Upon construction, all data members are defaulted, and the cube is in a solved state", passed);
    }

    // Test overloaded constructor
    {
        string name = "Test";
        string scramble = "RUR'URU2R'";
        string moves = "RURF";
        int totalMoves = 4;

        Cube cube(name, scramble, moves, totalMoves);
        bool passed = (cube.getName() == name) && (cube.getScramble() == scramble) && (cube.getMoves() == moves) && (cube.getTotalMoves() == totalMoves);
        tester.test("Overloaded construction results in data members being assigned properly", passed);
    }

    // Test getting a color from a valid location
    {
        Cube cube;
        bool passed = cube.getAt(0, 0, 0) == 'W'; // Top is white by default
        tester.test("Getting a color from a valid location results in the expected color being returned", passed);
    }

    // Test getting a color from an invalid location
    {
        Cube cube;
        bool passed = cube.getAt(-1, -1, -1) == '\0';
        tester.test("Getting a color from an invalid location is reflected accurately", passed);
    }

    // Test finding the face of a valid center color
    {
        Cube cube;
        bool passed = cube.findCenter('W') == 0; // White is on top (face 0)
        tester.test("Locating a face containing a valid color returns the correct face", passed);
    }

    // Test finding the face of an invalid center color
    {
        Cube cube;
        bool passed = cube.findCenter('x') == -1;
        tester.test("Locating a face containing an invalid color returns an indicator that it was not found", passed);
    }

    // Test locating an edge based on two valid colors.
    {
        Cube cube;
        StickerData edgeSticker = cube.findEdge('W', 'O'); // This sticker will be on the top left edge of the cube, on either side
        bool passed = (edgeSticker.row == 1 && edgeSticker.col == 0 || edgeSticker.row == 0 && edgeSticker.col == 1)
                    && (edgeSticker.face == Cube::TOP || edgeSticker.face == Cube::LEFT)
                    && (edgeSticker.color == 'W' || edgeSticker.color == 'O');
        tester.test("Locating an edge with two valid colors results in the expected edge properties", passed);
    }

    // Test locating an edge with invalid colors.
    {
        Cube cube;
        StickerData edgeSticker = cube.findEdge('x', 'x');
        bool passed = (edgeSticker.row == -1 && edgeSticker.col == -1) && (edgeSticker.face == -1) && (edgeSticker.color == '\0');
        tester.test("Locating an edge with invalid colors reflects that nothing was found", passed);
    }

    // Test getting an edge sticker's adjacent sticker using valid faces and coordinates.
    {
        Cube cube;
        StickerData adjSticker = cube.getAdjEdge(0, 1, 0);
        bool passed = (adjSticker.row == 0 && adjSticker.col == 1) && (adjSticker.face == 1) && (adjSticker.color == 'O');
        tester.test("Getting the adjacent edge sticker using valid data results in the expected sticker", passed);
    }

    // Test getting an edge sticker's adjacent sticker with invalid data.
    {
        Cube cube;
        StickerData adjSticker = cube.getAdjEdge(-1, -1, -1);
        bool passed = (adjSticker.row == -1 && adjSticker.col == -1) && (adjSticker.face == -1) && (adjSticker.color == '\0');
        tester.test("Getting the adjacent edge sticker using invalid data reflects that nothing was found", passed);
    }

    // Test the checking of an edge's adjacent sticker matching its face's center color when it is solved
    {
        Cube cube;
        bool passed = cube.checkEdgeMatch(0, 'W', { 1, 0 }); // Cube is already solved, so edges must match
        tester.test("When the adjacent stickers of an edge matches with their faces' center color, it is accurately reflected", passed);
    }

    // Test the checking of an edge's adjacent sticker matching its face's center color when it should not match
    {
        Cube cube;
        cube.doMoves("U", false); // Turn upper face to displace all adjacent stickers on the white face
        bool passed = !cube.checkEdgeMatch(0, 'W', { 1, 0 });
        tester.test("When the adjacent stickers of an edge does not match their faces' center color, it results in false", passed);
    }

    // Test finding a corner with valid data
    {
        Cube cube;
        pair<StickerData, pair<StickerData, StickerData>> corner = cube.findCorner('W', { 'O', 'G' }); // Top, left, front face
        StickerData baseSticker = corner.first;
        pair<StickerData, StickerData> adjStickers = corner.second;

        bool passed = (baseSticker.face == Cube::TOP)
                    && (adjStickers.first.face == Cube::LEFT && adjStickers.second.face == Cube::FRONT
                    || adjStickers.first.face == Cube::FRONT && adjStickers.second.face == Cube::LEFT);

        tester.test("Locating a corner based on valid data results in the correct corner found", passed);
    }

    // Test finding a corner with invalid data
    {
        Cube cube;
        pair<StickerData, pair<StickerData, StickerData>> corner = cube.findCorner('x', { 'x', 'x' });
        bool passed = corner.first.row == -1;
        tester.test("Locating a corner based on invalid data reflects that nothing was found", passed);
    }

    // Testing getting adjacent corners with valid data
    {
        Cube cube;
        pair<StickerData, StickerData> adjStickers = cube.getAdjCorners(0, 0, 0); // Top, left, back corner
        bool passed = (adjStickers.first.face == Cube::LEFT && adjStickers.second.face == Cube::BACK)
                    || (adjStickers.first.face == Cube::BACK && adjStickers.second.face == Cube::LEFT);
        tester.test("Getting a corner's adjacent stickers with valid data results in the expected corner", passed);
    }

    // Testing getting adjacent corners wil invalid data
    {
        Cube cube;
        pair<StickerData, StickerData> adjStickers = cube.getAdjCorners(-1, -1, -1);
        bool passed = adjStickers.first.row == -1;
        tester.test("Getting a corner's adjacent stickers with invalid data reflects that nothing was found", passed);
    }

    // Testing getting the side-facing stickers of a corner with valid data
    {
        Cube cube;
        pair<StickerData, pair<StickerData, StickerData>> topLeftFront = cube.findCorner('W', { 'O', 'G' }); // Orange and green are side-facing stickers
        vector<StickerData> stickers = { topLeftFront.first, topLeftFront.second.first, topLeftFront.second.second };
        pair<int, int> sideIdxs = cube.getSideCorners(stickers);

        char sideColor1 = stickers[sideIdxs.first].color;
        char sideColor2 = stickers[sideIdxs.second].color;

        bool passed = Cube::checkColors({ sideColor1, sideColor2 }, { 'O', 'G' });
        tester.test("Getting the indices of a corner's side-facing stickers results in the expected elements when supplied with valid stickers", passed);
    }

    // Test getting the side-facing stickers of a corner with invalid data
    {
        Cube cube;
        pair<StickerData, pair<StickerData, StickerData>> topLeftFront = cube.findCorner('x', { 'x', 'x' });
        vector<StickerData> stickers = { topLeftFront.first, topLeftFront.second.first, topLeftFront.second.second };
        pair<int, int> sideIdxs = cube.getSideCorners(stickers);

        bool passed = sideIdxs.first == -1 && sideIdxs.second == -1;
        tester.test("Getting the indices of side-facing stickers of an invalid corner reflects that nothing was found", passed);
    }

    // Test checking if a corner is in its correct position when it should be
    {
        Cube cube;
        bool passed = cube.checkCornerPosition('W', { 'O', 'G' });
        tester.test("Corners in their correct position are correctly regarded like so", passed);
    }

    // Test checking if a corner is in its correct position when it is not
    {
        Cube cube;
        cube.doMoves("U", false); // Turn the upper face, thereby displacing the top corners
        bool passed = !cube.checkCornerPosition('W', { 'O', 'G' });
        tester.test("Corners not in their correct position are correctly regarded like so", passed);
    }

    // Test checking if the Cube is solved when it is solved
    {
        Cube cube; // Solved by default
        tester.test("The cube's solved state is correctly reflected", cube.checkSolved());
    }

    // Test checking if the Cube is solved when it is not solved
    {
        Cube cube;
        cube.doMoves("U", false); // Breaks the solve
        tester.test("The cube's unsolved state is correctly reflected", !cube.checkSolved());
    }

    // Test getting the name
    {
        string name = "Test";
        Cube cube(name, "", "", 0);
        tester.test("The cube's name is properly received", cube.getName() == name);
    }

    // Test setting the name
    {
        Cube cube;
        string name = "Test";
        cube.setName(name);
        tester.test("The cube's name is properly set", cube.getName() == name);
    }

    // Test getting the scramble
    {
        string scramble = "RURF";
        Cube cube("", scramble, "", 0);
        tester.test("The cube's scramble is properly received", cube.getScramble() == scramble);
    }

    // Test setting the scramble
    {
        Cube cube;
        string scramble = "RURF";
        cube.setScramble(scramble);
        tester.test("The cube's scramble is properly set", cube.getScramble() == scramble);
    }

    // Test getting the moves
    {
        string moves = "RURF";
        Cube cube("", "", moves, moves.length());
        tester.test("The cube's moves is properly received", cube.getMoves() == moves);
    }

    // Test getting the current moves
    {
        Cube cube;
        string moves = "RURF";
        cube.doMoves(moves, true);
        tester.test("The current moves is properly received", cube.getCurrentMoves() == moves);
    }

    // Test getting the total moves
    {
        int totalMoves = 4;
        Cube cube("", "", "RURF", totalMoves);
        tester.test("The total moves are properly received", cube.getTotalMoves() == totalMoves);
    }

    // Test doing the moves
    {
        Cube cube;
        cube.doMoves("U", false); // Top row of green is now on the left side
        bool passed = cube.getAt(Cube::LEFT, 0, 0) == 'G';
        tester.test("Performing moves functions as expected", passed);
    }

    // Test undoing moves after performing moves
    {
        Cube cube;
        string moves = "RURF";
        cube.doMoves(moves, true);
        cube.undo();
        string expected = "RUR";
        tester.test("Undoing after performing moves results takes away a letter from the current moves", cube.getCurrentMoves() == expected);
    }

    // Test undoing moves without moves done
    {
        Cube cube;
        cube.undo();
        tester.test("Undoing nonexistent moves results in no change to the cube's state", cube.getCurrentMoves() == "");
    }

    // Test reset
    {
        Cube cube("Test", "RDLF", "U", 1);
        cube.reset();
        bool passed = (cube.getName() == "") && (cube.getScramble() == "") && (cube.getMoves() == "") && (cube.getTotalMoves() == 0) && cube.checkSolved();
        tester.test("Resetting the cube sets data members to default", passed);
    }

    return tester.finishTests();
}