#ifndef _CursorHandler_h_
#define _CursorHandler_h_

#include "shared.h"
class Connection;
class OutputGrid;
//==========================================================================================	
class CursorHandler : public TopWindow {
public:
	OutputGrid *outputGrid;
	Connection *controlConnection; // Our metadata about the query we will be running
	Connection *connection; // Our work connection, for fetching user output

	Vector<int> cw; // Column width calculator.  We add in heading widths and value widths as we load the data
	
	CursorHandler(Connection *pcontrolConnection, Connection *pconnection);
	void ColSize(Sql *cursor);
	bool Run(OutputGrid *poutputGrid, String sql, bool loadIntoTable = false);
};

#endif

