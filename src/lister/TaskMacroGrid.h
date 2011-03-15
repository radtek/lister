#ifndef _lister_lister_TaskMacroGrid_h_
#define _lister_lister_TaskMacroGrid_h_

#include "shared.h"
#include "shared_db.h"
class Connection;

//==============================================================================================
class TaskMacroGrid : public UrpSqlGrid {
public:
	typedef TaskMacroGrid CLASSNAME;
	
	int            taskId;
	EditString     searchFor, replaceWith, expansion, note, processOrder;
	Connection    *connection; // Our metadata about the query we will be running
	
	               TaskMacroGrid();
	void           UpdatingRow();
	void           Build(Connection *pconnection);
	void           Load();
	void           SetTaskId(int ptaskId);
	bool           CompleteNewRow();
	static void    LoadTaskMacro(Connection *connection, DropGrid &dropGrid);
};

#endif
