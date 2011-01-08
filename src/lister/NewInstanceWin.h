#ifndef _NewInstanceWin_h_
#define _NewInstanceWin_h_

//#include "shared.h"
#include "shared_layout.h"

class NewInstanceWin : public WithNewInstanceLayout<TopWindow> {
public:
	typedef NewInstanceWin CLASSNAME;

	NewInstanceWin();
};

#endif
