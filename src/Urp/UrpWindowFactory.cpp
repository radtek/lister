#include "UrpWindowFactory.h"

//==============================================================================================
// This is the only way to do this in U++.  Failed to embed in class.
/*static*/ VectorMap<String, UrpConfigWindow *>& UrpWindowFactory::Windows() {
	static VectorMap<String, UrpConfigWindow *> windows;
	return windows;
}

//==============================================================================================
// Can't force constructor to create link to top window since usually concrete instance in main window.
UrpWindowFactory::UrpWindowFactory() {
}

//==============================================================================================
UrpWindowFactory::~UrpWindowFactory() {
	for (int i = 0; i < Windows().GetCount(); i++) {
		UrpConfigWindow *window = Windows()[i];
		for (int j = 0; j < window->ctrls.GetCount(); j++) {
			Ctrl * c = window->ctrls[j];
			delete c;
		}
		delete window;
	}
}

//==============================================================================================
UrpConfigWindow * UrpWindowFactory::Open(UrpTopWindow *ptopWindow, String pwindowName) {
	// Use this clumsy method of Get/GetAdd to avoid creating a new window on the heap that just 
	// floats off into space when a match is found on the stack
	UrpConfigWindow *window = Windows().Get(pwindowName, (UrpConfigWindow *)NULL);
	if (!window) {
		window = Windows().GetAdd(pwindowName, new UrpConfigWindow(ptopWindow, pwindowName));
		window->wasCreatedNew = true;
	} else {
		window->wasCreatedNew = false; // Let caller know if we created so they can create controls to add.
	}
	window->Sizeable().Zoomable().NoCenter();
	return window;
}
