#ifndef _lister_lister_RecordViewGrid_h_
#define _lister_lister_RecordViewGrid_h_

#include <lister/Urp/UrpGrid.h>

class Connection;

//==============================================================================================
class RecordViewGrid : public UrpGrid {
public:
	typedef RecordViewGrid CLASSNAME;
	
                          RecordViewGrid                ();
	                      // Added manually from appending a row.
	virtual void          Build                         (Connection *pconnection);
	virtual void          Load                          ();
	void                  Add                           (const String &columnName, const String &columnValue);
};

#endif
