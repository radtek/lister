#include "UrpConfigWindow.h"
#include "UrpMain.h" // Call to UrpChild (may be moved out of UrpMain)

//==============================================================================================
UrpConfigWindow::UrpConfigWindow() {
	topWindow = NULL;
}

//==============================================================================================
UrpConfigWindow::UrpConfigWindow(UrpTopWindow *ptopWindow, String pconfigName) {
	topWindow = ptopWindow;
	configName = pconfigName;
}

//==============================================================================================
/*virtual*/ void UrpConfigWindow::Open() {
	
	// Attach close method
	//CtrlLayoutOKCancel(this, configName);
	// Load config, but don't let UrpChild try to Open the window or it will recurse
//	UrpChild(*topWindow, *((UrpWindow *)this), configName, CHILDWINDOWREQSTATE_LOAD);
	//UrpChild(*topWindow, *((UrpWindow *)this), configName, CHILDWINDOWREQSTATE_OPEN);
	UrpWindow::Open();
}

//==============================================================================================
// Call the UrpChild window handler and it will call UrpConfigWindow::Open()
void UrpConfigWindow::OpenWithConfig(UrpTopWindow *ptopWindow/*= NULL*/, String pconfigName/* = Null*/) {
	if (IsOpen()) {
		return;
	}
	
	if (ptopWindow)	topWindow = ptopWindow;
	if (!pconfigName.IsEmpty()) configName = pconfigName;
	
	
	if (!topWindow) {
		throw new Exc("Cannot create a window of UrpConfigWIndow without a topWindow");
	}
	
	if (configName.IsEmpty()) {
		throw new Exc("Cannot create a window of UrpConfigWIndow without a configname to build a config file uniquely");
	}

	// Order important!  The Open call must take place before Xmlize in Grid since dynamically created windows
	// with ArrayCtrl do not set szcx to !0 until painting.  Static ArrayCtrls
	// created in a main window have szcx set properly to non-zero.  See HeaderCtrl.cpp ln 348
	// in function SetTabWidth0 "if(szcx == 0) return;" prevents setting of column size by 
	// config Xmlize for dynamic windows.
	Open();
	// Now the Xmlize function in UrpSqlGrid can be called and sizes sent to the HeaderCtrl will be accepted.
	UrpChild(*topWindow, *((UrpWindow *)this), configName, CHILDWINDOWREQSTATE_LOAD);
	// Dependent on hack to Win32Wnd.cpp added line 937: return GetVirtualWorkArea(); // hack for multimon
	// to Rect Ctrl::GetWorkArea() const
	
}

//==============================================================================================
/*virtual*/ void UrpConfigWindow::Close() {
	if (!topWindow) {
		throw new Exc("Cannot close a window of UrpConfigWIndow without a topWindow");
	}
	
	if (configName.IsEmpty()) {
		throw new Exc("Cannot close a window of UrpConfigWIndow without a configname to build a config file uniquely");
	}
		
	// Save config (don't let UrpChild call Close or it will recurse)
	UrpChild(*topWindow, *((UrpWindow *)this), configName, CHILDWINDOWREQSTATE_SAVE);
	UrpWindow::Close();
}

//==============================================================================================
void UrpConfigWindow::AddCtrl(Ctrl *ctrl) {
	ctrls.Add(ctrl);
	Add(*ctrl);
	ctrl->SizePos();
}

//==============================================================================================
/*virtual*/ void UrpConfigWindow::Xmlize(XmlIO xml) {
	Ctrl *ctrl;

	for (int i = 0; i < ctrls.GetCount(); i++) {
		ctrl = ctrls.At(i);
		String name = Split(String(typeid(*ctrl).name()), ' ')[1];
		xml(name, *ctrl);
		//ctrl->Xmlize(xml);
	}
	
	UrpWindow::Xmlize(xml);
}

