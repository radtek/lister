/***********************************************************************************************
*  lister - TaskDefWin.cpp
*  
*  Task Editor Window
*  
*  This GUI screen allows the user to edit a Task and its attributes.
*
*  Accessed from the TaskGrid.cpp by double-clicking on a task row.  The task grid instance
*  currently sits in the lister class.  The linkage between the task grid's double-click event
*  and the opening of a TaskDefWin instance is made in lister's constructor.  This should be 
*  changed to be internal to TaskDefWin.  Dependency on lister class should be removed.
*  
*  Task Macros are managed from this screen.
*  
*  No other attributes are critical at this time.  Currently many of the objects on this screen
*  are defective, especially the ElementGrid.
*  
*  Author: Jeff Humphreys
*  
*  TODO: Make this a configurable serialized window.
*  Complete the TaskMacroByDriverGrid.
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
***********************************************************************************************/

#include "TaskDefWin.h"
#include "shared_db.h"
#include "Task.h"
#include "Connection.h"
#include "ContactGrid.h"
#include "image_shared.h"
#include <Draw/iml_header.h>

// Only in lister.cpp
//#include "image_shared.h"
//#include <Draw/iml_source.h>

//==============================================================================================
TaskDefWin::TaskDefWin() {
	// One exit button
	CtrlLayoutOKCancel(*this, "Task Detail");
	Icon(MyImages::edittask16());	
	Sizeable().Zoomable();
	CenterScreen();
}

//==============================================================================================
/*virtual*/ void TaskDefWin::Open() {
	UrpWindow::Open();
}

//==============================================================================================
/*virtual*/ void TaskDefWin::Open(Ctrl *owner) {
	UrpWindow::Open(owner);
}

//==============================================================================================
void TaskDefWin::Build(Connection *pconnection) {
	connection = pconnection;
	ok.WhenPush = THISBACK(SaveTask);
	
	if (!taskScriptGrid.built) {
		taskScriptGrid.Build(pconnection);
	}

	if (!linkGrid.built) {
		linkGrid.Build(pconnection);
	}

	if (!elementGrid.built) {
		elementGrid.Build(pconnection);
	}

	if (!taskMacroGrid.built) {
		taskMacroGrid.Build(pconnection);
	}

	if (!taskMacroByDriverGrid.built) {
		taskMacroByDriverGrid.Build(pconnection);
	}

	ContactGrid::LoadContact(connection, fldAssignedToWho);
	ContactGrid::LoadContact(connection, fldAssignedByWho);
	ContactGrid::LoadContact(connection, fldDepOnWho);
	ContactGrid::LoadContact(connection, fldDepOnWho2);
	ContactGrid::LoadContact(connection, fldDepOnWho3);
	ContactGrid::LoadContact(connection, fldDepOnWho4);
	ContactGrid::LoadContact(connection, fldDepOnWho5);
	
	ContactGrid::BuildContactList(fldAssignedToWho);
	ContactGrid::BuildContactList(fldAssignedByWho);
	ContactGrid::BuildContactList(fldDepOnWho);
	ContactGrid::BuildContactList(fldDepOnWho2);
	ContactGrid::BuildContactList(fldDepOnWho3);
	ContactGrid::BuildContactList(fldDepOnWho4);
	ContactGrid::BuildContactList(fldDepOnWho5);
}

//==============================================================================================
void TaskDefWin::Load(Task &ptask) {
	task = ptask;
	int taskId = task.taskId;
	fldTaskId.SetData(taskId);

	taskScriptGrid.SetWhere((TASKID == taskId));
	taskScriptGrid.Load();

	elementGrid.SetTaskId(taskId);
	elementGrid.Load();

	linkGrid.SetTaskId(taskId);
	linkGrid.Load();
	
	taskMacroGrid.SetTaskId(taskId);
	taskMacroGrid.Load();

	taskMacroByDriverGrid.SetTaskId(taskId);
	taskMacroByDriverGrid.Load();

	fldTaskName              .SetData(task.taskName);
	fldSrcCode               .SetData(task.srcCode);
	fldProjectName           .SetData(task.projectName);
	fldNote                  .SetData(task.note);
	fldTaskDesc              .SetData(task.taskDesc);
	fldSolutionDesc          .SetData(task.solutionDesc);
	fldNote                  .SetData(task.note);
	fldAssignedToWho         .SetData(task.assignedToWho);
	fldAssignedByWho         .SetData(task.assignedByWho);
	fldPriorityAssignedDate  .SetData(task.priorityAssignedDate);
	fldNextTargetDate        .SetData(task.nextTargetDate);
	fldTargetDateExpectation .SetData(task.targetDateExpectation);
	fldPriorityWithinProject .SetData(task.priorityWithinProject);
	fldDepOnWho              .SetData(task.depOnWho);
	fldDepOnWho2             .SetData(task.depOnWho2);
	fldDepOnWho3             .SetData(task.depOnWho3);
	fldDepOnWho4             .SetData(task.depOnWho4);
	fldDepOnWho5             .SetData(task.depOnWho5);
	fldDepAssignedWhen       .SetData(task.depAssignedWhen);
	fldDepExpectSampWhen     .SetData(task.depExpectSampWhen);
	fldDepAssignDesc         .SetData(task.depAssignDesc);
	fldDepFeedback           .SetData(task.depFeedback);
}

//==============================================================================================
void TaskDefWin::SaveTask() {
	task.taskName              = fldTaskName              .GetData();
	task.srcCode               = fldSrcCode               .GetData();
	task.projectName           = fldProjectName           .GetData();
	task.note                  = fldNote                  .GetData();
	task.taskDesc              = fldTaskDesc              .GetData();
	task.solutionDesc          = fldSolutionDesc          .GetData();
	task.note                  = fldNote                  .GetData();
	task.assignedToWho         = fldAssignedToWho         .GetData();
	task.assignedByWho         = fldAssignedByWho         .GetData();
	task.priorityAssignedDate  = fldPriorityAssignedDate  .GetData();
	task.nextTargetDate        = fldNextTargetDate        .GetData();
	task.targetDateExpectation = fldTargetDateExpectation .GetData();
	task.priorityWithinProject = fldPriorityWithinProject .GetData();
	task.depOnWho              = fldDepOnWho              .GetData();
	task.depOnWho2             = fldDepOnWho2             .GetData();
	task.depOnWho3             = fldDepOnWho3             .GetData();
	task.depOnWho4             = fldDepOnWho4             .GetData();
	task.depOnWho5             = fldDepOnWho5             .GetData();
	task.depAssignedWhen       = fldDepAssignedWhen       .GetData();
	task.depExpectSampWhen     = fldDepExpectSampWhen     .GetData();
	task.depAssignDesc         = fldDepAssignDesc         .GetData();
	task.depFeedback           = fldDepFeedback           .GetData();
	
	if (WhenSaveTask) WhenSaveTask(task);
	Close();
}

//==============================================================================================	
// Dependendent on HACK in Ctrl class to make Xmlize virtual
/*virtual*/ void TaskDefWin::Xmlize(XmlIO xml) {
	// Use generic names internally to xml with numbers so if splitters added/moved, we can 
	// retain users setting if lucky
	xml
		("grid1", elementGrid           )
		("grid2", taskScriptGrid        )
		("grid3", linkGrid              )
		("grid4", taskMacroGrid         )
		("grid5", taskMacroByDriverGrid )
	;
	
	UrpConfigWindow::Xmlize(xml); // Make sure and save our window position!
}