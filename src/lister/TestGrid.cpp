#include "TestGrid.h"
#include <CtrlLib/CtrlLib.h>
#include "TestState.h"
#include "TestButton.h"
#include "Connection.h"
#include "shared_db.h"
#include "ContactGrid.h"

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
		
	AddColumn(IDTestState       , ""               ).Ctrls(MakeTestState)    .Default(TestState::ConvertStateToColor(NOTEST_NEVER)).Fixed(16).CalculatedColumn();
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
	AddColumn(ACTUALOUTCOME   , "Actual Outcome" ).Edit(actualOutcomeList).SetConvert(actualOutcomeList);   // P)ass, F)ail, I)ndeterminate
	AddColumn(OUTPUTVALUE     , "Output"         );
	AddColumn(TASKID      , "Task", 50       ); // The task that owns this test
	AddColumn(PROCESSORDER, "processorder"   ).Hidden();  // Required for ordering
	AddColumn(ASSIGNTOWHO, "assgnd to"   ).Edit(assignToWhoList).SetConvert(assignToWhoList).Default(-1);
	AddColumn(LASTRUNWHEN, "runtime"     ).Edit(fldLastRunWhen).Locked(true);
	AddColumn(IDTEST            , ""               ).Ctrls(MakeTestButton).Fixed(20).SetImage(CtrlImg::go_forward()).CalculatedColumn();
	AddColumn(IDTESTDUMMY       , ""               ).Fixed(1).CalculatedColumn();
	
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
bool    TestGrid::GetInvertComparison  (int row)       			  		  { return Get(row, INVERTCOMPARISON); }
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
void    TestGrid::SetLastRunWhen       (int row, Time plastRunWhen) { Set(row, LASTRUNWHEN, plastRunWhen); }

//
//==============================================================================================
void TestGrid::SaveTestPrompt() {
	SaveTest(true);
}

//==============================================================================================
void TestGrid::SaveTestNoPrompt() {
	SaveTest(false);
}

//==============================================================================================
// Easily transform all grid columns to SQL Db columns if flagged as linked to db.
// This is a callback as defined "SqlInsert& operator()(Fields f, bool nokey = false);"
// Where Fields is just a sneaky "typedef Callback1<FieldOperator&> Fields;" in sqlexp.h!
// This is also used for 
void TestGrid::FieldLayout(FieldOperator& fo) {
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		GridCtrl::ItemRect& ir = GetFloatingColumn(i);
		if (ir.linkedtodbcol == 1) {
			// Call to "FieldOperator& operator()(Id id, Ref f) " in Sqlexp.h
			// Very complex to figger out.
			if (IsCursor()) {
				fo(GetFloatingColumnId(i), Get(GetFloatingColumnId(i)));
			} else if(IsSelection()) {
				fo(GetFloatingColumnId(i), Get(GetFirstSelection(), GetFloatingColumnId(i)));
			} else {
				// No row selected
				fo(GetFloatingColumnId(i), Value(Null));
			}
		}
	}
}

//==============================================================================================
void TestGrid::SaveTest(bool prompt) {
	ASSERT(connection);
	ASSERT(connection->session->IsOpen());
	ASSERT(IsCursor());
	int row = GetCursor();
	SqlStatement sts; 
	
	// Inserting a new test
	
	if (IsNewRow()) {
		Set(PROCESSORDER, GetNextProcessOrder());
		sts = SqlStatement(SqlInsert(TESTS)(THISBACK(FieldLayout)));

	// Updating an existing test
			
	} else {
		sts = SqlStatement(SqlUpdate(TESTS)(THISBACK(FieldLayout)).Where(TESTID == Get(TESTID)));
	}
	
	// Wrap in a SqlStatement object, which will somehow either call the SqlUpdate or
	// SqlInsert operator SqlStatement() const, which then constructs a string and forms
	// the constructor "explicit SqlStatement(const String& s) : text(s) {}", and so
	// sets the text member.  I don't know what the explicit command means.
	
	
	// Transform to proper Sql; Sql will not be properly formed until the dialect of the 
	// connection is determined, in this case PGSQL.  Do not use GetText().
	// This calls SqlCompile(dialect, text) to finalize the "text" member contents.
	String controlScript = sts.Get(connection->GetDialect());

	LOG(controlScript);
	int rsp = 1;
	
	if (prompt) {		
		rsp = PromptOKCancel(CAT << "Script being applied: " << controlScript);
	}		
	
	if (rsp == 1) {
		if (!connection->SendAddDataScript(controlScript)) {
			// Beep
			return;
		}
		
		if (IsNewRow()) {
			if (connection->GetRowsProcessed() > 0) {		
				int testid = connection->GetInsertedId("tests", "testid");
				if (testid >= 0) {
					SetTestId(row, testid);
				}
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

	actualOutcomeList.Clear();
	actualOutcomeList.Add("P", "Pass");
	actualOutcomeList.Add("F", "Fail");
	actualOutcomeList.Add("I", "Indeterminate");
	
	invertCompList.Clear();
	invertCompList.Add(Value((bool)false), "");
	invertCompList.Add(Value((bool)true), "NOT");

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

	ContactGrid::LoadContact(connection, assignToWhoList);
	ContactGrid::BuildContactList(assignToWhoList);
	
	// Load all the tests for current task
	
	Clear();
	
	SqlSelect s = SqlSelect(THISBACK(FieldLayout)).From(TESTS);
	SqlBool filter(TESTID >= 0);
	
	if (taskId != UNKNOWN) {
		filter&= TASKID == taskId;
	}
	
	
	String controlScript = SqlStatement(s.Where(filter).OrderBy(TESTNAME)).Get(connection->GetDialect());
	LOG(controlScript);

	if (connection->SendQueryDataScript(controlScript)) {
		while(connection->Fetch()) {
			Add();
			for (int i = 0; i < GetFloatingColumnCount(); i++) {
				GridCtrl::ItemRect& ir = GetFloatingColumn(i);
				if (ir.linkedtodbcol == 1) {
					Set(GetFloatingColumnId(i), connection->Get((SqlId)GetFloatingColumnId(i)));
				}
			}
		}
	}
	
	loaded = true;
}

//==============================================================================================
bool TestGrid::MeaningfulDataChange() {
	if (!IsModifiedRow()) return false; // No change
	
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		GridCtrl::ItemRect& ir = GetFloatingColumn(i);
		if (ir.linkedtodbcol == 1) {
			if (!IsModified(GetFloatingColumnId(i))) {
				return false;
			}
		}
	}

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
