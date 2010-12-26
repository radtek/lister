#ifndef _NewInstanceWin_h_
#define _NewInstanceWin_h_

#include "shared.h"

class NewInstanceWin : public WithNewInstanceLayout<TopWindow> {
public:
	typedef NewInstanceWin CLASSNAME;

	NewInstanceWin() {
		CtrlLayout(*this, "Create new instance (database)");
		CtrlLayoutOKCancel(*this, "Enter");
		Sizeable().Zoomable();
	}
};

#endif
