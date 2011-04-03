/***********************************************************************************************
*  lister - LinkGrid.cpp
*  
*  Simple grid that sits within the Task editor.  Useful for saving important links to documents
*  visio diagrams, BRDs, DFAs, S&As, etc.
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

#include "LinkGrid.h"
#include "Connection.h"

//==============================================================================================
LinkGrid::LinkGrid() : UrpSqlGrid() {
}

//==============================================================================================
void LinkGrid::UpdatingRow() {
	// Called from WhenStartEdit from inside StartEdit	
	if (IsInsert()) { // Not IsInserting, this checks insertmode
		Set(TASKID, taskId);
	}
}

//==============================================================================================
void LinkGrid::Build(Connection *pconnection) {
	// Build columns
	connection = pconnection;
	SetTable(LINKS);
	AddKey(LINKID);
	AddColumn(LINKNAME, "link", 100).Edit(linkName);
	AddColumn(LINKURL, "url", 100).Edit(linkUrl);
	AddColumn(NOTE, "note", 200).Edit(note);
	AddIndex(TASKID);
	built = true;
	// DoInsert()->DoEdit()->StartEdit()
	WhenStartEdit = THISBACK(UpdatingRow);
}

//==============================================================================================
void LinkGrid::Load() {
	SetSession(*(connection->session));
	SetSortColumn(ELEMENTNAME);
	Query();
	loaded = true;
}
//==============================================================================================
void LinkGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
	SetWhere((TASKID == taskId));
}

