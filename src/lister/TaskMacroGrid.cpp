#include "TaskMacroGrid.h"
#include "Connection.h"

//==============================================================================================
TaskMacroGrid::TaskMacroGrid() : UrpSqlGrid() {
}

//==============================================================================================
void TaskMacroGrid::UpdatingRow() {
	// Called from WhenStartEdit from inside StartEdit	
	if (IsInsert()) { // Not IsInserting, this checks insertmode
		Set(TASKID, taskId);
	}
}

//==============================================================================================
	// Trick to ask DB (Postgresql) to populate with its default function
struct ForceDBDefaultConvert : public Convert {
	Value Format(const Value& v) const {
		if (IsNull(v))
			return "DEFAULT";
		else
			return v;
	}
};

//==============================================================================================
void TaskMacroGrid::Build(Connection *pconnection) {
	// Build columns
	connection = pconnection;
	SetTable(TASKMACROS);
	AddKey(TASKMACID);
	AddColumn(SEARCHFOR, "searchfor", 100).Edit(searchFor);
	AddColumn(REPLACEWITH, "replacewith", 100).Edit(replaceWith);
	// Problem: Column must be visible in order to use SortColumn, but then the DEFAULT
	// value defined in the db is not fetched and an NULL is pushed.
	AddColumn(PROCESSORDER, "order", 20);
	
	HideColumn(PROCESSORDER);
	AddColumn(NOTE, "note", 200).Edit(note);
	AddIndex(TASKID);
	WhenAcceptRow = THISBACK(CompleteNewRow);
	built = true;
	// DoInsert()->DoEdit()->StartEdit()
	WhenStartEdit = THISBACK(UpdatingRow);
}

//==============================================================================================
bool TaskMacroGrid::CompleteNewRow() {
	Value currProcessOrder = Get(PROCESSORDER);
	if (currProcessOrder.IsNull()) {
		Set(PROCESSORDER, GetNextProcessOrder());
	}
	
	return true;
}

//==============================================================================================
void TaskMacroGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
	SetWhere((TASKID == taskId));
}

//==============================================================================================
void TaskMacroGrid::Load() {
	Clear(); // Just clear old content, in case its hanging around from last open task
	ClearSelection();
	ClearCache();
	SetSession(*(connection->session));
	SetSortColumn(PROCESSORDER); // List in order that they will be processed; very important since they are interdependent.
	Query();
	loaded = true;
}

//==============================================================================================
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
