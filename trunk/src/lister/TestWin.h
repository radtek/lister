#ifndef _lister_TestWin_h_
#define _lister_TestWin_h_

#include "shared.h"

class TestWin : public WithTestWinLayout<UrpWindow> {
public:
	typedef TestWin CLASSNAME;
	Rect realPosition;
	
	TestWin() {
		CtrlLayout(*this, "Create new Test");
		Sizeable().Zoomable();
	}
	
	void Open() {
		TopWindow::Open();
		SetRect(realPosition);
	}
	
	void Close() {
		realPosition = GetRect();
		TopWindow::Close();
	}
			
	virtual void Xmlize(XmlIO xml) {
		int x, y, cx, cy;
		
		if(xml.IsLoading()) {
			xml("x", x);
			xml("y", y);
			xml("cx", cx);
			xml("cy", cy);
			
			if (cy < 400) cy = 400;
			if (cx < 400) cx = 400;
			SetRect(x, y, cx, cy);
			realPosition = Rect(x,y,cx,cy);
			
		} else {
			// Storing
			Rect r = GetRect();
			xml("x", r.left);
			xml("y", r.top);
			xml("cx", r.right);
			xml("cy", r.bottom);
		}
	}
};

#endif
