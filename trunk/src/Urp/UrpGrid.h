#ifndef _Urp_UrpGrid_h_
#define _Urp_UrpGrid_h_

#include "UrpShared.h"

#define HIDDEN_COLUMN -2

//==========================================================================================	
class UrpGrid: public Upp::GridCtrl {
public:
	//==========================================================================================	
	UrpGrid() : GridCtrl() {
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
	}

	//==========================================================================================	
	// Obviously, U++ developer Uno was being a total dickwad when he refused to give a function
	// to extract column widths, so I've created it.
	int GetFloatingColumnWidth(int colno) {
		String colWidthsBunch = GetColumnWidths();
		Vector<String> colWidthsVector = UPP::Split(colWidthsBunch, ' ');
		if (colno < 0 || colno >= colWidthsVector.GetCount()) return -1;
		return atoi(colWidthsVector.At(colno));
	}
	
	//==========================================================================================	
	// Series of corrective functions to deal with Lamo Uno's confusion about when and when not to adjust references for fixed columns
	int GetFloatingColumnCount() {
		// Takes into account the fixed columns
		return GetColumnCount();
	}
	
	void UnhideFloatingColumnSilently(int i) {
		// Does NOT take into account fixed colunmns
		ShowColumn(i + fixed_cols, false /* Don't Repaint */);
	}
	
	void HideFloatingColumn(int i) {
		// Does NOT take into account fixed colunmns
		HideColumn(i + fixed_cols, true /* Repaint */);
	}
	
	void SetFloatingColumnWidth(int i, int w) {
		// Does NOT take into account fixed colunmns
		SetColWidth(i + fixed_cols, w);
	}
	
	Id GetFloatingColumnId(int n) const {
		// Takes into account the fixed columns
		return GetColumnId(n);
	}
	
	GridCtrl::ItemRect& GetFloatingColumn(int n) {
		// Takes into account the fixed columns
		return GetColumn(n);
	}
	
	//==========================================================================================	
	//  GridCtrl is supposed to Xmlize, but I don't see it doing anything, so I've written my own.
	//  Have to save by name so that code changes that add/subtract columns will not cause confusion.
	//  Also, we save hidden state by name instead of position, which can really mess with a grid.
	void Xmlize(XmlIO xml) {
		VectorMap<String, int> floatingColumnWidths;
		static VectorMap<String, int> requestedColumnWidths;

		Absolute(); // Allows settings to take affect

		if (xml.IsLoading()) {
			xml("columnwidths", floatingColumnWidths); // Read from store
			for (int i = 0; i < GetFloatingColumnCount(); i++) {
				String colIdName = GetFloatingColumnId(i).ToString();
				
				int colWidth = floatingColumnWidths.Get(colIdName, GetFloatingColumnWidth(i));
				
				// Check for label marked as a hidden column
				if (colWidth == HIDDEN_COLUMN) {
					UnhideFloatingColumnSilently(i); // Internal code will not set a min on a hidden column (!)
					GetFloatingColumn(i).Min(0); // Set it to zero so rest of screen adjusts (bug in GridCtrl for Absolute)
					SetFloatingColumnWidth(i, 0);
					HideFloatingColumn(i);
					colWidth = 0;
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
		} else {
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
				LOG("HIT");
			}
			
			xml("columnwidths", floatingColumnWidths); // Write to store
		}
	}
};

#endif
