#ifndef _lister_lister_Script_h_
#define _lister_lister_Script_h_

#include "shared.h"
class OutputGrid;

enum ScriptTarget { SO_UNDEF, SO_SCREEN, SO_TABLE };

class Script : public Moveable<Script> { // MoveableAndDeepCopyOption?? See Vcont.h for sample
public:
	int                     scriptId // key from scripts table
	,                       originalScriptId; // Should populate so original script can bne updated when shift key is held down
	ScriptTarget            scriptTarget; // Screen or table?
	String                  targetName; // Table name if target is a table
	String                  script; // text
	RichText                richscript; // display with QTF embedded
	bool                    fastFlushTarget; // set to true if truncate target first
	OutputGrid             *outputGrid; // For screen output
	int                     rowLimit;
	int						processOrder; // Order to process and list scripts in per task
	
	Script();
	Script(OutputGrid *poutputGrid);
};

#endif
