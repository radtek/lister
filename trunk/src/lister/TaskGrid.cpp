/***********************************************************************************************
*  lister - TaskGrid.cpp
*  
*  2nd major list that shows all tasks assigned to me.
*
*  TODO:
*  - Convert to UrpGrid if I ever get SQL enabled in UrpGrid better.
*  - Double-click should work even if in edit mode.
*  - Up/down arrow should act same as single-click.
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

#include "TaskGrid.h"
#include "Connection.h"
#include "shared_db.h"
#include "TaskDefWin.h"
#include "Task.h"
#include <CtrlLib/CtrlLib.h>

//==========================================================================================	
TaskGrid::TaskGrid() {
	SetTable(TASKS);
	AddKey(TASKID);
	AddColumn(TASKNAME, "task", 100).Edit(taskName);
	AddColumn(SRCCODE, "QC#", 100).Edit(srcCode);
	AddColumn(PROJECTNAME, "project", 100).Edit(projectName);
	AddColumn(NOTE, "tasknote", 100).Edit(taskNote);
	AddIndex(TASKDESC);
	AddIndex(SOLUTIONDESC);
	AddIndex(ASSIGNEDBYWHO);
	AddIndex(ASSIGNEDTOWHO);
	AddIndex(PRIORITYASSIGNEDDATE);
	AddIndex(NEXTTARGETDATE);
	AddIndex(TARGETDATEEXPECTATION);
	AddIndex(PRIORITYWITHINPROJECT);
	AddIndex(DEPONWHO);
	AddIndex(DEPONWHO2);
	AddIndex(DEPONWHO3);
	AddIndex(DEPONWHO4);
	AddIndex(DEPONWHO5);
	AddIndex(DEPASSIGNEDWHEN);
	AddIndex(DEPEXPECTSAMPWHEN);
	AddIndex(DEPASSIGNDESC);
	AddIndex(DEPFEEDBACK);
	AddIndex(TASKDRIVERID);
	
	// Construct query objects ONCE to reduce maintenance sync failure risk
	showVisibleQuery = (IsNull(HIDDEN) || HIDDEN == "0") && TASKID >= 0;
	showHiddenQuery                     = HIDDEN == "1"  && TASKID >= 0;
	
	// Hide anything flagged to be hidden (from context menu)
	SetWhere(showVisibleQuery);
}

//==========================================================================================	
TaskGrid::~TaskGrid() {
}

//==========================================================================================	
void TaskGrid::Build() {
	// We built this in the constructor for some reason.  Oh well.
	built = true;
}

//==========================================================================================	
void TaskGrid::ReQuery(bool showHiddenFlags) {
	if (showHiddenFlags) {
		UrpSqlGrid::ReQuery(showHiddenQuery);
	} else {
		UrpSqlGrid::ReQuery(showVisibleQuery);
	}
}

//==========================================================================================	
void TaskGrid::Load(Connection *pconnection) {
	connection = pconnection;
	SetSession(*(pconnection->session));
	SetSortColumn(TASKNAME);
	Query();
	loaded = true;
	
	if (lastKnownRow >= 0 && lastKnownRow < GetCount()) {
		SetCursor(lastKnownRow);
	}

}

//==========================================================================================	
int TaskGrid::GetTaskId(int row/*=-1*/) {
	return Get(CalcCorrectRow(row), TASKID);
}
//==========================================================================================	
String TaskGrid::GetTaskName(int row) {
	return Get(CalcCorrectRow(row), TASKNAME);
}
//==========================================================================================	
String TaskGrid::GetTaskDesc(int row) {
	return Get(CalcCorrectRow(row), TASKDESC);
}
//==========================================================================================	
String TaskGrid::GetSolutionDesc(int row) {
	return Get(CalcCorrectRow(row), SOLUTIONDESC);
}
//==========================================================================================	
String TaskGrid::GetSrcCode(int row) {
	return Get(CalcCorrectRow(row), SRCCODE);
}
//==========================================================================================	
String TaskGrid::GetProjectName(int row) {
	return Get(CalcCorrectRow(row), PROJECTNAME);
}
//==========================================================================================	
String TaskGrid::GetNote(int row) {
	return Get(CalcCorrectRow(row), NOTE);
}
//==========================================================================================	
int TaskGrid::GetAssignedToWho(int row) {
	return Get(row, ASSIGNEDTOWHO);
}
//==========================================================================================	
int TaskGrid::GetAssignedByWho(int row) {
	return Get(row, ASSIGNEDBYWHO);
}
//==========================================================================================	
Date TaskGrid::GetPriorityAssignedDate(int row) {
	return Get(CalcCorrectRow(row), PRIORITYASSIGNEDDATE);
}
//==========================================================================================	
Date TaskGrid::GetNextTargetDate(int row) {
	return Get(CalcCorrectRow(row), NEXTTARGETDATE);
}
//==========================================================================================	
String TaskGrid::GetTargetDateExpectation(int row) {
	return Get(CalcCorrectRow(row), TARGETDATEEXPECTATION);
}
//==========================================================================================	
int TaskGrid::GetPriorityWithinProject(int row) {
	return Get(CalcCorrectRow(row), PRIORITYWITHINPROJECT);
}
//==========================================================================================	
int TaskGrid::GetDepOnWho(int row) {
	return Get(CalcCorrectRow(row), DEPONWHO);
}
//==========================================================================================	
int TaskGrid::GetDepOnWho2(int row) {
	return Get(CalcCorrectRow(row), DEPONWHO2);
}
//==========================================================================================	
int TaskGrid::GetDepOnWho3(int row) {
	return Get(CalcCorrectRow(row), DEPONWHO3);
}
//==========================================================================================	
int TaskGrid::GetDepOnWho4(int row) {
	return Get(CalcCorrectRow(row), DEPONWHO4);
}
//==========================================================================================	
int TaskGrid::GetDepOnWho5(int row) {
	return Get(CalcCorrectRow(row), DEPONWHO5);
}
//==========================================================================================	
Date TaskGrid::GetDepAssignedWhen(int row) {
	Date d = Get(row, DEPASSIGNEDWHEN);
	return Get(CalcCorrectRow(row), DEPASSIGNEDWHEN);
}
//==========================================================================================	
Date TaskGrid::GetDepExpectSampWhen(int row) {
	return Get(CalcCorrectRow(row), DEPEXPECTSAMPWHEN);
}
//==========================================================================================	
String TaskGrid::GetDepAssignDesc(int row) {
	return Get(CalcCorrectRow(row), DEPASSIGNDESC);
}
//==========================================================================================	
String TaskGrid::GetDepFeedback(int row) {
	return Get(CalcCorrectRow(row), DEPFEEDBACK);
}
//==========================================================================================	
int TaskGrid::GetTaskDriverId(int row) {
	return Get(CalcCorrectRow(row), TASKDRIVERID);
}

//==========================================================================================	
Task TaskGrid::BuildTask(int row) {
	Task task(
				GetTaskId                (row)
			,	GetTaskName              (row)
			,	GetTaskDesc              (row)
			,	GetSolutionDesc          (row)
			,	GetProjectName           (row)
			,	GetSrcCode               (row)
			,	GetNote                  (row)
			,	GetAssignedToWho         (row)
			,	GetAssignedByWho         (row)
			,	GetPriorityAssignedDate  (row)
			,	GetNextTargetDate        (row)
			,	GetTargetDateExpectation (row)
			,	GetPriorityWithinProject (row)
			,	GetDepOnWho              (row)
			,	GetDepOnWho2             (row)
			,	GetDepOnWho3             (row)
			,	GetDepOnWho4             (row)
			,	GetDepOnWho5             (row)
			,	GetDepAssignedWhen       (row)
			,	GetDepExpectSampWhen     (row)
			,	GetDepAssignDesc         (row)
			,	GetDepFeedback           (row)
			,	GetTaskDriverId          (row)
	);
	return task;
}

//==========================================================================================	
bool TaskGrid::SaveTask(Task &task) {
	int row = Find(task.taskId, TASKID);
	if (row < 0) return false;
	Set(row, TASKNAME               , task.taskName);
	Set(row, TASKDESC               , task.taskDesc);
	Set(row, SOLUTIONDESC           , task.solutionDesc);
	Set(row, PROJECTNAME            , task.projectName);
	Set(row, SRCCODE                , task.srcCode);
	Set(row, NOTE                   , task.note);
	Set(row, ASSIGNEDTOWHO          , task.assignedToWho);
	Set(row, ASSIGNEDBYWHO          , task.assignedByWho);
	Set(row, PRIORITYASSIGNEDDATE   , task.nextTargetDate);
	Set(row, NEXTTARGETDATE         , task.nextTargetDate);
	Set(row, TARGETDATEEXPECTATION  , task.targetDateExpectation);
	Set(row, PRIORITYWITHINPROJECT  , task.priorityWithinProject);
	Set(row, DEPONWHO               , task.depOnWho);
	Set(row, DEPONWHO2              , task.depOnWho2);
	Set(row, DEPONWHO3              , task.depOnWho3);
	Set(row, DEPONWHO4              , task.depOnWho4);
	Set(row, DEPONWHO5              , task.depOnWho5);
	Set(row, DEPASSIGNEDWHEN        , task.depAssignedWhen);
	Set(row, DEPEXPECTSAMPWHEN      , task.depExpectSampWhen);
	Set(row, DEPASSIGNDESC          , task.depAssignDesc);
	Set(row, DEPFEEDBACK            , task.depFeedback);
	Set(row, TASKDRIVERID           , task.taskDriverId);
		
	Accept();
	return true;
}

