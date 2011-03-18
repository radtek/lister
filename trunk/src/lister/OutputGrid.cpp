#include "OutputGrid.h"

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
void OutputGrid::Build() {
	recordViewWin.recordViewGrid.Build();
}

//==============================================================================================
void OutputGrid::Load(Connection *pconnection) {
	connection = pconnection;
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
	bar.Add("View row as a vertical record", THISBACK(PopUpRecordView));
}

//==============================================================================================	
// Using an UrpConfigWindow so position gets remembered.
void OutputGrid::PopUpRecordView() {
	UrpConfigWindow *w = windowFactory->Open((UrpTopWindow *)this->GetTopWindow(), "recordview");
	RecordViewGrid *g = NULL;
	
	if (w->wasCreatedNew) {
		// one-time ops
		g = new RecordViewGrid();
		g->Build(); // Create the columns
		w->AddCtrl(g);
	} else {
		g = (RecordViewGrid *)w->ctrls.At(0);
	}

	g->Load(connection);
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		String colname = TrimRight(GetFloatingColumn(i).GetName());
		String colvalue = Get(i).ToString();
		g-> Add(colname, colvalue);
	}
	
	w->OpenWithConfig();

}

//==============================================================================================	
void OutputGrid::ToggleMainGridSelectRow() {
	maingridselectrow = !maingridselectrow;
	SelectRow(maingridselectrow);
}

//==============================================================================================	
void OutputGrid::CopyColListCommaDelimWthPrefix() {
	String co;
	String prefix("");
	String aliasprefix("");
	UrpInputBox(prefix, "Prefix per Column Name", "Enter the string you want prepended to each column name");
	UrpInputBox(aliasprefix, "Alias Prefix per Column Alias", "Enter the string you want prepended to each column alias");
	
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
void OutputGrid::CopyColListCommaDelim() {
	String co;
	
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		if (i) co << ", ";
		co << TrimRight(GetFloatingColumn(i).GetName());
	}
	
	WriteClipboardText(co);
}

//==============================================================================================	
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