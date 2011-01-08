#ifndef _CursorHandler_h_
#define _CursorHandler_h_

#include "shared.h"
#include "SoundHandler.h"
#include "Connection.h"

//==========================================================================================	
class CursorHandler : public TopWindow {
public:
	GridCtrl *outputGrid;
	Connection *connection;
	Vector<int> cw; // Column width calculator.  We add in heading widths and value widths as we load the data
	
	CursorHandler(Connection *pconnection);
	void ColSize(Sql *cursor);
	bool Run(GridCtrl *poutputGrid, String sql);
};

#endif

