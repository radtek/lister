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
