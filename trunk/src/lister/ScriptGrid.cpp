#include "ScriptGrid.h"
#include "Connection.h"
#include "shared_db.h"

//==========================================================================================	
ScriptGrid::ScriptGrid() {
	SetTable(TASKS_R); // All relations to a task, regardless of type
	AddKey(RELID);
	AddColumn(TASKID, "taskid", 50);
	HideColumn(TASKID); // For joining to task grid only.
	AddColumn(WHY, "why", 160).Edit(why);
	AddColumn(SCRIPT, "script", 200);
	AddColumn(SCRIPTNAME, "script", 140).Edit(scriptName);
	AddColumn(SCRIPTNOTE, "script note", 200).Edit(scriptNote);
	AddColumn(SCRIPTID, "scriptid", 50);
}

//==========================================================================================	
void ScriptGrid::Build() {
}

//==========================================================================================	
void ScriptGrid::Load(Connection *pconnection) {
	SetSession(*(pconnection->session));
	SetSortColumn(SCRIPTNAME);
	Query();
}

//==========================================================================================	
// Link Many table (this) to one table
void ScriptGrid::LinkScriptToTaskGrid(ArrayCtrl &taskGrid) {
	Join(TASKID, taskGrid);
}

//==========================================================================================	
int ScriptGrid::GetScriptId(int row) {
	return Get(row, SCRIPTID);
}

//==========================================================================================	
String ScriptGrid::GetScript(int row) {
	return Get(row, SCRIPT);
}

