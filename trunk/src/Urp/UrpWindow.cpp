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

//==============================================================================================
// Little sheller; needs some work
bool UrpWindow::Shell(String shellCommand) {
	String copyerr;
	String shellLine = shellCommand;
	LocalProcess lp(shellLine);
	GuiSleep(10);
	int e = lp.GetExitCode();
	String estr = lp.Get();
	bool errorincopy = false;
	bool isrunning = lp.IsRunning();
	
	if (!isrunning) {
		copyerr = (CAT << "Never even started! e=" << e << ", stdout=" << estr);
		errorincopy = true;
	}
	
	while (isrunning) {
		GuiSleep(10);
		ProcessEvents();		
		estr = lp.Get();
		if (estr.Find("ERROR:") >= 0) {
			errorincopy = true;
			lp.Kill();
			
			copyerr = (CAT <<"Export error: " << estr);
			break;
		}
		
		e = lp.GetExitCode(); // Appears to be useless
		isrunning = lp.IsRunning();
	}

	if (errorincopy) {
		copyerr = (CAT << "Error in export db proc:" << estr);
		LOG(copyerr);
		return 0;
	} else {
		return 1;
	}	
	
}