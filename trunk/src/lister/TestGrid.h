#ifndef _TestGrid_h_
#define _TestGrid_h_

#include "shared.h"
#include "TestGrid.h"
#include "TestState.h"
#include "TestButton.h"
#include "ConnectedCtrl.h"
#include <Urp/Urp.h>

// Taken from testtyps table

#define TESTTYP_UNKNOWN                              -1
#define TESTTYP_RETURNED_ROW_COUNT                    1
#define TESTTYP_SELECTED_ROW_COUNT                    2
#define TESTTYP_ERROR_CODE                            3
#define TESTTYP_GENERIC_ERROR_CODE                    4
#define TESTTYP_FIRST_COLUMN_VALUE                    5
#define TESTTYP_FIRST_COLUMNS_DATATYPE                6
#define TESTTYP_FIRST_COLUMNS_GENERIC_DATATYPE        7
#define TESTTYP_FIRST_COLUMNS_DATALENGTH              8
#define TESTTYP_EXECUTION_TIME                        9
#define TESTTYP_PARSE_ERROR_CODE                     10
#define TESTTYP_PARSE_GENERIC_ERROR_CODE             11
#define TESTTYP_SCRIPT_TEXT                          12
#define TESTTYP_SCRIPT_TEXT_CHARACTER_COUNT          13
#define TESTTYP_SCRIPT_TEXT_WORD_COUNT               14
#define TESTTYP_SCRIPT_TEXT_LINE_COUNT               15
#define TESTTYP_SCRIPT_TEXT_LINE_WORD_COUNT          16
#define TESTTYP_SCRIPT_TEXT_LINE_CHARACTER_COUNT     17

// Comparison types when testing (see tests table)
#define COMPTYP_UNKNOWN                   -1
#define COMPTYP_GREATER_THAN_X             1
#define COMPTYP_EQUAL_TO                   2
#define COMPTYP_LESS_THAN_X                3
#define COMPTYP_GREATER_THAN_OR_EQUAL_TO_X 4
#define COMPTYP_LESS_THAN_OR_EQUAL_TO_X    5
#define COMPTYP_STARTS_WITH_X              6
#define COMPTYP_START_WITH_X_AFTER_TRIM    7
#define COMPTYP_ENDS_WITH_X                8
#define COMPTYP_ENDS_WITH_X_AFTER_TRIM     9
#define COMPTYP_IN_LIST_X                 10
#define COMPTYP_IN_SERIES_X               11
#define COMPTYP_CONTAINED_WITHIN_X        12
#define COMPTYP_CONTAINS_X                13
#define COMPTYP_BETWEEN_X_AND_Y           14
#define COMPTYP_WITHIN_X_PCT_OF_Y         15
#define COMPTYP_WITHIN_X_OF_Y             16

//==========================================================================================	
Id IDTestState("State");
Id IDTestId("TestId");
Id IDTestName("TestName");
Id IDTestScriptId("TestScriptId");
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
Id IDTEST("TEST!");
Id IDTESTDUMMY("Err");

//==========================================================================================	
void MakeTestButton(One<Ctrl>& ctrl) {
	ctrl.Create<TestButton>();
	ctrl->WantFocus();
}

//==========================================================================================	
void MakeTestState(One<Ctrl>& ctrl) {
	ctrl.Create<TestState>();
	ctrl->WantFocus();
}

//==========================================================================================	
class TestGrid : public UrpGrid, ConnectedCtrl {
public:
	EditInt fldTestId;
	EditStringNotNull fldTestName;               // 0) A meaningful name; too many tests to track without a name
	EditString fldTestNote;
	EditString fldCompareUsingX;
	EditString fldCompareUsingY;
	
	DropGrid connList;
	DropGrid scriptList;
	DropGrid invertCompList;
	DropGrid desiredOutcomeList;
	DropGrid envList;
	DropGrid testTypList;
	DropGrid compTypList;

	typedef TestGrid CLASSNAME;
	
	bool built;
	bool loaded;
	
	//==========================================================================================	
	TestGrid() {
		built = false;
	}
	
	//==========================================================================================	
	// Added manually from appending a row.
	void NewTest() {
	}
	
	//==========================================================================================	
	// GridCtrl will remove the row if we do not cancel the remove.
	void RemoveTest() {
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
	
	//==========================================================================================	
	// Added from the Script Editor, which only provides these three pieces of info.
	// Create a bare-bones frame for a new test based on a script with a valid scriptid.
	void AddTest(String script, int scriptId, int connId) {
		Add();
		if (connection->SendQueryDataScript("select scriptid, script from scripts")) {
			scriptList.Clear();
			while(connection->Fetch()) {
				scriptList.Add(connection->Get(0), connection->Get(1));
			}
		}
		
		Set(IDTestScriptId, scriptId);
		Set(IDTestConnId, connId);
	}
	
	//==========================================================================================	
	void Build() {
		WhenNewRow = THISBACK(NewTest);
		WhenRemoveRow = THISBACK(RemoveTest);
		
		
		// Make the dropgrids of values extend past the width of the column they are for, to improve readability without wasting grid space.
		connList.SearchHideRows().Resizeable().Width(200);
		scriptList.SearchHideRows().Resizeable().Width(200);
		testTypList.SearchHideRows().Resizeable().Width(200);
		compTypList.SearchHideRows().Resizeable().Width(200);
		
		AddColumn(IDTestState, "").Ctrls(MakeTestState).Default(TestState::ConvertStateToColor(NOTEST_NEVER)).Fixed(16);
		AddColumn(IDTestId, "Id").Edit(fldTestId).Default(Null);
		AddColumn(IDTestName, "Name").Edit(fldTestName);
		AddColumn(IDTestScriptId, "Script").Edit(scriptList).SetConvert(scriptList).Default(-1);
		AddColumn(IDTestNote, "Note").Edit(fldTestNote);
		AddColumn(IDTestConnId, "Conn").Edit(connList).SetConvert(connList).Default(-1);
		AddColumn(IDTestTypId, "Type").Edit(testTypList).SetConvert(testTypList).Default(-1);
		AddColumn(IDInvertComparison, "NOT?").Edit(invertCompList).SetConvert(invertCompList).Default(false);
		AddColumn(IDCompTypId, "Type").Edit(compTypList).SetConvert(compTypList).Default(-1);
		AddColumn(IDCompareUsingX, "X").Edit(fldCompareUsingX);
		AddColumn(IDCompareUsingY, "Y").Edit(fldCompareUsingY);
		AddColumn(IDDesiredOutcome, "Desired Outcome").Edit(desiredOutcomeList).SetConvert(desiredOutcomeList).Default("P");  // P)ass or F)ail
		AddColumn(IDActualOutcome, "Actual Outcome");   // P)ass, F)ail, I)ndeterminate
		AddColumn(IDOutputValue, "Output");
		AddColumn(IDTEST, "Test!").Ctrls(MakeTestButton).Fixed(20).SetImage(CtrlImg::go_forward());
		AddColumn(IDTESTDUMMY, "").Fixed(1);
		WhenAcceptedRow = THISBACK(SaveTest);
		SizePos();
		built = true;
	}
	
	//==========================================================================================	
	int GetTestId(int row)					{ return Get(row, IDTestId); }
	void SetTestId(int row, int ptestId)	{ Set(row, IDTestId, ptestId); }
	String GetTestName(int row)				{ return TrimBoth(Get(row, IDTestName)); }
	String GetTestNote(int row)             { return TrimBoth(Get(row, IDTestNote)); }
	int GetTestScriptId(int row)            { return Get(row, IDTestScriptId); }
	int GetTestConnId(int row)              { return Get(row, IDTestConnId); }
	int GetTestTypId(int row)               { return Get(row, IDTestTypId); }
	bool GetInvertComparison(int row)       { return Get(row, IDInvertComparison); }
	int GetCompTypId(int row)               { return Get(row, IDCompTypId); }
	String GetCompareUsingX(int row)        { return Get(row, IDCompareUsingX); }
	String GetCompareUsingY(int row)        { return Get(row, IDCompareUsingY); }
	String GetDesiredOutcome(int row)       { return Get(row, IDDesiredOutcome); }
	String GetActualOutcome(int row)        { return Get(row, IDActualOutcome); } // Null returns INT_MIN, I think
	String GetOutputValue(int row)          { return Get(row, IDOutputValue); }
	
	
	//==========================================================================================	
	void SaveTest() {
		ASSERT(connection);
		ASSERT(connection->session->IsOpen());
		ASSERT(IsCursor());
		int row = GetCursor();

		String s = GetDesiredOutcome(row);
		
		String tokenizedActualOutcome = GetActualOutcome(row) == "" ? "NULL" : UrpString::Apostrophize(GetActualOutcome(row));
		String tokenizedDesiredOutcome = GetDesiredOutcome(row) == "" ? "NULL" : UrpString::Apostrophize(GetDesiredOutcome(row));
		String tokenizedInvertComparison = GetInvertComparison(row) ? "T" : "F";
		String tokenizedCompareUsingY = GetCompareUsingY(row);
		String testName = GetTestName(row);
		String testNote = GetTestNote(row);
		int testScriptId = GetTestScriptId(row);
		int testConnId = GetTestConnId(row);

		if (IsNewRow()) {
		
			// Determine if teset already exists
			String controlScript = Format(
				"insert into tests(testname, note, scriptid, connid, desiredoutcome, actualoutcome, outputvalue, x,    y,    invertcomparison, comptypid, testtypid) "
				"values(          '%s',     '%s',  %d,       %d,     %s,             %s,            '%s',        '%s', '%s', '%s',             %d,        %d)"
					,             connection->PrepTextDataForSend(testName)
					,                       connection->PrepTextDataForSend(testNote)
					,                              testScriptId
					,                                        testConnId
					,                                                tokenizedDesiredOutcome // apostrophize at token level in case a NULL is required
					,                                                                tokenizedActualOutcome // apostrophize at token level in case a NULL is required
					,                                                                               connection->PrepTextDataForSend(GetOutputValue(row))
					,                                                                                            connection->PrepTextDataForSend(GetCompareUsingX(row))
					,                                                                                                  connection->PrepTextDataForSend(GetCompareUsingY(row))
					,                                                                                                        tokenizedInvertComparison
					,                                                                                                                          GetCompTypId(row)
					,                                                                                                                                     GetTestTypId(row)
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
			// Update
			int testId = GetTestId(row);
			String controlScript = Format(
				"UPDATE tests set"
			 		"  testname = '%s'"
			 		", note = '%s', scriptid = %d, connid = %d, desiredoutcome = %s, actualoutcome = %s, outputvalue = '%s'"
			 		", x = '%s', y = '%s', invertcomparison = '%s', comptypid = %d, testtypid = %d"
			     	" WHERE testid = %d"
					,             connection->PrepTextDataForSend(testName)
					,                       connection->PrepTextDataForSend(testNote)
					,                              testScriptId
					,                                        testConnId
					,                                                tokenizedDesiredOutcome // apostrophize at token level in case a NULL is required
					,                                                                tokenizedActualOutcome // apostrophize at token level in case a NULL is required
					,                                                                               connection->PrepTextDataForSend(GetOutputValue(row))
					,                                                                                            connection->PrepTextDataForSend(GetCompareUsingX(row))
					,                                                                                                  connection->PrepTextDataForSend(GetCompareUsingY(row))
					,                                                                                                        tokenizedInvertComparison
					,                                                                                                                          GetCompTypId(row)
					,                                                                                                                                     GetTestTypId(row)
					,                testId
					);
			     
			LOG(controlScript);
			
			int rsp = PromptOKCancel(CAT << "Updating Test: " << controlScript);
			if (rsp == 1) {
				connection->SendChangeDataScript(controlScript);
			}
		}
	}
	
	//==========================================================================================	
	virtual void Load(Connection *pconnection) {
		ConnectedCtrl::Load(pconnection);

		desiredOutcomeList.Add("P","Pass");
		desiredOutcomeList.Add("F", "Fail");
		invertCompList.Add(false, "");
		invertCompList.Add(true, "NOT");
		
		if (connection->SendQueryDataScript("select comptypid, comptypname from comptyps")) {
			while(connection->Fetch()) {
				compTypList.Add(connection->Get(0), connection->Get(1));
			}
		}

		if (connection->SendQueryDataScript("select testtypid, testtypname from testtyps")) {
			while(connection->Fetch()) {
				testTypList.Add(connection->Get(0), connection->Get(1));
			}
		}

		if (connection->SendQueryDataScript("select scriptid, script from scripts")) {
			while(connection->Fetch()) {
				scriptList.Add(connection->Get(0), connection->Get(1));
			}
		}

		if (connection->SendQueryDataScript("select connid, connname from connections order by connname")) {
			while(connection->Fetch()) {
				connList.Add(connection->Get(0), connection->Get(1));
			}
		}

		if (connection->SendQueryDataScript(
				"select "
					"  testid"           // 0
					", testname"         // 1
					", note"             // 2
					", scriptid"         // 3
					", connid"           // 4
					", desiredoutcome"   // 5
					", actualoutcome"    // 6
					", outputvalue"      // 7
					", x"                // 8
					", y"                // 9
					", invertcomparison" // 10: Boolean
					", comptypid"        // 11
					", testtypid"        // 12
					" from tests  where testid >= 0 order by testname")) {
			while(connection->Fetch()) {
				Add();
				Set(IDTestId          , connection->Get(0));
				Set(IDTestName        , connection->Get(1));
				Set(IDTestNote        , connection->Get(2));
				Set(IDTestScriptId    , connection->Get(3));
				Set(IDTestConnId      , connection->Get(4));
				Set(IDDesiredOutcome  , connection->Get(5));  // P)ass or F)ail
				Set(IDActualOutcome   , connection->Get(6));   // P)ass, F)ail, I)ndeterminate
				Set(IDOutputValue     , connection->Get(7));
				Set(IDCompareUsingX   , connection->Get(8));
				Set(IDCompareUsingY   , connection->Get(9));
				Set(IDInvertComparison, connection->GetBool(10)); // A "NOT" test.  We know its Boolean, but PostgreSQL driver does not.  I returns a string of 1 or 0
				Set(IDTestTypId       , connection->Get(11));
				Set(IDCompTypId       , connection->Get(12));
			}
		}
		
		loaded = true;
	}

	//==========================================================================================	
	bool MeaningfulDataChange() {
		if (!IsModifiedRow()) return false; // No change
		
		if (
			!IsModified(IDTestName)         &&
			!IsModified(IDTestNote)         &&
			!IsModified(IDTestScriptId)     &&
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

	//==========================================================================================	
	bool WasTestRequested() {
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

};

#endif
