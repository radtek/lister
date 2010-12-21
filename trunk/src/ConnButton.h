#ifndef _MyTool001_ConnButton_h_
#define _MyTool001_ConnButton_h_

#include "shared.h"

class ConnButton : public Button {
public:
	typedef ConnButton CLASSNAME;
	bool wasClicked;
	
	ConnButton() : Button() {
		WhenPush = THISBACK(Connect); // Forces this to be a nested class
		wasClicked = false;
	}

	~ConnButton() {
	}

	// Only called if we use WhenCtrlsAction, NOT WhenCtrlAction.	
	void Connect() {
		wasClicked = true;
	}
};

#endif
