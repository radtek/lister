#include "MyDropGrid.h"

//==========================================================================================	
/*virtual*/ Image MyDropGrid::MouseEvent(int event, Point p, int zdelta, dword keyflags) {
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
void MyDropGrid::CursorChanged() {
}

//==========================================================================================	
MultiButton::SubButton& MyDropGrid::AddPlus(const Callback &cb) {
	WhenPushAddButton = cb;
	return DropGrid::AddPlus(cb);
}

//==========================================================================================	
// User selected a script in the dropdown.
void MyDropGrid::SelectedScript() {
	Tip(Get(1).ToString()); // Set the tip; may help to see full script with newlines.
	if (GetCtrl()) { // !!(GetKeyState(VK_CONTROL) & 0x8000);
		if (WhenPushAddButton) WhenPushAddButton(); // Simulate button push to avoid duping code.
	}
}                 

void MyDropGrid::Action() {
}
