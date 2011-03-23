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
	,             **lastActiveConnection;
	ToolBar        toolbar;           // Tools for user to control test processing.
	ContextMacros *activeContextMacros; // Active for this test window.
	Callback       WhenToolBarNeedsUpdating;
	
	               TestWin();
	void           Build(Connection *pconnection, Connection **plastActiveConnection, ContextMacros *pactiveContextMacros);
	void           MyToolBar(Bar& bar);
	void           RunAllTestsAsBatch();
	void           UpdateToolBar(UrpGrid::GridAction gridAction);
	void           ClickedTest(); // GUI wrapper for RunTest function
	String         RunTest(int row);
	virtual void   Close();
};

#endif
