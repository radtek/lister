#include "lister.h"

GUI_APP_MAIN
{
	// Construct first.  Some serialization/xmilization doesn't take affect when in constructor. 
	// (i.e., grid column widths)
	
	Lister lister; 

	UrpMain(lister, __FILE__);
}

