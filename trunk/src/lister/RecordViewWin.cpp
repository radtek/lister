#include "RecordViewWin.h"

#include "image_shared.h"
#include <Draw/iml_header.h>

// Only in lister.cpp
//#include "image_shared.h"
//#include <Draw/iml_source.h>

//==============================================================================================
RecordViewWin::RecordViewWin() {
	CtrlLayout(*this, "Record View");
	Sizeable().Zoomable();
	// Icon ignored when set here. Icon(MyImages::recview16());
	recordViewGrid.SizePos(); // Note: Cannot SizePos in TestGrid.Load() or Graphics will cycle onto all cells
}
