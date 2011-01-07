#include <Urp/Urp.h>
#include "Lister.h"

#include "image_shared.h"
#include <Draw/iml_source.h>

GUI_APP_MAIN
{
	// Construct first.  Some serialization/xmilization doesn't take affect when in constructor. 
	// (i.e., grid column widths)
	
	Lister lister; 

	UrpMain(lister, __FILE__);
}

