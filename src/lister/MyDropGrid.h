#ifndef _MyDropGrid_h_
#define _MyDropGrid_h_

#include "shared.h"

#include <DropGrid/DropGrid.h>

//==========================================================================================	
struct MyDropGrid : public DropGrid {
	typedef MyDropGrid CLASSNAME;
	MyDropGrid() : DropGrid() {
		list.WhenPopDown = THISBACK(SelectedScript);
		list.WhenCursor = THISBACK(CursorChanged);
		//list.WhenLeftClick = THISBACK(
		list.DrawFocus().FocusLostAccepting();
		ctrlKeyIsDown = false;
	}

	bool ctrlKeyIsDown;
	//==========================================================================================	
	virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags) {
		if (event == Ctrl::MOUSELEAVE) {
			ctrlKeyIsDown = false;
		}
		
		if (keyflags & K_CTRL && event == Ctrl::LEFTDOWN) {
			ctrlKeyIsDown = true;
		} 
		
		if (event == Ctrl::LEFTUP) {
			ctrlKeyIsDown = false;
		}
		
		return DropGrid::MouseEvent(event, p, zdelta, keyflags);
	}

	//==========================================================================================	
	void CursorChanged() {
	}

	Callback WhenPushAddButton;
		
	//==========================================================================================	
	MultiButton::SubButton& AddPlus(const Callback &cb) {
		WhenPushAddButton = cb;
		return DropGrid::AddPlus(cb);
	}
	
	//==========================================================================================	
	// User selected a script in the dropdown.
	void SelectedScript() {
		Tip(Get(1).ToString()); // Set the tip; may help to see full script with newlines.
		if (GetCtrl()) { // !!(GetKeyState(VK_CONTROL) & 0x8000);
			if (WhenPushAddButton) WhenPushAddButton(); // Simulate button push to avoid duping code.
		}
	}                 
	
	void Action() {
	}
};

#endif
