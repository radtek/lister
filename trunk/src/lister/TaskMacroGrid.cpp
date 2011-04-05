/***********************************************************************************************
*  lister - TaskMacroGrid.cpp
*  
*  Replaced by TaskMacroByDriverGrid.  This version only display a list of macros assigned to
*  a task.  The new version lists macros down the side as rows, with a column per Driver/
*  Execution Path.  Each Driver is for a different scenario, different feed, etc.
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

#include "TaskMacroGrid.h"
#include "Connection.h"

//==============================================================================================
TaskMacroGrid::TaskMacroGrid() : UrpSqlGrid() {
	taskId = UNKNOWN;
}

//==============================================================================================
// Called from WhenStartEdit from inside StartEdit	
//==============================================================================================
void TaskMacroGrid::UpdatingRow() {
	if (IsInsert()) { // Not IsInserting, this checks insertmode
		// Make sure we stay linked to the active task in the taskgrid.
		Set(TASKID, taskId);
	}
}

//==============================================================================================
// Trick to ask DB (Postgresql) to populate with its default function
//==============================================================================================
struct ForceDBDefaultConvert : public Convert {
	Value Format(const Value& v) const {
		if (IsNull(v))
			return "DEFAULT";
		else
			return v;
	}
};

//==============================================================================================
// Build columns
//==============================================================================================
void TaskMacroGrid::Build(Connection *pconnection) {
	connection = pconnection;
	SetTable(TASKMACROS);
	AddKey(TASKMACID);
	AddColumn(SEARCHFOR, "searchfor", 100).Edit(searchFor);
	AddColumn(REPLACEWITH, "replacewith", 100).Edit(replaceWith);
	// Problem: Column must be visible in order to use SortColumn, but then the DEFAULT
	// value defined in the db is not fetched and an NULL is pushed.
	AddColumn(PROCESSORDER, "order", 20);
	
	HideColumn(PROCESSORDER);
	AddColumn(NOTE, "note", 200).Edit(note);
	AddIndex(TASKID);
	WhenAcceptRow = THISBACK(CompleteNewRow);
	// DoInsert()->DoEdit()->StartEdit()
	WhenStartEdit = THISBACK(UpdatingRow);
	built = true;
}

//==============================================================================================
// Make sure the process order gets a default value that should be unique. Return true to
// signify accept and post to db.
//==============================================================================================
bool TaskMacroGrid::CompleteNewRow() {
	Value currProcessOrder = Get(PROCESSORDER); // Perhaps they set it manually?
	if (currProcessOrder.IsNull()) { // Nope.
		Set(PROCESSORDER, GetNextProcessOrder()); // Scan all the available orders and grab the next #
	}
	
	return true;
}

//==============================================================================================
// Must be set or this will show everything.
//==============================================================================================
void TaskMacroGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
	SetWhere((TASKID == taskId));
}

//==============================================================================================
void TaskMacroGrid::Load() {
	Clear(); // Just clear old content, in case its hanging around from last open task
	ClearSelection();
	ClearCache();
	SetSession(*(connection->session));
	SetSortColumn(PROCESSORDER); // List in order that they will be processed; very important since they are interdependent.
	Query();
	loaded = true;
}

//==============================================================================================
// Called from others who want a task macro grid.  By placing here we can hopefully 
// share some related SQL.
//==============================================================================================
/*static */void TaskMacroGrid::LoadTaskMacro(Connection *connection, DropGrid &dropGrid) {
	
	// OOPS! Buggy!
	if (!connection->SendQueryDataScript("select taskmacid!!!!!???, elementname, elementdesc, taskid, note, status from elements order by elementname")) {
		return;
	}
	
	dropGrid.Clear();
	while(connection->Fetch()) {
		dropGrid.Add(connection->Get(0), connection->Get(1));
	}

	dropGrid.SetKeyColumn(0);
	dropGrid.Add(Null, Null); // Make sure we support a null value or we won't be able to edit other attributes without assigning a connection, which isn't what we want.
}

