/***********************************************************************************************
*  lister - lister.cpp
*  
*  Main Window
*  
*  This GUI screen is the entry point for the user into the application. Called from main.cpp.
*
*  The current perspective supports 6 panes, with a 7th currently empty.
*  The screen is divided top to bottom into 2 halfs. The top half is divided into 4 panes. The
*  bottom half is divided into 3, with the bottom-left pane currently undefined.
*
*  The top-left pane is a list of connections created and maintained by the user.  By clicking
*  on the button to the right of each connection line, the user can open that connection.
*  Asynchronously, the app will try to connect to server instance defined in the connection
*  details.  Upon success or failure, an icon to the left of the connection line is colored
*  green or red accordingly.
*  The mid-left pane is a list of tasks created and maintained by the user.  Each task can
*  be edited in place, or it can be opened into a task editor upon double-clicking.
*  The mid-right pane is the list of scripts attached and owned by the currently selected task.
*  It changes as tasks are selected.  Each script can have a connection assigned to it, and 
*  when a script is selected, its connection in the left pane will be highlighted (but not connected
*  to).  Each script can either target a database table or the screen grid.  When a script is
*  selected, all the macros attached to its parent task are constructed based on the connection
*  assigned to the selected script.  Also, upon selection, the text of the script is loaded
*  into the right-hand script editor window pane.  If you are editing a script already, the
*  app will ask if its ok to overwrite it.
*  If the connection for this script is open (green) or an open connection is selected in the 
*  Connection pane, a button on the toolbar over the script text will be available to run the
*  script.  It always runs to the screen from here.  It attempts to run asynchronously.

*  On the bottom half of the screen, the middle pane is where the script output is drawn as
*  a grid of rows and columns.  The pane to the right of the output will list execution time,
*  start/stop time, status of the run, rows and columns count.
*
*  Over each pane there is usually a context menu with options specific to that pane.
*  
*  Author: Jeff Humphreys
*  
*  TODO: Move all grid-specific code to their specific grid code files.
*  Move key handlers that are isolated to one pane to that pane's code file.
*  Improve load time.
*  Add support for pane movement, like perspectives in Eclipse.
*  Allow each pane to temporarily zoom.
*  Create method to toggle between output grid and script.
*  Convert all grids to UrpGrid for search filtering.
*  Add tabs for output grids and script text.  How to manage?
*
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

#include "lister.h"
#include "Script.h"
#include "ContactGrid.h"
#include "OutputSpec.h"

#include <lister/Urp/UrpMain.h>
#include <lister/Urp/UrpInput.h> // Has Ok() in it
#include <lister/Urp/UrpConfigWindow.h>

#include "OutputStat.h" // Our panel
#include "MacroHandler.h"

#include "KeyHandler.h"
#include "JobSpec.h"
#include "Task.h"

#include "image_shared.h"
#include <Draw/iml_header.h>

#include "image_shared.h"
// Only can occur in one object file, so I picked the main app file
#include <Draw/iml_source.h>

#define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>
// Only can occur in one object file, so I picked the main app file
#include "Sql/sch_source.h"

// see shared.h for the extern ref
UrpWindowFactory  *windowFactory = NULL;
ConnectionFactory *connectionFactory = NULL;

//==============================================================================================
// Construct all objects that have to be concretely referenced (for now)
// Load all grids.  TODO: Move loading to lazy style.
Lister::Lister() {
	// Capture the dev folder path in mainFilePath
	Init(__FILE__);
	
	windowFactory = new UrpWindowFactory();
	connectionFactory = new ConnectionFactory();
	
	CtrlLayout(*this, "Lister - A SQL Connection and Execution Tool");
	showHiddenTasks = false;
	activeConnection = NULL;
	activeDropGrid = NULL;
	
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

	// Menu
	
	AddFrame(mainMenu);
	mainMenu.Set(THISBACK(MainMenu));
	mainMenu.WhenHelp = statusbar; // Set Help() values on all menu and toolbars

	// Status bar to show help messages
	
	AddFrame(statusbar);
		
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
	
	bottomMidPane.Add(mainGrid);
	mainGrid.SizePos();
	
	// Connect to our metadata control database using raw params

	if ((controlConnection 
		= connectionFactory->Connect(
				this, CONTROL_CONN_NAME, ConnectionFactory::ControlInstanceType(), "postgres"
				, "postgres", "localhost", "postgres"))->enumConnState 
				!= CON_SUCCEED) 
	{
		Exclamation(CAT << "Cannot connect to control, " << controlConnection->connectErrorMessage);
		exit(-1);
	}

	// Set the dialect manually so that GetText functions work and do not corrupt return SQL 
	// from SqlSelect, SqlInsert, SqlUpdate.
	//controlConnection->session->Dialect(PGSQL);
	
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

	//String s = Script::GetScriptListQuery();

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
	topLeftPane.Add(connGrid);
	connGrid.Build(controlConnection);
	connGrid.Load(); // Grid can create connections
	
	connGrid.SizePos();
	connGrid.WhenCursor      = THISBACK(SelectedConnection); // Call be when user selects a different connection in grid
	connGrid.WhenCtrlsAction = THISBACK(ClickedConnect);

	// Construct Task Grid
	
	taskGrid.Description("taskGrid");
	topMidLeftPane.Add(taskGrid);
	taskGrid.Build();
	taskGrid.Load(controlConnection);
	taskGrid.SizePos();
	taskGrid.WhenNeedLastInsertedPkId = THISBACK(GetTaskLastInsertedPkId);
	
	// Main sets callback so that the scripting toolbar state can be updated to reflect that 
	// a script can be assigned to this task.

	taskGrid.WhenCursor     = THISBACK(ActiveTaskChanged);
	taskGrid.WhenBar        = THISBACK(TaskGridContextMenu);
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
	
	// Since the activeConnection is part of this thread (window), the testWin shouldn't just
	// take a copy of what "was" the  active connection, since it could change, so we pass
	// the address of the activeConnection.
	testWin.Build(controlConnection, &activeConnection, &activeContextMacros);
	testWin.WhenToolBarNeedsUpdating = THISBACK(ToolBarRefresh);
	
	macrosAvailableList.AddColumn("Search For");
	macrosAvailableList.AddColumn("Replace With");
	macrosAvailableList.AddColumn("Expansion");
	macrosAvailableList.SetValueColumn(2);
	macrosAvailableList.AddValueColumn(0).AddValueColumn(1).AddValueColumn(2);
	macrosAvailableList.GetList().WhenLeftClick = THISBACK(SelectedAvailableMacro);  // Not sure which "When" this is.
	
	macrosAvailableList.Width(450);

	// By spinning this off as a callback, we get the screen displayed while autoconnecting, and plus the cursor on conn grid is properly set to center
	//SetTimeCallback(100, THISBACK(AutoConnect));		
}

//==============================================================================================	
Lister::~Lister() {
	delete windowFactory;
	delete connectionFactory;
}

//==============================================================================================	
void Lister::ViewMappings() {
	// Constructs a window that manages its own configuration
//	UrpConfigWindow *w = windowFactory.Open(this, "mappings");
//	if (w->wasCreatedNew) {
//		MappingGrid *g = new MappingGrid();
//		g->Load(controlConnection);
//		w->AddCtrl(g);
//	}
//	
//	w->OpenWithConfig();
}

//==============================================================================================	
void Lister::ExpandScript() {
	String msg = Format("[* \1%s\1].", ExpandMacros(scriptEditor.GetScriptPlainText(), &activeContextMacros));
	Exclamation(msg);
}

//==============================================================================================	
void Lister::ToggleHiddenTasks() {
	showHiddenTasks = !showHiddenTasks;
	taskGrid.ReQuery(showHiddenTasks);
}

//==============================================================================================	
// Hack to view Excel's output HTML raw so I can snag the special codes that tell that a number is Text, etc
void Lister::ViewClipboardHTML() {
	// CF_HTML not part of U++ as a numeric, so we'll register it.
	RegisterClipboardFormat("HTML Format");
	String s = ReadClipboard("HTML Format");
	
	Exclamation(Format("[* \1%s\1].", s));
}

//==============================================================================================	
void Lister::FileMenu(Bar& bar) {
	bar.Add("Mappings", THISBACK(ViewMappings))
	.Help("List mappings between source columns, staging columns and target columns, with ETL descriptors"
	      " and notes on usage, links to tests for columns");
	bar.Add("Expand Script", THISBACK(ExpandScript))
	.Help("Display the transformed result the current script text in the script editor, converting all macros into their"
		  " correct text based on the currently selected task and script. The script editor text is not changed.  Will not work"
		  " if a task/script has not been selected");
	bar.Add("Show Hidden Tasks", THISBACK(ToggleHiddenTasks)).Check(showHiddenTasks)
	.Help("If checked, reload the task grid with any tasks marked as hidden. Unchecked shows all the unhidden tasks,"
	      " the default state."
		  " This is for if you hid something and forgot you still need it");
	bar.Add("View Clipboard as CF_HTML", THISBACK(ViewClipboardHTML))
	.Help("Take the text currently on the keyboard and assume it is CF_HTML type, instead of the usual assumption"
		  " that it is CF_TEXT.  This will display any data copied from Excel worksheets as xml tags that you can"
		  " copy and determine how to hack and make your own Excel formats");
	bar.Add("Export Database Structure", THISBACK(ExtractDatabase))
	.Help("Generate all the SQL DDL files for tables, views, and functions in the control database and write directly"
		  " to the dev database SVN folder.  Unchanged files will not get flagged by SVN even"
		  " though the modify timestamp will change");
}

//==============================================================================================	
// All the options I can't fit anywhere else
void Lister::MainMenu(Bar& bar) {
	bar.Add("File", THISBACK(FileMenu))
	.Help("Just a mish-mash of functions I needed to put somewhere");
}

//==============================================================================================	
// We'll insert it into the script with brackets
void Lister::SelectedAvailableMacro() {
	String newMac;
	if (macrosAvailableList.GetList().GetCurrentRow() == -1) return;
	int row = macrosAvailableList.GetList().GetCurrentRow();
	String macro = macrosAvailableList.GetList().Get(row, 0).ToString();
	// TODO: Get tip or notes from grid and display as label in input box
	
	if (macro.Find("{n}") != -1) {
		String number;
		if (UrpInputBox(number, "Enter a number", "Complete macro definition")) {
			macro = UrpString::ReplaceInWhatWith(macro, "{n}", number);
		}
	}
	
	newMac << "[[" << macro << "]]";
	scriptEditor.PasteText(newMac);
	scriptEditor.SetModify(); // For some reason, pasting text does not modify the state
	macrosAvailableList.Close(); // Raise the draw bridge.  Should be PickUp as opposite of Drop, or RaiseUp, RollUp?
}

//==============================================================================================	
// Task Grid Options (context menu)
void Lister::TaskGridContextMenu(Bar &bar) {
	bar.Add("Edit Task Detail", THISBACK(OpenTaskDefWin));
	bar.Add("Hide", THISBACK(HideSelectedTasks));
	bar.Add("Copy Macros From Another Task", THISBACK(CopyMacrosFromTask));
	taskGrid.StdBar(bar); // Pickup standards
}

//==============================================================================================	
// Only callable from dynamic DropGrid created in CopyMacrosFromTask when item is clicked.
void Lister::SelectedTaskToCopyMacrosFrom() {
	ASSERT_(activeDropGrid, "No active drop grid is set, not a proper callback");
	
	
	// Copy over macs.
	
	int selectedTaskId = activeDropGrid->GetKey(); // Assume a row was selected
	int targetedTaskId = taskGrid.GetTaskId();
	
	if (!controlConnection->SendQueryDataScript(Format("SELECT taskMacId, searchFor, ReplaceWith, ProcessOrder, Note FROM TaskMacros WHERE taskId = %d", selectedTaskId))) {
		Exclamation("Error trying find the task to copy macros from");
		return;
	}
	
	if (controlConnection->GetRowsProcessed() == 0) {
		Exclamation("Nothing to copy");
		return;
	}
	
	// For now, we ASS-U-ME that the target is clean.
	// If SearchFor value exists, it will choke (I think)
	
	int macrosInserted = 0;
	
	// Create a separate connection so as not to break our fetch cursor (Will destruct)
	Connection insertConn(controlConnection);
	
	while (controlConnection->Fetch()) {
		int fromTaskMacroId = controlConnection->Get("TASKMACID");
		String script = Format("INSERT INTO TaskMacros(taskId, searchFor, replaceWith, processOrder, note, copiedFromTaskMacroId)"
		                       " VALUES(%s, %s, %s, %s, %s, %s)"
		                       , ToSQL(targetedTaskId)
		                       , ToSQL(controlConnection->Get("SEARCHFOR"))
		                       , ToSQL(controlConnection->Get("REPLACEWITH"))
		                       , ToSQL(controlConnection->Get("PROCESSORDER")) // May conflict with existing macros for this task
		                       , ToSQL(controlConnection->Get("NOTE"))
		                       , ToSQL(controlConnection->Get("TASKMACID"))
		                       );
		int toTaskMacroId = UNKNOWN;
		
		if (insertConn.SendQueryDataScript(script, NULL, true/*silent*/)) { // Don't care about dups
			macrosInserted++;
			toTaskMacroId = insertConn.GetInsertedId("taskmacros", "taskmacid");

			// Note that task drivers are not cloned, but shared.  Changes to one will affect the other(!)
			
		} else {
			// Need to fetch if it was a collide on dup searchFor
			String searchFor = controlConnection->Get("SEARCHFOR");
			script = Format("SELECT taskMacId FROM TaskMacros WHERE taskId = %s AND searchFor = %s"
		                       , ToSQL(targetedTaskId)
		                       , ToSQL(controlConnection->Get("SEARCHFOR"))
		                       );
			
			if (insertConn.SendQueryDataScript(script)) {
				int rowsProcessed = insertConn.GetRowsProcessed();
				ASSERT(insertConn.GetRowsProcessed() == 1);
				insertConn.Fetch();
				toTaskMacroId = insertConn.Get(0);
				
				// Update other attributes?
			} else {
				continue; // Can't get a key in place, skip to next
			}
			
		}

		// Then we merge the replacements with the existing one, or else we can't do updates
		
		script = Format("INSERT INTO TaskMacroDriverReplacement(taskMacroId,  ReplaceWith, note, taskDriverId)"
		                       " SELECT %1$d, a.ReplaceWith, a.note, a.taskDriverId"
		                       " FROM TaskMacroDriverReplacement a"
		                       " WHERE a.taskMacroId = %2$d"
		                       // Avoid collisions if this target macro already exists with some replacements
		                       " AND (%1$d, a.ReplaceWith) NOT IN (SELECT taskMacroId, ReplaceWith)"
		                       , toTaskMacroId, fromTaskMacroId)
		;

		if (!insertConn.SendQueryDataScript(script)) {
			// Update ReplaceWith value
		}
	}
	
	Exclamation(Format("Cloned %d macros into this task", macrosInserted));
}

//==============================================================================================	
void Lister::CopyMacrosFromTask() {
	// Constructs a window that manages its own configuration
	String script = "SELECT taskId, taskName FROM Tasks WHERE Hidden IS NULL OR Hidden = false ORDER BY taskName";
	
	if (!controlConnection->SendQueryDataScript(script)) {
		Exclamation("Failed to fetch list of tasks");
		return;
	}
	
	UrpConfigWindow *w = windowFactory->Open(this, "copymacrosfromtask");
	if (w->wasCreatedNew) {
		w->Title("Select task to copy macros from");
		//w->Icon(MyImages::contacts16());
		
		DropGrid *g = new DropGrid();
		activeDropGrid = g;
		Button *b = new Button();
		b->SetLabel("Ok");
		g->SetKeyColumn(0);
		g->SetValueColumn(1);
		b->WhenPush = THISBACK(SelectedTaskToCopyMacrosFrom);
		while (controlConnection->Fetch()) {
			g->Add(controlConnection->Get(0), controlConnection->Get(1));
		}
		
		w->AddCtrl(g);
		
		g->LeftPosZ(2, 260).TopPosZ(4, 20); // Size it or it will take over the screen.
		w->AddCtrl(b);
		b->LeftPosZ(30, 50).TopPosZ(30, 20);
	}
	
	w->OpenWithConfig();
	
}

//==============================================================================================	
void Lister::HideSelectedTasks() {
	Vector<int> tasksToHide;
	if (taskGrid.IsSelection()) {
		int lastSelectedTask = -1;
		
		for (int i = taskGrid.GetCount() - 1; i >= 0; i--) {
			if (taskGrid.IsSelected(i)) {
				int taskId = taskGrid.GetTaskId(i);
				tasksToHide.Add(taskId);
				taskGrid.Remove(i);
				lastSelectedTask = i; // Save place so we can reposition cursor on same physical location on grid (different task)
			}
		}

		if (lastSelectedTask != -1) {
			taskGrid.Select(lastSelectedTask); // Will trigger ActiveTaskChanged()
		}
	}

	if (tasksToHide.GetCount() == 0) return;
	
	String controlScript;
	
	controlScript = "update tasks set hidden = true where taskid in(";
	for (int i=0; i < tasksToHide.GetCount(); i++) {
		if (i) controlScript << ",";
		controlScript << tasksToHide.At(i);
	}
	controlScript << ")";
	
	controlConnection->SendChangeDataScript(controlScript);
	
	
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
	Connection *c = connectionFactory->Connect(this
		, connGrid.GetConnName         (row)
		, connGrid.GetInstanceTypeName (row)
		, connGrid.GetLoginStr         (row)
		, connGrid.GetLoginPwd         (row)
		, connGrid.GetInstanceAddress  (row)
		, connGrid.GetDatabaseName     (row)
		, connGrid.GetPortNo           (row)
		, connGrid.GetOSAuth           (row)
		, log
		);
		
	UrpString::FromTo(connGrid.GetConnId(row), c->connId);
	connGrid.SetConnState(row, c->enumConnState);
	return c;
}

//==============================================================================================
// Save script in editor to its current script id, don't make a new script record.  This will
// cut down on wasted new records.  Plus easy to keep modifying.
void Lister::SaveScript() {

	String richTextAsStr = scriptEditor.GetScriptRichTextInStrForm();
	String script = scriptEditor.GetScriptPlainText();

	if (script.IsEmpty()) {
		NotifyUser("No point adding an empty script");
		return;
	}

	String controlScript;

	// If the scriptId is -1, then it probably is an edited script.
	if (scriptEditor.scriptId == UNKNOWN && scriptEditor.originalScriptId == UNKNOWN) {
		AddScriptToHistory();
		// It's new, so we just automagically attach it!  (See if this works, may never be called)
		AttachScriptToTask();
	} else {
		int scriptId;
		if (scriptEditor.scriptId != UNKNOWN) {
			scriptId = scriptEditor.scriptId;
		} else if(scriptEditor.originalScriptId != UNKNOWN) {
			scriptId = scriptEditor.originalScriptId;
		} else {
			throw new Exc("Error: Code error on scriptEditor.scriptId expected value");
		}
		
		controlScript = Format("update scripts set scriptrichtext = %s, scriptplaintext = %s where scriptid = %d"
		    // Not sending these as scripts to run, but to update as text
			, controlConnection->PrepTextDataForSend(richTextAsStr)
			, controlConnection->PrepTextDataForSend(script)
			, scriptId
		); 

		if (!controlConnection->SendChangeDataScript(controlScript)) {
			scriptEditor.Tip(DeQtf(controlConnection->GetLastError()));
			scriptEditor.SetScriptPlainText(controlScript);
			NotifyUser("Failed to update script to script database");
			return;
		}
		
		// For now, we are just updating the script where ever script id is found in task script grid.
		// Later we will have to update the specifics to the specific taskscript.
		scriptGrid.UpdateTaskScriptGrid(scriptId, script);
		scriptEditor.scriptId = scriptId;
		Speak(EVS_SAVE_SUCCEEDED);
		scriptEditor.ClearModify();  // Saved, or else we won't be able to attach it to a task
		ToolBarRefresh();
	}
	
	
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
	
	String controlScript;
	bool massiveScript = false;
	
	// Postgres will only support an in clause if script is around 9000 or less due to a temporary index creation
	if (script.GetLength() > 9000) {
		massiveScript = true;
	}
	
	if (massiveScript) {

		// A little trick to only insert into a table new scripts
		controlScript = Format("insert into scripts(scriptrichtext, scriptplaintext) select %s, %s where %s not in(select scriptplaintext from scripts)"
		    // Not sending these as scripts to run, but to insert as text
			, controlConnection->PrepTextDataForSend(richTextAsStr)
			, controlConnection->PrepTextDataForSend(script)
			, controlConnection->PrepTextDataForSend(script)
		); 
	} else {
		controlScript = Format("insert into scripts(scriptrichtext, scriptplaintext) values(%s, %s)"
		    // Not sending these as scripts to run, but to insert as text
			, controlConnection->PrepTextDataForSend(richTextAsStr)
			, controlConnection->PrepTextDataForSend(script)
		); 
	}
	
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
		
		if (massiveScript) {
			NotifyUser("Failed to add script to script database");
			return;
		}
		
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
// Called from Key() handler, with key code.
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
		// Copy script text to screen editor window (using Rich Text)
		scriptEditor.SetScript(activeConnection, taskScriptConnId, sob);
		if (jspec.log) scriptEditor.Log(); // For batches, the log window is running
		// Update screen dropdowns with db info
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
			LogLine(CAT << "Connection is " << lconnection->connName << ", " 
				<< (lconnection->connName != lconnection->instanceAddress ? lconnection->instanceAddress : "")
				<< ", " << lconnection->loginStr
				<< (lconnection->dbName.IsEmpty()? "" : lconnection->dbName)
				<< (lconnection->instanceTypeName)
			); // TODO: Add port for sybase
			SetActiveConnection(lconnection); // Updates toolbar too, as well as passing connection to comdScript editor
			if (lconnection && lconnection->session && lconnection->session->IsOpen()) {
				lconnection->ProcessQueryDataScript(sob, jspec, &activeContextMacros);
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
	// We must assign a task Id or the test grid will be unfiltered. (Already built)
	int taskId = taskGrid.GetTaskId();
	testWin.SetTaskId(taskGrid.GetTaskId());
	// Set task driver after we set task
	int taskDriverId = taskGrid.GetTaskDriverId();
	testWin.SetActiveTaskDriverId(taskGrid.GetTaskDriverId());
	
	testWin.testGrid.Load(); // It will filter by taskId if its set
	testWin.Open(); 
	// async
	ToolBarRefresh();
	
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
	UrpConfigWindow *w = windowFactory->Open(this, "contacts");
	if (w->wasCreatedNew) {
		w->Icon(MyImages::contacts16());
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
	Connection *selectedConnection = connectionFactory->GetConnection(connName);
	
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
	activeContextMacros.rebuildTaskMacros = true; // Set flag so that when they eventually select a script, they need to load up task macros
	activeContextMacros.taskId = taskGrid.GetTaskId();
	
	// We'll rebuild the macros based on the first script to the left, if there

	int connId = UNKNOWN;
	
	if (scriptGrid.GetCount()) {
		if (!IsNull(scriptGrid.GetConnId(0)) && scriptGrid.GetConnId(0) != UNKNOWN) {
			connId = scriptGrid.GetConnId(0);
		} else {
			connId = UNKNOWN; // Let RebuildMacros use the active connection
		}
		
		activeContextMacros.RebuildMacros(connId, taskGrid.GetTaskDriverId(), &activeContextMacros, controlConnection, activeConnection, &macrosAvailableList);
	}
}

//==============================================================================================	
// When user selects (WhenLeftClick) a task script, that script is stuffed in editor.
void Lister::ActiveTaskScriptChanged() {
	int row = -1;
	bool hasFocus = scriptGrid.HasFocus(); // Capture early
	bool hasCapture = scriptGrid.HasCapture();

	if (!scriptGrid.IsCursor()) {
		if (scriptGrid.IsSelection()) {
			row = scriptGrid.GetFirstSelection();
		} else {
			return;
		}
	} else {
		row = scriptGrid.GetCursor();
	}
	
	// When you select tasks, this WhenCursor is triggered, but we don't want to copy a script
	// over unless it was actually selected.

	// Set connection-based macros first
	// Nice ENV macro to allow code reuse across environments where there are stereotypical differences between them
	// Use task scripts assigned connection whether its active or not

	int connId = UNKNOWN;
	
	if (!IsNull(scriptGrid.GetConnId(row)) && scriptGrid.GetConnId(row) != UNKNOWN) {
		connId = scriptGrid.GetConnId(row);
	} else {
		connId = UNKNOWN; // Let RebuildMacros use the active connection
	}

	// We pass activeMacros to itself for future case when we need alternate macro control sets (It's gonna get hairy)	
	activeContextMacros.RebuildMacros(connId, taskGrid.GetTaskDriverId(), &activeContextMacros, controlConnection, activeConnection, &macrosAvailableList);
	
	ToolBarRefresh();
	
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
	
	JobSpec js;
	WaitCursor wc; // Some scripts are slow to load, so show an hourglass
	ProcessTaskScript(row, true /* load */, false /* don't execute */, js);
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
	String envName;
	
	switch (envCode) {
	case ENV_DEV: envName = "Dev"; break;
	case ENV_PROD: envName = "Release"; break;
	default: envName = "?";
	}
	
	String envLabel = ", Env=";
	envLabel << envName;
	
	if (activeConnection) {
		if (!In(activeConnection->enumConnState, CON_SUCCEED, CON_STALE)) {
			Title(CAT << APP_TITLE << " - Not connected to " << activeConnection->connName << envLabel);
		} else {
			Title(CAT << APP_TITLE << " - Connected to " << activeConnection->connName << envLabel);
		}
	} else {
		Title(CAT << APP_TITLE << " - Not connected" << envLabel);
	}
}

//==============================================================================================	
void Lister::CancelRunningScriptOnActiveConn() {
	activeConnection->Cancel(); // Sends OCICancel (Need reset?)
}

//==============================================================================================
// If Shift key held, replace current script with editor script, otherwise add as new script for task	
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
	int processorder;

	// Whether new or replacement, we want to use the current selection 
	if (scriptGrid.IsCursor()) {
		currentlySelectedTaskScript = scriptGrid.GetCursor();
	// If we don't have focus, IsCursor is not set.
	} else if (scriptGrid.IsSelection()) {
		currentlySelectedTaskScript = scriptGrid.GetFirstSelection();
	} else {
		currentlySelectedTaskScript = UNKNOWN;
	}
	
	if (currentlySelectedTaskScript != UNKNOWN) {
		why = scriptGrid.GetWhy(currentlySelectedTaskScript); // Display the current why so user can edit
		processorder = scriptGrid.GetProcessOrder(currentlySelectedTaskScript);
		relId = scriptGrid.GetRelId(currentlySelectedTaskScript);
	}
	
	// User requested to replace current script
	if (GetShift()) {
		// Allow for user to have left the script grid at some time and still want to
		// update the visible script selected
		if (currentlySelectedTaskScript == UNKNOWN) {
			Exclamation("No script selected");
			return;
		}
		
		if (scriptEditor.scriptId < 0) {
			Exclamation("No script id assned to editor");
			return;
		}

		if (scriptGrid.SelectionCount() > 1) {
			Exclamation("Can't replace multiple scripts with the same script");
			return;
		}
		
		replaceCurrentlySelectedTaskScript = true;
		
		title = "Replace script currently selected for task:";
		
	// No shift key, so we are a new script attachment
	} else {
		title = "New Task-Script Attachment to task:";
		processorder = scriptGrid.GetMaxProcessOrder() + 10; // Skip a few like we did in BASIC
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
	
	// Convert nulls (no drop down) to false since insert function will choke
	Value isfastFlushTargetList = IfNull(fastFlushTargetList.GetData(), "0");
	
//	if (isfastFlushTargetList.IsNull()) {
//		isfastFlushTargetList = "0"; // A string must be passed to the driver; 0 = false
//	}

	// If user does not select a value, enforce one.
	Value scriptTargetListSelection = scriptTargetList.GetData();
	if (scriptTargetListSelection.IsNull()) {
		scriptTargetListSelection = Script::SO_UNDEF;
	}
	
	why = TrimBoth(why);
	
	// Force user to enter a semi-meaningful explanation for this script
	if (why.IsEmpty() || why.GetLength() < 10) {
		Exclamation("Must provide a reason for attachment so it makes sense later (when you forget)");
		return;
	}
	
	String scriptMap;
	String updateMap;
	
	// Construct mappings for building the script from tokens
	
	if (replaceCurrentlySelectedTaskScript) {
		scriptMap = "update relations set "
							   "  fromid           =  %d "
							   ", toid             =  %d "
							   ", fromtbid         =  %d "
							   ", totbid           =  %d "
							   ", why              =  %s "
						       ", connid           =  %d "
							   ", targetname       = '%s'"
						       ", scripttarget     =  %d "
						       ", fastflushtarget  = '%s'"
						       ", rowlimit         =  %d "  
						       ", processorder     =  %d "
						;	   
		updateMap = " where relid = %d";
	} else {
		scriptMap = "insert into relations("
									    "fromid"
									    ",  toid"
									    ",      fromtbid"
									    ",          totbid"
									    ",              why"
									    ",                  connid"
									    ",                      targetname"
									    ",                            scripttarget"
									    ",                                fastflushtarget" // "0" or "1"
									    ",                                      rowlimit"
									    ",                                          processorder"
									    ")"
		                       " values(%d, %d, %d, %d, %s, %d, '%s', %d, '%s', %d, %d)"
		                       ;
	}
	
	String script = Format(scriptMap
		                       ,        taskId
		                       ,            scriptEditor.scriptId
		                       ,                OB_TASKS
		                       ,                    OB_SCRIPTS
		                       ,                        controlConnection->PrepTextDataForSend(why)
		                       ,                            scriptEditor.connId
		                       ,                                targetNameList.GetData()
		                       ,                                      scriptTargetListSelection
		                       ,                                          isfastFlushTargetList
		                       ,											    GetFieldInt(fldRowLimit) // Null becomes -1
		                       ,                                                    processorder
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
// Extract SQL for building the database, with some sample values.
void Lister::ExtractDatabase() {
	// I push these out to a svn preserved folder
	
	// Use Init(__FILE__) path to detect proper export folder
	String outputPath = AppendFileName(GetFileFolder(mainFilePath),  + "../database/");
	String pg_dump = "C:/Program Files/PostgreSQL/9.0/bin/pg_dump.exe";
	
	String baseScript = pg_dump;
	baseScript << " --host localhost --port 5432 --username postgres %s postgres";

	String extractionScript;


	// Extract function DDL and its proper function name.  We need a signature name
	// without the argument names in order to uniquely save the function, since the function
	// name is not unique.
	
	controlConnection->SendQueryDataScript(
		" SELECT  pg_catalog.pg_get_functiondef(p.oid)"
		", p.oid::regprocedure "
		" FROM    pg_catalog.pg_namespace n"
		" JOIN    pg_catalog.pg_proc p"
		" ON      pronamespace = n.oid"
		" WHERE   nspname = 'public'"
    );
    
    // Write each DDL statement to a separate file to allow change tracking in SVN
    while (controlConnection->Fetch()) {
        String functionDDL = controlConnection->Get(0);
        String functionFullSignature = controlConnection->Get(1);
        String fnm = outputPath + "lister.function." + functionFullSignature + ".sql";
        FileOut fo;
        fo.Open(fnm);
        fo.Put(functionDDL);
        fo.Close();
    }
    
	// Binary backup is not stored to svn since we can't track changes easily
	extractionScript = Format(baseScript,
		CAT << "--format custom --file "
		<< outputPath << "lister.schema.compressed.bkp "
		<< " --schema public"
		<< " --schema-only"  //  We only want data structures first
		);
	Shell(extractionScript);

	// Svn'd.  Verbose option off since timestamp causes svn modify bit to be set regardless
	// of meaningful change.
	extractionScript = Format(baseScript,
		CAT << "--format plain --file "
		<< outputPath << "lister.schema.sql"
		<< " --schema public"
		<< " --schema-only"  //  We only want data structures first
		);
	Shell(extractionScript);

	// Save schema definition for tables individually so that changes per table can more
	// easily be tracked and commented on, rolled back.
	Vector<String> tablelist = controlConnection->session->EnumTables("public");
	for (int i = 0; i < tablelist.GetCount(); i++) {
		String tablename = tablelist.At(i);
		extractionScript = Format(baseScript,
			CAT << "--format plain --file "
			<< outputPath << "lister." << tablename << ".schema.sql"
			<< " --schema public"
			<< " --table " << tablename
			<< " --schema-only"  //  We only want data structures first
			);
		Shell(extractionScript);
		
	}

	Vector<String> viewlist = controlConnection->session->EnumViews("public");
	for (int i = 0; i < viewlist.GetCount(); i++) {
		String viewname = viewlist.At(i);
		extractionScript = Format(baseScript,
			CAT << "--format plain --file "
			<< outputPath << "lister." << viewname << ".schema.sql"
			<< " --schema public"
			<< " --table " << viewname
			<< " --schema-only"  //  We only want data structures first
			);
		Shell(extractionScript);
		
	}
	
	
}

//==============================================================================================
// Helper to copy executable out to "production" directory.
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
	
	//__________________________________________________________________________________________
	
	// Save editor text to its Script Id, regardless of selection to left (if its modified)
	
	bar.Add(!scriptEditor.GetScriptPlainText().IsEmpty()
			&& scriptEditor.IsModified(), "File", MyImages::save16(), 
		THISBACK(SaveScript))
		
		.Tip("Save/Overwrite Script")
		.Key(K_CTRL_S);
	
	//__________________________________________________________________________________________
	
	// Add Script To History
	
	bar.Add(!scriptEditor.GetScriptPlainText().IsEmpty()
			&& scriptEditor.IsModified(), "File", CtrlImg::smalldown(), 
		THISBACK(AddScriptToHistory))
		
		.Tip("Save as new Script");
	
	//__________________________________________________________________________________________
	
	// Create Test From Script if we have an id
	
	bar.Add( // Only allow test creation if there is a script, a scriptid, and a connection
		(!scriptEditor.GetScriptPlainText().IsEmpty()
		 && scriptEditor.connection
		 && scriptEditor.scriptId >= 0), "File", MyImages::addtotest16(), 
		THISBACK(CreateTestFromScript))
		
		.Tip("Create a Test around this Script");

	//__________________________________________________________________________________________
	
	// Browse existing Tests
	
	bar.Add(!testWin.IsOpen(), "", MyImages::browsetests16(), 
		THISBACK(BrowseTests))
		
		.Tip("Browse and edit tests");
		 
	//__________________________________________________________________________________________
	
	// Connect & Execute Script against current connection
	
	bar.Add( // Only allow execution if there is a script and a connection
		(!scriptEditor.GetScriptPlainText().IsEmpty() 
		 && !scriptEditor.connection), "File", MyImages::connectruntoscreen16(), 
		THISBACK(ConnRunScriptOutputToScreen))
		
		.Tip("Connect, execute Script and output to a grid on the screen")
		.Key(K_CTRL_F5);

	//__________________________________________________________________________________________
	
	// Execute Script against current connection
	
	bar.Add( // Only allow execution if there is a script present and a connection attached to it
		(!scriptEditor.GetScriptPlainText().IsEmpty() 
		 && scriptEditor.connection), "File", MyImages::runtoscreen16(), 
		THISBACK(RunScriptOutputToScreen))
		
		.Tip("Execute Script and output to a grid on the screen")
		.Key(K_F5);

	//__________________________________________________________________________________________
	
	// Execute Script and pass output to physical table in control db.
	
	bar.Add( // Only allow execution if there is a script and a connection
		(!scriptEditor.GetScriptPlainText().IsEmpty() 
		 && scriptEditor.connection), "File", MyImages::runtotable16(), 
		THISBACK(RunScriptOutputToTable))
		
		.Tip("Execute Script and create a table in the control database");
	        
	//__________________________________________________________________________________________
	
	// Cancel a running Script (Buggy)
	
	bar.Add(
		(!scriptEditor.GetScriptPlainText().IsEmpty() 
		 && scriptEditor.connection
		 && activeConnection 
		 && activeConnection->session 
		 && In(activeConnection->session->GetStatus(), activeConnection->session->START_EXECUTING, activeConnection->session->START_FETCHING)
	 	), "File", MyImages::cancelop16(), 
		THISBACK(CancelRunningScriptOnActiveConn))
		
		.Tip("Cancel executing script on active connection");

	//__________________________________________________________________________________________

	scriptEditor.FindReplaceTool(bar);
	
	//__________________________________________________________________________________________
	
	// Popup list of Contacts
	
	bar.Add(
		true, "ListContacts", MyImages::contacts16(), 
		THISBACK(ListContacts))
		
		.Tip("Browse and edit contact details");

	//__________________________________________________________________________________________
	
	// Deploy application to Program Files folder
	
	bar.Add(true, "Deploy", MyImages::deploylister16(), 
		THISBACK(DeployLister))
		
		.Tip("Deploy lister to program files");

	//__________________________________________________________________________________________
	
	// Attach this Script to the selected Task
	
	bar.Add(
		!scriptEditor.GetScriptPlainText().IsEmpty() && scriptEditor.scriptId >= 0
		&& taskGrid.IsCursor()
		, "Attach", MyImages::attachtotask16(),
		THISBACK(AttachScriptToTask))
		
		.Tip(
		    // If the shift key is held down, attachment replaces the current script selected
		 	GetShift()? "Update selected script currently assigned to selected task" : "Add script to selected task");

	//__________________________________________________________________________________________
	
	// Set the target table name
	
	bar.Add(targetNameList, 150); targetNameList.Tip("Target Table Name in local postgres db if target type is database for script output");
	bar.Add(scriptTargetList, 85); scriptTargetList.Tip("What type of target does the script output to?");
	bar.Add(fastFlushTargetList, 75); fastFlushTargetList.Tip("Truncate the target or leave as is?"); // TODO: More types: Create if not there, else append, replace on key, truncate if older than T+1
	bar.Add(fldRowLimit, 65); fldRowLimit.Tip("Limit row output; -1 = no limit"); // BUG: Being ignored
	bar.Add(chkAddSepToOutput); chkAddSepToOutput.Tip("For screen only, add x blank lines when the field selected changes value.  A cheap grouping mech for export to Excel and making easier to see groups or add summations manually to output.");
	bar.Add(outFldSepWhenValChange, 66); outFldSepWhenValChange.Tip("Select field from dropdown or type in the name of the output grid column header that you want to break on");
	bar.Add(fldSepRowCount, 15); fldSepRowCount.Tip("Number of blank lines to separate groups by");
	bar.Add(macrosAvailableList, 150); macrosAvailableList.Tip("Macros available for script insertion based on currently selected task.  Edit task to change/add macros");
	
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
void Lister::ScriptExecutionHandler(Script::ScriptTarget pscriptTarget) {
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


	OutputStat *outputStat = new OutputStat();
	// Push on the pane stak and users can close as needed.
	bottomRightPane.Add("OutputStat", outputStat);
		
	JobSpec jspec; // Take defaults for now
	jspec.outputStat = outputStat;
	jspec.outputStat->SetScriptId(scriptEditor.scriptId);
	jspec.outputStat->SetConnName(activeConnection->connName);
	
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
		,	chkAddSepToOutput.Get()
		,	outFldSepWhenValChange.GetData()
		,	AsInt(fldSepRowCount.GetData(), 1) // Default to 1 row sep (if separating)
	);
	
	jspec.outputStat->SetStatus("Calling runner");
	jspec.outputStat->SetStartedWhen(GetSysTime());
	bool ran = cursorHandler.Run(sob, jspec, &activeContextMacros);
	if (!ran) {
		// Identify if it was a parsing (during execution) error.  If so, place cursor
		// in script text on point of error.
		int parseErrorPos = activeConnection->GetParseErrorPosition();
		
		if (parseErrorPos >= 0) {
			scriptEditor.SetSelection(parseErrorPos, parseErrorPos);
			scriptEditor.SetFocus();
		}
	}
}
                            
//==============================================================================================
// Execute the script in the script editor against the active connection and push output to a
// table in the local database.
void Lister::RunScriptOutputToTable() {
	ScriptExecutionHandler(Script::SO_TABLE /* load into table, leave grid empty */);
}

//==============================================================================================
// Connect, Execute the script in the script editor against the active connection	
void Lister::ConnRunScriptOutputToScreen() {
	// Connect
	int connId = scriptEditor.connId;
	if (connId == UNKNOWN) {
		NotifyUser("No connection attached to this script");
		return;
	}
	Connection * runConn = connectionFactory->Connect(this, connId);
	if (!runConn) {
		NotifyUser("Error trying to connect.");
		return;
	}
	
	// Get all our ducks in a row.  ScriptExecutionHandler looks for an active connection.
	// Q: Will it freak since we didn't actually select the row in the conn grid?
	SetActiveConnection(runConn);
	ScriptExecutionHandler(Script::SO_SCREEN /* Don't load into table, load into grid */);
}

//==============================================================================================
// Execute the script in the script editor against the active connection	
void Lister::RunScriptOutputToScreen() {
	ScriptExecutionHandler(Script::SO_SCREEN /* Don't load into table, load into grid */);
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
		("win2", *GetLogWin())
		("edit1", scriptEditor) // Generically, the editor screen.
	;
	
	UrpWindow::Xmlize(xml); // Make sure and save our window position!
}
