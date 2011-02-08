#ifndef _lister_lister_AnalysisByDate_h_
#define _lister_lister_AnalysisByDate_h_

#include "shared.h"
#include "shared_db.h"
class Connection;

class AnalysisByDateGrid : public UrpSqlGrid {
public:
	typedef AnalysisByDateGrid CLASSNAME;
	
	int            analysisId, scriptId, taskId;
	EditString     analysisName, totalCount, note;
	EditDate       byDate;
	Connection    *connection; // Our metadata about the query we will be running
	
	               AnalysisByDateGrid();
	void           UpdatingRow();
	void           Build(Connection *pconnection);
	void           Load();
	void           SetTaskId(int ptaskId);
};

#endif
