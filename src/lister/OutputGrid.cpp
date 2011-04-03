/***********************************************************************************************
*  lister - OutputGrid.cpp
*  
*  Displayed on main screen.  Populated from the CursorHandler.  Each execution of a script
*  to screen dynamically reconstructs the columns based on the spec.  Context menu supports
*  copying sections, sorting.  Columns are resized to width based on the first 10 rows of data
*  including the width of the header.  Data type is always string.
*  You can copy sections to HTML format, which is a fragment using Microsoft-specific codes
*  that allow you to paste directly into Outlook emails and retain grids.
*  You can switch between cell and row sellection mode.
*
*  TODO:
*  - Maintain lists of COI (Columns of interest), or sets of detail, like readable columns,
*    dim keys, dim keys with effective dates, all dates with primary key, account identifiers,
     useless columns, empty columns, same value columns, low-cardinality columns, string-date
     or string-number columns, alpha/numeric value columns, comment/descriptive columns including
     long names.
   - Display rows immediately instead of waiting for cancel fetch of finish.
   - Save any column layout changes per script id, including sorts.
   - Annotate grid, column, row, cell, value, value like, column x and column y both present.
   - Add column notes to connectionid or connectionchain + columnname or set of columnnames and 
     aliases.
   - Add metric context analysis: partition into buckets, cardinality, top 10 values/counts per
     , % of nulls, compare to previous date range (gen query)
   - Define drills per column or column set. (TICKET+EXCHANGE drill to security).
   - Select discrete segments or columns within context.
   - Freeze any columns
   - Create a virtual scratch pad of rows onto a popup grid so can compare
   - "Diff" any set of rows.
   - Save output to table
   - Sync with another saved set and compare diffs, similarites (row counts, missnig/additional
     columns, differing values
   - Define default BusinessDate column (COB_DATE, BUSINESSDATE, COB_DT, etc) in a row set.  Can
     guess from a set of names.
   - Drill up/down into history (From source a, drill to staging on source b, then to conformed
     warehouse on source b
   - Define link expecations, like should linke 1-to-1 or 1-to-none, linke 1 to many, never link.
   - Create master detail grid pair, or tree structure.  Create master to multi-detail sections
     like you can on Access Forms 2.0.
   - Drill to tickets/tasks referencing the attribute or the cob_dt
   - List models/reports including this attribute, as display and/or logic
   - List known report users expectations about an attribute (always matches my screen for that #)
*
*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

#include "OutputGrid.h"

#include "image_shared.h"
#include <Draw/iml_header.h>

// Only in lister.cpp
//#include "image_shared.h"
//#include <Draw/iml_source.h>

//==============================================================================================
// UrpGrid sets up all the grid behavior to be consistent across the app.
OutputGrid::OutputGrid() : UrpGrid() {
	Description("mainGrid");
	maingridselectrow = false;
	WhenMenuBar = THISBACK(MainGridContextMenu);
}

//==============================================================================================
/*virtual=0*/ void OutputGrid::Build(Connection *pconnection) {
	BuildBase(pconnection); // pconnection must be non-null
	recordViewWin.recordViewGrid.Build(connection);
	BuildComplete(); // sets built
}

//==============================================================================================
/*virtual=0*/ void OutputGrid::Load() {
	LoadBase(); // Build must have been called
	LoadComplete(); //sets loaded
}

//==============================================================================================
void OutputGrid::ProcessCut() {
		
	GridCtrl::SetClipboard();
//	String c = ReadClipboardText();
//	String headingLine;
//	for (int i = 0; i < GetColumnCount(); i++) {
//		String columnName = GetColumnName(i);
//		if (i) headingLine << '\t';
//		headingLine << columnName;
//	}
//	String new_c;
//	new_c << headingLine << '\n' << c;
//	WriteClipboardText(new_c);
}

//==============================================================================================
bool OutputGrid::Key(dword key, int count) {
	KeyProcessorResponse keyProcessorResponse;

	switch (key) {
		
		// Attempt to paste 
		case K_CTRL_C:
			ProcessCut();
			keyProcessorResponse = SIGNALWEPROCESSEDKEY;
			return true; // Eat keystroke
	}

	return UrpGrid::Key(key, count);
}

//==============================================================================================
void OutputGrid::MainGridContextMenu(Bar &bar) {
	StdMenuBar(bar);
	bar.Add("Select entire row", THISBACK(ToggleMainGridSelectRow))
		.Check(maingridselectrow)
		.Help("Select the whole row or select individual cells");
	bar.Add("Copy columns to comma-delim list", THISBACK(CopyColListCommaDelim));
	bar.Add("Copy columns to comma-delim list w/pref", THISBACK(CopyColListCommaDelimWthPrefix));
	bar.Add("Copy columns order by data type", THISBACK(CopyColListCommaDelimByType));
	bar.Add("View row as a vertical record", THISBACK(PopUpRecordView))
		.Image(MyImages::recview16());
}

//==============================================================================================
// Using an UrpConfigWindow so position gets remembered.
void OutputGrid::PopUpRecordView() {
	// Require a connection, just for looking up info about stuff
	if (!connection) return;
	if (!GetRowCount()) return;
	if (!IsCursor()) return;
	
	UrpConfigWindow *w = windowFactory->Open((UrpTopWindow *)this->GetTopWindow(), "recordview");
	RecordViewGrid *g = NULL;
	
	if (w->wasCreatedNew) {
		// one-time ops
		g = new RecordViewGrid();
		w->Icon(MyImages::recview16()); // Have to set here, not in constructor for dynamics.
		g->Build(connection); // Create the columns
		w->AddCtrl(g);
	} else {
		g = (RecordViewGrid *)w->ctrls.At(0);
	}

	g->Load();
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		String colname = TrimRight(GetFloatingColumn(i).GetName());
		String colvalue = Get(i).ToString();
		g->Add(colname, colvalue);
	}
	
	w->OpenWithConfig();

}

//==============================================================================================
// From context menu, switch between cell selection and row highlighting.  Row view helps matching
// column values that are far apart.
void OutputGrid::ToggleMainGridSelectRow() {
	maingridselectrow = !maingridselectrow;
	SelectRow(maingridselectrow);
}

//==============================================================================================
// Helper to stuff the clipboard with something like "a.x as f_x, a.y as f_y".  You would enter "a" and "f_".
void OutputGrid::CopyColListCommaDelimWthPrefix() {
	String co;
	String prefix("colprefix");
	String aliasprefix("alias_");
	UrpInputBox(prefix, "Table Alias as Prefix to each Column Name", "Enter the string you want prepended to each column name, usually the table in the from clause or its alias.  Do not include the '.'");
	UrpInputBox(aliasprefix, "Column Alias Prefix per Column Alias", "Enter the string you want prepended to each column alias");
	
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		if (i) co << ", ";
		if (!prefix.IsEmpty()) co << prefix << ".";
		String colname = TrimRight(GetFloatingColumn(i).GetName());
		co << colname;
		if (!aliasprefix.IsEmpty()) co << " as " << aliasprefix << colname;
	}
	
	WriteClipboardText(co);
}

//==============================================================================================
// Copy onto the clipboard all the columns of the grid to a comma-delimited list, such as: "cusip, price, quantity"
void OutputGrid::CopyColListCommaDelim() {
	String co;
	
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		if (i) co << ", ";
		co << TrimRight(GetFloatingColumn(i).GetName());
	}
	
	WriteClipboardText(co);
}

//==============================================================================================
// Useful function that groups the columns by data type and labels each group with a comment.
// For very wide grids this help isolate problems where you are getting baffling error messages
// related to data type ("non-numeric") but no column name.
void OutputGrid::CopyColListCommaDelimByType() {
	String co;
	
	Index<int> types;
	
	// Find all the data types present; we don't care what they are we will just group column
	// output by them.
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		int coltype = outputSpec.outputColumnDefList[i].sqlType;
		if (types.Find(coltype) == -1) {
			types.Add(coltype);
		}
	}

	bool hit = false;
	
	for (int j = 0; j < types.GetCount(); j++) {
		bool nextype = true;
		for (int i = 0; i < GetFloatingColumnCount(); i++) {
			if (outputSpec.outputColumnDefList[i].sqlType == types[j]) {
				if (hit) {
					co << ", ";
				} 
				if (nextype) {
					co << "/* " << outputSpec.outputColumnDefList[i].sqlTypeName << " */ ";
					nextype = false;
				}
				hit = true;
				co << TrimRight(GetFloatingColumn(i).GetName());
			}
		}
	}
	
	WriteClipboardText(co);
}


/*

// REALLY COOL BLINKING TRICK

	if(!silent)
	{
		Color c0 = bg_select;
		Color c1 = White;
		Color c2 = bg_focus;
	
		for(int i = 0; i < 256; i += 64)
		{
			bg_select = Blend(c0, c1, i);
			bg_focus = Blend(c2, c1, i);
			Refresh(); Sync();
			Sleep(1);
		}
	
		for(int i = 0; i < 256; i += 32)
		{
			bg_select = Blend(c1, c0, i);
			bg_focus = Blend(c1, c2, i);
			Refresh(); Sync();
			Sleep(1);
		}
	}

*/