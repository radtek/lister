#ifndef _lister_lister_Task_h_
#define _lister_lister_Task_h_

#include "shared.h"

class Task: public Moveable<Task> {
public:
	int        taskId;
	String     taskName;
	String     taskDesc;
	String     solutionDesc;
	String     projectName;
	String     srcCode;
	String     note;
	int        assignedToWho;
	int        assignedByWho;
	Date       priorityAssignedDate;
	Date       nextTargetDate;
	String     targetDateExpectation;
	int        priorityWithinProject;
	int        depOnWho;
	int        depOnWho2;
	int        depOnWho3;
	int        depOnWho4;
	int        depOnWho5;
	Date       depAssignedWhen;
	Date       depExpectSampWhen;
	String     depAssignDesc;
	String     depFeedback;
	
		       Task();
	           Task(
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
	           		);
	
};

#endif
