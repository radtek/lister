#include "AnalysisByDate.h"
#include "Connection.h"

//==============================================================================================
AnalysisByDateGrid::AnalysisByDateGrid() : UrpSqlGrid() {
}

//==============================================================================================
void AnalysisByDateGrid::UpdatingRow() {
	// Called from WhenStartEdit from inside StartEdit	
	if (IsInsert()) { // Not IsInserting, this checks insertmode
		Set(TASKID, taskId);
	}
}

//==============================================================================================
void AnalysisByDateGrid::Build(Connection *pconnection) {
	// Build columns
	connection = pconnection;
	SetTable(ANALYSISBYDATE);
	AddKey(ANALYSISID);
	AddColumn(ANALYSISNAME, "analysis", 100).Edit(analysisName);
	//AddColumn(ENVID, "analysis", 100).Edit(analysisName);
	// Should have a default connection at sumary level
	AddColumn(BYDATE, "by date", 100).Edit(byDate);
	// BYDATERULE/MACRO
	AddColumn(TOTALCOUNT, "ttl cnt", 100).Edit(totalCount);
	AddColumn(NOTE, "note", 200).Edit(note);
	AddIndex(TASKID);
	built = true;
	// DoInsert()->DoEdit()->StartEdit()
	WhenStartEdit = THISBACK(UpdatingRow);
}

//==============================================================================================
void AnalysisByDateGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
	SetWhere((TASKID == taskId));
}

//==============================================================================================
void AnalysisByDateGrid::Load() {
	SetSession(*(connection->session));
	SetSortColumn(ANALYSISNAME);
	Query();
	loaded = true;
}

