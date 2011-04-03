/***********************************************************************************************
*  lister - JobSpec.cpp
*  
*  Control information about the execution (run) of a job.
*
*  An instance is passed along with the Script object (SOB) to the CursorHandler.  Various GUI
*  input from the user is used to determine the set of values established.  This isolates the
*  rest of the code from the vagaries of the user interface.
*
*  These attributes are not in Script.cpp because they seem to descript the running of a script
*  and were not attributes of the script itself.  A fine line.  Any attribute that could be applied
*  to any script that would affect its behavior transparently would go here.  For instance,
*  connection # doesn't go here since the structure of the script only makes sense to certain
*  connections of a certain type.  So connection # is part of the Script.cpp.

*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

#include "JobSpec.h"

//==============================================================================================
JobSpec::JobSpec(bool plog, bool ptestMode, bool pbatchMode, OutputStat *poutputStat) {
	log         = plog;
	testMode    = ptestMode;
	batchMode   = pbatchMode;
	outputStat  = poutputStat;
}

