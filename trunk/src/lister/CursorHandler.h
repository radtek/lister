#ifndef _CursorHandler_h_
#define _CursorHandler_h_

#include "shared.h"
#include "Script.h"

class Connection;
class OutputGrid;
//==============================================================================================
class CursorHandler : public TopWindow {
public:
	Connection *controlConnection; // Our metadata about the query we will be running
	Connection *connection; // Our work connection, for fetching user output
	Vector<int> cw; // Column width calculator.  We add in heading widths and value widths as we load the data

protected:
	// Internal support functions for the Run function
	void RebuildTableFromConnection(String outputTable, bool log = false);
	int LoadIntoTableFromConnectionPREP(String outputTable, int rowLimit, bool log = false);
	int LoadIntoTableFromConnectionCOPY(String outputTable, int rowLimit, bool log = false);
	int LoadIntoScreenGridFromConnection(OutputGrid *outputGrid, bool log = false);

public:
	CursorHandler(Connection *pcontrolConnection, Connection *pconnection);
	void ColSize(OutputGrid *outputGrid, Sql *cursor);
	bool Run(Script &sob, bool log = false);
};

#endif

