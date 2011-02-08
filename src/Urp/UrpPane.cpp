#include "UrpPane.h"

//==============================================================================================
UrpPane::~UrpPane() {
	for (int i = 0; i < ctrls.GetCount(); i++) {
		Ctrl *ctrl = ctrls[i];
		delete ctrl;
	}
}

//==============================================================================================
// Stak of controls that user can scroll through and they shuffle automatically into history.
void UrpPane::Add(String name, Ctrl *ctrl) {
	// Hack until we deal with a stak concept (not even sure what I mean!)
	if (ctrls.GetCount()) {
		Ctrl *c = ctrls[0];
		RemoveChild(c);
		delete c;
		ctrls.Clear();
	}
	
	ctrls.Add(name, ctrl);
	AddChild(ctrl);
	ctrl->SizePos();
	// Expand the pane a little??
	Rect r = GetRect();
//	r.Inflate(0, 0, 0, 100); // Expand downwards
//	SetRect(r);
}


