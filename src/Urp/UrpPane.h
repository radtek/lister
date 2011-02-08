#ifndef _lister_Urp_UrpPane_h_
#define _lister_Urp_UrpPane_h_

#include "UrpShared.h"

class UrpPane: public StaticRect {
public:
	VectorMap<String, Ctrl *> ctrls;
	virtual ~UrpPane();
	// Add and display control and hide others
	// Pane takes ownership
	void     Add(String name, Ctrl *ctrl);
};

#endif
