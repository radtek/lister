#ifndef _lister_lister_OutputGrid_h_
#define _lister_lister_OutputGrid_h_

#include "shared.h"
#include "OutputSpec.h"

class Connection;

class OutputGrid : public UrpGrid {
public:
	// What do we do after our custom keyhandler runs?
	enum KeyProcessorResponse {
		ALLOWDEFAULTKEYPROCESSORTORUN
	,	SIGNALWEPROCESSEDKEY
	,	SIGNALWEDIDNOTPROCESSKEY
	};
	OutputSpec        outputSpec;  // Reset each time from CursorHandler

	                  OutputGrid();
	void              Build();
	void              Load(Connection *pconnection);
	void              ProcessCut();
	virtual bool      Key(dword key, int count);
};

#endif
