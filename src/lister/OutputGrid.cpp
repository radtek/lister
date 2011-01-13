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
	
}

//==============================================================================================
void OutputGrid::Build() {
}

//==============================================================================================
void OutputGrid::Load(Connection *pconnection) {
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