/***********************************************************************************************
*  lister - Task.cpp
*  
*  Task detail packet for passing around.
*
*  This allows the Task database code be embedded in the TaskGrid.cpp.  All activity around tasks
*  is funnelled through here in case the GUI needs to be updated.
*  Possibly, task SQL could be moved to Task.cpp and referenced from TaskGrid.  That way the task
*  logic could be used without instantiating a grid.
*
*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

#include "Task.h"

//==============================================================================================
Task::Task() {
	taskId          = UNKNOWN;
}

//==============================================================================================
Task::Task(
			int       ptaskId
		,	String    ptaskName
		,	String    ptaskDesc
		,	String    psolutionDesc
		,	String    pprojectName
		,	String    psrcCode
		,	String    pnote
		,	int       passignedToWho
		,	int       passignedByWho
		,	Date      ppriorityAssignedDate
		,	Date      pnextTargetDate
		,	String    ptargetDateExpectation
		,	int       ppriorityWithinProject
		,	int       pdepOnWho
		,	int       pdepOnWho2
		,	int       pdepOnWho3
		,	int       pdepOnWho4
		,	int       pdepOnWho5
		,	Date      pdepAssignedWhen
		,	Date      pdepExpectSampWhen
		,	String    pdepAssignDesc
		,	String    pdepFeedback
		,	int       ptaskDriverId
	)
{
	taskId                = ptaskId;
	taskName              = ptaskName;
	taskDesc              = ptaskDesc;
	solutionDesc          = psolutionDesc;
	projectName           = pprojectName;
	srcCode               = psrcCode;
	note                  = pnote;
	assignedToWho         = passignedToWho;
	assignedByWho         = passignedByWho;
	priorityAssignedDate  = ppriorityAssignedDate;
	nextTargetDate        = pnextTargetDate;
	targetDateExpectation = ptargetDateExpectation;
	priorityWithinProject = ppriorityWithinProject;
	depOnWho              = pdepOnWho;
	depOnWho2             = pdepOnWho2;
	depOnWho3             = pdepOnWho3;
	depOnWho4             = pdepOnWho4;
	depOnWho5             = pdepOnWho5;
	depAssignedWhen       = pdepAssignedWhen;
	depExpectSampWhen     = pdepExpectSampWhen;
	depAssignDesc         = pdepAssignDesc;
	depFeedback           = pdepFeedback;
	taskDriverId          = ptaskDriverId;
}
