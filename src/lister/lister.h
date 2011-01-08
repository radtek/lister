#ifndef _Lister_h
#define _Lister_h

#include "shared.h"
#include "CursorHandler.h"
#include "ConnGrid.h"
#include "ConnState.h"
#include "Connection.h"
#include "SoundHandler.h"
#include "TestGrid.h"
#include "MyRichEdit.h"
#include "MyDropGrid.h"
#include "TaskGrid.h"
#include "ScriptGrid.h"
#include "TestWin.h"
#include "shared_db.h"

#define APP_TITLE "lister"

//==========================================================================================	
class Lister : public WithListerLayout<UrpTopWindow> {
public:
	MyDropGrid scriptDropDownList;
	UrpSplitter vertSplitter;
	UrpSplitter horizTopSplitter, horizTopMidAndRightSplitter, horizTopMidSplitter;
	UrpSplitter horizBottomSplitter, horizBottomMidAndRightSplitter, horizBottomMidSplitter;
	StaticRect topLeftPane, topMidLeftPane, topMidRightPane, topRightPane;
	StaticRect bottomLeftPane, bottomMidPane, bottomRightPane;
	ToolBar toolbar;
	GridCtrl mainGrid; // Not currently using UrpGrid since this is not saved
	ConnGrid connGrid;
	TestWin testWin;
	TaskGrid taskGrid;
	ScriptGrid scriptGrid;
	MyRichEdit cmdScript;
	Connection *controlConnection; // Connection to lister data
	Connection *activeConnection;
	ConnectionFactory connectionFactory;
	Button sendScript;
	typedef Lister CLASSNAME;
	enum EnumScreenZoom { ZOOM_NORMALSCREEN, ZOOM_FULLSCREEN, ZOOM_ALLSCREENS };
	EnumScreenZoom enumScreenZoom;
	DropGrid userList;

	Lister();
	
	// User clicked Test! on the TestGrid.
	void ClickedTest();

	// User clicked Connect! on the ConnGrid.
	void ClickedConnect();

	// Connect to the connection user selected in the ConnGrid.	
	Connection *ConnectUsingGrid(String connName);

	// Connect based on the connection listed on the ConnGrid based on passed row
	Connection *ConnectUsingGrid(int row);

	void AddScriptToHistory();
	
	// For multi-line scripts, this is the only way to see the full script.
	void SelectedScriptFromDropDown();
	
	// User pressed "+" button on script list dropdown, requesting that the script be loaded
	// into the editor.
	void PushScriptToEditor();
	
	void CreateTestFromScript();
	void BrowseTests();
	
	// Thought I needed this more	
	void ListUsers();
	
	// Capture when session->status value changed, usually during a Execute or Fetch.
	// We need the Cancel Execution button to only be active when a asynch execution call is away.
	void SessionStatusChanged(const SqlSession& session);
	
	// User clicked the connect button or clicked on a row in the conngrid.	
	void SetActiveConnection(Connection *newConnection);

	// Update all enabled/disabled buttons based on a change in the users selected active connection.
	void ActiveConnectionChanged();
	
	// This must be called when a connection changes or you won't be able to execute scripts.
	void ConnectionStatusRefresh();
	
	void CancelRunningScriptOnActiveConn();
	
	// Update the toolbar active/inactive state of each button when connection or something changes.
	void ToolBarRefresh();

	void AttachScriptToTask();
	
	void DeployLister();

	// Define the toolbar over the script editor.
	void MyToolBar(Bar& bar);
	
	// Called from main.cpp. to open the main window
	virtual void Run(bool appmodal = false);

	void ExecuteSQLActiveConn();
	
	bool Key(dword key, int count);

	void Serialize(Stream& s);
	
	// Hack in Ctrl class to make Xmlize virtual
	virtual void Xmlize(XmlIO xml);
};

#endif

