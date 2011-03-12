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
	typedef TaskDefWin CLASSNAME; // THISBACK won't work without this
	
	Connection          *connection;
	Task                 task;         // Define only one task at one time.  Create Id BEFORE opening this window.
    Callback1<Task&>     WhenSaveTask; // Let main program call TaskGrid
    
	                     TaskDefWin();
    virtual void         Open();
    virtual void         Open(Ctrl *owner);
	void                 Build(Connection *pconnection);
	void                 Load(Task &ptask);
	void                 SaveTask();
	virtual void         Xmlize(XmlIO xml); // Hack in Ctrl class to make Xmlize virtual
};

#endif
