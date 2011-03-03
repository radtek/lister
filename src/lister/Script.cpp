#include "Script.h"
#include "shared_db.h"

//==============================================================================================
Script::Script() {
	Script((OutputGrid *)NULL);
}

//==============================================================================================
// Script sob(pscriptTarget, (fastFlushTargetList.GetData() == "1"), &mainGrid, scriptId, lscriptText, GetFieldInt(fldRowLimit), targetNameList.GetData());}
Script::Script(
		ScriptTarget              pscriptTarget
	,	bool                      pfastFlushTarget
	,	OutputGrid               *poutputGrid
	,	int                       pscriptId
	,	String                    pscriptPlainText
	,	RichText                  pscriptRichText
	,	int                       prowLimit
	,	String                    ptargetName
	,	bool                      paddSepToOutput
	,	String                    poutFldSepWhenValChange
	,	int                       psepRowCount
	,	int                       ptaskScriptId/* = UNKNOWN*/ // Not always set if not dealing with a taskscript grid.
	)
{
	scriptTarget                = pscriptTarget;
	fastFlushTarget             = pfastFlushTarget;
	outputGrid                  = poutputGrid;
	scriptId                    = pscriptId;
	scriptPlainText             = pscriptPlainText; // No QTF codes present
	rowLimit                    = prowLimit;
	targetName                  = ptargetName;
	addSepToOutput              = paddSepToOutput;
	outFldSepWhenValChange      = poutFldSepWhenValChange;
	sepRowCount                 = psepRowCount;
	taskScriptId                = ptaskScriptId;
}

//==============================================================================================
Script::Script(OutputGrid *poutputGrid) {
	scriptTarget                = SO_SCREEN;
	scriptId                    = UNKNOWN;
	taskScriptId                = UNKNOWN;
	outputGrid                  = poutputGrid; // Can be null.  Connection object will break if executed and this is not set and Script Target is screen
}

//==============================================================================================
String Script::GetScriptListQuery() {
	// Use U++ formation techniques so as to get compile-time checking on column names	
	SqlSelect s = SqlSelect(SCRIPTID, SCRIPTPLAINTEXT, SCRIPTRICHTEXT).From(SCRIPTS);
	SqlStatement st(s);
	return st.GetText();
}

//==============================================================================================
String Script::FindScriptByPlainTextQuery(String pscriptPlainText) {
	SqlSelect s = SqlSelect(SCRIPTID, SCRIPTPLAINTEXT, SCRIPTRICHTEXT).From(SCRIPTS).Where(SCRIPTPLAINTEXT == pscriptPlainText);
	SqlStatement st(s);
	return st.Get(PGSQL);
}
//==============================================================================================
String Script::GetScriptDetailByIdQuery(int scriptId) {
	SqlSelect s = SqlSelect(SCRIPTPLAINTEXT, SCRIPTRICHTEXT).From(SCRIPTS).Where(SCRIPTID == scriptId);
	SqlStatement st(s);
	return st.GetText();
}

