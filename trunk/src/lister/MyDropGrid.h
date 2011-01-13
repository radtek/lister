#ifndef _MyDropGrid_h_
#define _MyDropGrid_h_

#include "shared.h"

#include <DropGrid/DropGrid.h>

//==============================================================================================
class MyDropGrid : public DropGrid {
public:
	typedef MyDropGrid CLASSNAME;
	Callback WhenPushAddButton;
	bool ctrlKeyIsDown;

	virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags);
	void CursorChanged();
	MultiButton::SubButton& AddPlus(const Callback &cb);
	// User selected a script in the dropdown.
	void SelectedScript();
	
	void Action();
};

#endif
