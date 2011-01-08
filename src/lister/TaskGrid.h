#ifndef _lister_TaskGrid_h_
#define _lister_TaskGrid_h_

#include "shared.h"

// Extracted from listertables table.

#define OB_ACTIONS             1
#define OB_COMPARISONTYPES     2
#define OB_DATABASES           3
#define OB_ENVIRONMENTS        4
#define OB_DOCUMENTS           5
#define OB_PROJECTS            6
#define OB_SIDES               7
#define OB_CYCLES              8
#define OB_TEAMS              10
#define OB_USERS              11
#define OB_TIMELINES          12
#define OB_DELIVERABLES       13
#define OB_DATES              14
#define OB_SIGNOFFS           15
#define OB_REQUIREMENTS       16
#define OB_AGREEMENTS         18
#define OB_APPROVALS          19
#define OB_SCOPEDIMENSIONS    20
#define OB_MILESTONES         21
#define OB_TICKETS            22
#define OB_HOT_SHEETS         23
#define OB_IMPRESSIONS        24
#define OB_MEETINGS           25
#define OB_FEEDBACK           26
#define OB_METRICS            27
#define OB_EMAILS             28
#define OB_PROCUREMENTS       29
#define OB_TESTS              30
#define OB_TESTTYPES          31
#define OB_SCRIPTS            32
#define OB_OPERATINGSYSTEMS   33
#define OB_LOGINS             34
#define OB_INSTANCES          35
#define OB_INSTANCETYPES      36
#define OB_SERVERS            37
#define OB_MACROS             38
#define OB_CONNECTIONS        39
#define OB_LISTERTABLES       40
#define OB_FEEDS              41
#define OB_REPORTINGAPPS      42
#define OB_TASKS              43

class TaskGrid : public UrpSqlGrid {
public:
	EditString taskName, taskNote, srcCode, projectName;

	TaskGrid();
	void Build();
	void Load(Connection *pconnection);
};


#endif
