#include "LinkGrid.h"
#include "Connection.h"

//==============================================================================================
LinkGrid::LinkGrid() : UrpSqlGrid() {
}

//==============================================================================================
void LinkGrid::UpdatingRow() {
	// Called from WhenStartEdit from inside StartEdit	
	if (IsInsert()) { // Not IsInserting, this checks insertmode
		Set(TASKID, taskId);
	}
}

//==============================================================================================
void LinkGrid::Build(Connection *pconnection) {
	// Build columns
	connection = pconnection;
	SetTable(LINKS);
	AddKey(LINKID);
	AddColumn(LINKNAME, "link", 100).Edit(linkName);
	AddColumn(LINKURL, "url", 100).Edit(linkUrl);
	AddColumn(NOTE, "note", 200).Edit(note);
	AddIndex(TASKID);
	built = true;
	// DoInsert()->DoEdit()->StartEdit()
	WhenStartEdit = THISBACK(UpdatingRow);
}

//==============================================================================================
void LinkGrid::Load() {
	SetSession(*(connection->session));
	SetSortColumn(ELEMENTNAME);
	Query();
	loaded = true;
}
//==============================================================================================
void LinkGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
	SetWhere((TASKID == taskId));
}

