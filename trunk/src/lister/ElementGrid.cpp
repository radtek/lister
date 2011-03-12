#include "ElementGrid.h"
#include "Connection.h"

//==============================================================================================
ElementGrid::ElementGrid() : UrpSqlGrid() {
}

//==============================================================================================
void ElementGrid::UpdatingRow() {
	// Called from WhenStartEdit from inside StartEdit	
	if (IsInsert()) { // Not IsInserting, this checks insertmode
		Set(TASKID, taskId);
	}
}

//==============================================================================================
void ElementGrid::Build(Connection *pconnection) {
	// Build columns
	connection = pconnection;
	SetTable(ELEMENTS);
	AddKey(ELEMENTID);
	AddColumn(ELEMENTNAME, "element", 100).Edit(elementName);
	AddColumn(ELEMENTDESC, "desc", 100).Edit(elementDesc);
	AddColumn(STATUS, "status", 100).Edit(status);
	AddColumn(NOTE, "note", 200).Edit(note);
	AddIndex(TASKID);
	built = true;
	// DoInsert()->DoEdit()->StartEdit()
	WhenStartEdit = THISBACK(UpdatingRow);
}

//==============================================================================================
void ElementGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
	SetWhere((TASKID == taskId));
}

//==============================================================================================
void ElementGrid::Load() {
	SetSession(*(connection->session));
	SetSortColumn(ELEMENTNAME);
	Query();
	loaded = true;
}

//==============================================================================================
/*static */void ElementGrid::LoadElement(Connection *connection, DropGrid &dropGrid) {
	
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
