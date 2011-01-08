#ifndef _TestButton_h_
#define _TestButton_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class TestButton : public Button {
public:
	typedef TestButton CLASSNAME;
	bool wasClicked;
	
	TestButton();

	// Only called if we use WhenCtrlsAction, NOT WhenCtrlAction.	
	void BeginTest();
};

#endif
