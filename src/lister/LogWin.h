#ifndef _lister_lister_LogWin_h_
#define _lister_lister_LogWin_h_

#include "shared.h"
#include "shared_layout.h"

// Controlled logging of task runs, not for general debugging, but for users reviewing batchs
class LogWin : public WithLogWinLayout<UrpWindow> {
public:
	bool activated; // Restrict logging to specific periods, like running a task set of scripts, otherwise it will be filled with junk.
	typedef LogWin CLASSNAME;
	
	LogWin();
	void Clear();
	void writeLine(String line);
	void writeSep();
};

void OpenLogWin();
void LogLine(String line);
void LogSep();
void ActivateLogging();
void DeactivateLogging();

#endif
