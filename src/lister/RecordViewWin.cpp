#include "RecordViewWin.h"

//==============================================================================================
RecordViewWin::RecordViewWin() {
	CtrlLayout(*this, "Record View");
	Sizeable().Zoomable();
	
	recordViewGrid.SizePos(); // Note: Cannot SizePos in TestGrid.Load() or Graphics will cycle onto all cells
}
