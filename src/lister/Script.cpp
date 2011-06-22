/***********************************************************************************************
*  lister - Script.cpp
*  
*  Script details bean.
*
*  Many functions and panes need to access the script details without being locked into a 
*  specific part of the code.  A link to an OutputGrid object is included and is definitely
*  an example of TMI for a bean, but it can help track the output, especially in visual terms.
*  I may take it out.

*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

#include "Script.h"
#include "shared_db.h"
#include "Connection.h"

//==============================================================================================
Script::Script() {
	Script((OutputGrid *)NULL);
}

//==============================================================================================
// Script sob(pscriptTarget, (fastFlushTargetList.GetData() == "1"), &mainGrid, scriptId, lscriptText, 
// GetFieldInt(fldRowLimit), targetNameList.GetData());}
//==============================================================================================
Script::Script(
		ScriptTarget              pscriptTarget
	,	bool                      pfastFlushTarget
	,	OutputGrid               *poutputGrid
	,	int                       pscriptId
	,	String                    pscriptPlainText
	,	RichText                  pscriptRichText
	,	int                       prowLimit
	,	String                    ptargetName
	,	String                    poutputToMacro
	,	String                    pscriptToMacro
	,	bool                      paddSepToOutput
	,	String                    poutFldSepWhenValChange
	,	int                       psepRowCount
	,	int                       ptaskScriptId/* = UNKNOWN*/ // Not always set if not dealing with a taskscript grid.
	,   int                       pconnId/* = UNKNOWN*/
	)
{
	scriptTarget                = pscriptTarget;
	fastFlushTarget             = pfastFlushTarget;
	outputGrid                  = poutputGrid;
	scriptId                    = pscriptId;
	scriptPlainText             = pscriptPlainText; // No QTF codes present
	rowLimit                    = prowLimit;
	targetName                  = ptargetName;
	outputToMacro               = poutputToMacro;
	scriptToMacro               = pscriptToMacro;
	addSepToOutput              = paddSepToOutput;
	outFldSepWhenValChange      = poutFldSepWhenValChange;
	sepRowCount                 = psepRowCount;
	taskScriptId                = ptaskScriptId;
	connId                      = pconnId;
}

//==============================================================================================
Script::Script(OutputGrid *poutputGrid) {
	scriptTarget                = SO_SCREEN;
	scriptId                    = UNKNOWN;
	taskScriptId                = UNKNOWN;
	outputGrid                  = poutputGrid; // Can be null.  Connection object will break if executed and this is not set and Script Target is screen
}

//==============================================================================================
// Construct a script object directly from the db instead of going through the Script Grid.
// Created for the Test Grid/TestWin in order to run tests without ScriptGrid visibility.
// Running the Setup "tests" requires detail about target, mostly that it is a TABLE and
// what tablename.
//==============================================================================================
bool Script::LoadFromDb(Connection *connection, int relationid) {
	// Fetch all detail from task_r view
	String script = Format("SELECT SCRIPTPLAINTEXT, SCRIPTRICHTEXT, SCRIPTID, SCRIPTTARGET, TARGETNAME, "
	              " FASTFLUSHTARGET, ROWLIMIT, PROCESSORDER, RELCONNID, OUTPUTTOMACRO, SCRIPTTOMACRO "
	              " FROM TASKS_R WHERE RELID = %d", relationid);

	if (!connection->Execute(script)) {
		return false;
	}
	
	if (!connection->GetRowsProcessed() == 1) {
		return false;
	}
	
	connection->Fetch();
	
	scriptPlainText = connection->Get(SCRIPTPLAINTEXT);
	scriptRichText  = AsRichText(WString(connection->Get(SCRIPTRICHTEXT)));
	scriptId        = connection->Get(SCRIPTID);
	scriptTarget    = (ScriptTarget)IfNull((int)connection->Get(SCRIPTTARGET), Script::SO_SCREEN);
	targetName      = connection->Get(TARGETNAME);
	outputToMacro   = connection->Get(OUTPUTTOMACRO);
	scriptToMacro   = connection->Get(SCRIPTTOMACRO);
	fastFlushTarget = connection->Get(FASTFLUSHTARGET);
	rowLimit        = connection->Get(ROWLIMIT);
	processOrder    = connection->Get(PROCESSORDER);
	connId          = connection->Get(RELCONNID);
	return true;
}

//==============================================================================================
/*static*/String Script::GetScriptListQuery() {
	// Use U++ formation techniques so as to get compile-time checking on column names	
	SqlSelect s = SqlSelect(SCRIPTID, SCRIPTPLAINTEXT, SCRIPTRICHTEXT).From(SCRIPTS);
	SqlStatement st(s);
	return st.GetText();
}

//==============================================================================================
/*static*/String Script::FindScriptByPlainTextQuery(String pscriptPlainText) {
	SqlSelect s = SqlSelect(SCRIPTID, SCRIPTPLAINTEXT, SCRIPTRICHTEXT).From(SCRIPTS).Where(SCRIPTPLAINTEXT == pscriptPlainText);
	SqlStatement st(s);
	return st.Get(PGSQL);
}
//==============================================================================================
/*static*/String Script::GetScriptDetailByIdQuery(int scriptId) {
	SqlSelect s = SqlSelect(SCRIPTPLAINTEXT, SCRIPTRICHTEXT).From(SCRIPTS).Where(SCRIPTID == scriptId);
	SqlStatement st(s);
	return st.GetText();
}

//==============================================================================================
/*static*/String Script::GetRelScriptDetailByIdQuery(int relId) {
	SqlSelect s = SqlSelect(SCRIPTID, SCRIPTPLAINTEXT, SCRIPTRICHTEXT, RELCONNID).From(TASKS_R).Where(RELID == relId);
	SqlStatement st(s);
	return st.GetText();
}
