#ifndef _lister_lister_LogWin_h_
#define _lister_lister_LogWin_h_

#include "shared.h"
#include "shared_layout.h"

// Controlled logging of task runs, not for general debugging, but for users reviewing batchs
class LogWin : public WithLogWinLayout<UrpWindow> {
public:
	typedef LogWin CLASSNAME;
	bool        activated; // Restrict logging to specific periods, like running a task set of scripts, otherwise it will be filled with junk.
	
	            LogWin();
	void        Clear();
	void        writeLine(String line, bool embellish = false);
	void        writeSep();
};

void      InitLogging();
void      TermLogging();
LogWin   *GetLogWin();
void      OpenLogWin();
void      LogLine(String line);
void      LogLineUnembellished(String line);
void      LogSep();
void      ActivateLogging();
void      DeactivateLogging();
void      LogGroupHeaderLine(String line);

#endif
