#ifndef _MyDropGrid_h_
#define _MyDropGrid_h_

#include "shared.h"

#include <DropGrid/DropGrid.h>

//==========================================================================================	
struct MyDropGrid : public DropGrid {
	typedef MyDropGrid CLASSNAME;
	MyDropGrid() : DropGrid() {
		list.WhenPopDown = THISBACK(SelectedItem);
		list.WhenCursor = THISBACK(CursorChanged);
		list.DrawFocus().FocusLostAccepting();
	}

	//==========================================================================================	
	virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags) {
		return DropGrid::MouseEvent(event, p, zdelta, keyflags);
	}

	//==========================================================================================	
	void CursorChanged() {
		int r = 0;
	}

	//==========================================================================================	
	void SelectedItem() {
		Tip(Get(1).ToString()); 
	}                 
	
	void Action() {
		int h = 3;
	}
};

#endif
