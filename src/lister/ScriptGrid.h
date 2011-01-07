#ifndef _lister_ScriptGrid_h_
#define _lister_ScriptGrid_h_

#include "shared.h"

class ScriptGrid : public UrpSqlGrid {
public:
	EditString scriptName, note, scriptBody;

	ScriptGrid() {
		SetTable(SCRIPTS);
		AddKey(SCRIPTID);
		AddColumn(SCRIPTNAME, "script", 140).Edit(scriptName);
		AddColumn(NOTE, "note", 200).Edit(note);
		AddColumn(SCRIPT, "body", 200).Edit(scriptBody);
	}
	
	void Build() {
	}
	
	void Load(Connection *pconnection) {
		SetSession(*(pconnection->session));
		SetSortColumn(SCRIPTNAME);
		Query();
	
	}
};

#endif
