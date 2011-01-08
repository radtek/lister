#include "lister.h"

#include "image_shared.h"
#include <Draw/iml_header.h>

#include "image_shared.h"
#include <Draw/iml_source.h>

#define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>
#include "Sql/sch_source.h"

	//==========================================================================================	
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
		vertSplitter.SetPos(2000); // Set default space reserved for connection list and command script
		
		horizTopSplitter.Horz(topLeftPane, horizTopMidSplitter);	 // Horizontal Splitter between top panes 1 and 2
		horizTopSplitter.SetPos(3000);

		horizTopMidSplitter.Horz(topMidLeftPane, horizTopMidAndRightSplitter);	 // Horizontal Splitter between top panes 1 and 2
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
					this, CONTROL_CONN_NAME, ConnectionFactory::ControlInstanceType(), "postgres", "postgres", "localhost", "postgres"))->enumConnState 
					!= CON_SUCCEED) {
			exit(-1);
		}

		// Construct Script List Drop Down Grid

		scriptDropDownList.AddColumn("ID", 0);	// Hidden Column
		scriptDropDownList.AddColumn(""); // No label so it doesn't show up 
		scriptDropDownList.AddColumn("RichScript", 0); // Hidden column
		scriptDropDownList.Header(false);
		scriptDropDownList.ColorRows();
		//scriptDropDownList.  .WhenDrop = THISBACK(SelectedScriptFromDropDown);
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
		connGrid.WhenCursor = THISBACK(ActiveConnectionChanged); // Call be when user selects a different connection in grid
	
		// Construct Task Grid
		
		taskGrid.Description("taskGrid");
		topMidLeftPane.Add(taskGrid);
		taskGrid.Build();
		taskGrid.Load(controlConnection);
		taskGrid.SizePos();	

		// Construct Script Grid
		
		scriptGrid.Description("scriptGrid");
		topMidRightPane.Add(scriptGrid);
		scriptGrid.Build();
		scriptGrid.Load(controlConnection);
		scriptGrid.SizePos();	

		testWin.testGrid.Build();
		testWin.testGrid.WhenCtrlsAction = THISBACK(ClickedTest);
		testWin.testGrid.Load(controlConnection);
		
		// By spinning this off as a callback, we get the screen displayed while autoconnecting, and plus the cursor on conn grid is properly set to center
		//SetTimeCallback(100, THISBACK(AutoConnect));		
	}

	//==========================================================================================	
	// User clicked Test! on the TestGrid.
	void Lister::ClickedTest() { 
		TestGrid &testGrid = testWin.testGrid;
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

	//==========================================================================================	
	// User clicked Connect! on the ConnGrid.
	void Lister::ClickedConnect() {
		if (connGrid.WasConnectionRequested()) {
			SetActiveConnection(ConnectUsingGrid(connGrid.GetCursor()));
			if (activeConnection && activeConnection->enumConnState != CON_SUCCEED)
			{
				Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), Format("Connect Error: [* %s].", DeQtf(activeConnection->connectErrorMessage)), t_("OK"));
			}
		}
	}

	//==========================================================================================
	// Connect to the connection user selected in the ConnGrid.	
	Connection *Lister::ConnectUsingGrid(String connName) {
		int row = connGrid.GetCursor();
		if (connGrid.FindConnName(connName)) {
			return ConnectUsingGrid(row);
		} else {
			return NULL;
		}
	}

	//==========================================================================================	
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
			, NULL // dbName
			);
			
		UrpString::FromTo(connGrid.GetConnId(row), c->connId);
		connGrid.SetConnState(row, c->enumConnState);
		return c;
	}

	//==========================================================================================	
	void Lister::AddScriptToHistory() {
		String richscript = cmdScript.GetQTF();
		String script = TrimBoth(cmdScript.Get().GetPlainText().ToString());

		if (script.IsEmpty()) {
			return;
		}
		
		// A little trick to only insert into a table new scripts
		String controlScript = Format("insert into scripts(richscript, script) select '%s', '%s' from dual left join scripts on 1=1 where script <> '%s' or script is null limit 1"
		    // Not sending these as scripts to run, but to insert as text
			, controlConnection->PrepTextDataForSend(richscript)
			, controlConnection->PrepTextDataForSend(script)
			, controlConnection->PrepTextDataForSend(script)); 

		if (!controlConnection->SendAddDataScript(controlScript)) {
			cmdScript.Tip(DeQtf(controlConnection->GetLastError()));
			cmdScript.SetData(controlScript);
			return;
		}

		// Add to the list if not a dup
		if (controlConnection->GetRowsProcessed() > 0) {		
			int scriptid = controlConnection->GetInsertedId("scripts", "scriptid");
			if (scriptid >= 0) {
				scriptDropDownList.Add(scriptid, script, richscript);
				cmdScript.scriptId = scriptid; // Link to script so that it can be uploaded to tests
			}
		}
	}
	
	//==========================================================================================
	// For multi-line scripts, this is the only way to see the full script.
	void Lister::SelectedScriptFromDropDown() {
		scriptDropDownList.Tip(scriptDropDownList.Get(1).ToString());
	}
	
	//==========================================================================================	
	// User pressed "+" button on script list dropdown, requesting that the script be loaded
	// into the editor.
	void Lister::PushScriptToEditor() {
		cmdScript.SetScript(scriptDropDownList.Get(2).ToString());
		Value scriptIdValue = scriptDropDownList.Get(0);
		
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
	
	//==========================================================================================
	void Lister::CreateTestFromScript() {
		
		if (!cmdScript.connection) {
			Exclamation("No connection attached to the current script editor");
			return;
		}
		
		if (cmdScript.scriptId < 0) {
			Exclamation("Script Id is negative. Can't save to test.");
			return; // Don't try and save tests with no script #, meaning not in database
		}
		
		testWin.Open();
		testWin.testGrid.AddTest(TrimBoth(cmdScript.Get().GetPlainText().ToString()), cmdScript.scriptId, cmdScript.connection->connId);
	}

	//==========================================================================================
	void Lister::BrowseTests() {
		testWin.Open();
	}
	
	//==========================================================================================
	// Thought I needed this more	
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
	
	//==========================================================================================
	// Capture when session->status value changed, usually during a Execute or Fetch.
	// We need the Cancel Execution button to only be active when a asynch execution call is away.
	void Lister::SessionStatusChanged(const SqlSession&session) {
		ToolBarRefresh();
		ConnectionStatusRefresh();
	}
	
	//==========================================================================================
	// User clicked the connect button or clicked on a row in the conngrid.	
	void Lister::SetActiveConnection(Connection *newConnection) {
		activeConnection = newConnection;
		cmdScript.connection = activeConnection;
		ActiveConnectionChanged();
	}

	//==========================================================================================	
	// Update all enabled/disabled buttons based on a change in the users selected active connection.
	void Lister::ActiveConnectionChanged() {
		ToolBarRefresh();
		ConnectionStatusRefresh();
		if (!activeConnection) return;
		if (!activeConnection->session) return;
		
		activeConnection->session->WhenDatabaseActivity = THISBACK(SessionStatusChanged);
	}
	
	//==========================================================================================	
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
	
	//==========================================================================================	
	void Lister::CancelRunningScriptOnActiveConn() {
		activeConnection->Cancel(); // Sends OCICancel (Need reset?)
	}
	
	//==========================================================================================	
	void Lister::AttachScriptToTask() {
		ASSERT(taskGrid.IsCursor());
		ASSERT(cmdScript.scriptId >= 0);
		int taskId = taskGrid.GetCursor();
		String script = Format("insert into relations(fromid, toid, fromtbid, totbid)"
		                       " values(%d, %d, '%s', '%s')", taskId, cmdScript.scriptId,
		                       OB_TASKS, OB_SCRIPTS);
		                       
		if (controlConnection->SendAddDataScript(script)) {
			
		}
		
	}
	
	//==========================================================================================	
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

	//==========================================================================================	
	// Update the toolbar active/inactive state of each button when connection or something changes.
	void Lister::ToolBarRefresh() {
		toolbar.Set(THISBACK(MyToolBar));
	}

	//==========================================================================================	
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

		bar.Add( true, "", MyImages::browsetests16(), 
			 THISBACK(BrowseTests)).Tip("Browse and edit tests");
			 
		// Only allow execution if there is a script and a connection
		bar.Add(
			(cmdScript.Get().GetPlainText().ToString().GetLength() > 0 
			 && cmdScript.connection), "File", MyImages::lightning_icon16(), 
			 THISBACK(ExecuteSQLActiveConn)).Tip("Execute Script");

		bar.Add(
			(cmdScript.Get().GetPlainText().ToString().GetLength() > 0 
			 && cmdScript.connection
			 && activeConnection 
			 && activeConnection->session 
			 && In(activeConnection->session->GetStatus(), activeConnection->session->START_EXECUTING, activeConnection->session->START_FETCHING)
		 	), "File", MyImages::cancelop16(), 
			 THISBACK(CancelRunningScriptOnActiveConn)).Tip("Cancel executing script on active connection");

		cmdScript.FindReplaceTool(bar);
		
		bar.Add(
			(cmdScript.connection), "ListUsers", MyImages::user16(), 
			 THISBACK(ListUsers)).Tip("List users/schemas for current connection");

		bar.Add(userList, 100);

		bar.Add(true, "Deploy", MyImages::deploylister16(), 
			 THISBACK(DeployLister)).Tip("Deploy lister to program files");

		bar.Add(
			cmdScript.Get().GetPlainText().ToString().GetLength() > 0 && cmdScript.scriptId >= 0
			&& taskGrid.IsCursor()
			, "Attach", MyImages::attachtotask16(),
			 THISBACK(AttachScriptToTask)).Tip("Attach script to selected task");
		
		//CtrlImg::exclamation(), CtrlImg::smallright(), CtrlImg::open(), CtrlImg::undo(), CtrlImg::remove
		//smallcheck, spinup3, smallreporticon, save, Plus, Minus, Toggle, help
	}
	
	//==========================================================================================	
	// Called from main.cpp. to open the main window
	/*virtual*/ void Lister::Run(bool appmodal /* = false */) {
		ToolBarRefresh();
		ConnectionStatusRefresh();
		TopWindow::SetFocus();
		TopWindow::Run(appmodal);
		
		// Make the interface seem to close faster, let destructor run invisibly
		Hide();
		ProcessEvents();
	}

	//==========================================================================================	
	void Lister::ExecuteSQLActiveConn() {
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
		
		CursorHandler cursorHandler(activeConnection);
		bool ran = cursorHandler.Run(&mainGrid, script);
	}
	
	//==========================================================================================	
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

	//==========================================================================================	
	void Lister::Serialize(Stream& s) {
		SerializePlacement(s);
	}
	
	//==========================================================================================	
	// Hack in Ctrl class to make Xmlize virtual
	/*virtual*/ void Lister::Xmlize(XmlIO xml) {
		// Use generic names internally to xml with numbers so if splitters added/moved, we can retain users setting if lucky
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
