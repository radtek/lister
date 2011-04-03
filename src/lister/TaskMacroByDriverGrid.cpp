/***********************************************************************************************
*  lister - TaskMacroByDriverGrid.cpp
*  
*  Replaces TaskMacroGrid.  Displays a grid with macros listed vertically (as TaskGrid did),
*  and Drivers or Execution Paths listed horizontally.  Internally a crosstab query joins 
*  several tables to make it appear this way.  Since the column count varies depending on
*  how many drivers are assigned to a task, this can't be stored as a simple table or view. 
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

#include "TaskMacroByDriverGrid.h"
#include "Connection.h"
#include "shared_db.h"
#include <lister/Urp/UrpString.h>

#define SUFFIX_DRIVERID "_driverid"
#define SUFFIX_TASKMACRODRIVERREPLID "_taskmacrodriverreplacementid"
//==============================================================================================
TaskMacroByDriverGrid::TaskMacroByDriverGrid() : UrpGrid() {
	taskId = UNKNOWN; // Clear filter
	driverColumnOffset = 0;
}

//==============================================================================================
// Clean up any memory allocated for EditString objects for the replacements per driver column.
TaskMacroByDriverGrid::~TaskMacroByDriverGrid() {
	for (int i = 0; i < editors.GetCount(); i++) {
		delete editors[i];
	}

	editors.Clear(); // Flush all pointers (no real memory)
}

//==============================================================================================
/*virtual=0*/void TaskMacroByDriverGrid::Build(Connection *pconnection) {
	BuildBase(pconnection);
	WhenNewRow      = THISBACK(EnterNewRow);    // At the creation of a new row; pre-populate fields
	WhenRemoveRow   = THISBACK(RemoveRow);
	WhenAcceptedRow = THISBACK(SavePrompt);   // Build/Run SQL to save data

	// SizePos(); GRIDCTRL BUG: // Cannot SizePos in TestGrid.Build() or Graphics will cycle onto all cells (see SetImage(CtrlImg::go_forward())
	// Make the dropgrids of values extend past the width of the column they are for, to improve readability without wasting grid space.
	
//	connList   .SearchHideRows().Resizeable().Width(200);
//	scriptList .SearchHideRows().Resizeable().Width(200);
//	testTypList.SearchHideRows().Resizeable().Width(200);
//	compTypList.SearchHideRows().Resizeable().Width(200);
	
	// Set a display controller to help user see statuses of tests easily
	//SetDisplay(Single<TestGraphicalStatusDisplay>());
//	SetDisplay(Single<TestGraphicalStatusDisplay>());
	
	AddColumn(TASKID            , "Task",  50      ).NoEditable().Hidden();
	AddColumn(TASKMACID         , "Name", 100      ).NoEditable();
	AddColumn(SEARCHFOR         , "SrchFor"        ).Edit(fldSearchFor);
	AddColumn(REPLACEWITH       , "ReplWith"       ).Edit(fldReplaceWith); // Common to all driver value
	AddColumn(NOTE              , "Note"           ).Edit(fldNote);
	AddColumn(PROCESSORDER      , "processorder"   ).Hidden();                                 // Required for ordering
	
	// Drivers start to right of our constant set
	driverColumnOffset = GetFloatingColumnCount();
	
	if (WhenToolBarNeedsUpdating) WhenToolBarNeedsUpdating(UrpGrid::ADDEDSTRUCTURE);
	BuildComplete();
}

//==============================================================================================
// Load the macros, their search fors and all the various replacewith values by driver path.
// Note that this cannot be a view due to the complexity of crosstabbing variance across tasks.
/*virtual=0*/ void TaskMacroByDriverGrid::Load() {
	LoadBase();
	
	Clear();
	
	int driverCount = 0;
	
	// Construct massively wacky script to crosstab all the drivers or alternate paths for the
	// task set of macros.  It works.
	
	if (connection->SendQueryDataScript(Format("SELECT COUNT(*) FROM TaskDrivers WHERE taskId = %d", taskId))) {
		connection->Fetch();
		driverCount = connection->Get(0);
		if (driverCount == 0) {
			Exclamation("No drivers yet for this task");
			return;
		}
	} else {
		return;
	}
	
	// Create different non-crosstab script if no drivers.  This script will fail if no drivers found.
	// http://en.wikipedia.org/wiki/Printf#Format_placeholders
	// http://www.postgresql.org/docs/current/static/tablefunc.html
	
	// The tripling nature of the query is necessary so I can return 3 values per crosstab column instead of just one.
	// If I only return the pk of the desired task-macro-driver-replacement, I won't have anything to display in the grid
	// until I traverse the grid and do a select against TMDR for each pk, which will be a lot of code and time.
	// If I just fetch the replacement value string, I can build a pretty grid, but I won't have any link back to the table row I need to update with any new user vfalue.
	// I also fetch the the name of the driver (path) to use as a column header, or else the user will be baffled as to what a column signifies.
	// I suppose the last column could be fetched separately...
	
	// This first piece fetches mainly the searchfor string for a task macro and its default replacewith text
	String script =
		"SELECT tm.taskmacid, tm.taskid, tm.note, tm.searchfor, tm.replacewith \n";
	
	// Construct the supra-dynamic portion, driver columns generated in crosstab	
	for (int i = 1; i <= driverCount; i++) {
		script+= Format(",	replacewith_%1$d, tskmacdrvrepid_%1$d, driverid_%1$d \n", i);
	}
	
	script+=
		" FROM taskmacros tm\n"
		"-- By using the crosstab module, we can bring sparsely populated normalized data into an easy grid format.\n"
		"-- The grid loader will have to use Load to construct columns at the time of query, not before, since the columns vary.\n"
		"-- Unfortunately, it will have to query taskdrivers by taskid to verify if there are any drivers defined for this.\n"
		" JOIN (\n"
		"  select * from crosstab('\n"
		"  -- You must install the tablefunc module, by \n"
		"  -- C:/Program Files/PostgreSQL/9.0/share/contrib>psql -d postgres -U postgres -f ./tablefunc.sql\n"
		"  -- http://www.postgresql.org/docs/9.0/static/tablefunc.html\n"
		"  SELECT \n"
		"    	tm.taskmacid      rowid          -- Our link back to the task macro details\n"
		"  ,	td.processorder   category       -- Our driver order control since crosstab sorts across by this value\n"
		"  ,	tmdr.replacewith \"value\"       -- The value output\n"
		"   FROM taskmacros tm JOIN tasks t ON tm.taskid = t.taskid\n"
		"   LEFT JOIN taskdrivers td ON tm.taskid = td.taskid\n"
		"   LEFT JOIN taskmacrodriverreplacement tmdr ON tm.taskmacid = tmdr.taskmacroid AND td.driverid = tmdr.taskdriverid\n"
		"  WHERE tm.taskmacid >= 0\n"
		"  AND t.taskid = %1$d\n" // Parameter "1$" here, the taskId  Posix extension
		"  ORDER BY 1,2'\n"
		"  -- Have to fetch processorder so that it matches the category column, for managing sparse populations\n"
		"  -- Note: Don't build this if no rows found, or error: ERROR: provided \"categories\" SQL must return 1 column of at least one row SQL state: 42601\n"
		", 'select processorder from taskdrivers where taskid = %1$d order by processorder') AS\n"
		"  taskmacros(taskmacid integer\n"
		"		, replacewith_1 character varying -- Note that datatype is always varchar since its a macro. Construct these in grid.\n"
		"		, replacewith_2 character varying\n"
		"		, replacewith_3 character varying\n"
		"		, replacewith_4 character varying\n"
		"		, replacewith_5 character varying\n"
		"		, replacewith_6 character varying\n"
		"	)\n"
		") driverreplacewith\n"
		" ON tm.taskmacid = driverreplacewith.taskmacid\n"
		;
		
		// Fetch pks (ids) for referencing  task macro driver replacement strings (per path/element)
		// This way as we traverse, we can fetch other detail, especially in future as attributes extend the replacement options
		// key is required to update or insert a new replacement string for the searchfor script match
		script+=
		" JOIN (\n"
		"  select * from crosstab('\n"
		"  SELECT \n"
		"	    tm.taskmacid         rowid        -- Our link back to the task macro details\n"
		"  ,	td.processorder      category     -- Our driver order control since crosstab sorts across by this value\n"
		"  ,	tmdr.tskmacdrvrepid \"value\"     -- The value output\n"
		"   FROM taskmacros tm JOIN tasks t ON tm.taskid = t.taskid\n"
		"   LEFT JOIN taskdrivers td ON tm.taskid = td.taskid\n"
		"   LEFT JOIN taskmacrodriverreplacement tmdr ON tm.taskmacid = tmdr.taskmacroid AND td.driverid = tmdr.taskdriverid\n"
		"  WHERE tm.taskmacid >= 0\n"
		"  AND t.taskid = %1$d\n"
		"  ORDER BY 1,2'\n"
		", 'select processorder from taskdrivers where taskid = %1$d order by processorder') AS\n"
		"  taskmacros(taskmacid integer\n"
		"		, tskmacdrvrepid_1 integer -- datatype is now int since its a key. Construct these in grid.\n"
		"		, tskmacdrvrepid_2 integer\n"
		"		, tskmacdrvrepid_3 integer\n"
		"		, tskmacdrvrepid_4 integer\n"
		"		, tskmacdrvrepid_5 integer\n"
		"		, tskmacdrvrepid_6 integer\n"
		"	)"
		") tskmacdrvrepids\n"
		" ON tm.taskmacid = tskmacdrvrepids.taskmacid\n"
		;
		
		// Now add section to fetch the driver id pk so we can use it to add new driver/paths as user requests
		script+=
		" JOIN (\n"
		"  select * from crosstab('\n"
		"  SELECT \n"
		"	    tm.taskmacid         rowid        -- Our link back to the task macro details\n"
		"  ,	td.processorder      category     -- Our driver order control since crosstab sorts across by this value\n"
		"  ,	td.driverid         \"value\"     -- The value output\n" // value is a keyword in postgres, but that exact name is required by crosstab function
		"   FROM taskmacros tm JOIN tasks t ON tm.taskid = t.taskid\n"
		"   LEFT JOIN taskdrivers td ON tm.taskid = td.taskid\n"
		"   LEFT JOIN taskmacrodriverreplacement tmdr ON tm.taskmacid = tmdr.taskmacroid AND td.driverid = tmdr.taskdriverid\n"
		"  WHERE tm.taskmacid >= 0\n"
		"  AND t.taskid = %1$d\n"
		"  ORDER BY 1,2'\n"
		", 'select processorder from taskdrivers where taskid = %1$d order by processorder') AS\n"
		"  taskmacros(taskmacid integer\n"
		"		, driverid_1 integer -- datatype is now int since its a key. Construct these in grid.\n"
		"		, driverid_2 integer\n"
		"		, driverid_3 integer\n"
		"		, driverid_4 integer\n"
		"		, driverid_5 integer\n"
		"		, driverid_6 integer\n"
		"	)\n"
		") driverids\n"
		" ON tm.taskmacid = driverids.taskmacid\n"
		;

	driverColumnsPresent = GetFloatingColumnCount() - driverColumnOffset;
	ASSERT (driverColumnsPresent >= 0);
	
	// Remove all driver columns so we can start fresh
	RemoveColumn(driverColumnOffset, driverColumnsPresent);
	
	// Clear up and previous Editors
	
	for (int i = 0; i < editors.GetCount(); i++) {
		delete editors[i];
	}

	editors.Clear(); // Flush all pointers (no real memory)
	
	// Fetch our column headers (we already counted them) for all POSSIBLE drivers currently defined
	
	if (connection->SendQueryDataScript(Format("SELECT driverName FROM TaskDrivers WHERE taskId = %d ORDER BY processOrder", taskId))) {
		while (connection->Fetch()) {
			String newColumnName = connection->Get(0);
			editors.Add(newColumnName, new EditString());
			AddColumn(newColumnName).Edit(*(editors.Get(newColumnName)));
			// Add our invisible support columns for linking us back to updates/deletes
			Id driveridcol(newColumnName + SUFFIX_DRIVERID);
			AddIndex(driveridcol); // U++ prefers Ids rather than strings :(
			Id taskmacrodriverreplidcol(newColumnName + SUFFIX_TASKMACRODRIVERREPLID);
			AddIndex(taskmacrodriverreplidcol);
		}
	}
	
	const Vector<String> &driverNames = editors.GetKeys();
	
	if (connection->SendQueryDataScript(Format(script, taskId))) {
		while (connection->Fetch()) {
			Add();
			Set(TASKMACID  , connection->Get(TASKMACID  ));
			Set(TASKID     , connection->Get(TASKID     ));
			Set(NOTE       , connection->Get(NOTE       ));
			Set(SEARCHFOR  , connection->Get(SEARCHFOR  ));
			Set(REPLACEWITH, connection->Get(REPLACEWITH));

			int targetColumnNo = driverColumnOffset;

			// Populate driver columns from our crosstab
			for (int i = 1; i <= driverCount; i++) {
				String getColumnName = Format("REPLACEWITH_%d", i); // Case sensitive; must be upper even though displays in pgadmin as lower (duh!)
				String replwithval = connection->Get(getColumnName);
				String setColumnName = driverNames[i-1];
				// Write our crosstabbed value to the proper column
				EditString *es = editors.Get(setColumnName);
				es->SetText(replwithval);

				// Now get the task driver id (a little repetitive, same each row)
				getColumnName = Format("DRIVERID_%d", i); // Case sensitive; must be upper even though displays in pgadmin as lower (duh!)
				int taskdriverid = connection->Get(getColumnName); // Should never be null
				ASSERT_(taskdriverid != INT_NULL, "taskdriverid in our crosstab was null");
				Id driveridcol(driverNames[i-1] + SUFFIX_DRIVERID);
				Set(driveridcol, taskdriverid);
				
				// Now get the all important task macro driver replacement id, which is often null
				getColumnName = Format("TSKMACDRVREPID_%d", i); // Case sensitive; must be upper even though displays in pgadmin as lower (duh!)
				int taskmacrodriverreplacementid = connection->Get(getColumnName); // Should never be null
				Id taskmacrodriverreplidcol(driverNames[i-1] + SUFFIX_TASKMACRODRIVERREPLID);
				Set(taskmacrodriverreplidcol, taskmacrodriverreplacementid);
			}
		}
	} else {
		Exclamation("Script failed");
		return;
	}

	// Create spare columns according to driver columns
	// Load into grid
	
	LoadComplete();
}

//==============================================================================================
// Must be set or this will show everything.
void TaskMacroByDriverGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
}

//==============================================================================================
// Added manually from appending a row.
void TaskMacroByDriverGrid::EnterNewRow() {
	if (taskId != UNKNOWN) {
		Set(TASKID, taskId);
	}

	GoTo(GetCurrentRow(), 2); // Move to 2nd coloumn, past the color status
	StartEdit(); // Puts you into editmode so you can start typing the instance name right away.
}

//==============================================================================================
// User right clicked and selected delete.
void TaskMacroByDriverGrid::RemoveRow() {
	ASSERT(connection);
	ASSERT(connection->session->IsOpen());
	ASSERT(IsCursor());
	int row = GetCursor();
	int id = Get(row, TASKMACID);
	
	String deleteDependentsScript = Format("DELETE FROM TaskMacroDriverReplacement WHERE taskMacroId = %d", id);
	String controlScript = Format("DELETE FROM TaskMacros WHERE taskMacroId = %d", id);
	
	int rsp = PromptOKCancel(CAT << "Remove Row from control database: " << controlScript << ", " << deleteDependentsScript);
	if (rsp == 1) {
		if (!connection->SendRemoveDataScript(deleteDependentsScript)) {
			CancelRemove();
			return;
		}
		if (!connection->SendRemoveDataScript(controlScript)) {
			CancelRemove();
			return;
		}
	} else {
		CancelRemove(); // Prevent GridCtrl from removing the row from screen.
	}
}

//==============================================================================================
void TaskMacroByDriverGrid::SavePrompt() {
	Save(true);
}

//==============================================================================================
void TaskMacroByDriverGrid::SaveNoPrompt() {
	Save(false);
}

//==============================================================================================
// Implement a simple save so UrpGrid can attempt to call when reordering
/*virtual*/ void TaskMacroByDriverGrid::SaveRow(int row, int newProcessOrder) {
	SetCursor(row);
	Set(row, PROCESSORDER, newProcessOrder);
	SaveNoPrompt();
}

//==============================================================================================
// Easily transform all grid columns to SQL Db columns if flagged as linked to db.
// This is a callback as defined "SqlInsert& operator()(Fields f, bool nokey = false);"
// Where Fields is just a sneaky "typedef Callback1<FieldOperator&> Fields;" in sqlexp.h!
// This is also used for 
void TaskMacroByDriverGrid::FieldLayout(FieldOperator& fo) {
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		GridCtrl::ItemRect& ir = GetFloatingColumn(i);
		if (ir.linkedtodbcol == 1) {
			// Call to "FieldOperator& operator()(Id id, Ref f) " in Sqlexp.h
			// Very complex to figger out.
			if (IsCursor()) {
				{
					String nm = GetFloatingColumnId(i);
					const char *x = nm.Begin();
					Value v = Get(GetFloatingColumnId(i));
					fo(GetFloatingColumnId(i), Get(GetFloatingColumnId(i)));
				}
			} else if(IsSelection()) {
				fo(GetFloatingColumnId(i), Get(GetFirstSelection(), GetFloatingColumnId(i)));
			} else {
				// No row selected
				fo(GetFloatingColumnId(i), Value(Null));
			}
		}
	}
}

//==============================================================================================
// Write this row of the grid to the database, either a new row or a modified row.
void TaskMacroByDriverGrid::Save(bool prompt) {
	ASSERT(connection);
	ASSERT(connection->session->IsOpen());
	ASSERT(IsCursor());
	int row = GetCursor();
	SqlStatement sts; 
	
	// Inserting a new task macro, with 0 to n new drivers

	if (IsNewRow()) {
		// Fetch the new searchFor string
		// Is it unique?
		// Generate a new process order
		// Insert a new taskmacro row ( but will not be all the fields in the grid!)
		// Fetch new taskmacroid
		// Update TASKMACID column value
		// For each driver string in driver columns, create new taskmacrodriverreplacement row
		;
	} else {
		// if any TaskMacro attributes changed, update TaskMacro table
		if (!IsModifiedRow()) return; // No change
		int taskMacroId = Get(row, TASKMACID);
		if (
			IsModified(NOTE)
		||	IsModified(SEARCHFOR)
		||	IsModified(REPLACEWITH)
		) {
			String updateScript = Format("UPDATE TaskMacros SET NOTE = %s, SEARCHFOR = %s, REPLACEWITH = %s WHERE TASKMACID = %d"
							, ToSQL(Get(row, NOTE))
							, ToSQL(Get(row, SEARCHFOR))
							, ToSQL(Get(row, REPLACEWITH))
							, taskMacroId);
			if (!connection->SendChangeDataScript(updateScript)) {
				Exclamation("Failed to update TaskMacro portion");
			}
		}

		const Vector<String> &driverNames = editors.GetKeys();
		
		// Check for any modified driver column values
		
		for (int i = 0; i < driverColumnsPresent; i++) {
			EditString *es = editors.operator[](i);
			ASSERT_(es, "No editor defined for this driver column");
			if (es->IsModified()) {
				
				// We should have a taskmacrodriverreplacement column populated
				
				String getColumnName = driverNames[i];
				getColumnName << SUFFIX_TASKMACRODRIVERREPLID;
				int taskmacrodriverreplacementid = Get(row, (const char *)getColumnName);
				// Lets see what changed, shall we?
				const WString &ws = (const WString &)es;
				if (ws.ToString().IsEmpty()) {
					// The user is trying to delete this replacement, and we don't support nulling a macrovalue :(
					
					// If we have a valid id to the replacement...
					if (!In(taskmacrodriverreplacementid, INT_NULL, UNKNOWN)) {
						//...then we will attempt to delete it from the db
						String deleteRepl = Format("DELETE FROM TaskMacroDriverReplacement WHERE TskMacDrvRepId = %d", taskmacrodriverreplacementid);
						if (!connection->SendChangeDataScript(deleteRepl)) {
							Exclamation("Failed to delete a driver replacement value");
							// Leave the id in the grid in case we want to update it, since it failed to delete
						} else {
							// We deleted it, so we make sure we don't accidentally try to delete it
							Set(row, (const char *)getColumnName, INT_NULL);
						}
					}
				}
				
				// The user either inserted a value, or edited a value
				else {
					String newReplValue = ToSQL(ws.ToString());
					if (!In(taskmacrodriverreplacementid, INT_NULL, UNKNOWN)) {
						// A key value is there, so we'll try to update it with the new string
						// Note that we don't trim it.  Spaces are a valid macrotization
						String updateRepl = Format("UPDATE TaskMacroDriverReplacement SET ReplaceWith = %s WHERE TskMacDrvRepId = %d", newReplValue, taskmacrodriverreplacementid);

						if (!connection->SendChangeDataScript(updateRepl)) {
							Exclamation("Failed to update a driver replacement value");
							// Leave the id in the grid in case we want to update it, since it failed to delete
						} else {
							// We deleted it, so we make sure we don't accidentally try to delete it (The Load() should take care of it)
							Set(row, (const char *)getColumnName, INT_NULL);
						}
					}
					
					// User is inserting a new replacement value
					else {
						int taskdriverid = Get(row, (const char *)(driverNames[i] + SUFFIX_DRIVERID));
						String addRepl = Format("INSERT INTO TaskMacroDriverReplacement(TaskMacroId, ReplaceWith, TaskDriverId) SET ReplaceWith = %s "
								"VALUES(%s, %s, %s)", ToSQL(taskMacroId), ToSQL(newReplValue), ToSQL(taskdriverid));

						if (!connection->SendChangeDataScript(addRepl)) {
							Exclamation("Failed to add a driver replacement value");
							
						} else {
							// We added it.  Since below we Load() the grid over, we don't have to fetch the new key and stuff it in
							// cuz the load will do that.  The key must be there so if the user edits the value, it recognizes it as an update
							// and doesn't try to insert a new one.
						}
					}
				}
			}
		}
	}
	
	// Reload the grid so we can see if it worked.
	Load();
}

//==============================================================================================
bool TaskMacroByDriverGrid::MeaningfulDataChange() {
	if (!IsModifiedRow()) return false; // No change
	
	for (int i = 0; i < GetFloatingColumnCount(); i++) {
		GridCtrl::ItemRect& ir = GetFloatingColumn(i);
		if (ir.linkedtodbcol == 1) {
			if (!IsModified(GetFloatingColumnId(i))) {
				return false;
			}
		}
	}

	return true;
}

/*
SELECT tm.*, 
	replacewith_1, tskmacdrvrepid_1, driverid_1, 
	replacewith_2, tskmacdrvrepid_2, driverid_2 -- Construct in grid.
FROM taskmacros tm
-- By using the crosstab module, we can bring sparsely populated normalized data into an easy grid format.
-- The grid loader will have to use Load to construct columns at the time of query, not before, since the columns vary.
-- Unfortunately, it will have to query taskdrivers by taskid to verify if there are any drivers defined for this.
JOIN (
  select * from crosstab('
  -- You must install the tablefunc module, by 
  -- C:/Program Files/PostgreSQL/9.0/share/contrib>psql -d postgres -U postgres -f ./tablefunc.sql
  -- http://www.postgresql.org/docs/9.0/static/tablefunc.html
  SELECT 
	tm.taskmacid      rowid        -- Our link back to the task macro details
  ,	td.processorder   category     -- Our driver order control since crosstab sorts across by this value
  ,	tmdr.replacewith "value"       -- The value output
   FROM taskmacros tm JOIN tasks t ON tm.taskid = t.taskid
   LEFT JOIN taskdrivers td ON tm.taskid = td.taskid
   LEFT JOIN taskmacrodriverreplacement tmdr ON tm.taskmacid = tmdr.taskmacroid AND td.driverid = tmdr.taskdriverid
  WHERE tm.taskmacid >= 0
  AND t.taskid = 66
  ORDER BY 1,2'
  -- Have to fetch processorder so that it matches the category column, for managing sparse populations
  -- Note: Don't build this if no rows found, or error: ERROR: provided "categories" SQL must return 1 column of at least one row SQL state: 42601
, 'select processorder from taskdrivers where taskid = 66 order by processorder') AS
  taskmacros(taskmacid integer
		, replacewith_1 character varying -- Note that datatype is always varchar since its a macro. Construct these in grid.
		, replacewith_2 character varying
		, replacewith_3 character varying
		, replacewith_4 character varying
		, replacewith_5 character varying
		, replacewith_6 character varying
	)
) driverreplacewith
ON tm.taskmacid = driverreplacewith.taskmacid
JOIN (
  select * from crosstab('
  SELECT 
	tm.taskmacid         rowid        -- Our link back to the task macro details
  ,	td.processorder      category     -- Our driver order control since crosstab sorts across by this value
  ,	tmdr.tskmacdrvrepid "value"       -- The value output
   FROM taskmacros tm JOIN tasks t ON tm.taskid = t.taskid
   LEFT JOIN taskdrivers td ON tm.taskid = td.taskid
   LEFT JOIN taskmacrodriverreplacement tmdr ON tm.taskmacid = tmdr.taskmacroid AND td.driverid = tmdr.taskdriverid
  WHERE tm.taskmacid >= 0
  AND t.taskid = 66
  ORDER BY 1,2'
, 'select processorder from taskdrivers where taskid = 66 order by processorder') AS
  taskmacros(taskmacid integer
		, tskmacdrvrepid_1 integer -- Note that datatype is always varchar since its a macro. Construct these in grid.
		, tskmacdrvrepid_2 integer
		, tskmacdrvrepid_3 integer
		, tskmacdrvrepid_4 integer
		, tskmacdrvrepid_5 integer
		, tskmacdrvrepid_6 integer
	)
) tskmacdrvrepids
ON tm.taskmacid = tskmacdrvrepids.taskmacid
JOIN (
  select * from crosstab('
  SELECT 
	tm.taskmacid         rowid        -- Our link back to the task macro details
  ,	td.processorder      category     -- Our driver order control since crosstab sorts across by this value
  ,	td.driverid         "value"       -- The value output
   FROM taskmacros tm JOIN tasks t ON tm.taskid = t.taskid
   LEFT JOIN taskdrivers td ON tm.taskid = td.taskid
   LEFT JOIN taskmacrodriverreplacement tmdr ON tm.taskmacid = tmdr.taskmacroid AND td.driverid = tmdr.taskdriverid
  WHERE tm.taskmacid >= 0
  AND t.taskid = 66
  ORDER BY 1,2'
, 'select processorder from taskdrivers where taskid = 66 order by processorder') AS
  taskmacros(taskmacid integer
		, driverid_1 integer -- Note that datatype is always varchar since its a macro. Construct these in grid.
		, driverid_2 integer
		, driverid_3 integer
		, driverid_4 integer
		, driverid_5 integer
		, driverid_6 integer
	)
) driverids
ON tm.taskmacid = driverids.taskmacid
*/