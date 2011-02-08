#include "NewInstanceWin.h"

//==========================================================================================	
NewInstanceWin::NewInstanceWin() {
	CtrlLayout(*this, "Create new instance (database)");
	CtrlLayoutOKCancel(*this, "Enter");
	Sizeable().Zoomable();
}

//==========================================================================================	
String NewInstanceWin::GetInstanceName() {
	return instanceName.GetData().ToString();
}
