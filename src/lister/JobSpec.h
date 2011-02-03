#ifndef _lister_lister_JobSpec_h_
#define _lister_lister_JobSpec_h_

#include "shared.h"
class OutputStat;

class JobSpec {
public:
	bool         log;
	bool         batchMode; // No popups
	bool         testMode; // 100 rows each
	bool         executingSelection;
	OutputStat  *outputStat;
	
	             JobSpec(bool plog = true, bool ptestMode = true, bool pbatchMode = false, OutputStat *poutputStat = NULL);
};

#endif
