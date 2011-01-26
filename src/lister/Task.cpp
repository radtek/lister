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
}
