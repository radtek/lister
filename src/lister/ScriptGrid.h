#ifndef _lister_ScriptGrid_h_
#define _lister_ScriptGrid_h_

#include <lister/Urp/Urp.h>
class Connection;

class ScriptGrid : public UrpSqlGrid {
public:
	EditString taskName, scriptName, taskNote, scriptNote, scriptBody, why;

	ScriptGrid();
	void Build();
	void Load(Connection *pconnection);
	void LinkScriptToTaskGrid(ArrayCtrl &taskGrid);
	int GetScriptId(int row);
	String GetScript(int row);
};

#endif
