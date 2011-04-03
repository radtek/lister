/***********************************************************************************************
*  lister - TestButton.cpp
*  
*  Cloned from ConnButton.  To be rolled up.
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

#include "TestButton.h"

//==============================================================================================
TestButton::TestButton() : Button() {
	WhenPush = THISBACK(BeginTest); // Forces this to be a nested class
	wasClicked = false;
}

//==============================================================================================
// Only called if we use WhenCtrlsAction, NOT WhenCtrlAction.	
void TestButton::BeginTest() {
	wasClicked = true; // TestGrid will pick up
}
