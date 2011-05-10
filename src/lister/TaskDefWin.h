#ifndef _lister_lister_TaskDefWin_h_
#define _lister_lister_TaskDefWin_h_

#include "shared.h"
#include "shared_layout.h"
#include "Task.h"
#include <lister/Urp/UrpConfigWindow.h>

class Connection;

//==============================================================================================
class TaskDefWin : public WithTaskDefWinLayout<UrpConfigWindow> {
public:
	typedef TaskDefWin   CLASSNAME; // THISBACK won't work without this
	
	Connection          *connection;
	
	                  // Define only one task at one time.  Create Id BEFORE opening this window.
	Task                 task;
	
	                  // Let main program call TaskGrid
    Callback1<Task&>     WhenSaveTask; 
    
	                     TaskDefWin        ();
    virtual 
    void                 Open              ();
    virtual 
    void                 Open              (Ctrl       *owner);
    virtual
	void                 Close             ();
	void                 Build             (Connection *pconnection);
	void                 Load              (Task       &ptask);
	void                 SaveTask          ();
	
	                  // Hack in Ctrl class required to make Xmlize virtual or this function
	                  // will never be called.
	virtual void         Xmlize            (XmlIO xml); 
};

#endif
