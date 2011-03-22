#include "TaskDefWin.h"

#include "shared_db.h"
#include "Task.h"
#include "Connection.h"
#include "ContactGrid.h"

//==============================================================================================
TaskDefWin::TaskDefWin() {
	// One exit button
	CtrlLayoutOKCancel(*this, "Task Detail");
	
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
		("grid1", elementGrid)
		("grid2", taskScriptGrid)
		("grid3", linkGrid)
		("grid4", taskMacroGrid)
	;
	
	UrpConfigWindow::Xmlize(xml); // Make sure and save our window position!
}