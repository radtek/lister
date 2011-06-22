/***********************************************************************************************
*  lister - NewInstanceWin.cpp
*  
*  Window popup for user to enter a new server/db instance that can be connected to.  Based on
*  the concept of Oracle instances, of which multiple can sit on a single server.  Not the same
*  as a database, though I suppose it could have gone that way.  MS SQL Server allows multiple
*  database servers (not sure what their word is) to be on the same physical server and multiple databases
*  within each, with multiple owners or dbos.
*  
*  TODO:
*  - Get Instance window to behave.  Kinda crappy.
*  - Get it to work as an edit window.
*  - Force to upper case as typing.  Get these names looking sharp.
*  - Protect "/" on entry.  Losing it as unescaped character on postgres inserts.
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

#include "NewInstanceWin.h"

//==============================================================================================
NewInstanceWin::NewInstanceWin() {
	CtrlLayout(*this, "Create new instance (database)");
	CtrlLayoutOKCancel(*this, "Enter");
	Sizeable().Zoomable();
	envList.SearchHideRows().Searching();
	instTypList.SearchHideRows().Searching();
}

//==============================================================================================
String NewInstanceWin::GetInstanceName() {
	return instanceName.GetData().ToString();
}
