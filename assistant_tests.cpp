#include "assistant_tests.h"
#include "tester.h"
#include "assistant.h"
#include "cube.h"
using namespace std;

int AssistantTests::runTests() const {
    Tester tester;

    // Test the solve when the cube is scrambled
    {
        Cube cube;
        string scramble = "RURFDLB'F2DLR'F2LB2xyz";
        cube.doMoves(scramble, false);
        Assistant assistant(cube);
        assistant.solve();
        tester.test("When the cube is scrambled, it will reach a solved state at the end", cube.checkSolved());
    }

    // Test the solve when the cube is already solved
    {
        Cube cube;
        Assistant assistant(cube);
        assistant.solve();
        tester.test("When the cube is solved, it is treated like so", cube.checkSolved());
    }

    return tester.finishTests();
}