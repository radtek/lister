#include "JobSpec.h"

JobSpec::JobSpec(bool plog, bool ptestMode, bool pbatchMode) {
	log         = plog;
	testMode    = ptestMode;
	batchMode   = pbatchMode;
}

