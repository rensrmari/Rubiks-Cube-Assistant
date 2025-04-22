#include "file_handler_tests.h"
#include "tester.h"
#include "file_handler.h"
#include "cube.h"
#include <fstream>
using namespace std;

int FileHandlerTests::runTests() const {
    Tester tester;

    // Test default constructor
    {
        FileHandler handler;
        tester.test("File name is empty after default constructor is called", "" == handler.getFileName());
    }

    // Test overloaded constructor
    {
        FileHandler handler("test.txt");
        tester.test("File name is assigned after overloaded constructor is called", "test.txt" == handler.getFileName());
    }

    // Test setting the filename
    {
        FileHandler handler;
        handler.setFileName("testSetName.txt");
        tester.test("File name is able to be set", "testSetName.txt" == handler.getFileName());
    }

    // Test if checking an empty file specified to be empty results in valid
    {
        FileHandler handler("test_files/empty.txt");
        bool passed = handler.checkValidFile(true) == FileHandler::VALID;
        tester.test("Empty files are considered valid if empty files are specified in the argument", passed);
    }

    // Test if checking an empty file specified not to be empty reports that there is no data
    {
        FileHandler handler;
        handler.setFileName("test_files/empty.txt");
        bool passed = handler.checkValidFile(false) == FileHandler::NO_DATA;
        tester.test("Empty files are considered invalid if empty files are specified", passed);
    }

    // Test if checking a valid file with CSVs results in valid
    {
        FileHandler handler("test_files/valid_csv.txt");
        bool passed = handler.checkValidFile(false) == FileHandler::VALID;
        tester.test("Valid CSV files are considered valid if empty files are not specified in the argument", passed);
    }

    // Test if checking an invalid file with improper formatting reports bad formatting
    {
        FileHandler handler("test_files/invalid_format.txt");
        bool passed = handler.checkValidFile(false) == FileHandler::BAD_FORMAT;
        tester.test("Files with improper formatting are caught", passed);
    }

    // Test if checking an invalid file with duplicate names reports duplicate names
    {
        FileHandler handler("test_files/duplicate_names.txt");
        bool passed = handler.checkValidFile(false) == FileHandler::DUPLICATE_NAMES;
        tester.test("Files with duplicate names are caught", passed);
    }

    // Test if checking an invalid file with invalid moves reports invalid moves
    {
        FileHandler handler("test_files/invalid_moves.txt");
        bool passed = handler.checkValidFile(false) == FileHandler::INVALID_MOVES;
        tester.test("Files with invalid moves are caught", passed);
    }

    // Test if checking an invalid file with conflicting moves and move counts reports the discrepancy
    {
        FileHandler handler("test_files/mismatched_moves.txt");
        bool passed = handler.checkValidFile(false) == FileHandler::MISMATCHED_MOVES;
        tester.test("Files with conflicting moves and move counts are caught", passed);
    }

    // Test if processing a valid file is accepted
    {
        FileHandler handler("test_files/valid_csv.txt");
        bool passed = handler.processValidFile();
        tester.test("The result of processing a valid file is displayed as successful", passed);
    }

    // Test if processing an invalid file is not accepted
    {
        FileHandler handler("test_files/invalid_moves.txt");
        bool passed = !handler.processValidFile();
        tester.test("The result of processing an invalid file is displayed as unsuccessful", passed);
    }

    // Test if setting a cube to data stored in a valid file is successful
    {
        FileHandler handler("test_files/valid_csv.txt");
        Cube cube;
        bool passed = handler.loadCube(cube, "Clarence Mariano");
        tester.test("Loading data from a valid file into a cube object is displayed as successful", passed);
    }

    // Test if setting a cube to data stored in an invalid file is unsuccessful
    {
        FileHandler handler("test_files/invalid_format.txt");
        Cube cube;
        bool passed = !handler.loadCube(cube, "test");
        tester.test("Loading data from an invalid file into a cube object is displayed as unsuccessful", passed);
    }

    // Test if checking a name not in a valid file results in displaying that it is available
    {
        FileHandler handler("test_files/valid_csv.txt");
        bool passed = !handler.checkTaken("Unique Name");
        tester.test("Using a name not already in a valid file reflects that the name is available", passed);
    }

    // Test if checking a name in a valid file results in displaying that is it unavailable
    {
        FileHandler handler("test_files/valid_csv.txt");
        bool passed = !handler.checkTaken("Clarence Mariano");
        tester.test("Using a name already in a valid file reflects that the name is unavailable", passed);
    }

    // Test if saving a cube to an invalid file is unsuccessful
    {
        FileHandler handler("test_files/invalid_format.txt");
        Cube cube;
        bool passed = !handler.saveCubeToFile(cube);
        tester.test("Saving a cube to an invalid file is displayed as unsuccessful", passed);
    }

    return tester.finishTests();
}

