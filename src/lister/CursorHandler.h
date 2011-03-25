#ifndef _CursorHandler_h_
#define _CursorHandler_h_

#include "shared.h"
#include "Script.h"
#include "JobSpec.h"
#include "ContextMacros.h"
#include "OutputGrid.h"

class Connection;
//class OutputGrid;
//==============================================================================================
class CursorHandler : public TopWindow {
public:
	typedef CursorHandler CLASSNAME;
	
	Connection *controlConnection; // Our metadata about the query we will be running
	Connection *connection; // Our work connection, for fetching user output
	Vector<int> cw; // Column width calculator.  We add in heading widths and value widths as we load the data

protected:
	// Internal support functions for the Run function
	void RebuildTableFromConnection(String schema, String outputTable, JobSpec &jobSpec, ContextMacros *contextMacros);
	int LoadIntoTableFromConnectionPREP(String schema, String outputTable, int rowLimit, JobSpec &jobSpec);
	int LoadIntoTableFromConnectionCOPY(String schema, String outputTable, int rowLimit, JobSpec &jobSpec, String &copyerr);
	int LoadIntoScreenGridFromConnection(OutputGrid *outputGrid, JobSpec &jobSpec, Script &sob);

public:
	CursorHandler(Connection *pcontrolConnection, Connection *pconnection);
	void ColSize(OutputGrid *outputGrid, Sql *cursor);
	void CursorOutputCancelled();
	bool Run(Script &sob, JobSpec &jobSpec, ContextMacros *contextMacros);
};

#endif

