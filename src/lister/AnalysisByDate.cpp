/***********************************************************************************************
*  lister - AnalysisByDate.cpp
*  
*  Great idea, but no time to do it.  Essentially create a grid that tackles a table segment
*  that is only one business day of data.  Each row represent as test of the data, a sql script
*  that classifies a number of rows as resolvable, irresolvable, not an issue, or ok.  A
*  column then describes what the resolution requires, perhaps a task number and a status.
*  The analysis is updated periodically with resolutions timelines for coming to fruition and
*  deployment.  Often resolutions fail or go dormant without someone chasing them.
*
*  The row counts roll down like an excel spreadsheet, with the remaining unclassified counts
*  and % at the bottom.  It also shows the ones with a solution scheduled and a party that
*  it is dependent on, last contact, etc.
*  Failed solutions should be saved in order to help future estimations.
*  Different source days can be loaded and graphed.  Sometimes behavior for different days of
*  input are extremely different and resolving a specific day is not enough.
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

#include "AnalysisByDate.h"
#include "Connection.h"

//==============================================================================================
AnalysisByDateGrid::AnalysisByDateGrid() : UrpSqlGrid() {
}

//==============================================================================================
void AnalysisByDateGrid::UpdatingRow() {
	// Called from WhenStartEdit from inside StartEdit	
	if (IsInsert()) { // Not IsInserting, this checks insertmode
		Set(TASKID, taskId);
	}
}

//==============================================================================================
void AnalysisByDateGrid::Build(Connection *pconnection) {
	// Build columns
	connection = pconnection;
	SetTable(ANALYSISBYDATE);
	AddKey(ANALYSISID);
	AddColumn(ANALYSISNAME, "analysis", 100).Edit(analysisName);
	//AddColumn(ENVID, "analysis", 100).Edit(analysisName);
	// Should have a default connection at sumary level
	AddColumn(BYDATE, "by date", 100).Edit(byDate);
	// BYDATERULE/MACRO
	AddColumn(TOTALCOUNT, "ttl cnt", 100).Edit(totalCount);
	AddColumn(NOTE, "note", 200).Edit(note);
	AddIndex(TASKID);
	built = true;
	// DoInsert()->DoEdit()->StartEdit()
	WhenStartEdit = THISBACK(UpdatingRow);
}

//==============================================================================================
void AnalysisByDateGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
	SetWhere((TASKID == taskId));
}

//==============================================================================================
void AnalysisByDateGrid::Load() {
	SetSession(*(connection->session));
	SetSortColumn(ANALYSISNAME);
	Query();
	loaded = true;
}

