#include "ScriptGrid.h"
#include "Connection.h"
#include "shared_db.h"

ScriptGrid::ScriptGrid() {
	SetTable(SCRIPTS);
	AddKey(SCRIPTID);
	AddColumn(SCRIPTNAME, "script", 140).Edit(scriptName);
	AddColumn(NOTE, "note", 200).Edit(note);
	AddColumn(SCRIPT, "body", 200).Edit(scriptBody);
}

void ScriptGrid::Build() {
}

void ScriptGrid::Load(Connection *pconnection) {
	SetSession(*(pconnection->session));
	SetSortColumn(SCRIPTNAME);
	Query();

}

