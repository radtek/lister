#ifndef _lister_TaskGrid_h_
#define _lister_TaskGrid_h_

#include "shared.h"

class TaskGrid : public UrpSqlGrid {
public:
	EditString taskName, taskNote, srcCode, projectName;

	TaskGrid();
	void Build();
	void Load(Connection *pconnection);
};


#endif
