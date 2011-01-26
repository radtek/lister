#include "UrpSqlGrid.h"

#define HIDDEN_COLUMN -2

//==========================================================================================	
UrpSqlGrid::UrpSqlGrid() : SqlArray(), UrpGridCommon() {
	HeaderObject().Absolute();
	Appending();
	EvenRowColor();
	Removing();
	AllSorting();
	MultiSelect(); // ArrayCtrl
//GRIDCTRL		Editing();
//GRIDCTRL		EditsInNewRow();
//GRIDCTRL		Accepting();
	Moving();
//GRIDCTRL		MovingCols();
//GRIDCTRL		MovingRows();
//GRIDCTRL		Hiding();
	Duplicating();
//GRIDCTRL		Canceling();
	AskRemove();
	Duplicating();
//GRIDCTRL		Searching();
//GRIDCTRL		SearchDisplay();
//GRIDCTRL		SearchImmediate();
//GRIDCTRL		SearchHideRows();
//GRIDCTRL		SearchMoveCursor();
//GRIDCTRL		Closing();
//GRIDCTRL		Inserting();
//GRIDCTRL		RowChanging();
//GRIDCTRL		ResizingRows();
//GRIDCTRL		LiveCursor();
//GRIDCTRL		RejectNullRow(false);
	Clipboard();
//GRIDCTRL		Proportional();
//GRIDCTRL		ColorRows(true);
//GRIDCTRL		FixedPaste();

//	.SetDisplay()
}

//==========================================================================================	
// Override since the standard function expects the pk to be named "id" in order to fetch
// from Postgre sequence.  We let creator of grid control getting last pk id.
bool UrpSqlGrid::UpdateRow() {
	bool updated = SqlArray::UpdateRow();
	
	if (updated && IsInsert() && WhenNeedLastInsertedPkId) {
		WhenNeedLastInsertedPkId();
		Set(GetKeyId(), lastinsertedpkid);
	}
	
	return updated;
}

//==========================================================================================	
// Obviously, U++ developer Uno was being a total dickwad when he refused to give a function
// to extract column widths, so I've created it.
int UrpSqlGrid::GetFloatingColumnWidth(int colno) {
//		String colWidthsBunch = .Head GetColumnWidths();
//		Vector<String> colWidthsVector = UPP::Split(colWidthsBunch, ' ');
//		if (colno < 0 || colno >= colWidthsVector.GetCount()) return -1;
	if (colno < 0 || colno >= GetColumnCount()) return -1;
//		return atoi(colWidthsVector.At(colno));
	return HeaderObject().GetTabWidth(colno);
}

//==========================================================================================	
void UrpSqlGrid::SetSortColumn(Id id, bool desc/* = false*/) {
	ArrayCtrl::SetSortColumn(FindColumnWithId(id), desc); // ARRAYCTL
}

//==========================================================================================	
int UrpSqlGrid::GetColumnNo(Id id) {
	return FindColumnWithId(id);
}

//==========================================================================================	
// Series of corrective functions to deal with Lamo Uno's confusion about when and when not to adjust references for fixed columns
int UrpSqlGrid::GetFloatingColumnCount() {
	// Takes into account the fixed columns
	return GetColumnCount();
}

//==========================================================================================	
void UrpSqlGrid::UnhideFloatingColumnSilently(int i) {
	// Does NOT take into account fixed colunmns
//GRIDCTRL		ShowColumn(i + fixed_cols, false /* Don't Repaint */);
	HeaderObject().ShowTab(i);
}

//==========================================================================================	
void UrpSqlGrid::HideFloatingColumn(int i) {
	// Does NOT take into account fixed colunmns
//GRIDCTRL		HideColumn(i + fixed_cols, true /* Repaint */);
	HeaderObject().HideTab(i);

}

//==========================================================================================	
void UrpSqlGrid::HideColumn(Id id) {
	// Does NOT take into account fixed colunmns
//GRIDCTRL		HideColumn(i + fixed_cols, true /* Repaint */);
	HeaderObject().HideTab(GetColumnNo(id));

}

//==========================================================================================	
void UrpSqlGrid::SetFloatingColumnWidth(int i, int w) {
	// Does NOT take into account fixed colunmns
//GRIDCTRL		SetColWidth(i + fixed_cols, w);
	HeaderObject().SetTabWidth(i, w);
}

//==========================================================================================	
Id UrpSqlGrid::GetFloatingColumnId(int n) const {
	// Takes into account the fixed columns
//GRIDCTRL		return GetColumnId(n);
	return GetId(n);
}

//==========================================================================================	
bool UrpSqlGrid::IsColumnHidden(int i) {
	return !HeaderObject().IsTabVisible(i);
}

//==========================================================================================	
void UrpSqlGrid::SetColumnMinWidth(int i, int w) {
	HeaderTab(i).Min(w);
}

//	GridCtrl::ItemRect& GetFloatingColumn(int n) {
//		// Takes into account the fixed columns
//		return GetColumn(n);
//	}

//==========================================================================================	
//  GridCtrl is supposed to Xmlize, but I don't see it doing anything, so I've written my own.
//  Have to save by name so that code changes that add/subtract columns will not cause confusion.
//  Also, we save hidden state by name instead of position, which can really mess with a grid.
void UrpSqlGrid::Xmlize(XmlIO xml) {
	VectorMap<String, int> floatingColumnWidths;
	static VectorMap<String, int> requestedColumnWidths;

//GRIDCTRL		Absolute(); // Allows settings to take affect
	HeaderObject().Absolute();
	
	if (xml.IsLoading()) {
		xml("columnwidths", floatingColumnWidths); // Read from store
		int floatingColumnCount = GetFloatingColumnCount();
		for (int i = 0; i < floatingColumnCount; i++) {
			String colIdName = GetFloatingColumnId(i).ToString();
			
			int colWidth = floatingColumnWidths.Get(colIdName, GetFloatingColumnWidth(i));
			
			if (colWidth == 0) {
				colWidth = 20;
			} else
			// Check for label marked as a hidden column
			if (colWidth == HIDDEN_COLUMN) {
				UnhideFloatingColumnSilently(i); // Internal code will not set a min on a hidden column (!)
				SetColumnMinWidth(i, 0); // Set it to zero so rest of screen adjusts (bug in GridCtrl for Absolute)
				SetFloatingColumnWidth(i, 0);
				HideFloatingColumn(i);
				colWidth = 0;
			} else {
				
				HeaderCtrl::Column &c = HeaderObject().Tab(i);
				c.MinMax(0, 10000);
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
			if (IsColumnHidden(i)) { // Includes hidden indicator as 0!
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
