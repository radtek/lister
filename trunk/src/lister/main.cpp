#include "lister.h"

GUI_APP_MAIN
{
	//Draw::SetStdFont(Font(Font::COURIER, 9));
	//Draw::SetStdFont(Font(Font::ROMAN, 9));
	//Draw::SetStdFont(Font(Font::SERIF, 11));
	Draw::SetStdFont(Font(Font::ARIAL, 11));
	
	// Construct first.  Some serialization/xmilization doesn't take affect when in constructor. 
	// (i.e., grid column widths)
	
	Lister lister; 

	UrpMain(lister, __FILE__);
}

