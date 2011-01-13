#include "TaskGrid.h"
#include "Connection.h"
#include "shared_db.h"

//==========================================================================================	
TaskGrid::TaskGrid() {
	SetTable(TASKS);
	AddKey(TASKID);
	AddColumn(TASKNAME, "task", 100).Edit(taskName);
	AddColumn(SRCCODE, "QC#", 100).Edit(srcCode);
	AddColumn(PROJECTNAME, "project", 100).Edit(projectName);
	AddColumn(NOTE, "tasknote", 100).Edit(taskNote);
}

//==========================================================================================	
void TaskGrid::Build() {
}

//==========================================================================================	
void TaskGrid::Load(Connection *pconnection) {
	SetSession(*(pconnection->session));
	SetSortColumn(TASKNAME);
	Query();
}

//==========================================================================================	
int TaskGrid::GetTaskId(int row) {
	return Get(row, TASKID);
}



