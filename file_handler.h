#ifndef FILE_HANLDER_H
#define FILE_HANDLER_H

#include "cube.h"
#include <string>
#include <fstream>
#include <map>
using namespace std;

class FileHandler {
public:
	enum VALID_STATUS { VALID, CANT_OPEN, NO_DATA, BAD_FORMAT, DUPLICATE_NAMES, INVALID_MOVES, MISMATCHED_MOVES };

	/**
	 * Default constructor for FileHandler.
	 */
	FileHandler();

	/**
	 * Constructor for FileHandler.
	 * @param file The name of file.
	 */
	FileHandler(const string& file);

	/**
	 * Gets the stored file name.
	 * @return The name of the file.
	 */
	string getFileName() const;

	/**
	 * Sets the stored file name.
	 * @param file The file name to store.
	 */
	void setFileName(const string& file);
	
	/**
	 * Checks if the given file is valid (CSV of unique names, scramble, moves, and total moves).
	 * @param acceptEmpty Whether or not empty files should be considered valid.
	 * @return An integer representing whether or not the file is valid.
	 */
	int checkValidFile(bool acceptEmpty) const;

	/**
	 * Displays an error message that shows up when a file is invalid.
	 * @param status An integer representing the status of a file.
	 */
	void displayError(int status) const;

	/**
	 * Stores all the Cube objects from a valid file.
	 * @return Whether or not the process was successful.
	 */
	bool processValidFile();

	/**
	 * Displays a table of all the stored names associated with saved Cubes as well as their move counts.
	 */
	void displaySavedCubes() const;

	/**
	 * Sets a given Cube to a Cube mapped to a name.
	 * @param cube The cube to modify.
	 * @param name The name of the Cube.
	 * @return Whether or not the load was successful.
	 */
	bool loadCube(Cube& cube, const string& name) const;

	/**
	 * Checks if a name is taken in a file.
	 * @param name A name of a cube.
	 * @return Whether or not the name is taken.
	 */
	bool checkTaken(const string& name) const;

	/**
	 * Saves a given Cube into the file.
	 * @param cube The Cube to save.
	 */
	void saveCubeToFile(const Cube& cube);

	/**
	 * Resets the FileHandler object.
	 */
	void reset();
private:
	/**
	 * Processes a portion of a line until a comma is reached.
	 * @param iss The istringstream to update.
	 * @param str The string to update.
	 * return A string before the commas.
	 */
	void processLine(istringstream& iss, string& str) const;

	/**
	 * Displays a row of a table.
	 * @param first The first value.
	 * @param second The second value.
	 * @param width The column width.
	 */
	void displayTableRow(const string& first, const string& second, int width) const;

	/**
	 * Produces a line of Cube data and saves it to a file.
	 * @param ofs The ofstream to write to.
	 * @param cube The Cube to process.
	 */
	void convertCubeData(ofstream& ofs, const Cube& cube) const;

	string file;
	map<string, Cube> savedCubes;
};

#endif