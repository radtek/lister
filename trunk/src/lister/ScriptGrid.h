#ifndef _lister_ScriptGrid_h_
#define _lister_ScriptGrid_h_

#include <lister/Urp/Urp.h>
class Connection;

class ScriptGrid : public UrpSqlGrid {
public:
	EditString scriptName, note, scriptBody;

	ScriptGrid();
	void Build();
	void Load(Connection *pconnection);
};

#endif
