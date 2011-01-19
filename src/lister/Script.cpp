#include "Script.h"

Script::Script() {
	Script((OutputGrid *)NULL);
}

Script::Script(OutputGrid *poutputGrid) {
	scriptTarget = SO_SCREEN;
	scriptId = UNKNOWN;
	outputGrid = poutputGrid; // Can be null.  Connection object will break if executed and this is not set and Script Target is screen
}

