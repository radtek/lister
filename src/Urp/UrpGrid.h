#ifndef _Urp_UrpGrid_h_
#define _Urp_UrpGrid_h_

#include "UrpShared.h"
#include "UrpGridCommon.h"
#include <lister/GridCtrl/GridCtrl.h>
class Connection; // Urp has no connection class (yet) so lister, our primary user, defines it.

#define HIDDEN_COLUMN -2
#define NOSELECTION   -4

//==============================================================================================
// abstract base class; ye canna instantiate
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
	
                       // Build function of subclass should set to true, but no biggy; Its more for the subclasses.  Urp doesn't use yet or expect any settings.
                       // TaskDefWin does look at this flag.
	bool                  built;  
	
	                   // Load function of subclass should set to true; The idea is to reduce work of reloading unnecessarily, but hard to get it to work
	bool                  loaded; 
	
	                   // Loose add so grids like TestGrid will call TestWin to update its
                       // toolbar when testrows are added, columns added, etc.
	Callback1<GridAction> WhenToolBarNeedsUpdating;

	                      UrpGrid                     ();
	                      
	                   // Mandatory implementations (pure virtual functions)
	virtual void          Build                       (Connection * pconnection) = 0;
	virtual void          Load                        () = 0;

	virtual void          BuildBase                   (Connection * pconnection); // Call this from your implementor to do general stuff
	virtual void          LoadBase                    (); // Call this from your implementor to do general stuff 
	virtual void          BuildComplete               (); 
	virtual void          LoadComplete                (); 
	
	virtual bool          Key                         (dword key, int count) { return GridCtrl::Key(key, count); };
	
	                   // Old windows have coked-up XML with bizarre widths.  This should help (menu item)
	void                  NormalizeColumnWidth        ();

	                   // Obviously, U++ developer Uno was being a total dickwad when he refused to give a function
	                   // to extract column widths, so I've created it.
	int                   GetFloatingColumnWidth      (int colno);
	
	                   // Series of corrective functions to deal with Lamo Uno's confusion about when and when not to adjust references for fixed columns
	int                   GetFloatingColumnCount      ();
	void                  UnhideFloatingColumnSilently(int colno);
	void                  HideFloatingColumn          (int colno);
	void                  SetFloatingColumnWidth      (int colno, int w);
	Id                    GetFloatingColumnId         (int colno) const;
	                   // Strange that this function was not present in U++ before.
	int                   GetFloatingColumnNo         (Id id);
	GridCtrl::ItemRect&   GetFloatingColumn           (int colno);
	int                   GetFirstSelection           ();
	int                   CalcCorrectRow              (int row);
	int                   GetProcessOrder             (int row = -1);
	int                   GetMaxProcessOrder          ();
	                   // After the last(max) one, that is.
	int                   GetNextProcessOrder         ();
	                   // Find an available "hole" after the one passed, needed for renumbering
	int                   GetNextFreeProcessOrder     (int startAfterRow, int startAtProcessOrder);
	
	                   // Called when rows were dragged and dropped back in the same grid.
	void                  MovedRows                   ();
	void                  HideColumnUnderMouse        ();
	
	                   // Trick: Impementor must implement SaveRow or any reordering by drag/drop won't be persisted to the database.
    virtual void          SaveRow                     (int row, int newProcessOrder);
	void                  StdMenuBar                  (Bar &bar);
	
	                   // GridCtrl is supposed to Xmlize, but I don't see it doing anything, so I've written my own.
	                   // Have to save by column name so that the adding or subtracting of
	                   // columns does not cause corruption.
	                   // Also, we save hidden state by name instead of position, which can really mess with a grid.
	                   // Needs work.  Gets confused around hiding/unhiding of columns with negative widths.
	void                  Xmlize                      (XmlIO xml);
	
};

#endif
