#ifndef _lister_TestWin_h_
#define _lister_TestWin_h_

#include "shared.h"
#include "shared_layout.h"

//==============================================================================================
class TestWin : public WithTestWinLayout<UrpWindow> {
public:
	typedef TestWin CLASSNAME;
	
	TestWin();
};

#endif
