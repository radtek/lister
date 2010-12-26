#ifndef _TestGrid_h_
#define _TestGrid_h_

#include "shared.h"
#include "TestState.h"
#include "TestButton.h"
#include "Err.h"
#include "ConnectedCtrl.h"

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
class TestGrid : public TopWindow, ConnectedCtrl {
public:
	EditInt fldTestId;
	EditStringNotNull fldTestName;               // 0) A meaningful name; too many tests to track without a name
	EditString fldTestNote;
	EditString fldCompareUsingX;
	EditString fldCompareUsingY;
	
	DropList connList;
	DropList scriptList;
	DropList invertCompList;
	DropList desiredOutcomeList;
	DropList envList;
	DropList testTypList;
	DropList compTypList;
	GridCtrl grid;

	typedef TestGrid CLASSNAME;
	
	//==========================================================================================	
	TestGrid() {
	}
	
	//==========================================================================================	
	void NewTest() {
		// Added manually from appending a row.
	}
	
	//==========================================================================================	
	//  Added from the Script Editor
	void AddTest(String script, int scriptId, int connId) {
		grid.Add();
		grid.Set(IDTestScriptId, scriptId);
		grid.Set(IDTestConnId, connId);
	}
	
	//==========================================================================================	
	void Build() {
		grid.Appending().Removing().Editing();
		grid.EditsInNewRow().Accepting().Moving();
		grid.MovingCols().MovingRows().Hiding().Duplicating().Canceling().AskRemove();
		grid.Duplicating().Searching().SearchDisplay().SearchImmediate().SearchHideRows().SearchMoveCursor();
		grid.Closing().Inserting().RowChanging().ResizingRows();
		grid.LiveCursor();
		grid.RejectNullRow(false);
		grid.Clipboard();
		grid.WhenNewRow = THISBACK(NewTest);
		grid.Proportional();
		grid.ColorRows(true);
		grid.FixedPaste();

		Add(grid);
		grid.AddColumn(IDTestState, "").Ctrls(MakeTestState).Default(TestState::ConvertStateToColor(NOTEST_NEVER)).Fixed(16);
		grid.AddColumn(IDTestId, "Id").Edit(fldTestId).Default(Null);
		grid.AddColumn(IDTestName, "Name").Edit(fldTestName);
		grid.AddColumn(IDTestScriptId, "Script").Edit(scriptList).SetConvert(scriptList).Default(-1);
		grid.AddColumn(IDTestNote, "Note").Edit(fldTestNote);
		grid.AddColumn(IDTestConnId, "Conn").Edit(connList).SetConvert(connList).Default(-1);
		grid.AddColumn(IDTestTypId, "Type").Edit(testTypList).SetConvert(testTypList).Default(-1);
		grid.AddColumn(IDCompTypId, "Type").Edit(compTypList).SetConvert(compTypList).Default(-1);
		grid.AddColumn(IDDesiredOutcome, "Desired Outcome").Edit(desiredOutcomeList).SetConvert(desiredOutcomeList).Default(0);  // P)ass or F)ail
		grid.AddColumn(IDActualOutcome, "Actual Outcome");   // P)ass, F)ail, I)ndeterminate
		grid.AddColumn(IDOutputValue, "Output");
		grid.AddColumn(IDCompareUsingX, "X").Edit(fldCompareUsingX);
		grid.AddColumn(IDCompareUsingY, "Y").Edit(fldCompareUsingY);
		grid.AddColumn(IDInvertComparison, "NOT?").Edit(invertCompList).SetConvert(invertCompList).Default("");
		grid.AddColumn(IDTEST, "Test!").Ctrls(MakeTestButton).Fixed(20).SetImage(CtrlImg::go_forward());
		grid.AddColumn(IDTESTDUMMY, "").Fixed(1);
		
		grid.SizePos();
	}
	
	//==========================================================================================	
	virtual void Load(Connection *pconnection) {
		ConnectedCtrl::Load(pconnection);

		Sql sql(connection->GetSession());
				
		desiredOutcomeList.Add(0,"Pass");
		desiredOutcomeList.Add(1, "Fail");
		invertCompList.Add(0, "");
		invertCompList.Add(1, "NOT");
		
		if (connection->SendQueryDataScript(sql, "select comptypid, comptypname from comptyps")) {
			while(sql.Fetch()) {
				compTypList.Add(atoi(sql[0].ToString()), sql[1].ToString());
			}
		}

		if (connection->SendQueryDataScript(sql, "select testtypid, testtypname from testtyps")) {
			while(sql.Fetch()) {
				testTypList.Add(atoi(sql[0].ToString()), sql[1].ToString());
			}
		}

		if (connection->SendQueryDataScript(sql, "select scriptid, script from scripts")) {
			while(sql.Fetch()) {
				scriptList.Add(atoi(sql[0].ToString()), sql[1].ToString());
			}
		}

		if (connection->SendQueryDataScript(sql, "select connid, connname from connections order by connname")) {
			while(sql.Fetch()) {
				connList.Add(atoi(sql[0].ToString()), sql[1].ToString());
			}
		}
	}
};

#endif
