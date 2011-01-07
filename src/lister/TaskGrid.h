#ifndef _lister_TaskGrid_h_
#define _lister_TaskGrid_h_

#include "shared.h"

class TaskGrid : public UrpSqlGrid {
public:
	EditString taskName, taskNote, srcCode, projectName;

	TaskGrid() {
		SetTable(TASKS);
		AddKey(TASKID);
		AddColumn(TASKNAME, "task", 100).Edit(taskName);
		AddColumn(SRCCODE, "QC#", 100).Edit(srcCode);
		AddColumn(PROJECTNAME, "project", 100).Edit(projectName);
		AddColumn(NOTE, "tasknote", 100).Edit(taskNote);
	}
	
	void Build() {
	}
	
	void Load(Connection *pconnection) {
		SetSession(*(pconnection->session));
		SetSortColumn(TASKNAME);
		Query();
	
	}
};


#endif
