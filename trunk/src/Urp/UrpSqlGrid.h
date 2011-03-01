#ifndef _Urp_UrpSqlGrid_h_
#define _Urp_UrpSqlGrid_h_

#include "UrpShared.h"
#include "UrpGridCommon.h"

#define HIDDEN_COLUMN -2 // TODO: Being used?
#define NOSELECTION   -4

class UrpSqlGrid: public SqlArray, public UrpGridCommon {
public:
	typedef UrpSqlGrid CLASSNAME;

	int           lastinsertedpkid;
	Callback      WhenNeedLastInsertedPkId;
	              UrpSqlGrid();
	void          DragRow(); // Support drag and drop; May not be that useful.
	void          DropRow(int line, PasteClip& d); // except in taskscripts ordering/re-ordering easily.
	// Override since the standard function expects the pk to be named "id" in order to fetch
	// from Postgre sequence.
	virtual bool  UpdateRow();
	// Obviously, U++ developer Uno was being a total dickwad when he refused to give a function
	// to extract column widths, so I've created it.
	int           GetFloatingColumnWidth(int colno);
	void          SetSortColumn(Id id, bool desc = false);
	int           GetColumnNo(Id id);
	// Series of corrective functions to deal with Lamo Uno's confusion about when and when not to adjust references for fixed columns
	int           GetFloatingColumnCount();
	void          UnhideFloatingColumnSilently(int i);
	void          HideFloatingColumn(int i);
	void          HideColumn(Id id); // TODO: Clean up names
	void          SetFloatingColumnWidth(int i, int w);
	Id            GetFloatingColumnId(int n) const;
	bool          IsColumnHidden(int i);
	void          SetColumnMinWidth(int i, int w);
	int           SelectionCount();
	// Useful for when grid is not active and cursor is -1, but an item is selected
	int           GetFirstSelection();
	Value         GetMaxValue(Id column);
		
	//	GridCtrl::ItemRect& GetFloatingColumn(int n);
	//  GridCtrl is supposed to Xmlize, but I don't see it doing anything, so I've written my own.
	//  Have to save by name so that code changes that add/subtract columns will not cause confusion.
	//  Also, we save hidden state by name instead of position, which can really mess with a grid.
	void          Xmlize(XmlIO xml);
	
};

#endif
