#ifndef ASSISTANT_H
#define ASSISTANT_H

#include "cube.h"

class Assistant {
public:
	/**
	 * Constructor for Assistant object.
	 * @param cube The Cube to access.
	 */
	Assistant(Cube& cube);

	/**
	 * 
	 */
	void solve();

	/**
	 * 
	 */

private:
	/**
	 * 
	 */
	void checkNext();

	Cube cube; // TODO: really? or pointer? pointer?
};

// TODO: check valid moves

#endif