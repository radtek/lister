#include "UrpGrid.h"
#include "UrpPaint.h"
#include "UrpString.h" // IfNull

// BAD Design, I know, but I don't have time to move Connection into Urp
#include "../lister/Connection.h" // ->enumConnState method
#include "../lister/ConnState.h" // CON_SUCCEED definition

//==============================================================================================
UrpGrid::UrpGrid() : GridCtrl(), UrpGridCommon() {
	Indicator(); // Critical.  Sizing of columns will be distorted without this.
	Appending();
	Removing();
	Editing();
	EditsInNewRow();
	Accepting();
	Moving();
	MovingCols();
	MovingRows();
	MultiSelect();        // Select any rows we wish to operate on
	MultiSorting();       // Too complex?
	Dragging();           // Allows us to reorder by drag and drop
	Hiding();
	Duplicating();
	Canceling();
	AskRemove(false); // Can be a bear if opened a row then try to exit app
	Duplicating();
	Searching();
	SearchDisplay();
	SearchImmediate();
	SearchHideRows();
	SearchMoveCursor();
	SelectRow(false);  // Always start out with cell selection
	Closing();
	Inserting();
	RowChanging();
	ResizingRows();
	// Rows are highlighted as you pass over them.  I like it, but It can be annoying.
	LiveCursor();            
	RejectNullRow(false);
	Clipboard();
	Proportional();
	ColorRows(true);
	FixedPaste();
	CopyColumnNames(true);
	Navigating();
	Absolute();             // Critical for proper column width export/import
	ResizePaintMode(1); // Defaults to 2, and we get big blank spaces when we hide columns, so lets try 1.

	WhenMenuBar           = THISBACK(StdMenuBar);
	WhenMovedRows         = THISBACK(MovedRows);

	built     = false;
	loaded    = false;

}

//==============================================================================================
void UrpGrid::BuildBase(Connection * pconnection) {
	ASSERT(pconnection);
	ASSERT(pconnection->enumConnState == CON_SUCCEED);
	connection = pconnection;
}

//==============================================================================================
void UrpGrid::BuildComplete() {
	ASSERT(connection);
	ASSERT(connection->enumConnState == CON_SUCCEED);
	built = true;
	loaded = false; // Just built; can't be loaded.
}

//==============================================================================================
void UrpGrid::LoadBase() {
	Ready(false);
	ASSERT(connection);
	ASSERT(connection->enumConnState == CON_SUCCEED);
	ASSERT(built);
}

//==============================================================================================
void UrpGrid::LoadComplete() {
	Ready(true);
	ASSERT(connection);
	ASSERT(connection->enumConnState == CON_SUCCEED);
	ASSERT(built);
	loaded = true;
}

//==============================================================================================
void UrpGrid::MovedRows() {
	Ready(false); // Prevent flicker
	int currentRow = GetCurrentRow();
	
	int newProcessOrder = 0; // Start with zero
	
	for (int i = 0; i < GetCount(); i++) {
		int currentProcessOrder = GetProcessOrder(i);
		// If the current order is within 20 of our target, we'll leave it
		if (WithinOffset(currentProcessOrder, newProcessOrder, 20)) {
			// No need to update this row, its already in the right order; save some db time
			newProcessOrder = currentProcessOrder + 1;
		} else {
			newProcessOrder = GetNextFreeProcessOrder(i, newProcessOrder);
			SaveRow(i, newProcessOrder); // If implementor (See testgrid) implemented, this will be persisted somewhere.
			// We know this one's taken
			newProcessOrder++;
		}
	}
	
	// Set our cursor back to where it was.  Did we scrol?
	if (currentRow != -1) {
		SetCursor(currentRow);
	}
	Ready(true);
}

//==============================================================================================
// Called from context menu when over a column.
//==============================================================================================
void UrpGrid::HideColumnUnderMouse() {
	// curpos is storing the row numbers, not the mouse position of the last RightDown mouse click.	
	int colno = GetCursorPos().x; // Not really a point, even though using Point class.
	if (colno == -1) return;
	HideFloatingColumn(colno);
}

//==============================================================================================
void UrpGrid::SaveRow(int row, int newProcessOrder) {
	// Do nothing; for implementor it they want to write processorder to db.
}

//==============================================================================================
void UrpGrid::StdMenuBar(Bar &bar) {
	GridCtrl::StdMenuBar(bar);
	bar.Add(true, "Hide Column", THISBACK(HideColumnUnderMouse)).Tip("Hide selected column");
	// Hide Row?
	bar.Add(true, "Shrink any columns over 1000 width", THISBACK(NormalizeColumnWidth)).Tip("Caused by config corruption");
}

//==============================================================================================
int UrpGrid::CalcCorrectRow(int row) {
	if (row == -1) {
		if (IsCursor()) {
			return GetCursor();
		} else if (IsSelection()) {
			for (int i=0; i < GetCount(); i++) {
				if (IsSelected(i)) {
					return i; // Can't force visible since this may be part of an informational lookup
				}
			}
			throw Exc("Internal error: IsSelection() says a selection exists, but no rows are flagged in IsSelected()");
		} else {
			return -1;
		}
	} else {
		return row;
	}
}

//==============================================================================================
int UrpGrid::GetProcessOrder(int row) {
	return Upp::max((int)Get(CalcCorrectRow(row), "PROCESSORDER"), (int)-1);
}

//==============================================================================================
int UrpGrid::GetMaxProcessOrder() {
	int max = 0;
	
	for (int i = 0; i < GetCount(); i++) {
		int v = GetProcessOrder(i);
		if (v > max) {
			max = v;
		}
	}
	
	return max;
}

//==============================================================================================
int UrpGrid::GetNextProcessOrder() {
	return GetMaxProcessOrder() + 10; // Gap it so that renumbering is easier
}

//==============================================================================================
int UrpGrid::GetNextFreeProcessOrder(int startAfterRow, int startAtProcessOrder) {
	int freeone = startAtProcessOrder; // May be a good order, we don't know
	Index<int> ordersTaken;
	
	// Always send us the row you are trying to fill since we skip that one assuming it will be overwritten
	// Plus, the MovedRows() function checks if it can use the current row's order already.
	for (int i = startAfterRow + 1; i < GetCount(); i++) {
		int v = GetProcessOrder(i);
		ordersTaken.Add(v);
	}
	
	// increment until we don't find it
	while (FindIndex(ordersTaken, freeone) != -1) freeone++;
	
	return freeone;
}

//==============================================================================================
// Sometimes column widths get goofed up.
void UrpGrid::NormalizeColumnWidth() {
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		int colWidth = 0;
		colWidth = GetFloatingColumnWidth(i);

		bool isFixed = GetColumn(i).IsFixed();
		int min = GetColumn(i).min;
		int max = GetColumn(i).max;
		
		// Ignore hidden columns and fixed width columns
		
		if (colWidth != HIDDEN_COLUMN && !isFixed) {
			if (colWidth > 1000) // Outrageous
			SetFloatingColumnWidth(i, 100);
		}
		
		if (!GetFloatingColumn(i).IsHidden() && colWidth <= 0) {
			Exclamation("Accidentally hidden column");
			SetFloatingColumnWidth(i, 40);
		}
	}
}

//==============================================================================================
// Obviously, U++ developer Uno was being a total dickwad when he refused to give a function
// to return individual column widths, so I've created it.
int UrpGrid::GetFloatingColumnWidth(int colno) {
	String colWidthsBunch = GetColumnWidths();
	Vector<String> colWidthsVector = UPP::Split(colWidthsBunch, ' ');
	if (colno < 0 || colno >= colWidthsVector.GetCount()) return -1;
	return atoi(colWidthsVector.At(colno));
}

//==============================================================================================
// Series of corrective functions to deal with Lamo Uno's confusion about consistent coordinate bases
int UrpGrid::GetFloatingColumnCount() {
	// Takes into account the fixed columns
	return GetColumnCount();
}

//==============================================================================================
void UrpGrid::UnhideFloatingColumnSilently(int colno) {
	// U++ function does NOT take into account fixed colunmns, so we created wrapper
	ShowColumn(colno + fixed_cols, false /* Don't Repaint */);
}

//==============================================================================================
void UrpGrid::HideFloatingColumn(int colno) {
	// Does NOT take into account fixed colunmns
	HideColumn(colno + fixed_cols, true /* Repaint */);
}

//==============================================================================================
void UrpGrid::SetFloatingColumnWidth(int colno, int w) {
	// Does NOT take into account fixed colunmns
	SetColWidth(colno + fixed_cols, w, true /* Let's try not recalcing */);
}

//==============================================================================================
Id UrpGrid::GetFloatingColumnId(int colno) const {
	// U++ function does take into account the fixed columns
	return GetColumnId(colno);
}

//==============================================================================================
int UrpGrid::GetFloatingColumnNo(Id id) {
	// Find returns the physical index so we must strip the floating portion off
	int colno = aliases.Find(id);
	
	if (colno == -1) {
		return colno;
	} else {
		colno-= fixed_cols;
		ASSERT_(colno >= 0, "A floating column number cannot be negative");
		return colno;
	}
}


//==============================================================================================
GridCtrl::ItemRect& UrpGrid::GetFloatingColumn(int colno) {
	// Takes into account the fixed columns
	return GetColumn(colno);
}

//==============================================================================================
// Add function from ArrayCtrl
int UrpGrid::GetFirstSelection() {
	for (int i = 0; i < GetCount(); i++) {
		if (IsSelected(i)) {
			return i;
		}
	}
	
	return INT_MIN; // Our fake NULL
}

//==============================================================================================
//  GridCtrl is supposed to Xmlize, but I don't see it doing anything, so I've written my own.
//  Have to save by name so that code changes that add/subtract columns will not cause confusion.
//  Also, we save hidden state by name instead of position, which can really mess with a grid.
void UrpGrid::Xmlize(XmlIO xml) {
	VectorMap<String, int> floatingColumnWidths;
	static VectorMap<String, int> requestedColumnWidths;
	int rowselected;

	Absolute(); // Allows settings to take affect

	if (xml.IsLoading()) {
		// Restore last known row selection
		lastKnownRow = -1;
		xml("rowselected", rowselected);
		// Trap if no such attribute
		if (IfNull(rowselected, NOSELECTION) != NOSELECTION) {
			// the application can determine whether to repos on last row.
			lastKnownRow = rowselected;
		}
		
		xml("columnwidths", floatingColumnWidths); // Read from store
		for (int i = 0; i < GetFloatingColumnCount(); i++) {
			// Allow developer to build/load dynamic columns that he doesn't want Xmlize
			// trying to display or hide, depending on whether this grid was saved with columns
			// X,Y,Z or A,B,C,D.  That way, when xml widths for X,Y,Z is zapped when the last open
			// grid shows A,B,C,D, X,Y,Z doesn't come all flying back from the dead next time
			// an A-B-C grid is generated.
			// Next solution might be to have a template name that is saved and reloaded.
			
			if (GetColumn(i).alwayshidden) {
				continue;
			}
			
			// if GetColumn(i).istemplatized then colIdName = Get ColumnGenericName()
			// Note that several columns in the same grid could refer to the same generic col name.
			// if GetColumn(i).ifnoxmlfoundthenhide?
			
			String colIdName = GetFloatingColumnId(i).ToString();
			int colWidth = 0;
			
			if (floatingColumnWidths.Find(colIdName) == -1) {
				colWidth = 60;
			} else {
				colWidth = floatingColumnWidths.Get(colIdName, GetFloatingColumnWidth(i));
			}

			bool isFixed = GetColumn(i).IsFixed();
			int min = GetColumn(i).min;
			int max = GetColumn(i).max;
			
			// Check for label marked as a hidden column
			if (colWidth == HIDDEN_COLUMN) {
				UnhideFloatingColumnSilently(i); // Internal code will not set a min on a hidden column (!)
				GetFloatingColumn(i).Min(0); // Set it to zero so rest of screen adjusts (bug in GridCtrl for Absolute)
				SetFloatingColumnWidth(i, 0);
				HideFloatingColumn(i);
				colWidth = 0;
				
			// If its a fixed width column, we can't modify the width
			} else if (isFixed) {
				colWidth = min; // Actual nsize value internally not set yet since grid not drawn, so we use what the constructor requested.
			} else {
				SetFloatingColumnWidth(i, colWidth);
			}

			int actualColWidth = GetFloatingColumnWidth(i);
			
			if (actualColWidth != colWidth) {
//					ASSERT(actualColWidth - colWidth == 1); // Any other value is unexpected
				requestedColumnWidths.Add(colIdName, colWidth);
				// GridCtrl Adjusted the width unaccountably, so we have to save a marker to adjust out 
				// Impossible to detect user calls to resize, since no when resize clause
			}
		}
	} else { // Storing
		// Save the first selection the user made
		
		if (IsSelection()) {
			rowselected = GetFirstSelection();
		} else {
			rowselected = NOSELECTION;
		}

		xml("rowselected", rowselected);

		for (int i = 0; i < GetFloatingColumnCount(); i++) {
			String colIdName = GetFloatingColumnId(i).ToString();
			int colWidth = GetFloatingColumnWidth(i);
			if (GetFloatingColumn(i).IsHidden()) { // Includes hidden indicator as 0!
				colWidth = HIDDEN_COLUMN; // Our made-up code for Hidden
			} else if(requestedColumnWidths.Find(colIdName) >= 0) {
				// GridCtrl corrupted our requested width, so we won't save if current width is exactly +1 of requested width
				// Without this adjustment the grid just keeps creeping wider each session (GridCtrl always +1s the same columns)
				int reqColWidth = requestedColumnWidths.Get(colIdName);
				if (reqColWidth - colWidth == -1) {
					colWidth = reqColWidth; // Overwrite the incorrect one.  Notice that if the user altered it in any other measure manually, we will still save the change
				}
				
			}
			
			floatingColumnWidths.FindAdd(colIdName, colWidth);  // In case of dups, we only add the first one
		}
		
		xml("columnwidths", floatingColumnWidths); // Write to store
	}
}
