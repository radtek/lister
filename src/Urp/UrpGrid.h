#ifndef _Urp_UrpGrid_h_
#define _Urp_UrpGrid_h_

#include "UrpShared.h"
#include "UrpGridCommon.h"

#include <lister/GridCtrl/GridCtrl.h>

#define HIDDEN_COLUMN -2
#define NOSELECTION   -4

//==========================================================================================	
class UrpGrid: public GridCtrl, public UrpGridCommon {
public:
	typedef UrpGrid CLASSNAME;
	
	// action taken within grid and passed back to updator
	enum GridAction { 
		ADDEDROWSFROMDB, 
		USERADDEDROW, 
		ADDEDSTRUCTURE, 
		USERCHANGEDDATA, 
		DELETEDROWS 
	};
	

	bool                  built; // Build function of subclass should set to true, but no biggy; Its more for the subclasses.  Urp doesn't use yet or expect any settings
	bool                  loaded; // Load function of subclass should set to true; The idea is to reduce work of reloading unnecessarily, but hard to get it to work
	Callback1<GridAction> WhenToolBarNeedsUpdating;  // Loose add so grids like TestGrid will call TestWin to update its toolbar when testrows are added, columns added, etc.

	                    UrpGrid();
	virtual bool        Key(dword key, int count) { return GridCtrl::Key(key, count); };
	                    // Old windows have coked-up XML with bizarre widths.  This should help (menu item)
	void                NormalizeColumnWidth();
	                    // Obviously, U++ developer Uno was being a total dickwad when he refused to give a function
	                    // to extract column widths, so I've created it.
	                    
	int                 GetFloatingColumnWidth(int colno);
	                    // Series of corrective functions to deal with Lamo Uno's confusion about when and when not to adjust references for fixed columns
	int                 GetFloatingColumnCount();
	void                UnhideFloatingColumnSilently(int i);
	void                HideFloatingColumn(int i);
	void                SetFloatingColumnWidth(int i, int w);
	Id                  GetFloatingColumnId(int n) const;
	GridCtrl::ItemRect& GetFloatingColumn(int n);
	int                 GetFirstSelection();
	int                 CalcCorrectRow(int row);
	int                 GetProcessOrder(int row = -1);
	int                 GetMaxProcessOrder();
	int                 GetNextProcessOrder();
	void                MovedRows();
	                    // Trick: Impementor must implement SaveRow or any reordering by drag/drop won't be persisted to the database.
    virtual void        SaveRow(int row, int newProcessOrder);
	void                StdMenuBar(Bar &bar);
	                    // GridCtrl is supposed to Xmlize, but I don't see it doing anything, so I've written my own.
	                    // Have to save by name so that code changes that add/subtract columns will not cause confusion.
	                    // Also, we save hidden state by name instead of position, which can really mess with a grid.
	void                Xmlize(XmlIO xml);
	
};

#endif
