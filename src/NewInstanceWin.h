#ifndef _lister_MyTool002_NewInstanceWin_h_
#define _lister_MyTool002_NewInstanceWin_h_

#include "shared.h"

class NewInstanceWin : public WithNewInstanceLayout<TopWindow> {
public:
	typedef NewInstanceWin CLASSNAME;

	NewInstanceWin() {
		CtrlLayout(*this, "Create new instance (database)");
		Sizeable().Zoomable();
	}
	
};

#endif
