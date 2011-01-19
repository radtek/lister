#include "UrpWindow.h"
#include "UrpMain.h"

//==============================================================================================
UrpWindow::UrpWindow() : TopWindow() {
}

//==============================================================================================
/*virtual*/ void UrpWindow::Run(bool appmodal/* = false*/) {};

//==============================================================================================
// Serialize (binary files) placement ONLY.  To be moved to Xmlize to improve maintainability
// of configuration/customizations	
void UrpWindow::Serialize(Stream& s) {
	SerializePlacement(s);
}

//==============================================================================================
void UrpWindow::AddCtrl(Ctrl *ctrl) {
	ctrls.Add(ctrl);
	Add(*ctrl);
}

//==============================================================================================
/*virtual*/ void UrpWindow::Open() {
	TopWindow::Open();
	//SetRect(realPosition);
};

//==============================================================================================
/*virtual*/ bool UrpWindow::OpenIfNotOpen() {
	if (!IsOpen()) {
		Open();
		return true;
	} else {
		return false;
	}
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
/*virtual*/ void UrpWindow::Close() {
	//realPosition = GetRect();
	TopWindow::Close();
};

//==============================================================================================
void UrpWindow::SerializePlacement(Stream& s, bool reminimize/* = false*/) {
	return;
	GuiLock __;
#ifndef PLATFORM_WINCE
	int version = 0;
	s / version;
	
	Rect rect = GetRect();
	s % overlapped % rect;
	bool mn = state == MINIMIZED;
	bool mx = state == MAXIMIZED;
	s.Pack(mn, mx);
	
	if (s.IsLoading()) {
		if(mn)  rect = overlapped;
		Rect limit = GetVirtualWorkArea();
#ifdef PLATFORM_WIN32
		Rect outer = rect;
		AdjustWindowRect(outer, WS_OVERLAPPEDWINDOW, FALSE);
		limit.left   += rect.left   - outer.left;
		limit.top    += rect.top    - outer.top;
		limit.right  += rect.right  - outer.right;
		limit.bottom += rect.bottom - outer.bottom;
#endif
#ifdef PLATFORM_X11
		Rect fm = windowFrameMargin;
		if((fm.left|fm.right|fm.top|fm.bottom) == 0)
			fm = Rect(8, 32, 8, 8);
		limit.left += fm.left;
		limit.right -= fm.right;
		limit.top += fm.top;
		limit.bottom -= fm.bottom;
#endif
		Size sz = min(rect.Size(), limit.Size());
		rect = RectC(
			minmax(rect.left, limit.left, limit.right - sz.cx),
			minmax(rect.top,  limit.top,  limit.bottom - sz.cy),
			sz.cx, sz.cy);
		state = OVERLAPPED;
		if(mn && reminimize)
			state = MINIMIZED;
		if(mx)
			state = MAXIMIZED;
#ifdef PLATFORM_WIN32
		if(state == OVERLAPPED)
#endif
			SetRect(rect);
		if(IsOpen()) {
	#ifdef PLATFORM_WIN32
			HWND hwnd = GetHWND();
			switch(state) {
			case MINIMIZED:
				if(!IsIconic(hwnd))
					::ShowWindow(hwnd, SW_MINIMIZE);
				break;
			case MAXIMIZED:
				if(!IsZoomed(hwnd))
					::ShowWindow(hwnd, SW_MAXIMIZE);
				break;
			default:
				if(IsIconic(hwnd) || IsZoomed(hwnd))
					::ShowWindow(hwnd, SW_RESTORE);
				break;
			}
	#endif
		}
	}
#endif
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
		SetRect(x, y, cx, cy); // SetPos(PosLeft(x, cx), PosTop(y, cy));
		
		realPosition = Rect(x,y,cx,cy);
		
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

//==============================================================================================
UrpTopWindow::UrpTopWindow() : UrpWindow() {
}

//==============================================================================================
// This must be present to extend the virtuality up to client classes, else a Run in the client 
// class would be ignored from a call to an abstracted class.
/*virtual*/ void UrpTopWindow::Run(bool appmodal/* = false*/) {};

//==============================================================================================
// This is the only way to do this in U++.  Failed to embed in class.
/*static*/ VectorMap<String, UrpConfigWindow *>& UrpWindowFactory::Windows() {
	static VectorMap<String, UrpConfigWindow *> windows;
	return windows;
}

//==============================================================================================
// Can't force constructor to create link to top window since usually concrete instance in main window.
UrpWindowFactory::UrpWindowFactory() {
}

//==============================================================================================
UrpWindowFactory::~UrpWindowFactory() {
	for (int i = 0; i < Windows().GetCount(); i++) {
		UrpConfigWindow *window = Windows()[i];
		for (int j = 0; j < window->ctrls.GetCount(); j++) {
			Ctrl * c = window->ctrls[j];
			delete c;
		}
		delete window;
	}
}

//==============================================================================================
UrpConfigWindow * UrpWindowFactory::Open(UrpTopWindow *ptopWindow, String pwindowName) {
	// Use this clumsy method of Get/GetAdd to avoid creating a new window on the heap that just 
	// floats off into space when a match is found on the stack
	UrpConfigWindow *window = Windows().Get(pwindowName, (UrpConfigWindow *)NULL);
	if (!window) {
		window = Windows().GetAdd(pwindowName, new UrpConfigWindow(ptopWindow, pwindowName));
	}
	window->Sizeable().Zoomable();
	return window;
}

//==============================================================================================
UrpConfigWindow::UrpConfigWindow(UrpTopWindow *ptopWindow, String pconfigName) {
	topWindow = ptopWindow;
	configName = pconfigName;
}

//==============================================================================================
void UrpConfigWindow::Open() {
	// Attach close method
	//CtrlLayoutOKCancel(this, configName);
	// Load config, but don't let UrpChild try to Open the window or it will recurse
	UrpChild(*topWindow, *((UrpWindow *)this), configName, CHILDWINDOWREQSTATE_LOAD);
	UrpWindow::Open();
}

//==============================================================================================
void UrpConfigWindow::Close() {
	// Save config (don't let UrpChild call Close or it will recurse)
	UrpChild(*topWindow, *((UrpWindow *)this), configName, CHILDWINDOWREQSTATE_SAVE);
	UrpWindow::Close();
}

