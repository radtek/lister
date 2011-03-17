#include "TestGrid.h"
#include <CtrlLib/CtrlLib.h>
#include "TestState.h"
#include "TestButton.h"
#include "Connection.h"
#include "shared_db.h"

//==============================================================================================
Id IDTestState("State");
Id IDTestId("TestId");
Id IDTestName("TestName");
Id IDTestRelId("TestRelId");
Id IDTestNote("TestNote");
Id IDTestConnId("TestConnId");
Id IDTestTypId("TestTypId");
Id IDCompTypId("CompTypId");
Id IDDesiredOutcome("DesiredOutcome");  // P)ass or F)ail
Id IDActualOutcome("ActualOutcome");   // P)ass, F)ail, I)ndeterminate
Id IDOutputValue("OutputValue");
Id IDCompareUsingX("X");
Id IDCompareUsingY("Y");
Id IDInvertComparison("InvertComparison"); // A "NOT" test
Id IDTestTaskId("TestTaskId");
Id IDTestProcessOrder("processorder");
Id IDTEST("TEST!");
Id IDTESTDUMMY("Dummy");

using namespace Upp;

//==============================================================================================
void MakeTestButton(One<Ctrl>& ctrl) {
	ctrl.Create<TestButton>();
	ctrl->WantFocus();
}

//==============================================================================================
void MakeTestState(One<Ctrl>& ctrl) {
	ctrl.Create<TestState>();
	ctrl->WantFocus();
}

//==============================================================================================
TestGrid::TestGrid() {
	taskId = UNKNOWN; // Clear filter
}

//==============================================================================================
// Added manually from appending a row.
void TestGrid::NewTest() {
	if (taskId != UNKNOWN) {
		Set(IDTestTaskId, taskId);
	}
}

//==============================================================================================
// GridCtrl will remove the row if we do not cancel the remove.
void TestGrid::RemoveTest() {
	ASSERT(connection);
	ASSERT(connection->session->IsOpen());
	ASSERT(IsCursor());
	int row = GetCursor();

	int testId = GetTestId(row);
	
	String controlScript = Format("delete from tests where testid = %d", testId);
	
	int rsp = PromptOKCancel(CAT << "Remove Row from control database: " << controlScript);
	if (rsp == 1) {
		if (!connection->SendRemoveDataScript(controlScript)) {
			CancelRemove();
		}
	} else {
		CancelRemove(); // Prevent GridCtrl from removing the row from screen.
	}
}

//==============================================================================================
// Added from the Script Editor, which only provides these three pieces of info.
// Create a bare-bones frame for a new test based on a script with a valid scriptid.
void TestGrid::AddTest(String script, int scriptId, int connId) {
	// TODO: Refactor to deal with task scripts only.
//	Add();
//	if (connection->SendQueryDataScript(Script::GetScriptListQuery())) {
//		scriptList.Clear();
//		while(connection->Fetch()) {
//			scriptList.Add(connection->Get(0), connection->Get(1));
//		}
//	}
//	
//	Set(IDTestScriptId, scriptId);
//	Set(IDTestConnId, connId);
}

//==============================================================================================
void TestGrid::Build() {
	WhenNewRow      = THISBACK(NewTest);    // At the creation of a new row; pre-populate fields
	WhenRemoveRow   = THISBACK(RemoveTest);
	WhenAcceptedRow = THISBACK(SaveTest);   // Build/Run SQL to save data

	// SizePos(); GRIDCTRL BUG: // Cannot SizePos in TestGrid.Build() or Graphics will cycle onto all cells (see SetImage(CtrlImg::go_forward())
	// Make the dropgrids of values extend past the width of the column they are for, to improve readability without wasting grid space.
	
	connList   .SearchHideRows().Resizeable().Width(200);
	scriptList .SearchHideRows().Resizeable().Width(200);
	testTypList.SearchHideRows().Resizeable().Width(200);
	compTypList.SearchHideRows().Resizeable().Width(200);
		
	AddColumn(IDTestState       , ""               ).Ctrls(MakeTestState)    .Default(TestState::ConvertStateToColor(NOTEST_NEVER)).Fixed(16);
	AddColumn(IDTestId          , "Id"  , 100      ).Edit(fldTestId)         .Default(Null);
	AddColumn(IDTestName        , "Name", 100      ).Edit(fldTestName);
	AddColumn(IDTestRelId       , "Script"         ).Edit(scriptList)        .SetConvert(scriptList)        .Default(-1);
	AddColumn(IDTestNote        , "Note"           ).Edit(fldTestNote);
	AddColumn(IDTestConnId      , "Conn"           ).Edit(connList)          .SetConvert(connList)          .Default(-1);
	AddColumn(IDTestTypId       , "TestType"       ).Edit(testTypList)       .SetConvert(testTypList)       .Default(-1);
	AddColumn(IDInvertComparison, "NOT?"           ).Edit(invertCompList)    .SetConvert(invertCompList)    .Default(false);
	AddColumn(IDCompTypId       , "ComparisonType" ).Edit(compTypList)       .SetConvert(compTypList)       .Default(-1);
	AddColumn(IDCompareUsingX   , "X"              ).Edit(fldCompareUsingX);
	AddColumn(IDCompareUsingY   , "Y"              ).Edit(fldCompareUsingY);
	AddColumn(IDDesiredOutcome  , "Desired Outcome").Edit(desiredOutcomeList).SetConvert(desiredOutcomeList).Default("P");  // P)ass or F)ail
	AddColumn(IDActualOutcome   , "Actual Outcome" );   // P)ass, F)ail, I)ndeterminate
	AddColumn(IDOutputValue     , "Output"         );
	AddColumn(IDTestTaskId      , "Task", 50       ); // The task that owns this test
	AddColumn(IDTestProcessOrder, "processorder"   ).Hidden();  // Required for ordering
	
	AddColumn(IDTEST            , ""               ).Ctrls(MakeTestButton).Fixed(20).SetImage(CtrlImg::go_forward());
	AddColumn(IDTESTDUMMY       , ""               ).Fixed(1);
	
	built = true;
}

//==============================================================================================
int    TestGrid::GetTestId            (int row)							  { return Get(row, IDTestId); }
void   TestGrid::SetTestId            (int row, int ptestId)			  {        Set(row, IDTestId, ptestId); }
String TestGrid::GetTestName          (int row)							  { return TrimBoth(Get(row, IDTestName)); }
String TestGrid::GetTestNote          (int row)             			  { return TrimBoth(Get(row, IDTestNote)); }
int    TestGrid::GetTestRelId         (int row)              			  { return Get(row, IDTestRelId); }
int    TestGrid::GetTestConnId        (int row)              			  { return Get(row, IDTestConnId); }
int    TestGrid::GetTestTypId         (int row)               			  { return Get(row, IDTestTypId); }
bool   TestGrid::GetInvertComparison  (int row)       			  		  { return Get(row, IDInvertComparison); }
int    TestGrid::GetCompTypId         (int row)              			  { return Get(row, IDCompTypId); }
String TestGrid::GetCompareUsingX     (int row)        				  	  { return Get(row, IDCompareUsingX); }
String TestGrid::GetCompareUsingY     (int row)   					      { return Get(row, IDCompareUsingY); }
String TestGrid::GetDesiredOutcome    (int row)       					  { return Get(row, IDDesiredOutcome); }
String TestGrid::GetActualOutcome     (int row)        					  { return Get(row, IDActualOutcome); } // Null returns INT_MIN, I think
void   TestGrid::SetActualOutcome     (int row, String pactualOutcome)	  {        Set(row, IDActualOutcome, pactualOutcome); }
String TestGrid::GetOutputValue       (int row)          				  { return Get(row, IDOutputValue); }
int    TestGrid::GetTaskId            (int row)                           { return Get(row, IDTestTaskId); }
//==============================================================================================
void TestGrid::SaveTest() {
	ASSERT(connection);
	ASSERT(connection->session->IsOpen());
	ASSERT(IsCursor());
	int row = GetCursor();

	String tokenizedActualOutcome    = GetActualOutcome   (row) == "" ? "NULL" : UrpString::Apostrophize(GetActualOutcome(row));
	String tokenizedDesiredOutcome   = GetDesiredOutcome  (row) == "" ? "NULL" : UrpString::Apostrophize(GetDesiredOutcome(row));
	String tokenizedInvertComparison = GetInvertComparison(row) ? "T" : "F";
	String tokenizedCompareUsingY    = GetCompareUsingY   (row);
	String testName                  = GetTestName        (row);
	String testNote                  = GetTestNote        (row);
	int    testRelId                 = GetTestRelId       (row);
	int    testConnId                = GetTestConnId      (row);
	
	if (IsNewRow()) {
		int newProcessOrder = GetNextProcessOrder();
	
		// Determine if test already exists
		String controlScript = Format(
			"insert into tests(testname, note, relid,    connid, desiredoutcome, actualoutcome, outputvalue, x,    y,    invertcomparison, comptypid, testtypid, taskid, processorder) "
			"values(           %s,       %s,   %s,       %s,     %s,             %s,            %s,          %s,   %s,   '%s',             %s,        %s,        %s    , %s)"
				,              connection->PrepTextDataForSend(testName)
				,                        connection->PrepTextDataForSend(testNote)
				,                              ToSQL(testRelId)
				,                                        ToSQL(testConnId)
				,                                                tokenizedDesiredOutcome // apostrophize at token level in case a NULL is required
				,                                                                tokenizedActualOutcome // apostrophize at token level in case a NULL is required
				,                                                                               connection->PrepTextDataForSend(GetOutputValue(row))
				,                                                                                            connection->PrepTextDataForSend(GetCompareUsingX(row))
				,                                                                                                  connection->PrepTextDataForSend(GetCompareUsingY(row))
				,                                                                                                        tokenizedInvertComparison // Not apposted:)
				,                                                                                                                          ToSQL(GetCompTypId(row))
				,                                                                                                                                     ToSQL(GetTestTypId(row))
				,                                                                                                                                                ToSQL(GetTaskId(row))
				,                                                                                                                                                        ToSQL(newProcessOrder)
				);

		LOG(controlScript);
		if (!connection->SendAddDataScript(controlScript)) {
			// Beep
			return;
		}

		// Add to the list if not a dup
		if (connection->GetRowsProcessed() > 0) {		
			int testid = connection->GetInsertedId("tests", "testid");
			if (testid >= 0) {
				SetTestId(row, testid);
			}
		}
	} else {
		// Update existing test
		int testId = GetTestId(row);
		String controlScript = Format(
			"UPDATE tests set"
		 		"  testname = %s"
		 		", note = %s, relid = %s, connid = %s, desiredoutcome = %s, actualoutcome = %s, outputvalue = %s"
		 		", x = %s, y = %s, invertcomparison = '%s', comptypid = %s, testtypid = %s, taskid = %s"
		     	" WHERE testid = %d"
				,            connection->PrepTextDataForSend(testName)
				,                       connection->PrepTextDataForSend(testNote)
				,                              ToSQL(testRelId)
				,                                        ToSQL(testConnId)
				,                                                tokenizedDesiredOutcome // apostrophize at token level in case a NULL is required
				,                                                                tokenizedActualOutcome // apostrophize at token level in case a NULL is required
				,                                                                               connection->PrepTextDataForSend(GetOutputValue(row))
				,                                                                                            connection->PrepTextDataForSend(GetCompareUsingX(row))
				,                                                                                                  connection->PrepTextDataForSend(GetCompareUsingY(row))
				,                                                                                                        tokenizedInvertComparison
				,                                                                                                                          ToSQL(GetCompTypId(row))
				,                                                                                                                                     ToSQL(GetTestTypId(row))
				,                                                                                                                                                ToSQL(GetTaskId(row))
				,                testId
				);
		     
		LOG(controlScript);
		
		int rsp = PromptOKCancel(CAT << "Updating Test: " << controlScript);
		if (rsp == 1) {
			connection->SendChangeDataScript(controlScript);
		}
	}
}

//==============================================================================================
// Hope someone set the taskId, or all tests will be fetched.
/*virtual*/ void TestGrid::Load(Connection *pconnection) {
	if (!loaded) {
		ConnectedCtrl::Load(pconnection);
	}
	
	desiredOutcomeList.Clear();
	desiredOutcomeList.Add("P", "Pass");
	desiredOutcomeList.Add("F", "Fail");
	
	invertCompList.Clear();
	invertCompList.Add(false, "");
	invertCompList.Add(true, "NOT");

	// Types of comparisons
	compTypList.Clear();
	if (connection->SendQueryDataScript("select comptypid, comptypname from comptyps")) {
		while(connection->Fetch()) {
			compTypList.Add(connection->Get(0), connection->Get(1));
		}
	}

	testTypList.Clear();
	if (connection->SendQueryDataScript("select testtypid, testtypname from testtyps")) {
		while(connection->Fetch()) {
			testTypList.Add(connection->Get(0), connection->Get(1));
		}
	}

	// Not scripts really, but the relations which hold the task, script, and connection together
	scriptList.Clear();
	if (taskId != UNKNOWN) {
		String s = Format("select relid, why, scriptplaintext, relconnid, relconnname from tasks_r where taskid = %d order by processorder", taskId);
		if (connection->SendQueryDataScript(s)) {
			while(connection->Fetch()) {
				scriptList.Add(connection->Get(0), connection->Get(1));
			}
		}
	}
	
	connList.Clear();
	if (connection->SendQueryDataScript("select connid, connname from connections order by connname")) {
		while(connection->Fetch()) {
			connList.Add(connection->Get(0), connection->Get(1));
		}
	}

	Clear();
	String script = 
	 			"select "
				"  testid"           // 0
				", testname"         // 1
				", note"             // 2
				", relid"            // 3
				", connid"           // 4
				", desiredoutcome"   // 5
				", actualoutcome"    // 6
				", outputvalue"      // 7
				", x"                // 8
				", y"                // 9
				", invertcomparison" // 10: Boolean
				", comptypid"        // 11
				", testtypid"        // 12
				", taskid"           // 13
				" from tests where testid >= 0 ";
				
	if (taskId != UNKNOWN) {
		script << " and taskId = " << taskId;
	}
	script << " order by testname";

	if (connection->SendQueryDataScript(script)) {
		while(connection->Fetch()) {
			Add();
			Set(IDTestId           , connection->Get(0));
			Set(IDTestName         , connection->Get(1));
			Set(IDTestNote         , connection->Get(2));
			Set(IDTestRelId        , connection->Get(3));
			Set(IDTestConnId       , connection->Get(4));
			Set(IDDesiredOutcome   , connection->Get(5));       // P)ass or F)ail
			Set(IDActualOutcome    , connection->Get(6));       // P)ass, F)ail, I)ndeterminate
			Set(IDOutputValue      , connection->Get(7));
			Set(IDCompareUsingX    , connection->Get(8));
			Set(IDCompareUsingY    , connection->Get(9));
			Set(IDInvertComparison , connection->GetBool(10));  // A "NOT" test.  We know its Boolean, but PostgreSQL driver does not.  I returns a string of 1 or 0
			Set(IDTestTypId        , connection->Get(11));
			Set(IDCompTypId        , connection->Get(12));
			Set(IDTestTaskId       , connection->Get(13));
		}
	}
	
	loaded = true;
}

//==============================================================================================
bool TestGrid::MeaningfulDataChange() {
	if (!IsModifiedRow()) return false; // No change
	
	if (
		!IsModified(IDTestName)         &&
		!IsModified(IDTestNote)         &&
		!IsModified(IDTestRelId)        &&
		!IsModified(IDTestConnId)       &&
		!IsModified(IDDesiredOutcome)   &&
		!IsModified(IDActualOutcome)    &&
		!IsModified(IDCompareUsingX)    &&
		!IsModified(IDCompareUsingY)    &&
		!IsModified(IDInvertComparison) &&
		!IsModified(IDTestTypId)        &&
		!IsModified(IDCompTypId)
		) return false;

	return true;
}

//==============================================================================================
bool TestGrid::WasTestRequested() {
	int rowno = GetCursor();
	if (rowno == -1) return false;
	TestButton *btn = (TestButton *)GetCtrl(rowno, FindCol(IDTEST));
	if (!btn) return false;
	if (btn->wasClicked) {
		btn->wasClicked = false; // Clear the bit for next click
		return true;
	} else {
		return false;
	}
}
