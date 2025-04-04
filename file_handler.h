#ifndef FILE_HANLDER_H
#define FILE_HANDLER_H

#include "cube.h" // valid moves check
#include <string>
#include <fstream> // output file overwrite? input file for sure. fstream need? maybe not because need multiple search
#include <map>
using namespace std;

class FileHandler { // input same as output. i will overwrite.
public:
	FileHandler();
	bool checkValidFile(const string& file) const;
	void getCubesFromFile(const string& file);
	void saveCubeToFile(const string& file, const Cube& cube);
private: //TODO: save to map, then overwrite output file, or fstream it
	map<string, Cube> savedCubes; // << Cube.name << ", " << Cube.moves << etc.
};

#endif