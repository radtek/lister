#ifndef _lister_lister_OutputGrid_h_
#define _lister_lister_OutputGrid_h_

#include "shared.h"
#include "OutputSpec.h"
#include <lister/Urp/UrpGrid.h>
#include "RecordViewWin.h" // For vertical view of a single row.

class Connection;

//==============================================================================================
class OutputGrid : public UrpGrid {
public:
	typedef OutputGrid CLASSNAME;

	// What do we do after our custom keyhandler runs?
	enum KeyProcessorResponse {
		                  ALLOWDEFAULTKEYPROCESSORTORUN
	,	                  SIGNALWEPROCESSEDKEY
	,	                  SIGNALWEDIDNOTPROCESSKEY
	};
	
	OutputSpec            outputSpec;  // Reset each time from CursorHandler
	bool                  maingridselectrow;
	RecordViewWin         recordViewWin;
	
	                      OutputGrid                    ();
	virtual void          Build                         (Connection *pconnection);
	virtual void          Load                          ();
	void                  ProcessCut                    ();
	virtual bool          Key                           (dword key, int count);
	void                  MainGridContextMenu           (Bar &bar);
	void                  PopUpRecordView               ();
	                  
	                      // Either select whole row or cells.  For long rows it is helpful to 
	                      // select the entire row for visual comparison.
	void                  ToggleMainGridSelectRow       (); 
	                  
	                      // Copy all the column names to a list so we can paste them back into the query, 
	                      // replacing "*" markers so we can subtract specific columns or insert calc cols at certain points
	void                  CopyColListCommaDelimWthPrefix(); 
	void                  CopyColListCommaDelim         (); 
	
	                      // List grouped by data type so we can filter
	void                  CopyColListCommaDelimByType   (); 
};

#endif
