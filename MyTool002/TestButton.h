#ifndef _lister_MyTool002_TestButton_h_
#define _lister_MyTool002_TestButton_h_

#include "shared.h"

class TestButton : public Button {
public:
	typedef TestButton CLASSNAME;
	bool wasClicked;
	
	TestButton() : Button() {
		WhenPush = THISBACK(BeginTest); // Forces this to be a nested class
		wasClicked = false;
	}

	~TestButton() {
	}

	// Only called if we use WhenCtrlsAction, NOT WhenCtrlAction.	
	void BeginTest() {
		wasClicked = true; // TestGrid will pick up
	}
};

#endif
