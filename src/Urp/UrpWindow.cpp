#include "UrpWindow.h"

UrpWindow::UrpWindow() : TopWindow() {
}

/*virtual*/ void UrpWindow::Run(bool appmodal/* = false*/) {};

void UrpWindow::SerializePlacement(Stream& s, bool reminimize/* = false*/) {
	
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

UrpTopWindow::UrpTopWindow() : UrpWindow() {
}

// This must be present to extend the virtuality up to client classes, else a Run in the client 
// class would be ignored from a call to an abstracted class.
/*virtual*/ void UrpTopWindow::Run(bool appmodal/* = false*/) {};
