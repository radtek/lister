#ifndef _lister_Urp_UrpConfigWindow_h_
#define _lister_Urp_UrpConfigWindow_h_

#include "UrpShared.h"
#include "UrpWindow.h"
#include "UrpTopWindow.h" // Needs to know where config files are stored

//==============================================================================================
// Manages its own configuration, will be standard SOON
class UrpConfigWindow : public UrpWindow {
public:
	String                 configName;
	UrpTopWindow          *topWindow;
	Vector<Ctrl *>         ctrls;
	bool                   wasCreatedNew;
	
						   UrpConfigWindow(); // Default constructor required for U++ layout use
	// Construct a configurable window; take config path from topWindow
	                       UrpConfigWindow(UrpTopWindow *ptopWindow, String pconfigName);
	virtual void           Open(); // Load configuration file based on 
	void                   OpenWithConfig(UrpTopWindow *ptopWindow = NULL, String pconfigName = Null);
	virtual void           Close();
	void                   AddCtrl(Ctrl *ctrl); // Only used for dynamically instanced windows
	virtual void           Xmlize(XmlIO xml);

};


#endif
