#include "TestGrid.h"
#include <CtrlLib/CtrlLib.h>
#include "TestState.h"
#include "TestButton.h"
#include "Connection.h"
#include "shared_db.h"

//==============================================================================================
Id IDTestState                ("state");
Id IDTEST                     ("TEST!");
Id IDTESTDUMMY               ("Dummy");

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
		Set(TASKID, taskId);
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
	WhenAcceptedRow = THISBACK(SaveTestPrompt);   // Build/Run SQL to save data

	// SizePos(); GRIDCTRL BUG: // Cannot SizePos in TestGrid.Build() or Graphics will cycle onto all cells (see SetImage(CtrlImg::go_forward())
	// Make the dropgrids of values extend past the width of the column they are for, to improve readability without wasting grid space.
	
	connList   .SearchHideRows().Resizeable().Width(200);
	scriptList .SearchHideRows().Resizeable().Width(200);
	testTypList.SearchHideRows().Resizeable().Width(200);
	compTypList.SearchHideRows().Resizeable().Width(200);
		
	AddColumn(IDTestState       , ""               ).Ctrls(MakeTestState)    .Default(TestState::ConvertStateToColor(NOTEST_NEVER)).Fixed(16);
	AddColumn(TESTID            , "Id"  , 100      ).Edit(fldTestId)         .Default(Null);
	AddColumn(TESTNAME          , "Name", 100      ).Edit(fldTestName);
	AddColumn(RELID       , "Script"         ).Edit(scriptList)        .SetConvert(scriptList)        .Default(-1);
	AddColumn(NOTE        , "Note"           ).Edit(fldTestNote);
	AddColumn(CONNID      , "Conn"           ).Edit(connList)          .SetConvert(connList)          .Default(-1);
	AddColumn(TESTTYPID       , "TestType"       ).Edit(testTypList)       .SetConvert(testTypList)       .Default(-1);
	AddColumn(INVERTCOMPARISON, "NOT?"           ).Edit(invertCompList)    .SetConvert(invertCompList)    .Default(false);
	AddColumn(COMPTYPID       , "ComparisonType" ).Edit(compTypList)       .SetConvert(compTypList)       .Default(-1);
	AddColumn(X   , "X"              ).Edit(fldCompareUsingX);
	AddColumn(Y   , "Y"              ).Edit(fldCompareUsingY);
	AddColumn(DESIREDOUTCOME  , "Desired Outcome").Edit(desiredOutcomeList).SetConvert(desiredOutcomeList).Default("P");  // P)ass or F)ail
	AddColumn(ACTUALOUTCOME   , "Actual Outcome" );   // P)ass, F)ail, I)ndeterminate
	AddColumn(OUTPUTVALUE     , "Output"         );
	AddColumn(TASKID      , "Task", 50       ); // The task that owns this test
	AddColumn(PROCESSORDER, "processorder"   ).Hidden();  // Required for ordering
	
	AddColumn(IDTEST            , ""               ).Ctrls(MakeTestButton).Fixed(20).SetImage(CtrlImg::go_forward());
	AddColumn(IDTESTDUMMY       , ""               ).Fixed(1);
	
	built = true;
}

//==============================================================================================
// Purely for external use
int     TestGrid::GetTestId            (int row)							  { return Get(row, TESTID); }
void    TestGrid::SetTestId            (int row, int ptestId)			  {        Set(row, TESTID, ptestId); }
String  TestGrid::GetTestName          (int row)							  { return TrimBoth(Get(row, TESTNAME)); }
String  TestGrid::GetTestNote          (int row)             			  { return TrimBoth(Get(row, NOTE)); }
int     TestGrid::GetTestRelId         (int row)              			  { return Get(row, RELID); }
int     TestGrid::GetTestConnId        (int row)              			  { return Get(row, CONNID); }
int     TestGrid::GetTestTypId         (int row)               			  { return Get(row, TESTTYPID); }
// Special SqlBool type required in order to properly apostrophize '0' and '1'
SqlBool TestGrid::GetInvertComparison  (int row)       			  		  { return (SqlBool)Get(row, INVERTCOMPARISON); }
int     TestGrid::GetCompTypId         (int row)              			  { return Get(row, COMPTYPID); }
String  TestGrid::GetCompareUsingX     (int row)        				  	  { return Get(row, X); }
String  TestGrid::GetCompareUsingY     (int row)   					      { return Get(row, Y); }
String  TestGrid::GetDesiredOutcome    (int row)       					  { return Get(row, DESIREDOUTCOME); }
String  TestGrid::GetActualOutcome     (int row)        					  { return Get(row, ACTUALOUTCOME); } // Null returns INT_MIN, I think
void    TestGrid::SetActualOutcome     (int row, String pactualOutcome)	  {        Set(row, ACTUALOUTCOME, pactualOutcome); }
String  TestGrid::GetOutputValue       (int row)          				  { return Get(row, OUTPUTVALUE); }
void    TestGrid::SetOutputValue       (int row, String poutputValue)      {        Set(row, OUTPUTVALUE, poutputValue); }
int     TestGrid::GetTaskId            (int row)                           { return Get(row, TASKID); }
int     TestGrid::GetProcessOrder      (int row)                           { return Get(row, PROCESSORDER); }

//==============================================================================================
void TestGrid::SaveTestPrompt() {
	SaveTest(true);
}

//==============================================================================================
void TestGrid::SaveTestNoPrompt() {
	SaveTest(false);
}

void FieldLayoutRaw(FieldOperator& f, const String& prefix = String());

//==============================================================================================
void TestGrid::FieldLayout(FieldOperator& fo) {
	fo
		(TESTNAME        , Get(TESTNAME))
		(NOTE            , Get(NOTE))
		(RELID           , Get(RELID))
		(CONNID          , Get(CONNID))
		(DESIREDOUTCOME  , Get(DESIREDOUTCOME))
		(ACTUALOUTCOME   , Get(ACTUALOUTCOME))
		(X               , Get(X))
		(Y               , Get(Y))
		(INVERTCOMPARISON, Get(INVERTCOMPARISON))
		(COMPTYPID       , Get(COMPTYPID))
		(TESTTYPID       , Get(TESTTYPID))
		(TASKID          , Get(TASKID))
		(OUTPUTVALUE     , Get(OUTPUTVALUE))
		(PROCESSORDER    , Get(PROCESSORDER))
	;
}

//==============================================================================================
void TestGrid::SaveTest(bool prompt) {
	ASSERT(connection);
	ASSERT(connection->session->IsOpen());
	ASSERT(IsCursor());
	int row = GetCursor();
	
	if (IsNewRow()) {
		Set(PROCESSORDER, GetNextProcessOrder());
		SqlInsert s = SqlInsert(TESTS)(THISBACK(FieldLayout));
		SqlStatement st(s);
		String controlScript = st.Get(connection->GetDialect()); // Transform to proper Sql

		LOG(controlScript);
		int rsp = 1;
		
		if (prompt) {		
			rsp = PromptOKCancel(CAT << "Inserting Test: " << controlScript);
		}		
		
		if (rsp == 1) {
			if (!connection->SendAddDataScript(controlScript)) {
				// Beep
				return;
			}
			if (connection->GetRowsProcessed() > 0) {		
				int testid = connection->GetInsertedId("tests", "testid");
				if (testid >= 0) {
					SetTestId(row, testid);
				}
			}
		}

	// Updating an existing test
			
	} else {
		SqlUpdate s = SqlUpdate(TESTS)(THISBACK(FieldLayout)).Where(TESTID == Get(TESTID));
		SqlStatement st(s);
		String controlScript = st.Get(connection->GetDialect());

		LOG(controlScript);
		int rsp = 1;
		
		if (true || prompt) {		
			rsp = PromptOKCancel(CAT << "Updating Test: " << controlScript);
		}		
		
		if (rsp == 1) {
			if (!connection->SendChangeDataScript(controlScript)) {
				// Beep
				return;
			}
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
				", processorder"     // 14
				" from tests where testid >= 0 ";
				
	if (taskId != UNKNOWN) {
		script << " and taskId = " << taskId;
	}
	script << " order by testname";

	if (connection->SendQueryDataScript(script)) {
		while(connection->Fetch()) {
			Add();
			Set(TESTID             , connection->Get(0));
			Set(TESTNAME           , connection->Get(1));
			Set(NOTE               , connection->Get(2));
			Set(RELID        , connection->Get(3));
			Set(CONNID       , connection->Get(4));
			Set(DESIREDOUTCOME   , connection->Get(5));       // P)ass or F)ail
			Set(ACTUALOUTCOME    , connection->Get(6));       // P)ass, F)ail, I)ndeterminate
			Set(OUTPUTVALUE      , connection->Get(7));
			Set(X    , connection->Get(8));
			Set(Y    , connection->Get(9));
			Set(INVERTCOMPARISON , connection->GetBool(10));  // A "NOT" test.  We know its Boolean, but PostgreSQL driver does not.  I returns a string of 1 or 0
			Set(COMPTYPID        , connection->Get(11));
			Set(TESTTYPID        , connection->Get(12));
			Set(TASKID       , connection->Get(13));
			Set(PROCESSORDER, connection->Get(14));
		}
	}
	
	loaded = true;
}

//==============================================================================================
bool TestGrid::MeaningfulDataChange() {
	if (!IsModifiedRow()) return false; // No change
	
	if (
		!IsModified(TESTNAME)         &&
		!IsModified(NOTE)         &&
		!IsModified(RELID)        &&
		!IsModified(CONNID)       &&
		!IsModified(DESIREDOUTCOME)   &&
		!IsModified(ACTUALOUTCOME)    &&
		!IsModified(X)    &&
		!IsModified(Y)    &&
		!IsModified(INVERTCOMPARISON) &&
		!IsModified(TESTTYPID)        &&
		!IsModified(COMPTYPID)
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
