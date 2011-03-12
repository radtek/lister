#ifndef _lister_lister_LinkGrid_h_
#define _lister_lister_LinkGrid_h_

#include "shared.h"
#include "shared_db.h"

class Connection;

//==============================================================================================
class LinkGrid : public UrpSqlGrid {
public:
	typedef LinkGrid CLASSNAME;
	
	int            taskId;
	EditString     linkName, linkUrl, note;
	Connection    *connection; // Our metadata about the query we will be running
	
	               LinkGrid();
	void           UpdatingRow();
	void           Build(Connection *pconnection);
	void           Load();
	void           SetTaskId(int ptaskId);
};

#endif
