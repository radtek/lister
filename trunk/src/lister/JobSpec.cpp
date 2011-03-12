#include "JobSpec.h"

//==============================================================================================
JobSpec::JobSpec(bool plog, bool ptestMode, bool pbatchMode, OutputStat *poutputStat) {
	log         = plog;
	testMode    = ptestMode;
	batchMode   = pbatchMode;
	outputStat  = poutputStat;
}

