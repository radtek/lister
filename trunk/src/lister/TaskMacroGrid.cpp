#include "TaskMacroGrid.h"

#include "Connection.h"

TaskMacroGrid::TaskMacroGrid() : UrpSqlGrid() {
}

void TaskMacroGrid::UpdatingRow() {
	// Called from WhenStartEdit from inside StartEdit	
	if (IsInsert()) { // Not IsInserting, this checks insertmode
		Set(TASKID, taskId);
	}
}
void TaskMacroGrid::Build(Connection *pconnection) {
	// Build columns
	connection = pconnection;
	SetTable(TASKMACROS);
	AddKey(TASKMACID);
	AddColumn(SEARCHFOR, "searchfor", 100).Edit(searchFor);
	AddColumn(REPLACEWITH, "replacewith", 100).Edit(replaceWith);
	AddColumn(NOTE, "note", 200).Edit(note);
	AddIndex(TASKID);
	built = true;
	// DoInsert()->DoEdit()->StartEdit()
	WhenStartEdit = THISBACK(UpdatingRow);
}

void TaskMacroGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
	SetWhere((TASKID == taskId));
}

void TaskMacroGrid::Load() {
	SetSession(*(connection->session));
	SetSortColumn(PROCESSORDER);
	Query();
	loaded = true;
}

/*static */void TaskMacroGrid::LoadTaskMacro(Connection *connection, DropGrid &dropGrid) {
	
	if (!connection->SendQueryDataScript("select elementid, elementname, elementdesc, taskid, note, status from elements order by elementname")) {
		return;
	}
	
	dropGrid.Clear();
	while(connection->Fetch()) {
		dropGrid.Add(connection->Get(0), connection->Get(1));
	}

	dropGrid.SetKeyColumn(0);
	dropGrid.Add(Null, Null); // Make sure we support a null value or we won't be able to edit other attributes without assigning a connection, which isn't what we want.
}
