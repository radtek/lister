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
	connection = pconnection;
	SetTable(TASKS_R); // All relations to a task, regardless of type
	AddKey(RELID);
	AddColumn(TASKID, "taskid", 50);
	HideColumn(TASKID); // For joining to task grid only.
	AddColumn(WHY, "why", 160).Edit(why);
	AddColumn(SCRIPTPLAINTEXT, "script", 200);
	AddColumn(SCRIPTRICHTEXT, "rich", 200); HideColumn(SCRIPTRICHTEXT); // No need to try editing in grid
	AddColumn(SCRIPTNOTE, "script note", 200).Edit(scriptNote);
	AddColumn(SCRIPTID, "scriptid", 50);
	HideColumn(SCRIPTID);
	// Nastiest way to make a dropdown, but what the hey.
	// Took forever to figure out undocumented method for getting edited value to store the number key and display the string.
	AddColumn(RELCONNID, "connection", 50).Edit(connectionList).SetConvert(connectionList); 
	AddColumn(SCRIPTTARGET, "output to", 20).Edit(scriptTargetList).SetConvert(scriptTargetList);
	AddColumn(TARGETNAME, "targetname", 30).Edit(targetName);
	// Example of a boolean handler
	AddColumn(FASTFLUSHTARGET, "truncate target", 10).Edit(fastFlushTargetList).SetConvert(fastFlushTargetList);
	AddColumn(ROWLIMIT, "rowlimit", 20).Edit(rowLimit);
	AddColumn(PROCESSORDER, "order", 25).Edit(processOrder);
	HideColumn(PROCESSORDER); // No need to see it anymore since it is generated
	connectionList.SearchHideRows().Resizeable().Width(200);
	connectionList.NotNull(); //.AddPlus(THISBACK(NewConnection));
	built = true;
}

//==============================================================================================
void ScriptGrid::Load() {
	
	if (!connection->SendQueryDataScript("select ConnId, ConnName from v_conn order by ConnName")) {
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
	fastFlushTargetList.Add("0", "Don't delete target");
	fastFlushTargetList.Add("1", "Truncate target table");
}

//==============================================================================================
// Link Many table (this) to one table
void ScriptGrid::LinkScriptToTaskGrid(ArrayCtrl &taskGrid) {
	Join(TASKID, taskGrid); // BUG: This seems to affect the tab key from field to field.
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
int ScriptGrid::GetMaxProcessOrder() {
	int max = -1;
	
	for (int i = 0; i < GetCount(); i++) {
		int v = GetProcessOrder(i);
		if (v > max) {
			max = v;
		}
	}
	
	return max;
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
RichText ScriptGrid::GetScriptRichText(int row) {
	Value v;
	
	if (v.IsNull()) {
		v = Get(row, SCRIPTPLAINTEXT);
		String s = v.ToString();
		return ParseQTF(DeQtfLf(s));
	} else {
		v = Get(row, SCRIPTRICHTEXT);
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
	psob->scriptPlainText = GetScriptPlainText  (row);
	psob->scriptRichText  = GetScriptRichText   (row);
	psob->scriptId        = GetScriptId         (row);
	psob->scriptTarget    = GetScriptTarget     (row);
	psob->targetName      = GetTargetName       (row);
	psob->fastFlushTarget = GetFastFlushTarget  (row);
	psob->rowLimit        = GetRowLimit         (row); // Important for testing
	psob->processOrder    = GetProcessOrder     (row);
}

//==============================================================================================
Script::ScriptTarget ScriptGrid::GetScriptTarget(int row) {
	int intScriptTarget = Get(row, SCRIPTTARGET);
	Script::ScriptTarget scriptTarget = (Script::ScriptTarget)intScriptTarget;
	return Upp::max(scriptTarget, Script::SO_UNDEF); // Put a 0 if big negative returned for a null.
}

//==============================================================================================
String ScriptGrid::GetTargetName(int row) {
	return Get(row, TARGETNAME);
}

//==============================================================================================
int ScriptGrid::GetRelId(int row) {
	return Get(row, RELID);
}

//==============================================================================================
String ScriptGrid::GetWhy(int row) {
	return Get(row, WHY);
}

//==============================================================================================
// Case for dealing with PostgreSQL booleans as strings "1" and "0" (Bug in U++ driver)
// Convert them from strings to boolean
bool ScriptGrid::GetFastFlushTarget(int row) {
	String v = Get(row, FASTFLUSHTARGET);
	return (v == "1");
}

//==============================================================================================
void ScriptGrid::UpdateTaskScriptGrid(int pscriptId, String pscript) {
	ReQuery();
}



//
//BTW, speaking about it, you should be aware of Sql refactoring plan....
//
//The problem there now is with dialects AND using multiple engines at once (like Oracle and MySql in single app).
//
//There are two problems - SQL dialects differ AND of course, "implicit session" are now two...
//
//Well, several month ago plan was to bind the dialect with "execution" Sql - that would involve storing SqlExp into some intermediate language form and then "compiling" it for target engine. Also, implicit session and default SQL cursor (which in fact are related) would be gone in multisession apps.
//
//However, further thinking revealed that it would still be difficult to use - you would have to quote session when makeing any Sql cursor. So the current plan, somewhat less elegant but I believe more productive, is simply to have per-thread "current session" variable and change it using RAII push/pops, something like
//
//void MyFn() {
//UseSqlSession __(MySqlSession);
//// now until __ destructor, MySqlSession and MySQL dialect are used for everything, SQL reffers to MySqlSession
//}
//
//maybe, using macro hackery, we could also introduce form
//
//void MyFn() {
//SQLCONTEXT(MySqlSession) {
//.....
//}
//}
//
//That also means that those "dialect" members of SqlS are likely to be gone in favor of single per-thread global variable.

//sql * Select(SqlCountRows()).From(SqlId(sqlTable)).Where(PRODUKT=="prohibis");
//SqlBool sqlWhere = (PRODUKT=='prohibis');
//if(something_true){
//    sqlWhere = sqlWhere && Like(SOME_FIELD, "a%");
//}
