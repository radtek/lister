#include "OutputGrid.h"

#include "image_shared.h"
#include <Draw/iml_header.h>

// Only in lister.cpp
//#include "image_shared.h"
//#include <Draw/iml_source.h>

//==============================================================================================
OutputGrid::OutputGrid() : UrpGrid() {
	Indicator(); // Critical.  Sizing of columns will be distorted without this.
	MultiSelect();
	MultiSorting();
	ColorRows();
	Dragging();
	Absolute(); // Critical.  Columns will be scrunched to 2 bits without this.
	Clipboard();
	SearchDisplay();
	Searching();
	Moving();
	MovingCols();
	MovingRows();
	AskRemove(false);
	Hiding();
	SelectRow(false);
	FixedPaste();
	Navigating();
	LiveCursor(true);
	
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
		g-> Add(colname, colvalue);
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