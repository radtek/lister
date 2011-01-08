#ifndef _lister_TestWin_h_
#define _lister_TestWin_h_

#include "shared.h"

class TestWin : public WithTestWinLayout<UrpWindow> {
public:
	typedef TestWin CLASSNAME;
	Rect realPosition;
	
	TestWin();
	
	void Open();
	
	void Close();
			
	virtual void Xmlize(XmlIO xml);
};

#endif
