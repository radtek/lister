#ifndef _CursorHandler_h_
#define _CursorHandler_h_

#include "shared.h"
#include "Script.h"
#include "JobSpec.h"

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
	void RebuildTableFromConnection(String outputTable, JobSpec &jobSpec);
	int LoadIntoTableFromConnectionPREP(String outputTable, int rowLimit, JobSpec &jobSpec);
	int LoadIntoTableFromConnectionCOPY(String outputTable, int rowLimit, JobSpec &jobSpec);
	int LoadIntoScreenGridFromConnection(OutputGrid *outputGrid, JobSpec &jobSpec);

public:
	CursorHandler(Connection *pcontrolConnection, Connection *pconnection);
	void ColSize(OutputGrid *outputGrid, Sql *cursor);
	bool Run(Script &sob, JobSpec &jobSpec);
};

#endif

