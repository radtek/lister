#ifndef _lister_TestWin_h_
#define _lister_TestWin_h_

#include "shared.h"
#include "shared_layout.h"
#include <lister/Urp/UrpGrid.h>
class ContextMacros;
class Connection;

//==============================================================================================
class TestWin : public WithTestWinLayout<UrpWindow> {
public:
	typedef TestWin CLASSNAME;
	
	Connection    *controlConnection
	
	               // Since this is passed to a thread (a window), it is a pointer to the pointer
	               // to the activeCOnnection (?), so if it is changed from the main window,
	               // we can hopefully pick it up.
	,            **lastActiveConnection;
	
	               // Tools for user to control test processing.
	ToolBar        toolbar;    
	
	               // List of drivers for current task that will control how any test runs
	               // access the macros and what replacement set is used.    
	DropGrid       taskDriverList;
	
	               // The active task driver as defined in the task table and task window.
	int            activeTaskDriverId;
	
	               // Active for this test window.
	ContextMacros *activeContextMacros; 
	Callback       WhenToolBarNeedsUpdating;
	
	               // Tests are tightly coupled with tasks for now
	int            taskId; 
	
	//__________________________________________________________________________________________
	
	               TestWin                ();
	void           Build                  (Connection    *pconnection
	                                      ,Connection   **plastActiveConnection
	                                      ,ContextMacros *pactiveContextMacros);
	                                     
	               // No Load() function
	               
	void           MyToolBar              (Bar& bar);
	
	               // We must set a task at the window level to manage the task driver list dropdown
	void           SetTaskId              (int ptaskId);
	
	               // And we must set the active task driver 
	void           SetActiveTaskDriverId  (int ptaskDriverId);
	void           RunSelectedTest        ();
	void           SetupTestSupportingData();
	void           RunAllTestsAsBatch     ();
	void           UpdateToolBar          (UrpGrid::GridAction gridAction);
	
	               // GUI wrapper for RunTest function
	void           ClickedTest            (); 
	String         RunTest                (int row, bool isPartOfBatch = false);
	virtual 
	void           Close                  ();
};

#endif
