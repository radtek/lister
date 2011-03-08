#ifndef _Lister_h
#define _Lister_h

#include "shared.h"
#include "CursorHandler.h"
#include "ConnState.h"
#include "Connection.h"
#include "SoundHandler.h"
#include "MyRichEdit.h"
#include "MyDropGrid.h"
#include "ConnGrid.h"
#include "TestGrid.h"
#include "TaskGrid.h"
#include "ScriptGrid.h"
#include "OutputGrid.h"
#include "TestWin.h"
#include "LogWin.h"
#include "shared_db.h"
#include "JobSpec.h"
#include "TaskDefWin.h"
#include "ContextMacros.h"

#include <lister/Urp/UrpTopWindow.h>
#include <lister/Urp/UrpWindowFactory.h>
#include <lister/Urp/UrpPane.h>

#include "OutputStat.h"

#define APP_TITLE "lister"

//==========================================================================================	
class Lister : public WithListerLayout<UrpTopWindow> {
public:
	typedef Lister CLASSNAME;
	
	enum EnumScreenZoom {
				ZOOM_NORMALSCREEN
			,	ZOOM_FULLSCREEN
			,	ZOOM_ALLSCREENS 
			};
	EnumScreenZoom    enumScreenZoom;
	UrpSplitter       vertSplitter;
	UrpSplitter       horizTopSplitter, horizTopMidAndRightSplitter, horizTopMidSplitter;
	UrpSplitter       horizBottomSplitter, horizBottomMidAndRightSplitter, horizBottomMidSplitter;
	StaticRect        topLeftPane, topMidLeftPane, topMidRightPane, topRightPane;
	StaticRect        bottomLeftPane, bottomMidPane;
	UrpPane           bottomRightPane;
	ToolBar           toolbar;
	OutputGrid        mainGrid; // Not currently using UrpGrid since the outputGrid's column specs are not saved (they change constantly)
	OutputStat        outputStat;
	ConnGrid          connGrid;
	TestWin           testWin;
	TaskGrid          taskGrid;
	ScriptGrid        scriptGrid;
	MyRichEdit        scriptEditor;
	Connection       *controlConnection; // Connection to lister data
	Connection       *activeConnection;
	ConnectionFactory connectionFactory;
	UrpWindowFactory  windowFactory;
	MyDropGrid        scriptDropDownList; // Detects ctrl-key held down during a selection
	WithDropChoice<EditString>            // Allows free text entry
	                  targetNameList;
	DropGrid          userList
			,		  scriptTargetList
			,		  fastFlushTargetList;
	EditInt           fldRowLimit;
	TaskDefWin        taskDefWin;
	MenuBar           mainMenu;
	bool              maingridselectrow;
	Option            chkAddSepToOutput; // Add bogus separator lines after groups of screen row output
	EditString        outFldSepWhenValChange; // Key in name of column header
	EditInt           fldSepRowCount; // How many blank rows would you like?  When copying to Excel, this saves a lot of time.
	DropGrid          outputColList; // Updated after output generated.
	ContextMacros     activeContextMacros; // input/output from taskmacro table; loaded per task selection.
	DropGrid          macrosAvailableList; // Coalesce of all macros in context for script
	
	                  Lister();
	                 ~Lister();
	void              ViewMappings();
	void              MainMenu(Bar& bar);
	                  // Either select whole row or cells.  For long rows it is helpful to select the entire row for visual comparison.
	void              ToggleMainGridSelectRow(); 
	                  // Copy all the column names to a list so we can paste them back into the query, 
	                  // replacing "*" markers so we can subtract specific columns or insert calc cols at certain points
	void              CopyColListCommaDelim(); 
	void              CopyColListCommaDelimByType(); // List grouped by data type so we can filter
	void              MainGridContextMenu(Bar &bar); // Right-click over main output data.
	void              FileMenu(Bar& bar);
	void              ClickedTest(); // User clicked Test! on the TestGrid.  Run the test.
	void              TaskGridContextMenu(Bar &bar);
	void              SelectedAvailableMacro(); // User selected a task macro and wants to insert it into his script
	void              HideSelectedTasks();
	void              OpenTaskDefWin();
	void              SaveTaskViaTaskGrid(Task &task); // All writing is done in taskgrid
	void              ClickedConnect(); // User clicked Connect! on the ConnGrid.
	Connection       *ConnectUsingGrid(String connName, bool log = false); // Connect to the connection user selected in the ConnGrid.	
	Connection       *ConnectUsingGrid(int row, bool log = false); // Connect based on the connection listed on the ConnGrid based on passed row
	void              SaveScript();
	void              AddScriptToHistory();
	void              SelectedScriptFromDropDown(); // For multi-line scripts, this is the only way to see the full script.
	void              ProcessSelectedTaskScripts(dword key);
	void              ProcessTaskScript(int taskScriptRow, bool loadScript, bool executeScript, JobSpec &jspec); // User pressed Ctrl-F8 to load into Editor
	void              GetTaskLastInsertedPkId();
	                  // User pressed "+" button on script list dropdown, requesting that the script be loaded
	                  // into the editor.
	void              PushScriptToEditor();
	void              CreateTestFromScript();
	void              BrowseTests();
	void              ListUsers(); // Thought I needed this more
	                  // Capture when session->status value changed, usually during a Execute or Fetch.
	                  // We need the Cancel Execution button to only be active when a asynch execution call is away.
	void              ListContacts(); // First attempt at dynamic dialog popup of UrpGrid container. May thread it.
	void              SessionStatusChanged(const SqlSession& session);
	bool              SelectConnectionById(int pconnId);
	void              SelectedConnection();
	void              SetActiveConnection(Connection *newConnection); // User clicked the connect button or clicked on a row in the conngrid.	
	void              ActiveConnectionChanged(); // Update all enabled/disabled buttons based on a change in the users selected active connection.
	void              ActiveTaskChanged();
	void              ActiveTaskScriptChanged();
	void              ScriptContentChanged(); // If script changed, we can't attach to a task until a script id is created.
	void              ConnectionStatusRefresh(); // This must be called when a connection changes or you won't be able to execute scripts.
	void              CancelRunningScriptOnActiveConn();
	void              AttachScriptToTask();
	void              DeployLister();
	void              NotifyUser(String message); // Simple wrapper
	void              ToolBarRefresh(); // Update the toolbar active/inactive state of each button when connection or something changes.
	void              MyToolBar(Bar& bar); // Define the toolbar over the script editor.
	virtual void      Run(bool appmodal = false); // Called from main.cpp. to open the main window
	void              ScriptExecutionHandler(Script::ScriptTarget pscriptTarget);
	void              ConnRunScriptOutputToScreen(); // Connect to saved connid in task script, not current connection
	void              RunScriptOutputToScreen(); // Load to grid only
	void              RunScriptOutputToTable(); // Load output of script into a table in control db.
	bool              Key(dword key, int count);
	virtual void      Xmlize(XmlIO xml); // Hack in Ctrl class to make Xmlize virtual
};

#endif

