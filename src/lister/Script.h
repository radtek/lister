#ifndef _lister_lister_Script_h_
#define _lister_lister_Script_h_

#include "shared.h"

class OutputGrid;
class OutputStat;

// These can't change; stuffed in database.


class Script : public Moveable<Script> { // MoveableAndDeepCopyOption?? See Vcont.h for sample
public:
	enum ScriptTarget { 
			SO_UNDEF
		,	SO_SCREEN
		,	SO_TABLE 
	};
	
	int                     scriptId // key from scripts table
	,                       originalScriptId; // Should populate so original script can bne updated when shift key is held down
	ScriptTarget            scriptTarget; // Screen or table?
	String                  targetName; // Table name if target is a table
	String                  scriptPlainText; // text
	String                  scriptQTFText; // Pre-RichText, which includes font information
	RichText                scriptRichText; // display with RichText embedded
	bool                    fastFlushTarget; // set to true if truncate target first
	OutputGrid             *outputGrid; // For screen output
	int                     rowLimit;
	int						processOrder; // Order to process and list scripts in per task
	
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
	);

	static String          GetScriptListQuery();
	static String          GetScriptDetailByIdQuery(int scriptId);
	static String          FindScriptByPlainTextQuery(String pscriptPlainText);
};

#endif
