#include "TestWin.h"

//==============================================================================================
TestWin::TestWin() {
	CtrlLayout(*this, "Browse All Tests");
	Sizeable().Zoomable();
	
	testGrid.SizePos(); // Note: Cannot SizePos in TestGrid.Load() or Graphics will cycle onto all cells
}
