#ifndef _lister_lister_Script_h_
#define _lister_lister_Script_h_

#include "shared.h"

class OutputGrid;
class OutputStat;

// These can't change; stuffed in database.

//==============================================================================================
class Script : public Moveable<Script> { // MoveableAndDeepCopyOption?? See Vcont.h for sample
public:
	enum ScriptTarget { 
			SO_UNDEF
		,	SO_SCREEN
		,	SO_TABLE 
	};
	
	int                     scriptId // key from scripts table
	,                       originalScriptId; // Should populate so original script can bne updated when shift key is held down
	int                     taskScriptId;           // If taken from taskscript grid, remember for updating.
	ScriptTarget            scriptTarget; // Screen or table?
	String                  targetName; // Table name if target is a table
	String                  scriptPlainText; // text
	String                  scriptQTFText; // Pre-RichText, which includes font information
	RichText                scriptRichText; // display with RichText embedded
	bool                    fastFlushTarget; // set to true if truncate target first
	OutputGrid             *outputGrid; // For screen output
	int                     rowLimit;
	int						processOrder; // Order to process and list scripts in per task
	bool                    addSepToOutput;
	String                  outFldSepWhenValChange; // Name of the field to watch when streaming rows out to screen
	int                     sepRowCount; // How many blank rows? between groups of rows with same value
	
	Script();
	Script(OutputGrid *poutputGrid);
	Script(
		ScriptTarget              pscriptTarget
	,	bool                      pfastFlushTarget
	,	OutputGrid               *poutputGrid
	,	int                       pscriptId
	,	String                    pscriptPlainText
	,   RichText                  pscriptRichText
	,	int                       prowLimit
	,	String                    ptargetName
	,	bool                      paddSepToOutput
	,	String                    pfldSepWhenChange
	,	int                       psepRowCount
	,	int                       ptaskScriptId = UNKNOWN // Not always set if not dealing with a taskscript grid.
	);

	static String          GetScriptListQuery();
	static String          GetScriptDetailByIdQuery(int scriptId);
    static String          GetRelScriptDetailByIdQuery(int relId);
	static String          FindScriptByPlainTextQuery(String pscriptPlainText);
};

#endif
