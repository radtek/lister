#include "UrpWindow.h"

//==============================================================================================
UrpWindow::UrpWindow() : TopWindow(), UrpGridCommon() {
}

//==============================================================================================
/*virtual*/ void UrpWindow::Run(bool appmodal/* = false*/) {};

//==============================================================================================
/*virtual*/ void UrpWindow::Open() {
	TopWindow::Open();
}

//==============================================================================================
/*virtual*/ void UrpWindow::Open(Ctrl *owner) {
	TopWindow::Open(owner);
}

//==============================================================================================
/*virtual*/ bool UrpWindow::OpenIfNotOpen(Ctrl *owner) {
	if (!IsOpen()) {
		Open(owner);
		return true;
	} else {
		return false;
	}
}

//==============================================================================================
/*virtual*/ void UrpWindow::Close() {
	TopWindow::Close();
}

//==============================================================================================
/*virtual*/ bool UrpWindow::CloseIfNotClosed() {
	if (IsOpen()) {
		Close();
		return true;
	} else {
		return false;
	}
}

//==============================================================================================
/*virtual*/ void UrpWindow::Xmlize(XmlIO xml) {
	int x, y, cx, cy;
	
	if(xml.IsLoading()) {
		xml("x", x);
		xml("y", y);
		xml("cx", cx);
		xml("cy", cy);
		
		if (cy < 400) cy = 400;
		if (cx < 400) cx = 400;
		SetRect(x, y, cx, cy);
		// Dependent on hack to Win32Wnd.cpp added line 937: return GetVirtualWorkArea(); // hack for multimon
		// to Rect Ctrl::GetWorkArea() const
		// Also set window to NoCenter() or will keep moving

	} else {
		// Storing
		Rect r = GetRect();
		x = r.left;
		y = r.top;
		cx = r.Width();  // Now these values do not match what was set in SetRect during the load, but the screen position comes up correctly
		cy = r.Height();
		xml("x", x);
		xml("y", y);
		xml("cx", cx);
		xml("cy", cy);
	}
}


