/***********************************************************************************************
*  lister - ConnButton.cpp
*  
*  Placed on ConnGrid.  Sets a flag for the ConnGrid to check when it WhenCtrlsAction is triggered.
*  This way if multiple buttons exist, this can be made clear to the caller.  Also, ctrl/shift/alt
*  key status may be detected here as well.
*
*  TODO:
*  - Rename to GridButton. Delete TestButton class and use this.  Genericize Connect() to Act().
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

#include "ConnButton.h"

//==============================================================================================
ConnButton::ConnButton() : Button() {
	WhenPush = THISBACK(Connect); // Forces this to be a nested class
	wasClicked = false;
}

//==============================================================================================
// Only called if we use WhenCtrlsAction, NOT WhenCtrlAction.	
void ConnButton::Connect() {
	wasClicked = true;
}

