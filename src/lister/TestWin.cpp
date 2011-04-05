/***********************************************************************************************
*  lister - TestWin.cpp
*  
*  Simple container for TestGrid and a toolbar of controls for managing tests.
*
*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

#include "TestWin.h"
#include "Connection.h"
#include "SoundHandler.h"
#include "image_shared.h"
#include <Draw/iml_header.h>
#include "KeyHandler.h"
#include "JobSpec.h"
#include "Script.h"
#include "OutputStat.h"

// Only in lister.cpp
//#include "image_shared.h"
//#include <Draw/iml_source.h>

//==============================================================================================
// Construct the window that will hold the test grid and toolbar for executing batches.
TestWin::TestWin() {
	CtrlLayout(*this, "Browse All Tests");
	Sizeable().Zoomable();
	Icon(MyImages::browsetests16());
	controlConnection = NULL;	
	toolbar.Set(THISBACK(MyToolBar));
	toolbar.VSizePos(0, 10);
	AddFrame(toolbar);
	// Notify grid to tell us (a bit vague) when "something" changes that we might be interested in.
	// We can't use WhenNew or WhenChange, since TestGrid needs those.  It be cool if a callback was a list!
	
	testGrid.WhenToolBarNeedsUpdating = THISBACK(UpdateToolBar);
	testGrid.SizePos(); // Note: Cannot SizePos in TestGrid.Load() or Graphics will cycle onto all cells
}

//==============================================================================================
void TestWin::Build(Connection *pcontrolConnection, Connection **plastActiveConnection, ContextMacros *pactiveContextMacros) {
	controlConnection = pcontrolConnection;
	lastActiveConnection = plastActiveConnection;  // Point to where the mainwindow would set the active connection
	activeContextMacros = pactiveContextMacros;
	testGrid.Build(controlConnection); // Construct the callbacks and columns
	testGrid.WhenCtrlsAction = THISBACK(ClickedTest);
}
	
//==============================================================================================
// Add each button with gentle goodness.
void TestWin::MyToolBar(Bar& bar) {

	//__________________________________________________________________________________________

	// Setup necessary pre-existing components and data for tests
	
	bar.Add(testGrid.GetCount() > 0, "File", MyImages::runbatchsetup16(), 
		THISBACK(SetupTestSupportingData))
		
		.Tip("Setup necessary pre-existing components and data for tests")
		.Key(K_ALT_CTRL_F9);

	//__________________________________________________________________________________________

	// Run all Tests as Batch
	
	bar.Add(testGrid.GetCount() > 0, "File", MyImages::runbatch16(), 
		THISBACK(RunAllTestsAsBatch))
		
		.Tip("Run all the tests for this task in order")
		.Key(K_ALT_CTRL_F8);

	//__________________________________________________________________________________________

	// Run Current Test (I added just for the hot key)
	
	bar.Add(testGrid.GetCount() > 0, "File", MyImages::runtoscreen16(), 
		THISBACK(RunSelectedTest))
		
		.Tip("Run selected test")
		.Key(K_CTRL_ENTER);
		
	//__________________________________________________________________________________________

	// List of Task Drivers/Execution Paths for any test runs
	
	bar.Add(taskDriverList, 250);
	
}

//==============================================================================================
void TestWin::SetTaskId(int ptaskId) {
	ASSERT_(ptaskId != UNKNOWN, "Cannot set the Test Win to UNKNOWN");
	testGrid.taskId = taskId = ptaskId;
	ASSERT_(controlConnection, "Cannot set task id until control connection is available or we can't load the driver list.");
	
	activeTaskDriverId = UNKNOWN;

	taskDriverList.Clear();

	if (!controlConnection->SendQueryDataScript(Format("SELECT driverId, driverName FROM taskDrivers WHERE taskId = %d ORDER BY processOrder", taskId))) {
		Exclamation("Unable to fetch a list of drivers");
		return;
	}
	
	if (controlConnection->GetRowsProcessed() == 0) {
		return;
	}
	
	while (controlConnection->Fetch()) {
		taskDriverList.Add(controlConnection->Get(0), controlConnection->Get(1));
	}
}

//==============================================================================================
// When the TestWin is set up, the last task driver is plugged in.  User can change.
void TestWin::SetActiveTaskDriverId(int ptaskDriverId) {
	ASSERT_(taskId != UNKNOWN, "Cannot set a task Driver to use without setting a task Id.");
	testGrid.activeTaskDriverId = activeTaskDriverId = ptaskDriverId;
	
	if (!In(activeTaskDriverId, UNKNOWN, INT_NULL)) {
		taskDriverList.Find(activeTaskDriverId);
	} else {
		; // Nothing selected
	}
	
	taskDriverList.SetData(activeTaskDriverId);
}

//==============================================================================================
void TestWin::RunSelectedTest() {
	if (!testGrid.IsCursor()) return;
	
	RunTest(testGrid.GetCursor());
}

//==============================================================================================
// Construct raw localized tabes and a days worth of test data.
//==============================================================================================
void TestWin::SetupTestSupportingData() {
	// Traverse all rows and call tester, highlight each one.
	for (int i = 0; i < testGrid.GetCount(); i++) {
		// Highlight the running row
		testGrid.SetCursor(i);
		bool isPartOfSetup = testGrid.GetIsPartOfSetup(i);
		if (!isPartOfSetup) {
			continue;
		}
		// Let Paint event run
		ProcessEvents();
		String actualOutcome = RunTest(i, true /*batchmode*/);
		if (actualOutcome != "P" && testGrid.Get(STOPBATCHRUNONFAIL)) {
			Exclamation(Format("Batch failed on must pass test <%s>", testGrid.GetTestName(i)));
			break;
		}
	}
}

//==============================================================================================
// From toolbar, User requests run all batches regardless of selection.  Fail on flagged as
// must pass.
//==============================================================================================
void TestWin::RunAllTestsAsBatch() {
	// Traverse all rows and call tester, highlight each one.
	for (int i = 0; i < testGrid.GetCount(); i++) {
		// Highlight the running row
		testGrid.SetCursor(i);
		bool isPartOfSetup = testGrid.GetIsPartOfSetup(i);
		if (isPartOfSetup) {
			continue;
		}
		// Let Paint event run
		ProcessEvents();
		// Run in batch mode so no dialogs
		String actualOutcome = RunTest(i, true /*batchmode*/);
		if (actualOutcome != "P" && testGrid.Get(STOPBATCHRUNONFAIL)) {
			Exclamation(Format("Batch failed on must pass test <%s>", testGrid.GetTestName(i)));
			break;
		}
	}
}

//==============================================================================================
// Strange way to reload...
//==============================================================================================
void TestWin::UpdateToolBar(UrpGrid::GridAction gridAction) {
	toolbar.Set(THISBACK(MyToolBar));
}

//==============================================================================================	
// User clicked Test! on the TestGrid. Execute the test script against the connection.
//==============================================================================================
void TestWin::ClickedTest() { 
	int row = testGrid.GetCursor();
	
	WaitCursor wc;
	if (!testGrid.WasTestRequested()) {
		return;
	}

	RunTest(row);
		
}

//==============================================================================================	
void TestWin::Close() {
	if (WhenToolBarNeedsUpdating)
	SetTimeCallback(100, WhenToolBarNeedsUpdating);
	UrpWindow::Close();
}


//==============================================================================================	
String TestWin::RunTest(int row, bool isPartOfBatch/*=false*/) { 	
	// Get test connection
	
	int    testConnId       = testGrid.GetTestConnId      (row);
	int    testTypId        = testGrid.GetTestTypId       (row);
	int    testRelId        = testGrid.GetTestRelId       (row);
	int    testCompTypId    = testGrid.GetCompTypId       (row);
	String testX            = testGrid.GetCompareUsingX   (row);
	String testY            = testGrid.GetCompareUsingY   (row); // Note that PostgreSQL can store Arrays in a single value
	String desiredOutcome   = testGrid.GetDesiredOutcome  (row);
	bool   invertComparison = testGrid.GetInvertComparison(row);
	
	if (testTypId == UNKNOWN) {
		Exclamation("Must assign a test type");
		return Null;
	}
	
	if (testRelId == UNKNOWN) {
		Exclamation("No relation assigned to this test");
		return Null;
	}

	// Create an outputstat so we can capture rowcount somewhere
	OutputStat outputStat; // Only need temp copy
	JobSpec jspec;
	jspec.outputStat = &outputStat;
	
	jspec.log = false;
	jspec.batchMode = isPartOfBatch;
	jspec.testMode = false; // test mode here just means a reduced set of rows.  Should be Trial or Demo
	jspec.executingSelection = false;
	
//	if (jspec.log) {
//		ActivateLogging();
//		OpenLogWin(); // User can close if they want to, later.
//	}

	Script sob;

	// Fetch all script attributes as single object to reduce points of attribute listing
	
	if (!sob.LoadFromDb(controlConnection, testRelId)) {
		Exclamation("Can't load script detail");
		return Null;
	}

	// Scratch out any screen output
	if (sob.scriptTarget == Script::SO_SCREEN) sob.scriptTarget = Script::SO_UNDEF;
		
	int testScriptId = sob.scriptId;
	int testRelConnId = sob.connId;
	jspec.outputStat->SetScriptId(testScriptId); // Just for shits n Grins, no known referencer

	// Get the script SQL to fetch the script attributes
	
	String getScriptText = sob.scriptPlainText;
	
	bool testCompCorrect = false; // Not same as Passed!
	bool testCompApplied = false;
	
	// Either a specific connection was assigned to this test, or we'll use the one
	// assigned in the relation (preferred).

	if (testConnId == UNKNOWN) {
		if (testRelConnId == UNKNOWN) {
			Exclamation("No connection set for test or in the relation either");
			return Null;
		} else {
			testConnId = testRelConnId;
		}
	}

	// Sync up our sob object before passing with a possibly overridden connection
	
	sob.connId = testConnId;

	// Connect or get a similar connection
	Connection * testConn = connectionFactory->Connect(this, testConnId);
	
	if (!testConn) {
		Exclamation("Can't get a test connection");
		return Null;
	}

	jspec.outputStat->SetConnName(testConn->connName); // Just for shits n Grins, no known referencer
	
	// Build macros based on current task and environment of selected connection
	
	activeContextMacros->RebuildMacros(testConnId, activeTaskDriverId, activeContextMacros, controlConnection, *lastActiveConnection);

	// Execute script

	if (!testConn->ProcessQueryDataScript(sob, jspec, activeContextMacros)) {
		return Null;
	}

	Value testResult;
	
	if (testTypId == TESTTYP_NOTEST) { // Not a test, just run and move on
		Speak(EVS_TEST_SUCCEEDED);
		testGrid.SetActualOutcome(row, "-"); // Just done, not pass or fail
		
	} else {
		if (testTypId == TESTTYP_RETURNED_ROW_COUNT) {
			int targetRowCount = atoi(testX);
			int actualRowCount = outputStat.GetRowCount();
			testResult = actualRowCount;
			
			// Comparison rule to use
			if (testCompTypId == COMPTYP_GREATER_THAN_X) {
				testCompCorrect = (actualRowCount > targetRowCount);
				testCompApplied = true;
			} else 
			if (testCompTypId == COMPTYP_EQUAL_TO) {
				testCompCorrect = (actualRowCount == targetRowCount);
				testCompApplied = true;
			} else {
				Exclamation(Format("Comparison type %d not supported for this Test type: %d", testCompTypId, testTypId));
			}
		} else 
		if (testTypId == TESTTYP_FIRST_COLUMN_VALUE) {
			int actualRowCount = testConn->GetRowsProcessed();
			if (actualRowCount == 0) {
				testCompApplied = false;
				
			} else {
				testConn->Fetch();
				testResult = testConn->Get(0);
				if (testResult.GetType() == INT64_V) {
					int64 testResultNum = testConn->Get(0);
					if (!UrpString::IsIntegerString(testX)) {
						testCompApplied = false; // Cannot test if n
					} else {
						int64 testAgainstNum = _atoi64(testX);
						if (testCompTypId == COMPTYP_EQUAL_TO) {
							testCompApplied = true;
							testCompCorrect = (testResultNum == testAgainstNum);
						} else
						if (testCompTypId == COMPTYP_GREATER_THAN_X) {
							testCompApplied = true;
							testCompCorrect = (testResultNum > testAgainstNum);
						} else
						if (testCompTypId == COMPTYP_LESS_THAN_X) {
							testCompApplied = true;
							testCompCorrect = (testResultNum < testAgainstNum);
						} else
						if (testCompTypId == COMPTYP_GREATER_THAN_OR_EQUAL_TO_X) {
							testCompApplied = true;
							testCompCorrect = (testResultNum >= testAgainstNum);
						} else
						if (testCompTypId == COMPTYP_LESS_THAN_OR_EQUAL_TO_X) {
							testCompApplied = true;
							testCompCorrect = (testResultNum <= testAgainstNum);
						} else {
							testCompApplied = false;
							Exclamation(Format("Comparison type %d not supported for this Test type: %d", testCompTypId, testTypId));
						}
					}
				} else if (testResult.GetType() == STRING_V) {
					testCompApplied = true;
					testCompCorrect = (testResult.ToString() == testX); // No way to test a null
				}
				
			}
		} else {
			Exclamation(Format("Test type not supported: %d", testTypId));
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
				testGrid.SetActualOutcome(row, "P");
			} else {
				// Grr, doh! woopsee
				Speak(EVS_TEST_FAILED);
				testGrid.SetActualOutcome(row, "F");
				
			}
			
			testGrid.SetOutputValue(row, testResult.ToString());
			testGrid.SetLastRunWhen(row, GetUtcTime()); // I'm not sure it saves the timezone
			
			// Write to database
			testGrid.SaveTestNoPrompt();
			
			// TODO: Set time, log
		} else {
			testGrid.SetActualOutcome(row, "I");
			testGrid.SaveTestNoPrompt();
		}
	}
	
	return testGrid.GetActualOutcome(row);
}

