#ifndef _lister_lister_OutputStat_h_
#define _lister_lister_OutputStat_h_

#include "shared.h"
#include "lister/Urp/UrpInterval.h"

#define LAYOUTFILE <lister/lister/panels.lay>
#include <CtrlCore/lay.h>

class OutputStat : public WithOutputStatLayout<ParentCtrl> { // Note: Not a TopWindow type so it can be embedded
public:
	typedef OutputStat CLASSNAME;
	Time   *startedWhen;
	        OutputStat();
	       ~OutputStat();
	String  PrepOutputValue(String in);
	// User calls these wrappers on completion of execution of a cursor/script and doesn't have
	// to know if this is display or write to a log or db, etc.
	void    SetRowCount(int prowCount);
	void    SetColumnCount(int pcolumnCount);
	void    SetConnName(String pconnName);
	void    SetFetchTime(Interval ptime);
	void    SetScriptId(int id);
	void    SetStartedWhen(Time pstartedWhen);
	Time    GetStartedWhen();
	void    SetStoppedWhen(Time pstoppedWhen);
	void    SetStatus(String pstatus);
};

#endif
