#include "lister.h"

#include "image_shared.h"
#include <Draw/iml_header.h>

#include "image_shared.h"
#include <Draw/iml_source.h>

#define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>
#include "Sql/sch_source.h"

//==============================================================================================
// Construct all objects that have to be concretely referenced (for now)
// Load all grids.  TODO: Move loading to lazy style.
Lister::Lister() {
	activeConnection = NULL;

	//		To create such a hotkey, do this:
	//		
	//		ATOM hotkey = GlobalAddAtom("Your hotkey atom name");
	//		if(hotkey) RegisterHotKey(hwnd, hotkey, MOD_CONTROL, VK_CONTROL);
	//		else { ...error... }
	//		And then handle the WM_HOTKEY message:
	//		
	//		case WM_HOTKEY:
	//		    if(wParam == hotkey)
	//		    {
	//		       // CTRL pressed!!!
	//		    }
	//		break;
	
	// Main screen
	
	LargeIcon(MyImages::icon32());
	CtrlLayout(*this, "Lister - A SQL Connection and Execution Tool");
	Sizeable().Zoomable();

	enumScreenZoom = ZOOM_NORMALSCREEN; // Should take from config
	
	// Vertical Splitter
	
	Add(vertSplitter);
	vertSplitter.Vert(horizTopSplitter, horizBottomSplitter);
	// Set default space reserved for connection list and command script
	vertSplitter.SetPos(2000);
	
	// Horizontal Splitter between top panes 1 and 2
	horizTopSplitter.Horz(topLeftPane, horizTopMidSplitter);
	horizTopSplitter.SetPos(3000);

	// Horizontal Splitter between top panes 1 and 2
	horizTopMidSplitter.Horz(topMidLeftPane, horizTopMidAndRightSplitter);
	horizTopMidSplitter.SetPos(2020);
	
	horizTopMidAndRightSplitter.Horz(topMidRightPane, topRightPane);
	horizTopMidAndRightSplitter.SetPos(1000);
	
	horizBottomSplitter.Horz(bottomLeftPane, horizBottomMidAndRightSplitter);
	horizBottomSplitter.SetPos(3000);

	horizBottomMidAndRightSplitter.Horz(bottomMidPane, bottomRightPane);		
	horizBottomMidAndRightSplitter.SetPos(1000);
	
	vertSplitter.Zoom(-1);

	horizTopSplitter.Zoom(-1);
	horizTopMidSplitter.Zoom(-1);
	horizTopMidAndRightSplitter.Zoom(-1);

	horizBottomSplitter.Zoom(-1);
	horizBottomMidAndRightSplitter.Zoom(-1);
	
	// Construct Main Output Grid

	//mainGrid.SizePos().HSizePosZ(1, 3).VSizePosZ(1, 2); // 2 bits from bottom
	mainGrid.Description("mainGrid");
	bottomMidPane.Add(mainGrid);
	mainGrid.SizePos();

	// Construct Send Script button

	Add(sendScript);
	sendScript.TopPos(2, 19).RightPos(18, 22).SetForeground();
	sendScript.SetImage(MyImages::lightning_icon16());
	sendScript.WhenPush = THISBACK(ExecuteSQLActiveConn);

	// Connect to our metadata control database using raw params

	if ((controlConnection 
		= connectionFactory.Connect(
				this, CONTROL_CONN_NAME, ConnectionFactory::ControlInstanceType(), "postgres"
				, "postgres", "localhost", "postgres"))->enumConnState 
				!= CON_SUCCEED) 
	{
		exit(-1);
	}

	// Construct Script List Drop Down Grid

	scriptDropDownList.AddColumn("ID", 0);	// Hidden Column                 #0
	scriptDropDownList.AddColumn(""); // No label so it doesn't show up      #1
	scriptDropDownList.AddColumn("RichScript", 0); // Hidden column          #2
	scriptDropDownList.Header(false);
	scriptDropDownList.ColorRows();
	scriptDropDownList.SetDropLines(30);
	scriptDropDownList.DropEnter();
	scriptDropDownList.DataAction();
	scriptDropDownList.Searching();
	scriptDropDownList.SearchHideRows(); // Very important.  Reduce size as you search, otherwise you have to scroll through non-matches
	scriptDropDownList.AddPlus(THISBACK(PushScriptToEditor));
	scriptDropDownList.SetKeyColumn(0);
	scriptDropDownList.SetFindColumn(1);
	scriptDropDownList.AddValueColumn(1);
	int ecy = EditField::GetStdHeight();
	topRightPane.Add(scriptDropDownList.BottomPos(0, ecy).HSizePos(0, 0)); // Cross entire pane

	if (controlConnection->SendQueryDataScript("select scriptid, script, richscript from scripts order by addtimestamp")) {
		while(controlConnection->Fetch()) {
			scriptDropDownList.Add(controlConnection->Get(0), controlConnection->Get(1), controlConnection->Get(2));
		}
	}

	// Construct Script Editor

	topRightPane.Add(cmdScript.VSizePos(0, ecy).HSizePos(0,0));
	cmdScript.WhenScriptContentChanged = THISBACK(ScriptContentChanged);
	
	// Construct tool bar
	
	toolbar.Set(THISBACK(MyToolBar));
	toolbar.VSizePos(0, 10);
	topRightPane.AddFrame(toolbar);

	// Construct Connection Grid
	
	connGrid.Description("connGrid");
	connGrid.WhenCtrlsAction = THISBACK(ClickedConnect);
	topLeftPane.Add(connGrid);
	connGrid.Build();
	connGrid.Load(controlConnection); // Grid can create connections
	connGrid.SizePos();
	connGrid.WhenCursor = THISBACK(SelectedConnection); // Call be when user selects a different connection in grid

	// Construct Task Grid
	
	taskGrid.Description("taskGrid");
	topMidLeftPane.Add(taskGrid);
	taskGrid.Build();
	taskGrid.Load(controlConnection);
	taskGrid.SizePos();
	taskGrid.WhenNeedLastInsertedPkId = THISBACK(GetTaskLastInsertedPkId);
	
	// Main sets callback so that the scripting toolbar state can be updated to reflect that 
	// a script can be assigned to this task.

	taskGrid.WhenCursor = THISBACK(ActiveTaskChanged);

	// Construct Script Grid
	
	scriptGrid.Description("scriptGrid");
	topMidRightPane.Add(scriptGrid);
	scriptGrid.Build();
	scriptGrid.Load(controlConnection);
	scriptGrid.SizePos();	
	scriptGrid.WhenCursor = THISBACK(SelectedTaskScript);
	
	// Join Many table Script to One table Task
	
	scriptGrid.LinkScriptToTaskGrid(taskGrid);

	// Offscreen build of test grid
	
	testWin.testGrid.Build();
	testWin.testGrid.WhenCtrlsAction = THISBACK(ClickedTest);
	testWin.testGrid.Load(controlConnection);
	
	
	// By spinning this off as a callback, we get the screen displayed while autoconnecting, and plus the cursor on conn grid is properly set to center
	//SetTimeCallback(100, THISBACK(AutoConnect));		
}

//==============================================================================================	
// User clicked Test! on the TestGrid. Execute the test script against the connection.
void Lister::ClickedTest() { 
	TestGrid &testGrid = testWin.testGrid; // Create a convenience handle
	int testRow = testGrid.GetCursor();
	
	WaitCursor wc;
	if (!testGrid.WasTestRequested()) {
		return;
	}
	
	// Get test connection
	int testConnId = testGrid.GetTestConnId(testRow);
	int testTypId = testGrid.GetTestTypId(testRow);
	int testScriptId = testGrid.GetTestScriptId(testRow);
	int testCompTypId = testGrid.GetCompTypId(testRow);
	String testX = testGrid.GetCompareUsingX(testRow);
	String testY = testGrid.GetCompareUsingY(testRow); // Note that PostgreSQL can store Arrays in a single value
	String desiredOutcome = testGrid.GetDesiredOutcome(testRow);
	bool invertComparison = testGrid.GetInvertComparison(testRow);
	
	if (testConnId == UNKNOWN) {
		Exclamation("No connection set for test");
		return;
	}
	
	// Connect or get a similar connection
	Connection * testConn = connectionFactory.Connect(this, testConnId);
	
	if (!testConn) {
		Exclamation("Can't get a test connection");
		return;
	}
	
	if (testScriptId == UNKNOWN) {
		Exclamation("No script assigned to this test");
		return;
	}
	
	String getScriptText = Format("select script from scripts where scriptid = %d", testScriptId);
	
	if (!controlConnection->SendQueryDataScript(getScriptText)) {
		return;
	}
	
	if (controlConnection->GetRowsProcessed() == 0) {
		Exclamation("Can't find script # " + testScriptId);
		return;
	}
	
	bool testCompCorrect = false; // Not same as Passed!
	bool testCompApplied = false;
	
	controlConnection->Fetch();
	String testScript = controlConnection->Get(0);

	if (testScript.IsEmpty()) {
		// TODO: IS TESTTYPE = "query text length"?  Length empty is a test value
		Exclamation("Empty script");
		return;
	}

	// Execute script
	
	if (!testConn->SendQueryDataScript(testScript)) {
		// TODO: IS TESTTYPE
		return;
	}

	if (testTypId == TESTTYP_RETURNED_ROW_COUNT) {
		int targetRowCount = atoi(testX);
		int actualRowCount = controlConnection->GetRowsProcessed();
		// Comparison rule to use
		if (testCompTypId == COMPTYP_GREATER_THAN_X) {
			testCompCorrect = (actualRowCount > targetRowCount);
			testCompApplied = true;
		}
	}
	
	if (testCompApplied) {
		if (invertComparison) {
			testCompApplied = !testCompCorrect;
		}

		if (
			(desiredOutcome == "P" && testCompCorrect)
			||
			(desiredOutcome == "F" && !testCompCorrect)
			)
		{
			// Make sound tada or yep or "uh-huh"
			Speak(EVS_TEST_SUCCEEDED);
			testGrid.SetActualOutcome(testRow, "P");
		} else {
			// Grr, doh! woopsee
			Speak(EVS_TEST_FAILED);
			testGrid.SetActualOutcome(testRow, "F");
		}
		
		testGrid.Accept();
		
		// Write to database
		// TODO: Set time, log
	}
					
	
	// Update Actual outcome, set timestamp, log a testrun
	
	// Update color state
	
	// TODO: IS TESTTYPE: Execution duration
}

//==============================================================================================	
// User clicked Connect! on the ConnGrid.
void Lister::ClickedConnect() {
	if (connGrid.WasConnectionRequested()) {
		SetActiveConnection(ConnectUsingGrid(connGrid.GetCursor()));
		if (activeConnection && activeConnection->enumConnState != CON_SUCCEED)
		{
			Prompt(Ctrl::GetAppName(), CtrlImg::exclamation()
			, Format("Connect Error: [* %s].", DeQtf(activeConnection->connectErrorMessage)), t_("OK"));
		}
	}
}

//==============================================================================================
// Connect to the connection user selected in the ConnGrid.	
Connection *Lister::ConnectUsingGrid(String connName) {
	int row = connGrid.GetCursor();
	if (connGrid.FindConnName(connName)) {
		return ConnectUsingGrid(row);
	} else {
		return NULL;
	}
}

//==============================================================================================	
// If script changed, we can't attach to a task until a script id is created.
void Lister::ScriptContentChanged() {
	ToolBarRefresh();
}

//==============================================================================================	
// Connect based on the connection listed on the ConnGrid based on passed row
Connection *Lister::ConnectUsingGrid(int row) {
	ASSERT(connGrid.IsCursor());
	connGrid.SetConnState(row, CONNECTING_START);
	ProcessEvents(); // Necessary to display color change immediately
	WaitCursor wc;
	Connection *c = connectionFactory.Connect(this
		, connGrid.GetConnName(row)
		, connGrid.GetInstanceTypeName(row)
		, connGrid.GetLoginStr(row)
		, connGrid.GetLoginPwd(row)
		, connGrid.GetInstanceAddress(row)
		, connGrid.GetDatabaseName(row)
		);
		
	UrpString::FromTo(connGrid.GetConnId(row), c->connId);
	connGrid.SetConnState(row, c->enumConnState);
	return c;
}

//==============================================================================================
// Add the script currently in the editor to the script database with a unique id.	
void Lister::AddScriptToHistory() {
	String richscript = cmdScript.GetQTF();
	String script = TrimBoth(cmdScript.Get().GetPlainText().ToString());

	if (script.IsEmpty()) {
		NotifyUser("Cannot add empty script to script database");
		return;
	}
	
	// A little trick to only insert into a table new scripts
	String controlScript = Format("insert into scripts(richscript, script) select '%s', '%s' where '%s' not in(select script from scripts)"
	    // Not sending these as scripts to run, but to insert as text
		, controlConnection->PrepTextDataForSend(richscript)
		, controlConnection->PrepTextDataForSend(script)
		, controlConnection->PrepTextDataForSend(script)); 

	if (!controlConnection->SendAddDataScript(controlScript)) {
		cmdScript.Tip(DeQtf(controlConnection->GetLastError()));
		cmdScript.SetData(controlScript);
		NotifyUser("Could failed to add script to script database");
		return;
	}

	// Add to the list if not a dup
	if (controlConnection->GetRowsProcessed() > 0) {		
		int scriptid = controlConnection->GetInsertedId("scripts", "scriptid");
		if (scriptid >= 0) {
			scriptDropDownList.Add(scriptid, script, richscript);
			cmdScript.scriptId = scriptid; // Link to script so that it can be uploaded to tests
			ToolBarRefresh();
		}
	} else {
		
		// Script is identical to another script, we grab its id.
		String searchScript = Format("select scriptId from scripts where script = '%s' limit 1"
		    // Not sending these as scripts to run, but to insert as text
			, controlConnection->PrepTextDataForSend(script)
			);
			
		if (!controlConnection->SendQueryDataScript(searchScript)) {
			NotifyUser("Error");
			return;
		}
		
		controlConnection->Fetch();
		cmdScript.scriptId = controlConnection->Get(0);
		ToolBarRefresh();
	}
}

//==============================================================================================
// For multi-line scripts, this is the only way to see the full script.
void Lister::SelectedScriptFromDropDown() {
	scriptDropDownList.Tip(scriptDropDownList.Get(1).ToString());
}

//==============================================================================================	
void Lister::SelectedTaskScript() {
	// Function is called during task-script-join op. op causes WhenCursor to be called, but without a valid cursor!
	if (!scriptGrid.IsCursor()) return;
	ASSERT(scriptGrid.IsCursor());
	int row = scriptGrid.GetCursor();
	if (GetCtrl()) { // User holding down ctrl key
		cmdScript.SetScript(scriptGrid.GetScriptId(row), scriptGrid.GetScript(row));
	}
}

//==============================================================================================	
// Convoluted way to get the last inserted id.  Needs to be better.
void Lister::GetTaskLastInsertedPkId() {
	taskGrid.lastinsertedpkid = controlConnection->GetInsertedId("tasks", "taskid");
}

//==============================================================================================	
// User pressed "+" button on script list dropdown, requesting that the script be loaded
// into the editor.
void Lister::PushScriptToEditor() {
	// 3rd field (2) is RichScript.
	cmdScript.SetRichScript(scriptDropDownList.GetKey(), scriptDropDownList.Get(2).ToString());
	
	Value scriptIdValue = scriptDropDownList.GetKey();
	
	if (scriptIdValue.IsNull()) {
		Exclamation("Script id is null. Internal error.");
		return;
	}

	if (!UrpString::IsIntegerString(scriptIdValue.ToString())) {
		Exclamation(CAT << "Script id is not an integer. " << scriptIdValue.ToString());
		return;
	}

	UrpString::FromTo(scriptIdValue, cmdScript.scriptId);
	ToolBarRefresh();
}

//==============================================================================================
// Take the script in the editor and the assigned connection and add to the test grid,
// to allow the user to fill in the test specifications and save.
void Lister::CreateTestFromScript() {
	
	// Force user to establish a connection.  (Too much?)
	if (cmdScript.connId < 0) {
		Exclamation("No connection attached to the current script editor");
		return;
	}
	
	if (cmdScript.scriptId < 0) {
		Exclamation("Script Id is negative. Can't save to test.");
		return; // Don't try and save tests with no script #, meaning not in database
	}
	
	testWin.Open();
	testWin.testGrid.AddTest(TrimBoth(cmdScript.GetScript()), cmdScript.scriptId, cmdScript.connection->connId);
}

//==============================================================================================
// Open the test grid as a floating window for selection, editing and addition
void Lister::BrowseTests() {
	testWin.Open();
}

//==============================================================================================
// Thought I needed this more, to identify schemas available.
void Lister::ListUsers() {
	if (userList.GetCount() == 0 && activeConnection && In(activeConnection->enumConnState, CON_SUCCEED, CON_STALE)) {
		Vector<String> users = activeConnection->session->EnumUsers();
		Sort(users);
		
		for (int i = 0; i < users.GetCount(); i++) {
			String userName = users.At(i);
			if (userName.StartsWith("NB")) {
				users.Remove(i);
				// Strip any users with no objects, or LDAP login (users)
			} else {
				userList.Add(userName);
			}
		}
	}
}

//==============================================================================================
// First attempt at dynamic dialog popup of UrpGrid container. May thread it.
void Lister::ListContacts() {
	// Create UrpWindow in thread?
	// Instance ContactGrid
	// Attach it to UrpWindow
	// Load Contacts (thread)
	// Show window
}

//==============================================================================================
// Capture when session->status value changed, usually during a Execute or Fetch.
// We need the Cancel Execution button to only be active when a asynch execution call is away.
void Lister::SessionStatusChanged(const SqlSession&session) {
	ToolBarRefresh();
	ConnectionStatusRefresh();
}

//==============================================================================================
// The user selected a different connection on the grid.
void Lister::SelectedConnection() {
	ASSERT(connGrid.IsCursor());
	int row = connGrid.GetCursor();
	String connName = connGrid.GetConnName(row);  // Stored in connection list by name
	// BUG: Name can change by user edit.  Will lose connection access.
	Connection *selectedConnection = connectionFactory.GetConnection(connName);
	
	if (selectedConnection != NULL) {
		SetActiveConnection(selectedConnection);
	}
}

//==============================================================================================
// User clicked the connect button or clicked on a row in the conngrid.	
void Lister::SetActiveConnection(Connection *newConnection) {
	activeConnection = newConnection;
	cmdScript.connection = activeConnection;
	ActiveConnectionChanged();
}

//==============================================================================================	
// When user selects a task, the script becomes eligible to be attached to it.
void Lister::ActiveTaskChanged() {
	ToolBarRefresh();
}

//==============================================================================================	
// Update all enabled/disabled buttons based on a change in the users selected active connection.
void Lister::ActiveConnectionChanged() {
	ToolBarRefresh();
	ConnectionStatusRefresh();
	if (!activeConnection) return;
	if (!activeConnection->session) return;
	
	activeConnection->session->WhenDatabaseActivity = THISBACK(SessionStatusChanged);
}

//==============================================================================================	
// This must be called when a connection changes or you won't be able to execute scripts.
void Lister::ConnectionStatusRefresh() {
	if (activeConnection) {
		if (!In(activeConnection->enumConnState, CON_SUCCEED, CON_STALE)) {
			Title(CAT << APP_TITLE << " - Not connected to " << activeConnection->connName);	
		} else {
			Title(CAT << APP_TITLE << " - Connected to " << activeConnection->connName);	
		}
	} else {
		Title(CAT << APP_TITLE << " - Not connected");
	}
}

//==============================================================================================	
void Lister::CancelRunningScriptOnActiveConn() {
	activeConnection->Cancel(); // Sends OCICancel (Need reset?)
}

//==============================================================================================	
void Lister::AttachScriptToTask() {
	ASSERT(taskGrid.IsCursor());
	ASSERT(cmdScript.scriptId >= 0);
	int taskGridRow = taskGrid.GetCursor();
	int taskId = taskGrid.GetTaskId(taskGridRow);
	int scriptId = cmdScript.scriptId;
	
	String why; // User must provide a reason for the mapping.
	
	if (!UrpInputBox(
			why
		,	"Task-Script Attachment"
		,	CAT << "Enter a reason for the attachment, how the script will help close out the task. " 
		<<	"(taskId=" << taskId << ", scriptId=" << cmdScript.scriptId)
		) 
	{
		return;
	}
	
	why = TrimBoth(why);
	if (why.IsEmpty() || why.GetLength() < 10) {
		Exclamation("Must provide a reason for attachment so it makes sense later (when you forget)");
		return;
	}
	
	String script = Format("insert into relations("
								    "fromid"
								    ",  toid"
								    ",      fromtbid"
								    ",          totbid"
								    ",              why"
								    ")"
	                       " values(%d, %d, %d, %d, '%s')"
	                       ,        taskId
	                       ,            cmdScript.scriptId
	                       ,                OB_TASKS
	                       ,                    OB_SCRIPTS
	                       ,                        controlConnection->PrepTextDataForSend(why));
	                       
	if (controlConnection->SendAddDataScript(script)) {
		// Update script grid filter (joined to task table), display just added script.
		scriptGrid.ReQuery();
	}
}

//==============================================================================================	
void Lister::DeployLister() {
	String targetProdPath = "C:/Program Files/lister/lister.exe";
	String targetConfigDir = GetHomeDirectory();
	FileCopy(exeFilePath, targetProdPath);
	// Only copy the dev configurations to release if the ctrl-key is hel down. 
	// Generally we don't need to destroy our release settings.
	if (GetCtrl()) {	

		String fileName = GetFileName(binConfigFile);
		String targetConfigFile = AppendFileName(targetConfigDir, fileName);
		String backupTargetConfigFile = targetConfigFile + ".bak";
		if (NormalizePath(binConfigFile) != NormalizePath(targetConfigFile)) {
			FileCopy(targetConfigFile, backupTargetConfigFile);
			FileCopy(binConfigFile, targetConfigFile);
		}
			         
		fileName = GetFileName(xmlConfigFile);
		targetConfigFile = AppendFileName(targetConfigDir, fileName);
		backupTargetConfigFile = targetConfigFile + ".bak";
		if (NormalizePath(xmlConfigFile) != NormalizePath(targetConfigFile)) {
			FileCopy(targetConfigFile, backupTargetConfigFile);
			FileCopy(xmlConfigFile, targetConfigFile);
		}
	}
}

//==============================================================================================	
// Update the toolbar active/inactive state of each button when connection or something changes.
void Lister::ToolBarRefresh() {
	toolbar.Set(THISBACK(MyToolBar));
}

//==============================================================================================	
// Define the toolbar over the script editor.
void Lister::MyToolBar(Bar& bar) {
	
	// Add Script To History
	bar.Add(!(cmdScript.Get().GetPlainText().ToString().IsEmpty()), "File", CtrlImg::smalldown(), THISBACK(AddScriptToHistory)).Tip("Memorize Script");
	
	// Create Test From Script if we have an id
	bar.Add( // Only allow test creation if there is a script, a scriptid, and a connection
		(cmdScript.Get().GetPlainText().ToString().GetLength() > 0 
		 && cmdScript.connection
		 && cmdScript.scriptId >= 0), "File", MyImages::addtotest16(), 
		 THISBACK(CreateTestFromScript)).Tip("Create a Test around this Script");

	// Browse existing Tests
	bar.Add( true, "", MyImages::browsetests16(), 
		 THISBACK(BrowseTests)).Tip("Browse and edit tests");
		 
	// Execute script against current connection
	bar.Add( // Only allow execution if there is a script and a connection
		(cmdScript.Get().GetPlainText().ToString().GetLength() > 0 
		 && cmdScript.connection), "File", MyImages::lightning_icon16(), 
		 THISBACK(ExecuteSQLActiveConn)).Tip("Execute Script");

	// Execute script and pass output to physical table in control db.
	bar.Add( // Only allow execution if there is a script and a connection
		(cmdScript.Get().GetPlainText().ToString().GetLength() > 0 
		 && cmdScript.connection), "File", MyImages::lightningload_icon16(), 
		 THISBACK(ExecuteAndLoadSQLActiveConn)).Tip("Execute Script and create a table in the control database");
	        
	// Cancel a running Script
	bar.Add(
		(cmdScript.Get().GetPlainText().ToString().GetLength() > 0 
		 && cmdScript.connection
		 && activeConnection 
		 && activeConnection->session 
		 && In(activeConnection->session->GetStatus(), activeConnection->session->START_EXECUTING, activeConnection->session->START_FETCHING)
	 	), "File", MyImages::cancelop16(), 
		 THISBACK(CancelRunningScriptOnActiveConn)).Tip("Cancel executing script on active connection");

	cmdScript.FindReplaceTool(bar);
	
	// List database users/schemas
	bar.Add(
		(cmdScript.connection), "ListUsers", MyImages::user16(), 
		 THISBACK(ListUsers)).Tip("List users/schemas for current connection");

	bar.Add(userList, 100);

	// Deploy application to Program Files folder
	bar.Add(true, "Deploy", MyImages::deploylister16(), 
		 THISBACK(DeployLister)).Tip("Deploy lister to program files");

	// Atach this Script to the selected Task
	bar.Add(
		cmdScript.Get().GetPlainText().ToString().GetLength() > 0 && cmdScript.scriptId >= 0
		&& taskGrid.IsCursor()
		, "Attach", MyImages::attachtotask16(),
		 THISBACK(AttachScriptToTask)).Tip("Attach script to selected task");
	
	//CtrlImg::exclamation(), CtrlImg::smallright(), CtrlImg::open(), CtrlImg::undo(), CtrlImg::remove
	//smallcheck, spinup3, smallreporticon, save, Plus, Minus, Toggle, help
}

//==============================================================================================	
// Called from main.cpp. to open the main window
/*virtual*/ void Lister::Run(bool appmodal /* = false */) {
	ToolBarRefresh(); // Crucial to update toolbar after main Xmlize run has loaded configuration, connids, saved script ids.
	ConnectionStatusRefresh();
	TopWindow::SetFocus();
	TopWindow::Run(appmodal);
	
	// Make the interface seem to close faster, let destructor run invisibly
	Hide();
	ProcessEvents();
}

//==============================================================================================
void Lister::ScriptExecutionHandler(bool loadIntoTable) {
	String script = cmdScript.GetScript();

	if (script.IsEmpty()) {
		BeepExclamation();
		return;
	}
	
	if (!activeConnection) {
		Exclamation("No connection");
		return;
	}
	
	if (activeConnection->enumConnState != CON_SUCCEED) {
		Exclamation("Disconnected");
		return;
	}
	
	CursorHandler cursorHandler(controlConnection, activeConnection);
	bool ran = cursorHandler.Run(&mainGrid, script, loadIntoTable);
}
                            
//==============================================================================================
// Execute the script in the script editor against the active connection and push output to a
// table in the local database.
void Lister::ExecuteAndLoadSQLActiveConn() {
	ScriptExecutionHandler(true /* load into table, leave grid empty */);
}

//==============================================================================================
// Execute the script in the script editor against the active connection	
void Lister::ExecuteSQLActiveConn() {
	ScriptExecutionHandler(false /* Don't load into table, load into grid */);
}

//==============================================================================================
// Capture screen zoomer keys	
bool Lister::Key(dword key, int count) {
	static dword style;
	static Rect normalwindowrect;
	
	switch (key) {
		case K_F11:
			// Rotate through 3 states
			if (enumScreenZoom != ZOOM_ALLSCREENS) {
				
				// Get the full size of the screen (single physical screen)
				long cx = GetSystemMetrics(SM_CXSCREEN);
   				long cy = GetSystemMetrics(SM_CYSCREEN);
				
				if (enumScreenZoom == ZOOM_FULLSCREEN) {
					// Zoom to multiple screens!
					cx = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	   				cy = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	   				enumScreenZoom = ZOOM_ALLSCREENS;
	   				
	   			// Only save the normal style and size, not the full screen size
				} else {
					// Get the current Top window's style bits
					style = GetStyle();
					// Strips the caption bit, which is the title bar
					style = (style & ~WS_CAPTION);
					// Save the "overlapped" or normal window shape
					normalwindowrect = GetRect(); 
					SetWindowLong(GetHWND(), GWL_STYLE, style);  
					enumScreenZoom = ZOOM_FULLSCREEN;
				}
				
   				// Expand the window to full size
   				SetWindowPos(GetHWND(),HWND_TOP,0,0,cx,cy,SWP_SHOWWINDOW); 
   				
				//or use Maximize(false) if you like a delayed resizing, even with zoom effects turned off
			} else {
				// Set the caption bit back so we can get our title bar back
				style |= WS_CAPTION;
				// Pass to windows (Style() method gets confused on SyncCaption0)
				SetWindowLong(GetHWND(), GWL_STYLE, style);
				// Return size to normal; key flag is SWP_FRAMECHANGED
				SetWindowPos(GetHWND(),HWND_TOP, 
					normalwindowrect.left, 
					normalwindowrect.top, 
					normalwindowrect.Width(), 
					normalwindowrect.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED);
					enumScreenZoom = ZOOM_NORMALSCREEN;
			}
			break;
	}
	return false;
}

//==============================================================================================
void Lister::NotifyUser(String message) {
	Exclamation(message);
}

//==============================================================================================
// Serialize (binary files) placement ONLY.  To be moved to Xmlize to improve maintainability
// of configuration/customizations	
void Lister::Serialize(Stream& s) {
	SerializePlacement(s);
}

//==============================================================================================	
// Dependendent on HACK in Ctrl class to make Xmlize virtual
/*virtual*/ void Lister::Xmlize(XmlIO xml) {
	// Use generic names internally to xml with numbers so if splitters added/moved, we can 
	// retain users setting if lucky
	xml
		("vertsplit1", vertSplitter)
		("horzsplit1", horizTopSplitter) 
		("horzsplit11", horizTopMidSplitter) 
		("horzsplit12", horizTopMidAndRightSplitter) 
		("horzsplit2", horizBottomSplitter) 
		("horzsplit21", horizBottomMidAndRightSplitter) 
		("grid1", connGrid)
		("grid2", taskGrid)
		("grid3", scriptGrid)
		("grid4", testWin.testGrid)
		("win1", testWin)
		("edit1", cmdScript) // Generically, the editor screen.
	;
}