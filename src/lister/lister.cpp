#include "lister.h"
#include "Script.h"
#include "ContactGrid.h"
#include <lister/Urp/UrpMain.h>
#include <lister/Urp/UrpInput.h> // Has Ok() in it
#include <lister/Urp/UrpConfigWindow.h>

#include "KeyHandler.h"
#include "JobSpec.h"
#include "Task.h"

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
	Init(__FILE__);
	CtrlLayout(*this, "Lister - A SQL Connection and Execution Tool");

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

	String s = Script::GetScriptListQuery();
	if (controlConnection->SendQueryDataScript(s)) {
		while(controlConnection->Fetch()) {
			scriptDropDownList.Add(controlConnection->Get(0), controlConnection->Get(1), controlConnection->Get(2));
		}
	}

	// Construct Script Editor

	topRightPane.Add(scriptEditor.VSizePos(0, ecy).HSizePos(0,0));
	scriptEditor.WhenScriptContentChanged = THISBACK(ScriptContentChanged);
	scriptEditor.WhenToolBarMayBeAffected = THISBACK(ToolBarRefresh);
	
	// Construct tool bar
	
	toolbar.Set(THISBACK(MyToolBar));
	toolbar.VSizePos(0, 10);
	topRightPane.AddFrame(toolbar);

	// Populate dropdowns on toolbar
	scriptGrid.LoadScriptTargetList(scriptTargetList);
	scriptGrid.PrepTargetFastFlush(fastFlushTargetList);
	
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
	taskGrid.WhenBar = THISBACK(TaskGridContextMenu);
	taskGrid.WhenLeftDouble = THISBACK(OpenTaskDefWin);
	
	taskDefWin.WhenSaveTask = THISBACK(SaveTaskViaTaskGrid);
	// Construct Script Grid
	
	scriptGrid.Description("scriptGrid");
	topMidRightPane.Add(scriptGrid);
	scriptGrid.Build(controlConnection);
	scriptGrid.Load();
	scriptGrid.SizePos();	
	// We need to capture when the user clicked on a script, not just when the cursor changed.
	// Due to the Join to Task, the cursor changes whenever a task is selected.
	// For some reason, HasFocus is not set on the first click into the script grid when
	// WhenCursorChanged is called.  This may be a U++ bug, since the click in the grid is what
	// triggered the event, hence focus.  But, we'll use left click, which is triggered even
	// when the cursor (row) doesn't change, which may be a problem, but is correctable and
	// detectable.
	scriptGrid.WhenLeftClick = THISBACK(ActiveTaskScriptChanged);
	
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
Lister::~Lister() {
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
	
	int testConnId          = testGrid.GetTestConnId(testRow);
	int testTypId           = testGrid.GetTestTypId(testRow);
	int testScriptId        = testGrid.GetTestScriptId(testRow);
	int testCompTypId       = testGrid.GetCompTypId(testRow);
	String testX            = testGrid.GetCompareUsingX(testRow);
	String testY            = testGrid.GetCompareUsingY(testRow); // Note that PostgreSQL can store Arrays in a single value
	String desiredOutcome   = testGrid.GetDesiredOutcome(testRow);
	bool invertComparison   = testGrid.GetInvertComparison(testRow);
	
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
	
	String getScriptText = Script::GetScriptDetailByIdQuery(testScriptId);
	
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
	String testScript = controlConnection->Get(0); // == SCRIPTPLAINTEXT

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
// Task Grid Options
void Lister::TaskGridContextMenu(Bar &bar) {
	bar.Add("Edit Task Detail", THISBACK(OpenTaskDefWin));
}

//==============================================================================================	
void Lister::OpenTaskDefWin() {
	if (!taskGrid.IsCursor()) return;
	int selectedTaskRow = taskGrid.GetCursor();

	if (!taskDefWin.built) {
		taskDefWin.Build(controlConnection);
		taskDefWin.TopMost();
	}
	
	Task task = taskGrid.BuildTask(selectedTaskRow);
	
	taskDefWin.Load(task);
	taskDefWin.OpenWithConfig(this, "taskdefwin");
	taskDefWin.Show(true);
}

//==============================================================================================	
void Lister::SaveTaskViaTaskGrid(Task &task) {
	if (!taskGrid.SaveTask(task)) {
		Exclamation("Failed to save task");
	}
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
Connection *Lister::ConnectUsingGrid(String connName, bool log) {
	int row = connGrid.GetCursor();
	if (connGrid.FindConnName(connName)) {
		return ConnectUsingGrid(row, log);
	} else {
		if (log) LogLine(CAT << "Connection " << connName << " not found on connection grid.");
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
// Returns a connection, but may not be connected!  Always check for session non-zero
Connection *Lister::ConnectUsingGrid(int row, bool log) {
	ASSERT(connGrid.IsCursor());
	connGrid.SetConnState(row, CONNECTING_START);
	if (log) LogLine("Starting connection...");
	ProcessEvents(); // Necessary to display color change immediately
	WaitCursor wc;
	Connection *c = connectionFactory.Connect(this
		, connGrid.GetConnName(row)
		, connGrid.GetInstanceTypeName(row)
		, connGrid.GetLoginStr(row)
		, connGrid.GetLoginPwd(row)
		, connGrid.GetInstanceAddress(row)
		, connGrid.GetDatabaseName(row)
		, log
		);
		
	UrpString::FromTo(connGrid.GetConnId(row), c->connId);
	connGrid.SetConnState(row, c->enumConnState);
	return c;
}

//==============================================================================================
// Add the script currently in the editor to the script database with a unique id.	
void Lister::AddScriptToHistory() {
	String richTextAsStr = scriptEditor.GetScriptRichTextInStrForm();
	String script = scriptEditor.GetScriptPlainText();

	if (script.IsEmpty()) {
		NotifyUser("Cannot add empty script to script database");
		return;
	}
	
	// A little trick to only insert into a table new scripts
	String controlScript = Format("insert into scripts(scriptrichtext, scriptplaintext) select %s, %s where %s not in(select scriptplaintext from scripts)"
	    // Not sending these as scripts to run, but to insert as text
		, controlConnection->PrepTextDataForSend(richTextAsStr)
		, controlConnection->PrepTextDataForSend(script)
		, controlConnection->PrepTextDataForSend(script)); 

	if (!controlConnection->SendAddDataScript(controlScript)) {
		scriptEditor.Tip(DeQtf(controlConnection->GetLastError()));
		scriptEditor.SetScriptPlainText(controlScript);
		NotifyUser("Failed to add script to script database");
		return;
	}

	// Add to the list if not a dup
	if (controlConnection->GetRowsProcessed() > 0) {		
		int scriptid = controlConnection->GetInsertedId("scripts", "scriptid");
		if (scriptid >= 0) {
			scriptDropDownList.Add(scriptid, script, richTextAsStr);
			scriptEditor.scriptId = scriptid; // Link to script so that it can be uploaded to tests
			scriptEditor.ClearModify();  // Saved, or else we won't be able to attach it to a task
			ToolBarRefresh();
		}
	} else {
		
		// Script is identical to another script, we grab its id.
		String preppedScript = controlConnection->PrepTextDataForSend(script);
		String searchQuery = Script::FindScriptByPlainTextQuery(script);
			
		if (!controlConnection->SendQueryDataScript(searchQuery)) {
			NotifyUser("Error");
			return;
		}
		
		controlConnection->Fetch();
		if (controlConnection->GetRowsProcessed() > 0) {
			scriptEditor.scriptId = controlConnection->Get(0);
			scriptEditor.ClearModify();  // Saved, or else we won't be able to attach it to a task
		} else {
			throw new Exc("Error: Failed to get existing script already found in previous query");
		}
		ToolBarRefresh();
	}
}

//==============================================================================================
// For multi-line scripts, this is the only way to see the full script.
void Lister::SelectedScriptFromDropDown() {
	scriptDropDownList.Tip(scriptDropDownList.Get(1).ToString());
}

//==============================================================================================
void Lister::ProcessSelectedTaskScripts(dword key) {
	
	// Pick off the list of selected script rows (should pick ids)
	Vector<int> selections;
	for (int i = 0; i < scriptGrid.GetCount(); i++) {
		if (scriptGrid.IsSelected(i)) {
			selections.Add(i);
		}
	}

	JobSpec jspec;
	
	if (selections.GetCount() == 0) {
		return;
	} else if (selections.GetCount() == 1) {
		jspec.log = false;
		jspec.batchMode = false;
	} else {
		jspec.log = true;
		jspec.batchMode = true;
	}

	// Holding down the shift key with all the others forces a true full batch run
	jspec.testMode = (In(key, K_ALT_SHIFT_CTRL_F8, K_ALT_SHIFT_F8));
	
	if (jspec.log) {
		ActivateLogging();
		OpenLogWin(); // User can close if they want to, later.
	}
	
	// We assume a task is selected since we are processing task scripts and tasks are one-to-many
	// joined to script grid.
	int taskRow = taskGrid.GetCursor(); 
	String taskName = taskGrid.GetTaskName(taskRow);
	if (jspec.log) {
		LogLine(CAT << "Task: " << taskName);
		LogSep();
	}
	
	for (int i = 0; i < selections.GetCount(); i++) {
		int scriptRow = selections.At(i);
		String why = scriptGrid.GetWhy(scriptRow);
		if (jspec.log) LogGroupHeaderLine(CAT << "Script: " << why);
		
		ProcessTaskScript(scriptRow
		  // In any of the F8 combinations, was the CTRL key held?
		, In(key, K_CTRL_F8, K_ALT_CTRL_F8, K_ALT_SHIFT_CTRL_F8)  /* load into editor? */
		  // In any of the F8 combinations, was the ALT key held?
		, In(key, K_ALT_F8, K_ALT_CTRL_F8, K_ALT_SHIFT_CTRL_F8, K_ALT_SHIFT_F8)   /* Connect and execute? */
		, jspec);
	}

	if (jspec.log) {	
		LogSep();
		LogLine("Completed Task");
		
		DeactivateLogging();
	}
}

//==============================================================================================	
// User pressed Ctrl-F8 to load into Editor (single script selected)
void Lister::ProcessTaskScript(int taskScriptRow, bool loadScript, bool executeScript, JobSpec &jspec) {
	// Labeled to indicate that this is a connection for a script attached to a specific task
	int taskScriptConnId = scriptGrid.GetConnId(taskScriptRow); 
	// We must pass a link to some screen output, always when creating a script object
	Script sob(&mainGrid);
	// Fetch all script attributes as single object to reduce points of attribute listing
	scriptGrid.GetScriptOb(taskScriptRow, &sob);

	// Following function will establish the ActiveConnection *
	SelectConnectionById(taskScriptConnId);
	ToolBarRefresh(); // We changed connection properties, so enable some options for the user
	
	if (loadScript) {
		scriptEditor.SetScript(activeConnection, taskScriptConnId, sob);
		if (jspec.log) scriptEditor.Log();
		targetNameList.SetData(sob.targetName);
		scriptTargetList.SetData(sob.scriptTarget);
		fastFlushTargetList.SetData(sob.fastFlushTarget? "1":"0");
		fldRowLimit.SetData(sob.rowLimit); // Overridden if in batch mode and in test mode
		// TODO:Set scriptTarget type
		// TODO: Set limit rows
		ToolBarRefresh(); // We changed connection properties, so enable some options for the user
	}
	
	if (executeScript) {
		if (jspec.log) LogLine("Executing...");
		// Shifts position of cursor on Connection grid if id found
		if (SelectConnectionById(taskScriptConnId)) {
			int connRow = connGrid.GetCursor();
			// Make a local connection
			LogLine("Connecting...");
			Connection *lconnection = ConnectUsingGrid(connRow, jspec.log);
			LogLine(CAT << "Connection is " << lconnection->connName);
			SetActiveConnection(lconnection); // Updates toolbar too, as well as passing connection to comdScript editor
			if (lconnection && lconnection->session && lconnection->session->IsOpen()) {
				lconnection->ProcessQueryDataScript(sob, jspec);
			} else {
				if (jspec.log) LogLine("Not running script due to connection error");
				if (!lconnection) {
					if (jspec.log) LogLine("No connection set");
				} else if (!lconnection->session) {
					if (jspec.log) LogLine("No session attached to connection");
				} else if (!lconnection->session->IsOpen()) {
					if (jspec.log) LogLine(CAT << "Connection session is not open. Error message:" << lconnection->connectErrorMessage);
				}
			}
		}
		if (jspec.log) LogLine("Completed Executing...");
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
	// 3rd field (2) is RichScript (But how is it stored?? As Value?
	scriptEditor.SetScriptRichTextFromStrForm(scriptDropDownList.Get(2).ToString());
	
	Value scriptIdValue = scriptDropDownList.GetKey();
	
	if (scriptIdValue.IsNull()) {
		Exclamation("Script id is null. Internal error.");
		return;
	}

	if (!UrpString::IsIntegerString(scriptIdValue.ToString())) {
		Exclamation(CAT << "Script id is not an integer. " << scriptIdValue.ToString());
		return;
	}

	UrpString::FromTo(scriptIdValue, scriptEditor.scriptId);
	ToolBarRefresh();
}

//==============================================================================================
// Take the script in the editor and the assigned connection and add to the test grid,
// to allow the user to fill in the test specifications and save.
void Lister::CreateTestFromScript() {
	
	// Force user to establish a connection.  (Too much?)
	if (scriptEditor.connId < 0) {
		Exclamation("No connection attached to the current script editor");
		return;
	}
	
	if (scriptEditor.scriptId < 0) {
		Exclamation("Script Id is negative. Can't save to test.");
		return; // Don't try and save tests with no script #, meaning not in database
	}
	
	testWin.Open();
	testWin.testGrid.AddTest(TrimBoth(scriptEditor.GetScriptPlainText()), scriptEditor.scriptId, scriptEditor.connection->connId);
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
	// Constructs a window that manages its own configuration
	UrpConfigWindow *w = windowFactory.Open(this, "contacts");
	if (w->wasCreatedNew) {
		ContactGrid *g = new ContactGrid();
		g->Load(controlConnection);
		w->AddCtrl(g);
	}
	
	w->OpenWithConfig();
}

//==============================================================================================
// Capture when session->status value changed, usually during a Execute or Fetch.
// We need the Cancel Execution button to only be active when a asynch execution call is away.
void Lister::SessionStatusChanged(const SqlSession& session) {
	ToolBarRefresh();
	ConnectionStatusRefresh();
}

//==============================================================================================
// Select a connection from the grid based on its id, position on its row, and update the 
// active connection and toolbar information.  No connection is automade.
bool Lister::SelectConnectionById(int pconnId) {
	if (pconnId == UNKNOWN) return false; // Don't move connection cursor if no connection set.
	bool found = connGrid.FindConnId(pconnId);
	if (found) {
		SelectedConnection(); // Reuse code to fetch info from grid
		return true;
	}
	
	return false;
}

//==============================================================================================
// The user selected a different connection on the grid. Also call when we select a connection programmatically.
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
	if (!newConnection) return;
	activeConnection = newConnection;
	scriptEditor.connection = activeConnection;
	ActiveConnectionChanged();
}

//==============================================================================================	
// When user selects a task, the script becomes eligible to be attached to it.
void Lister::ActiveTaskChanged() {
	ToolBarRefresh();
}

//==============================================================================================	
// When user selects (WhenLeftClick) a task script, that script is stuffed in editor.
void Lister::ActiveTaskScriptChanged() {
	if (!scriptGrid.IsCursor()) return;
	// When you select tasks, this WhenCursor is triggered, but we don't want to copy a script
	// over unless it was actually selected.
	
	bool hasFocus = scriptGrid.HasFocus();
	bool hasCapture = scriptGrid.HasCapture();
	
	// Activity in the Task Grid triggers cursor movement here, ignore.
	if (!hasFocus) return;	

	// Don't try to bring multiple scripts in (yet)
	if (scriptGrid.GetSelectCount() > 1) return;
	
	// We don't want to lose any changes
	if (scriptEditor.IsModified()) {
		if (!Ok("Script modified. Overwrite?")) {
			return;
		}
	}
	
	int scriptRow = scriptGrid.GetCursor();
	JobSpec js;
	WaitCursor wc; // Some scripts are slow to load, so show an hourglass
	ProcessTaskScript(scriptRow, true /* load */, false /* don't execute */, js);
	scriptGrid.SetFocus(); // Maintain our focus here, don't let scriptEditor capture it
}

//==============================================================================================	
// Update all enabled/disabled buttons based on a change in the users selected active connection.
void Lister::ActiveConnectionChanged() {
	ToolBarRefresh();
	ConnectionStatusRefresh();
	if (!activeConnection) return;
	if (!activeConnection->session) return;
	
	activeConnection->session->WhenDatabaseActivity = THISBACK(SessionStatusChanged);
	scriptEditor.connection = activeConnection;
	scriptEditor.connId = activeConnection->connId; // TODO: Shift all systems to either connection pointer or id
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
// If Shift key held, replace current script	
void Lister::AttachScriptToTask() {
	ASSERT(taskGrid.IsCursor());
	ASSERT(scriptEditor.scriptId >= 0);
	int taskGridRow = taskGrid.GetCursor();
	int taskId = taskGrid.GetTaskId(taskGridRow);
	int scriptId = scriptEditor.scriptId;
	bool replaceCurrentlySelectedTaskScript = false;
	int currentlySelectedTaskScript;
	String taskName = taskGrid.GetTaskName(taskGridRow);
	int relId;
	String title;
	String why; // User must provide a reason for the mapping.

	// User requested to replace current script
	if (GetShift()) {
		if (!scriptGrid.IsCursor()) {
			Exclamation("No script selected");
			return;
		}
		
		if (scriptEditor.scriptId < 0) {
			Exclamation("No script id assned to editor");
			return;
		}
		
		replaceCurrentlySelectedTaskScript = true;
		currentlySelectedTaskScript = scriptGrid.GetCursor();
		title = "Replace script currently selected for task:";
		relId = scriptGrid.GetRelId(currentlySelectedTaskScript);
		why = scriptGrid.GetWhy(currentlySelectedTaskScript); // Display the current why so user can edit
		
	} else {
		title = "New Task-Script Attachment to task:";
	}
	
	title << taskName;
	
	if (!UrpInputBox(
			why
		,	title
		,	CAT << "Enter a reason for the attachment, how the script will help close out the task. " 
		<<	"(taskId=" << taskId << ", scriptId=" << scriptEditor.scriptId)
		) 
	{
		return;
	}
	
	why = TrimBoth(why);
	if (why.IsEmpty() || why.GetLength() < 10) {
		Exclamation("Must provide a reason for attachment so it makes sense later (when you forget)");
		return;
	}
	
	String scriptMap;
	String updateMap;
	
	// Construct mappings for building the script from tokens
	
	if (replaceCurrentlySelectedTaskScript) {
		scriptMap = "update relations set "
							   "  fromid           = %d"
							   ", toid             = %d"
							   ", fromtbid         = %d"
							   ", totbid           = %d"
							   ", why              = %s"
						       ", connid           = %d"
							   ", targetname       = '%s'"
						       ", scripttarget     = %d"
						       ", fastflushtarget  = '%s'"
						       ", rowlimit         = %d"  
						;	   
		updateMap = " where relid = %d";
	} else {
		scriptMap = "insert into relations("
									    "fromid"
									    ",  toid"
									    ",      fromtbid"
									    ",          totbid"
									    ",              why"
									    ",                    connid"
									    ",                        targetname"
									    ",                              scripttarget"
									    ",                                  fastflushtarget"
									    ",                                        rowlimit"
									    ")"
		                       " values(%d, %d, %d, %d, %s, %d, '%s', %d, '%s', %d)"
		                       ;
	}
	
	String script = Format(scriptMap
		                       ,        taskId
		                       ,            scriptEditor.scriptId
		                       ,                OB_TASKS
		                       ,                    OB_SCRIPTS
		                       ,                        controlConnection->PrepTextDataForSend(why)
		                       ,                              scriptEditor.connId
		                       ,                                  targetNameList.GetData()
		                       ,                                        scriptTargetList.GetData()
		                       ,                                            fastFlushTargetList.GetData()
		                       ,											      GetFieldInt(fldRowLimit)
		                       );
		         
	if (!updateMap.IsEmpty()) {
		String updateScript = Format(updateMap, relId);
		script.Cat(updateScript);
	}
		
	if (controlConnection->SendAddDataScript(script)) {
		// Update script grid filter (joined to task table), display just added script.
		scriptGrid.ReQuery();
		scriptEditor.ClearModify(); // Script is no longer unattached
	}
}

//==============================================================================================	
void Lister::DeployLister() {
	String targetProdPath = "C:/Program Files/lister/lister.exe";
	String targetConfigDir = GetHomeDirectory();
	if (!FileCopy(exeFilePath, targetProdPath)) {
		Exclamation(CAT << "Unable to deploy from " << exeFilePath << " to " << targetProdPath);
		return;
	}
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
	bar.Add(!scriptEditor.GetScriptPlainText().IsEmpty(), "File", CtrlImg::smalldown(), THISBACK(AddScriptToHistory)).Tip("Memorize Script");
	
	// Create Test From Script if we have an id
	bar.Add( // Only allow test creation if there is a script, a scriptid, and a connection
		(!scriptEditor.GetScriptPlainText().IsEmpty()
		 && scriptEditor.connection
		 && scriptEditor.scriptId >= 0), "File", MyImages::addtotest16(), 
		 THISBACK(CreateTestFromScript)).Tip("Create a Test around this Script");

	// Browse existing Tests
	bar.Add( true, "", MyImages::browsetests16(), 
		 THISBACK(BrowseTests)).Tip("Browse and edit tests");
		 
	// Execute script against current connection
	bar.Add( // Only allow execution if there is a script and a connection
		(!scriptEditor.GetScriptPlainText().IsEmpty() 
		 && scriptEditor.connection), "File", MyImages::runtoscreen16(), 
		 THISBACK(RunScriptOutputToScreen)).Tip("Execute Script and output to a grid on the screen");

	// Execute script and pass output to physical table in control db.
	bar.Add( // Only allow execution if there is a script and a connection
		(!scriptEditor.GetScriptPlainText().IsEmpty() 
		 && scriptEditor.connection), "File", MyImages::runtotable16(), 
		 THISBACK(RunScriptOutputToTable)).Tip("Execute Script and create a table in the control database");
	        
	// Cancel a running Script
	bar.Add(
		(!scriptEditor.GetScriptPlainText().IsEmpty() 
		 && scriptEditor.connection
		 && activeConnection 
		 && activeConnection->session 
		 && In(activeConnection->session->GetStatus(), activeConnection->session->START_EXECUTING, activeConnection->session->START_FETCHING)
	 	), "File", MyImages::cancelop16(), 
		 THISBACK(CancelRunningScriptOnActiveConn)).Tip("Cancel executing script on active connection");

	scriptEditor.FindReplaceTool(bar);
	
/*
	// Load list of database users/schemas from the connection
	bar.Add(
		(scriptEditor.connection), "ListUsers", MyImages::users16(), 
		 THISBACK(ListUsers)).Tip("List users/schemas for current connection");

	// Actual dropdown list, does nothing
	bar.Add(userList, 100);
*/

	// Quirky popup for list of contacts
	bar.Add(
		true, "ListContacts", MyImages::contacts16(), 
		 THISBACK(ListContacts)).Tip("Browse and edit contact details");

	// Deploy application to Program Files folder
	bar.Add(true, "Deploy", MyImages::deploylister16(), 
		 THISBACK(DeployLister)).Tip("Deploy lister to program files");

	// Attach this Script to the selected Task
	bar.Add(
		!scriptEditor.GetScriptPlainText().IsEmpty() && scriptEditor.scriptId >= 0
		&& taskGrid.IsCursor()
		, "Attach", MyImages::attachtotask16(),
		 THISBACK(AttachScriptToTask)).Tip(
		    // If the shift key is held down, attachment replaces the current script selected
		 	GetShift()? "Update selected script currently assigned to selected task" : "Add script to selected task");

	// Set the target table name
	bar.Add(targetNameList, 150); //.Tip("Target object name (if targeting a table or spreadsheet)");
	
	bar.Add(scriptTargetList, 85); //.Tip("What type of target does the script output to?");
	
	bar.Add(fastFlushTargetList, 75); //.Tip("Truncate the target or leave as is?");
	
	bar.Add(fldRowLimit, 65);
	
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
void Lister::ScriptExecutionHandler(ScriptTarget pscriptTarget) {
	String lplainText = scriptEditor.GetScriptPlainText();

	if (lplainText.IsEmpty()) {
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

	bool executingselection = false;
	// If a piece of text is selected, only execute that
	if (scriptEditor.IsSelection())	{
		lplainText = scriptEditor.GetSelection().GetPlainText().ToString();
		executingselection = true;
		// TODO: Enhance MyRichEdit to support multiple selections; very useful
	}

		
	JobSpec jspec; // Take defaults for now
	jspec.executingSelection = executingselection;  // Track that only a portion was executed, so we don't skew stats on the script
	// TODO: Move call to cursorHandler to Connection
	CursorHandler cursorHandler(controlConnection, activeConnection);
	Script sob(
			pscriptTarget
		,	(fastFlushTargetList.GetData() == "1")
		,	&mainGrid
		,	scriptEditor.scriptId
		,	lplainText
		,	scriptEditor.GetScriptRichText()
		,	GetFieldInt(fldRowLimit)
		,	targetNameList.GetData()
	);
	bool ran = cursorHandler.Run(sob, jspec);
}
                            
//==============================================================================================
// Execute the script in the script editor against the active connection and push output to a
// table in the local database.
void Lister::RunScriptOutputToTable() {
	ScriptExecutionHandler(SO_TABLE /* load into table, leave grid empty */);
}

//==============================================================================================
// Execute the script in the script editor against the active connection	
void Lister::RunScriptOutputToScreen() {
	ScriptExecutionHandler(SO_SCREEN /* Don't load into table, load into grid */);
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
		case K_CTRL_F8:
		case K_ALT_CTRL_F8:
		case K_ALT_F8:
		
			// Determine what has focus
			if (scriptGrid.HasFocus()) {
				if (scriptGrid.IsCursor()) {
					// Pass key information so control keys can be detected and actions applied to each selected script executed.
					ProcessSelectedTaskScripts(key);
				}
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
		("win2", *GetLogWin());
		("edit1", scriptEditor) // Generically, the editor screen.
	;
	
	UrpWindow::Xmlize(xml); // Make sure and save our window position!
}
