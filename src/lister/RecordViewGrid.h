#ifndef _lister_lister_RecordViewGrid_h_
#define _lister_lister_RecordViewGrid_h_

#include "ConnectedCtrl.h"
#include <lister/Urp/UrpGrid.h>

class Connection;

//==============================================================================================
class RecordViewGrid : public UrpGrid, ConnectedCtrl {
public:
	typedef RecordViewGrid CLASSNAME;
                      RecordViewGrid();
	                  // Added manually from appending a row.
	void              Build();
	virtual void      Load(Connection *pconnection);
	void              Add(const String &columnName, const String &columnValue);
};

#endif
