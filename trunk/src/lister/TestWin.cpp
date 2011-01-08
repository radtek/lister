#include "TestWin.h"

	TestWin::TestWin() {
		CtrlLayout(*this, "Create new Test");
		Sizeable().Zoomable();
	}
	
	void TestWin::Open() {
		TopWindow::Open();
		SetRect(realPosition);
	}
	
	void TestWin::Close() {
		realPosition = GetRect();
		TopWindow::Close();
	}
			
	/*virtual*/ void TestWin::Xmlize(XmlIO xml) {
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
