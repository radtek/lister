#ifndef _lister_lister_OutputSpec_h_
#define _lister_lister_OutputSpec_h_

#include "shared.h"

class OutputColumnDef : Moveable<OutputColumnDef> {
public:
	String name;
	//int    valType; // U++ Value type
	int    sqlType; // Code from sql.h, odbc def, etc
	String sqlTypeName;
	int    visibleWidth; // Calculated when loading OutputGrid, useful for printing
	int    groupNo; // If used for separation grouping; -1 if not.
};

class OutputSpec : Moveable<OutputSpec> {
public:
	Vector<OutputColumnDef> outputColumnDefList;
	
	void Clear();
};

#endif
