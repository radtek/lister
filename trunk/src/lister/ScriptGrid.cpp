#include "ScriptGrid.h"
#include "Connection.h"
#include "shared_db.h"
#include "Script.h"

#include "shared.h"

//==============================================================================================
ScriptGrid::ScriptGrid() : UrpSqlGrid() {
	
}

//==============================================================================================
void ScriptGrid::Build(Connection *pconnection) {
	connection = pconnection;
	SetTable(TASKS_R); // All relations to a task, regardless of type
	AddKey(RELID);
	AddColumn(TASKID, "taskid", 50);
	HideColumn(TASKID); // For joining to task grid only.
	AddColumn(WHY, "why", 160).Edit(why);
	AddColumn(SCRIPT, "script", 200);
	AddColumn(SCRIPTNOTE, "script note", 200).Edit(scriptNote);
	AddColumn(SCRIPTID, "scriptid", 50);
	HideColumn(SCRIPTID);
	// Nastiest way to make a dropdown, but what the hey.
	// Took forever to figure out undocumented method for getting edited value to store the number key and display the string.
	AddColumn(RELCONNID, "connection", 50).Edit(connectionList).SetConvert(connectionList); 
	AddColumn(SCRIPTTARGET, "output to", 20).Edit(scriptTargetList).SetConvert(scriptTargetList);
	AddColumn(TARGETNAME, "targetname", 30).Edit(targetName);
	AddColumn(FASTFLUSHTARGET, "truncate target", 10).Edit(fastFlushTargetList).SetConvert(fastFlushTargetList);
	AddColumn(ROWLIMIT, "rowlimit", 20).Edit(rowLimit);
	AddColumn(PROCESSORDER, "order", 25).Edit(processOrder);
	connectionList.SearchHideRows().Resizeable().Width(200);
	connectionList.NotNull(); //.AddPlus(THISBACK(NewConnection));
}

//==============================================================================================
void ScriptGrid::Load() {
	
	if (!connection->SendQueryDataScript("select ConnId, ConnName from v_conn order by ConnName")) {
		return;
	}
	
	while(connection->Fetch()) {
		connectionList.Add(connection->Get(0), connection->Get(1));
	}

	connectionList.Add(Null, Null); // Make sure we support a null value or we won't be able to edit other attributes without assigning a connection, which isn't what we want.

	LoadScriptTargetList(scriptTargetList);
		
	PrepTargetFastFlush(fastFlushTargetList);
	SetSession(*(connection->session));
	SetOrderBy(PROCESSORDER, ADDTIMESTAMP, RELID); // If addtimestamp isn't populated, we sort by key generation order
	Query();
}

//==============================================================================================
void ScriptGrid::LoadScriptTargetList(DropGrid &pscriptTargetList) {
	pscriptTargetList.Add(SO_UNDEF, "Target Undefined");
	pscriptTargetList.Add(SO_SCREEN, "Screen Grid");
	pscriptTargetList.Add(SO_TABLE, "Database Table");
	// Excel
	// Log
}

//==============================================================================================
void ScriptGrid::PrepTargetFastFlush(DropGrid &fastFlushTargetList) {
	fastFlushTargetList.Add("0", "Don't delete target");
	fastFlushTargetList.Add("1", "Truncate target table");
}

//==============================================================================================
// Link Many table (this) to one table
void ScriptGrid::LinkScriptToTaskGrid(ArrayCtrl &taskGrid) {
	Join(TASKID, taskGrid);
}

//==============================================================================================
int ScriptGrid::GetRowLimit(int row) {
	return Upp::max((int)Get(row, ROWLIMIT), (int)-1); // -1 in this case indicates no limit
}

//==============================================================================================
int ScriptGrid::GetProcessOrder(int row) {
	return Upp::max((int)Get(row, PROCESSORDER), (int)-1);
}

//==============================================================================================
int ScriptGrid::GetConnId(int row) {
	return Upp::max((int)Get(row, RELCONNID), UNKNOWN);
}

//==============================================================================================
int ScriptGrid::GetScriptId(int row) {
	return Upp::max((int)Get(row, SCRIPTID), UNKNOWN);
}

//==============================================================================================
String ScriptGrid::GetScript(int row) {
	return Get(row, SCRIPT);
}

//==============================================================================================
void ScriptGrid::GetScriptOb(int row, Script *psob) {
	ASSERT(psob);
	psob->script          = GetScript           (row);
	psob->scriptId        = GetScriptId         (row);
	psob->scriptTarget    = GetScriptTarget     (row);
	psob->targetName      = GetTargetName       (row);
	psob->fastFlushTarget = GetFastFlushTarget  (row);
	psob->rowLimit        = GetRowLimit         (row); // Important for testing
	psob->processOrder    = GetProcessOrder     (row);
}

//==============================================================================================
ScriptTarget ScriptGrid::GetScriptTarget(int row) {
	int intScriptTarget = Get(row, SCRIPTTARGET);
	ScriptTarget scriptTarget = (ScriptTarget)intScriptTarget;
	return Upp::max(scriptTarget, SO_UNDEF); // Put a 0 if big negative returned for a null.
}

//==============================================================================================
String ScriptGrid::GetTargetName(int row) {
	return Get(row, TARGETNAME);
}

//==============================================================================================
int ScriptGrid::GetRelId(int row) {
	return Get(row, RELID);
}

//==========================================================================================	
String ScriptGrid::GetWhy(int row) {
	return Get(row, WHY);
}

//==============================================================================================
// Case for dealing with PostgreSQL booleans as strings "1" and "0" (Bug in U++ driver)
bool ScriptGrid::GetFastFlushTarget(int row) {
	String v = Get(row, FASTFLUSHTARGET);
	return (v == "1");
}