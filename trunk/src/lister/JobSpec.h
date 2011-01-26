#ifndef _lister_lister_JobSpec_h_
#define _lister_lister_JobSpec_h_

#include "shared.h"

class JobSpec {
public:
	bool     log;
	bool     batchMode; // No popups
	bool     testMode; // 100 rows each
	bool     executingSelection;
	
	         JobSpec(bool plog = true, bool ptestMode = true, bool pbatchMode = false);
};

#endif
