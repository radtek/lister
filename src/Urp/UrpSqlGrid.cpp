#include "UrpSqlGrid.h"
#include <float.h>
#include <lister/Urp/UrpString.h> // IfNull

#define HIDDEN_COLUMN -2
#define NOSELECTION   -4

//==============================================================================================
UrpSqlGrid::UrpSqlGrid() : SqlArray(), UrpGridCommon() {
	HeaderObject().Absolute();
	Appending();
	EvenRowColor();
	Removing();
	AllSorting();
	// Tab doesn't seem to work when they are joined.
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
	WhenDrag       = THISBACK(DragRow);
	WhenDropInsert = THISBACK(DropRow);
	
}

//==============================================================================================
void UrpSqlGrid::DragRow() {
	if(this->DoDragAndDrop(InternalClip(*this, "array"), this->GetDragSample()) == DND_MOVE)
		this->RemoveSelection();
}

//==============================================================================================
void UrpSqlGrid::DropRow(int line, PasteClip& d) {
	if(AcceptInternal<UrpSqlGrid>(d, "array")) {
		const UrpSqlGrid& src = GetInternal<UrpSqlGrid>(d);
		bool self = &src == this;
		Vector< Vector<Value> > data;
		for(int i = 0; i < src.GetCount(); i++) {
			if(src.IsSel(i)) {
				Vector<Value> &dline = data.Add();
				Value v;
				// For some inexplicable reason we need to add one to get all the columns.  Why?
				int floatingColumnCount = GetFloatingColumnCount() + 1;
				for (int j = 0; j < floatingColumnCount; j++) {
					v = src.Get(i, j);
					dline.Add(v);
				}
			}
		}
		
		this->InsertDrop(line, data, d, self);
		
		if (WhenDropComplete) {
			WhenDropComplete(line, src.GetCount());
		}
		
		this->SetFocus();
	}
}

//==============================================================================================
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

//==============================================================================================
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

//==============================================================================================
void UrpSqlGrid::SetSortColumn(Id id, bool desc/* = false*/) {
	ArrayCtrl::SetSortColumn(FindColumnWithId(id), desc); // ARRAYCTL
}

//==============================================================================================
int UrpSqlGrid::GetColumnNo(Id id) {
	return FindColumnWithId(id);
}

//==============================================================================================
// Series of corrective functions to deal with Lamo Uno's confusion about when and when not to 
// adjust references for fixed columns.
int UrpSqlGrid::GetFloatingColumnCount() {
	// Takes into account the fixed columns
	return GetColumnCount();
}

//==============================================================================================
void UrpSqlGrid::UnhideFloatingColumnSilently(int i) {
	// Does NOT take into account fixed colunmns
//GRIDCTRL		ShowColumn(i + fixed_cols, false /* Don't Repaint */);
	HeaderObject().ShowTab(i);
}

//==============================================================================================
void UrpSqlGrid::HideFloatingColumn(int i) {
	// Does NOT take into account fixed colunmns
//GRIDCTRL		HideColumn(i + fixed_cols, true /* Repaint */);
	HeaderObject().HideTab(i);

}

//==============================================================================================
void UrpSqlGrid::HideColumn(Id id) {
	// Does NOT take into account fixed colunmns
//GRIDCTRL		HideColumn(i + fixed_cols, true /* Repaint */);
	HeaderObject().HideTab(GetColumnNo(id));

}

//==============================================================================================
void UrpSqlGrid::SetFloatingColumnWidth(int i, int w) {
	// Does NOT take into account fixed colunmns
//GRIDCTRL		SetColWidth(i + fixed_cols, w);
	HeaderObject().SetTabWidth(i, w);
}

//==============================================================================================
Id UrpSqlGrid::GetFloatingColumnId(int n) const {
	// Takes into account the fixed columns
//GRIDCTRL		return GetColumnId(n);
	return GetId(n);
}

//==============================================================================================
bool UrpSqlGrid::IsColumnHidden(int i) {
	return !HeaderObject().IsTabVisible(i);
}

//==============================================================================================
void UrpSqlGrid::SetColumnMinWidth(int i, int w) {
	HeaderTab(i).Min(w);
}

//	GridCtrl::ItemRect& GetFloatingColumn(int n) {
//		// Takes into account the fixed columns
//		return GetColumn(n);
//	}

//==============================================================================================
int UrpSqlGrid::SelectionCount() {
	int count = 0;
	for (int i = 0; i < GetCount(); i++) {
		if (IsSelected(i)) {
			count++;
		}
	}
	
	return count;
}

//==============================================================================================
// Useful for when grid is not active and cursor is -1, but an item is selected
int UrpSqlGrid::GetFirstSelection() {
	for (int i = 0; i < GetCount(); i++) {
		if (IsSelected(i)) {
			return i;
		}
	}
	
	return -1;
}

//==============================================================================================
Value UrpSqlGrid::GetMaxValue(Id column) {
	Value max = Null;
	int maxi = INT_MIN; // http://en.wikipedia.org/wiki/Limits.h
	double maxd = DBL_MIN; // http://en.wikipedia.org/wiki/Float.h
	String maxs = "";
	
	// Cache the data type once (All columns same type??)
	
	for (int i = 0; i < GetCount(); i++) {
		Value v = Get(i, column);
		
		// If a non-null value, we'll test against the various values we have
		if (!v.IsNull()) {
			// Bug: If datatype changes over rows, final value may be inconsistent
			switch (v.GetType()) {
				case INT_V: { 
					int vi = v; // Convert the value to an int since Value doesn't support ><
					double vd = v;
					
					if (vi > maxi) {
						maxi = vi;
						maxd = vd;
						max = vi;
					}
					break;
				}
				case DOUBLE_V: {
					double vd = v; // Convert the value to a double since Value doesn't support ><
					int vi = INT_MIN;
					if (vd > maxd) {
						if (vd > INT_MAX) {
							vi = INT_MAX;
						} else if (vd < INT_MAX) {
							vi = INT_MIN;
						} else {
							vi = (int)vd; // truncation warning
						}
						
						maxd = vd;
						max = vd;
					}
					break;
				}
				case STRING_V: {
					String vs = v; // Convert the value to a string since Value doesn't support ><
					if (vs > maxs) {
						maxs = vs;
						max = vs;
						// TODO: if is a number, then set maxi and maxd
					}
					break;
				}
				// TODO: Support BOOL_V and other types
			}
		}
	}
	
	return max;
}

//==============================================================================================
int UrpSqlGrid::CalcCorrectRow(int row) {
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
			ASSERT(1==0);
			return -2;
		} else {
			return -1;
		}
	} else {
		return row;
	}
}

//==============================================================================================
// This depends on the user creating an AddColumn for processorder, and the column
// processorder existing in the table, but then, you wouldn't be trying to persist an order
// if you didn't have a column, would you?
int UrpSqlGrid::GetProcessOrder(int row) {
	return Upp::max((int)Get(CalcCorrectRow(row), "PROCESSORDER"), (int)-1);
}

//==============================================================================================
// To add columns, you need to find a free key.
int UrpSqlGrid::GetMaxProcessOrder() {
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
// Allows us to change the increment, or us cached values.
int UrpSqlGrid::GetNextProcessOrder() {
	return GetMaxProcessOrder() + 1;
}

//==============================================================================================
//  GridCtrl is supposed to Xmlize, but I don't see it doing anything, so I've written my own.
//  Have to save by name so that code changes that add/subtract columns will not cause confusion.
//  Also, we save hidden state by name instead of position, which can really mess with a grid.
void UrpSqlGrid::Xmlize(XmlIO xml) {
	VectorMap<String, int> floatingColumnWidths;
	static VectorMap<String, int> requestedColumnWidths;
	int rowselected;

	HeaderObject().Absolute();

	if (xml.IsLoading()) {
		// Restore last known row selection
		xml("rowselected", rowselected);
		// Trap if no such attribute
		lastKnownRow = -1;
		if (IfNull(rowselected, NOSELECTION) != NOSELECTION) {
			// An empty grid sets this to -1
			if (rowselected == -1) { // Correct to be NOSELECTION
				rowselected = NOSELECTION;
			} else {
				// the application can determine whether to repos on last row.
				lastKnownRow = rowselected;
//				SetCursor(rowselected);
//				CenterCursor();
//				Select(rowselected, true /*sel*/); // Trigger click?
			}
		}
		
		// Read vector from store
		xml("columnwidths", floatingColumnWidths);

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
				requestedColumnWidths.Add(colIdName, colWidth);
				// GridCtrl Adjusted the width unaccountably, so we have to save a marker to adjust out 
				// Impossible to detect user calls to resize, since no when resize clause
			}
		}
		
	// We are in storing mode
	} else {
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
		}
		
		xml("columnwidths", floatingColumnWidths); // Write to store
	}
	
}
