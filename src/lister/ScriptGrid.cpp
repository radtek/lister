/***********************************************************************************************
*  lister - ScriptGrid.cpp
*  
*  Updated list of Scripts that are assigned to the selected Task.  When a row is selected,
*  that scripts's text is placed in the script editor.  Multiple scripts can be executed in
*  series if selected whilst holding the ctrl-key, then pressing CTRL-ALT-F8.  A log window
*  will show run progress with sounds at each scripts completion.
*
*  Scripts can be reordered by drag n drop.
*
*  TODO:
*  - Convert to UrpGrid if I ever get SQL enabled in UrpGrid better.
*  - Drag n Drop always moves cursor to last row and leaves it selected along with the 
*    dropped rows.  Sometimes it creates a blank row.
*  - The script still active in the script editor should remain colored in the script grid somehow
*    to show user what he is saving to.
*  - Selecting a new row whilst unsaved text in script editor should give three choices:
*     1) overwrite, 2) cancel op, 3) Save that text to its home script (tell us which one) and
*     then overwrite
*  - When selecting a script that is the same, popup may say "Restore script text to original
*    or show difference?"
*  - Selecting 2 or more script rows should give context option to display together, merge.
*
*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

#include "ScriptGrid.h"
#include "Connection.h"
#include "shared_db.h"
#include "Script.h"
#include <lister/Sql/Sql.h>

//==============================================================================================
ScriptGrid::ScriptGrid() : UrpSqlGrid() {
	WhenDropComplete = THISBACK(DroppedNewRows);
}

//==============================================================================================
void ScriptGrid::RebuildOrder() {
	for (int i = 0; i < GetCount(); i++) {
		// Cursor must be set so that Set(int ii, const Value& v) is called and modify bit
		// is set.  If other than cursor, then no mod bit, so no write out.
		SetCursor(i);
		Set(PROCESSORDER, i);
		UpdateRow();
	}
}

//==============================================================================================
void ScriptGrid::DroppedNewRows(int line, int howmanylines) {
	RebuildOrder();
	// Reselect
	Select(line);
}

//==============================================================================================
void ScriptGrid::Build(Connection *pconnection) {
	connection = pconnection; // control connection
	SetTable(TASKS_R); // All relations to a task, regardless of type
	
	AddKey(RELID                                     );
	AddColumn(TASKID         , "taskid"         , 50 );HideColumn(TASKID); // For joining to task grid only.
	AddColumn(WHY            , "why"            , 160).Edit(why);
	AddColumn(SCRIPTPLAINTEXT, "script"         , 200);
	AddColumn(SCRIPTRICHTEXT , "rich"           , 200);HideColumn(SCRIPTRICHTEXT); // No need to try editing in grid
	AddColumn(SCRIPTNOTE     , "script note"    , 200).Edit(scriptNote);
	AddColumn(SCRIPTID       , "scriptid"       , 50 );HideColumn(SCRIPTID);
	// Nastiest way to make a dropdown, but what the hey.
	// Took forever to figure out undocumented method for getting edited value to store the number key and display the string.
	AddColumn(RELCONNID      , "connection"     , 50 ).Edit(connectionList     ).SetConvert(connectionList); 
	AddColumn(SCRIPTTARGET   , "output to"      , 20 ).Edit(scriptTargetList   ).SetConvert(scriptTargetList);
	AddColumn(TARGETNAME     , "targetname"     , 30 ).Edit(targetName);
	// Example of a boolean handler
	AddColumn(FASTFLUSHTARGET, "truncate target", 10 ).Edit(fastFlushTargetList).SetConvert(fastFlushTargetList);
	AddColumn(ROWLIMIT       , "rowlimit"       , 20 ).Edit(rowLimit);
	AddColumn(PROCESSORDER   , "order"          , 25 );HideColumn(PROCESSORDER);
	AddColumn(OUTPUTTOMACRO  , "macro"               ).Edit(outputToMacro);
	AddColumn(SCRIPTTOMACRO  , "macro"               ).Edit(scriptToMacro);

	connectionList.SearchHideRows().Resizeable().Width(200);
	connectionList.NotNull(); //.AddPlus(THISBACK(NewConnection));
	built = true;
}

//==============================================================================================
void ScriptGrid::Load() {
	
	if (!connection->SendQueryDataScript("SELECT ConnId, ConnName FROM v_conn ORDER BY ConnName")) {
		return;
	}
	
	connectionList.Clear();
	
	while(connection->Fetch()) {
		connectionList.Add(connection->Get(0), connection->Get(1));
	}

	connectionList.Add(Null, Null); // Make sure we support a null value or we won't be able to edit other attributes without assigning a connection, which isn't what we want.
	LoadScriptTargetList(scriptTargetList);
		
	PrepTargetFastFlush(fastFlushTargetList);
	SetSession(*(connection->session));
	SetOrderBy(PROCESSORDER, ADDTIMESTAMP, RELID); // If addtimestamp isn't populated, we sort by key generation order
	Query();
	loaded = true;
}

//==============================================================================================
void ScriptGrid::LoadScriptTargetList(DropGrid &pscriptTargetList) {
	pscriptTargetList.Clear();
	pscriptTargetList.Add(Script::SO_UNDEF, "Target Undefined");
	pscriptTargetList.Add(Script::SO_SCREEN, "Screen Grid");
	pscriptTargetList.Add(Script::SO_TABLE, "Database Table");
	// Excel
	// Log
}

//==============================================================================================
void ScriptGrid::PrepTargetFastFlush(DropGrid &fastFlushTargetList) {
	fastFlushTargetList.Clear();
	fastFlushTargetList.Add(true, "Truncate target table");
	fastFlushTargetList.Add(false, "Don't delete target");
}

//==============================================================================================
// Link Many table (this) to one table
void ScriptGrid::LinkScriptToTaskGrid(ArrayCtrl &taskGrid) {
	Join(TASKID, taskGrid); // BUG: This seems to affect the tab key from field to field.
}

//==============================================================================================
int ScriptGrid::GetRowLimit(int row) {
	return Upp::max((int)Get(CalcCorrectRow(row), ROWLIMIT), (int)-1); // -1 in this case indicates no limit
}

//==============================================================================================
int ScriptGrid::GetConnId(int row) {
	return Upp::max((int)Get(CalcCorrectRow(row), RELCONNID), UNKNOWN);
}

//==============================================================================================
int ScriptGrid::GetScriptId(int row) {
	return Upp::max((int)Get(CalcCorrectRow(row), SCRIPTID), UNKNOWN);
}

//==============================================================================================
RichText ScriptGrid::GetScriptRichText(int row) {
	Value v;
	
	if (v.IsNull()) {
		v = Get(CalcCorrectRow(row), SCRIPTPLAINTEXT);
		String s = v.ToString();
		return ParseQTF(DeQtfLf(s));
	} else {
		v = Get(CalcCorrectRow(row), SCRIPTRICHTEXT);
		return AsRichText(WString(v));
	}
}

//==============================================================================================
String ScriptGrid::GetScriptPlainText(int row) {
	return Get(row, SCRIPTPLAINTEXT);
}

//==============================================================================================
void ScriptGrid::GetScriptOb(int row, Script *psob) {
	ASSERT(psob);
	psob->scriptPlainText = GetScriptPlainText  (CalcCorrectRow(row));
	psob->scriptRichText  = GetScriptRichText   (CalcCorrectRow(row));
	psob->scriptId        = GetScriptId         (CalcCorrectRow(row));
	psob->scriptTarget    = GetScriptTarget     (CalcCorrectRow(row));
	psob->targetName      = GetTargetName       (CalcCorrectRow(row));
	psob->fastFlushTarget = GetFastFlushTarget  (CalcCorrectRow(row));
	psob->rowLimit        = GetRowLimit         (CalcCorrectRow(row)); // Important for testing
	psob->processOrder    = GetProcessOrder     (CalcCorrectRow(row));
	psob->connId          = GetConnId           (CalcCorrectRow(row));
}

//==============================================================================================
Script::ScriptTarget ScriptGrid::GetScriptTarget(int row) {
	if (row == -1) return Script::SO_UNDEF;
	int intScriptTarget = Get(CalcCorrectRow(row), SCRIPTTARGET);
	Script::ScriptTarget scriptTarget = (Script::ScriptTarget)intScriptTarget;
	return Upp::max(scriptTarget, Script::SO_UNDEF); // Put a 0 if big negative returned for a null.
}

//==============================================================================================
String ScriptGrid::GetTargetName(int row) {
	if (row == -1) return "";
	return Get(CalcCorrectRow(row), TARGETNAME);
}

//==============================================================================================
int ScriptGrid::GetRelId(int row) {
	if (row == -1) return -1;
	return Get(CalcCorrectRow(row), RELID);
}

//==============================================================================================
String ScriptGrid::GetWhy(int row) {
	if (row == -1) return "";
	return Get(CalcCorrectRow(row), WHY);
}

//==============================================================================================
String ScriptGrid::GetOutputToMacro(int row) {
	if (row == -1) return "";
	return Get(CalcCorrectRow(row), OUTPUTTOMACRO);
}

//==============================================================================================
String ScriptGrid::GetScriptToMacro(int row) {
	if (row == -1) return "";
	return Get(CalcCorrectRow(row), SCRIPTTOMACRO);
}

//==============================================================================================
// Case for dealing with PostgreSQL booleans as strings "1" and "0" (Bug in U++ driver)
// Convert them from strings to boolean
bool ScriptGrid::GetFastFlushTarget(int row) {
	if (row == -1) return false;
	return Get(CalcCorrectRow(row), FASTFLUSHTARGET);
}

//==============================================================================================
void ScriptGrid::UpdateTaskScriptGrid(int pscriptId, String pscript) {
	ReQuery();
}
