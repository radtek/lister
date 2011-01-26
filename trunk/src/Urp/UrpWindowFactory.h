#ifndef _lister_Urp_UrpWindowFactory_h_
#define _lister_Urp_UrpWindowFactory_h_

#include "UrpShared.h"
#include "UrpConfigWindow.h"
#include "UrpTopWindow.h"

//==============================================================================================
// Generator for dynamic creation of configurated windows
class UrpWindowFactory {
public:
	static VectorMap<String, UrpConfigWindow *>& Windows();
	UrpWindowFactory();
	~UrpWindowFactory();
	UrpConfigWindow *Open(UrpTopWindow *ptopWindow, String pwindowName); // Create and open window if not open, then show it
};


#endif
