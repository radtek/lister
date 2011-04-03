/***********************************************************************************************
*  lister - RecordViewWin.cpp
*  
*  Simple container for RecordViewGrid.  Could probably be dynamic generation.
*
*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

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
