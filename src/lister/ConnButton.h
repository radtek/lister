#ifndef _ConnButton_h_
#define _ConnButton_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

//==============================================================================================
class ConnButton : public Button {
public:
	typedef ConnButton CLASSNAME;
	bool            wasClicked;
	
	                ConnButton();
	void            Connect(); // Only called if we use WhenCtrlsAction, NOT WhenCtrlAction.	
};

#endif
