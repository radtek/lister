#include "UrpGrid.h"
#include "UrpPaint.h"
#include "UrpString.h" // IfNull
//==============================================================================================
UrpGrid::UrpGrid() : GridCtrl(), UrpGridCommon() {
	Appending();
	Removing();
	Editing();
	EditsInNewRow();
	Accepting();
	Moving();
	MovingCols();
	MovingRows();
	Hiding();
	Duplicating();
	Canceling();
	AskRemove();
	Duplicating();
	Searching();
	SearchDisplay();
	SearchImmediate();
	SearchHideRows();
	SearchMoveCursor();
	Closing();
	Inserting();
	RowChanging();
	ResizingRows();
	LiveCursor();
	RejectNullRow(false);
	Clipboard();
	Proportional();
	ColorRows(true);
	FixedPaste();
	CopyColumnNames(true);
	WhenMenuBar = THISBACK(StdMenuBar);
	built = false;
	loaded = false;
	
	//SetDisplay(Single<TightFontDisplayForGridCtrl>());
}

//==============================================================================================
void UrpGrid::StdMenuBar(Bar &bar) {
	GridCtrl::StdMenuBar(bar);
	bar.Add(true, "Shrink any columns over 100 width", THISBACK(NormalizeColumnWidth));
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
			// Error if here
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
	return GetMaxProcessOrder() + 1;
}

//==============================================================================================
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
	}
}

//==============================================================================================
// Obviously, U++ developer Uno was being a total dickwad when he refused to give a function
// to extract column widths, so I've created it.
int UrpGrid::GetFloatingColumnWidth(int colno) {
	String colWidthsBunch = GetColumnWidths();
	Vector<String> colWidthsVector = UPP::Split(colWidthsBunch, ' ');
	if (colno < 0 || colno >= colWidthsVector.GetCount()) return -1;
	return atoi(colWidthsVector.At(colno));
}

//==============================================================================================
// Series of corrective functions to deal with Lamo Uno's confusion about when and when not to adjust references for fixed columns
int UrpGrid::GetFloatingColumnCount() {
	// Takes into account the fixed columns
	return GetColumnCount();
}

//==============================================================================================
void UrpGrid::UnhideFloatingColumnSilently(int i) {
	// Does NOT take into account fixed colunmns
	ShowColumn(i + fixed_cols, false /* Don't Repaint */);
}

//==============================================================================================
void UrpGrid::HideFloatingColumn(int i) {
	// Does NOT take into account fixed colunmns
	HideColumn(i + fixed_cols, true /* Repaint */);
}

//==============================================================================================
void UrpGrid::SetFloatingColumnWidth(int i, int w) {
	// Does NOT take into account fixed colunmns
	SetColWidth(i + fixed_cols, w, true /* Let's try not recalcing */);
}

//==============================================================================================
Id UrpGrid::GetFloatingColumnId(int n) const {
	// Takes into account the fixed columns
	return GetColumnId(n);
}

//==============================================================================================
GridCtrl::ItemRect& UrpGrid::GetFloatingColumn(int n) {
	// Takes into account the fixed columns
	return GetColumn(n);
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
		xml("rowselected", rowselected);
		// Trap if no such attribute
		if (IfNull(rowselected, NOSELECTION) != NOSELECTION) {
			SetCursor(rowselected);
			CenterCursor();
			Select(rowselected); // Slightly different than ArrayCtrl-based UrpSqlGrid
		}
		xml("columnwidths", floatingColumnWidths); // Read from store
		for (int i = 0; i < GetFloatingColumnCount(); i++) {
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
