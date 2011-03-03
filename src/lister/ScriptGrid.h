#ifndef _lister_ScriptGrid_h_
#define _lister_ScriptGrid_h_

#include "shared.h"
#include <lister/Urp/Urp.h>
class Connection;
#include "Script.h"

class ScriptGrid : public UrpSqlGrid {
public:
	typedef ScriptGrid CLASSNAME;

	EditString           taskName, scriptName, taskNote, scriptNote, scriptBody, why, targetName;
	EditInt              rowLimit, processOrder;
	DropGrid             connectionList, scriptTargetList, fastFlushTargetList;
	Connection          *connection;
	
 	                     ScriptGrid();
 	void                 RebuildOrder();
 	void                 DroppedNewRows(int line, int howmanylines);
	void                 Build(Connection *pconnection);
	void                 Load();
	void                 LoadScriptTargetList(DropGrid &pscriptTargetList);
	void                 PrepTargetFastFlush(DropGrid &fastFlushTargetList);
	void                 LinkScriptToTaskGrid(ArrayCtrl &taskGrid);
	                     // Always construct a script object with an output grid pointer (or null)
	void                 GetScriptOb(int row, Script *psob); 
	int                  GetScriptId(int row);
	int                  GetConnId(int row);
	int                  GetRowLimit(int row);
	int                  GetProcessOrder(int row);
	int                  GetMaxProcessOrder();
	RichText             GetScriptRichText(int row);
	String               GetScriptPlainText(int row);
	Script::ScriptTarget GetScriptTarget(int row);
	String               GetTargetName(int row);
	int                  GetRelId(int row);
	String               GetWhy(int row);
	bool                 GetFastFlushTarget(int row);
	void                 UpdateTaskScriptGrid(int pscriptId, String pscript);
};

#endif
