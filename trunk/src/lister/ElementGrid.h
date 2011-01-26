#ifndef _lister_lister_ElementGrid_h_
#define _lister_lister_ElementGrid_h_

#include "shared.h"
#include "shared_db.h"
class Connection;

class ElementGrid : public UrpSqlGrid {
public:
	typedef ElementGrid CLASSNAME;
	
	int            taskId;
	EditString     elementName, elementDesc, status, note;
	Connection    *connection; // Our metadata about the query we will be running
	
	               ElementGrid();
	void           UpdatingRow();
	void           Build(Connection *pconnection);
	void           Load();
	void           SetTaskId(int ptaskId);
	static void    LoadElement(Connection *connection, DropGrid &dropGrid);
};

#endif
