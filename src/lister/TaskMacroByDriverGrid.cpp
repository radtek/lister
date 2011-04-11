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
//==============================================================================================
TaskMacroByDriverGrid::~TaskMacroByDriverGrid() {
	for (int i = 0; i < editors.GetCount(); i++) {
		delete editors[i];
	}

	editors.Clear(); // Flush all pointers (no real memory)
}

//==============================================================================================
/*virtual=0*/void TaskMacroByDriverGrid::Build(Connection *pconnection) {
	BuildBase(pconnection);
	
	// User opened edit mode in a new/existing row; we want to focus them on the first valid
	// column to enter data in.
	
	WhenNewRow      = THISBACK(EnterNewRow);    // At the creation of a new row; pre-populate fields
	WhenRemoveRow   = THISBACK(RemoveRow);
	
	// User requesting approval and new process order by leaving row
	
	WhenInsertRow   = THISBACK(CompleteNewRow);

	// New/changed row accepted, now writing it to database
	
	WhenAcceptedRow = THISBACK(SavePrompt);   // Build/Run SQL to save data
	
	
	AddColumn(TASKID            , "Task",  50      ).NoEditable().Hidden();
	AddColumn(TASKMACID         , "Id", 100      ).NoEditable();
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
//==============================================================================================
/*virtual=0*/ void TaskMacroByDriverGrid::Load() {
	LoadBase();
	
	Clear();
	
	driverCount = 0;
	
	// Construct massively wacky script to crosstab all the drivers or alternate paths for the
	// task set of macros.  It works.
	
	if (connection->SendQueryDataScript(Format("SELECT COUNT(*) FROM TaskDrivers WHERE taskId = %d", taskId))) {
		connection->Fetch();
		driverCount = connection->Get(0);
		if (driverCount == 0) {
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
		"SELECT tm.taskmacid, tm.taskid, tm.note, tm.searchfor, tm.replacewith, tm.processorder\n";
	
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
		;

		for (int i = 1; i <= driverCount; i++) {
			// Note that datatype is always varchar since its a macro.
			script+= Format(",	replacewith_%1$d character varying\n", i);
		}
		
		script+=
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
		;
		
		for (int i = 1; i <= driverCount; i++) {
			script+= Format(",	tskmacdrvrepid_%1$d integer\n", i);
		}

		script+=
		"	)\n"
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
		;

		// We have to loop these columns for the crosstab() function not to choke		
		for (int i = 1; i <= driverCount; i++) {
			script+= Format(",	driverid_%1$d integer\n", i);
		}

		script+=
		"	)\n"
		") driverids\n"
		" ON tm.taskmacid = driverids.taskmacid\n"
		" ORDER BY processorder\n"
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
			EditString *es = new EditString();
			es->AlignRight();
			editors.Add(newColumnName, es);
			Id replwithcolid(newColumnName);
			AddColumn(replwithcolid)
				// Align our new macro replace values to the right since the meaningful
				// part of the string tends to be on the tail of a bunch of setup macros.
				.AlignRight()
				.Edit(*(editors.Get(newColumnName)))
				// You must set a converter or you can't set values!
				.SetConvert(*(editors.Get(newColumnName)));
			// Add our invisible support columns for linking us back to updates/deletes
			Id driveridcol(newColumnName + SUFFIX_DRIVERID);
			// U++ prefers Ids rather than strings :(
			// Don't set Hidden() here; it is overwritten since in OpenTaskDefWin(), first it calls Build(), then OpenWithConfig(), which blows away the column widths here. 
			// Created AlwaysHidden that Xmlize() in UrpGrid checks, and, whether there's
			// a config ref or not, it stays hidden.
			AddColumn(driveridcol).AlwaysHidden(); 
			Id taskmacrodriverreplidcol(newColumnName + SUFFIX_TASKMACRODRIVERREPLID);
			AddColumn(taskmacrodriverreplidcol).AlwaysHidden();
		}
	}
	
	const Vector<String> &driverNames = editors.GetKeys();
	
	if (connection->SendQueryDataScript(Format(script, taskId))) {
		while (connection->Fetch()) {
			Add();
			Set(TASKMACID   , connection->Get(TASKMACID   ));
			Set(TASKID      , connection->Get(TASKID      ));
			Set(NOTE        , connection->Get(NOTE        ));
			Set(SEARCHFOR   , connection->Get(SEARCHFOR   ));
			Set(REPLACEWITH , connection->Get(REPLACEWITH ));
			Set(PROCESSORDER, connection->Get(PROCESSORDER));

			int targetColumnNo = driverColumnOffset;

			// Populate driver columns from our crosstab
			for (int i = 1; i <= driverCount; i++) {
				
				// First column fetched is the replace with value, which may not be set
				
				String getColumnName = Format("REPLACEWITH_%d", i); // Case sensitive; must be upper even though displays in pgadmin as lower (duh!)
				String replwithval   = connection->Get(getColumnName);
				String setColumnName = driverNames[i-1];
				Id replwithcolid(setColumnName);
				Set(replwithcolid, replwithval);
				
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
//==============================================================================================
void TaskMacroByDriverGrid::SetTaskId(int ptaskId) {
	taskId = ptaskId;
}

//==============================================================================================
// Added manually from appending a row.
//==============================================================================================
void TaskMacroByDriverGrid::EnterNewRow() {
	if (taskId != UNKNOWN) {
		Set(TASKID, taskId);
	}

	GoTo(GetCurrentRow(), 2); // Move to 2nd coloumn, past the color status
	StartEdit(); // Puts you into editmode so you can start typing the instance name right away.
}

//==============================================================================================
// Make sure the process order gets a default value that should be unique. Return true to
// signify accept and post to db.
//==============================================================================================
void TaskMacroByDriverGrid::CompleteNewRow() {
	Value currProcessOrder = Get(PROCESSORDER); // Perhaps they set it manually?
	if (currProcessOrder.IsNull()) { // Nope.
		Set(PROCESSORDER, GetNextProcessOrder()); // Scan all the available orders and grab the next #
	}
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
//==============================================================================================
/*virtual*/ void TaskMacroByDriverGrid::SaveRow(int row, int newProcessOrder) {
	SetCursor(row);
	Set(row, PROCESSORDER, newProcessOrder);
	// Boy, this code is a mother-fucker!  Trying to get the grid to think I've set a value that I
	// want propagated back to the database is near impossible.
	// The following lines need to be pushed to an attribute of Set or SetWithExtremePrejudice.
	row_modified = 1; // Force it to update.
	Item &it = items[vitems[rowidx].id][aliases.Get(PROCESSORDER)];
	it.modified = 1;
	bool x0 = IsModifiedRow();
	bool x1 = IsModified(PROCESSORDER);
	SaveNoPrompt();
}

//==============================================================================================
// Easily transform all grid columns to SQL Db columns if flagged as linked to db.
// This is a callback as defined "SqlInsert& operator()(Fields f, bool nokey = false);"
// Where Fields is just a sneaky "typedef Callback1<FieldOperator&> Fields;" in sqlexp.h!
//==============================================================================================
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
//==============================================================================================
void TaskMacroByDriverGrid::Save(bool prompt) {
	ASSERT(connection);
	ASSERT(connection->session->IsOpen());
	ASSERT(IsCursor());
	int row = GetCursor();
	SqlStatement sts; 
	
	// Inserting a new task macro, with 0 to n new drivers

	if (IsNewRow()) {
		String addScript = Format("INSERT INTO TaskMacros("
			" NOTE, SEARCHFOR, REPLACEWITH, PROCESSORDER, TASKID)"
			" VALUES(%s, %s, %s, %s, %s)"
			, ToSQL(Get(row, NOTE))
			, ToSQL(Get(row, SEARCHFOR))
			, ToSQL(Get(row, REPLACEWITH))
			, ToSQL(Get(row, PROCESSORDER)) // Don't forget this or reordering stuff won't take any affect
			, ToSQL(Get(row, TASKID))
			);
			
		if (!connection->SendChangeDataScript(addScript)) {
			Exclamation("Failed to add");
		} else {
			// Get the newly created key from database sequencer
			int newTaskMacroId = connection->GetInsertedId("taskmacros", "taskmacid");
			ASSERT_(newTaskMacroId >= 0, "Insert did not return a key for new taskmacro, check database");
			// Stuff it in the user user's grid so future actions update this row
			Set(row, TASKMACID, newTaskMacroId);
		}
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
		||  IsModified(PROCESSORDER) // Don't forget this one!
		) {
			String updateScript = Format("UPDATE TaskMacros SET NOTE = %s, SEARCHFOR = %s, REPLACEWITH = %s, PROCESSORDER = %s WHERE TASKMACID = %d"
							, ToSQL(Get(row, NOTE))
							, ToSQL(Get(row, SEARCHFOR))
							, ToSQL(Get(row, REPLACEWITH))
							, ToSQL(Get(row, PROCESSORDER)) // Don't forget this or reordering stuff won't take any affect
							, taskMacroId);
			if (!connection->SendChangeDataScript(updateScript)) {
				Exclamation("Failed to update TaskMacro portion");
			}
		}

		const Vector<String> &driverNames = editors.GetKeys();
		
		// Check for any modified driver column values
		
		for (int i = 0; i < driverCount; i++) {
			Id driverColId(driverNames[i]);
			if (IsModified(row, driverColId)) {
				
				// We should have a taskmacrodriverreplacement column populated
				
				Id taskMacroDriverReplColId(driverNames[i] + SUFFIX_TASKMACRODRIVERREPLID);
				int taskMacroDriverReplacementId = Get(row, taskMacroDriverReplColId);
				// Lets see what changed, shall we?
				String newReplValue = Get(row, driverColId);
				
				if (newReplValue.IsEmpty()) {
					// The user is trying to delete this replacement, and we don't support nulling a macrovalue :(
					
					// If we have a valid id to the replacement...
					if (!In(taskMacroDriverReplacementId, INT_NULL, UNKNOWN)) {
						//...then we will attempt to delete it from the db
						String deleteRepl = Format("DELETE FROM TaskMacroDriverReplacement WHERE TskMacDrvRepId = %d", taskMacroDriverReplacementId);
						if (!connection->SendChangeDataScript(deleteRepl)) {
							Exclamation("Failed to delete a driver replacement value");
							// Leave the id in the grid in case we want to update it, since it failed to delete
						} else {
							// We deleted it, so we make sure we don't accidentally try to delete it
							Set(row, taskMacroDriverReplColId, INT_NULL);
						}
					} else {
						; // Not sure what state this is, so ignore
					}
				}
				
				// The user either inserted a value, or edited a value
				else {
					if (!In(taskMacroDriverReplacementId, INT_NULL, UNKNOWN)) {
						// A key value is there, so we'll try to update it with the new string
						// Note that we don't trim it.  Spaces are a valid macrotization
						String updateRepl = Format("UPDATE TaskMacroDriverReplacement SET ReplaceWith = %s WHERE TskMacDrvRepId = %d", ToSQL(newReplValue), taskMacroDriverReplacementId);

						if (!connection->SendChangeDataScript(updateRepl)) {
							Exclamation("Failed to update a driver replacement value");
							// Leave the id in the grid in case we want to update it, since it failed to update
						}
					}
					
					// User is inserting a new replacement value
					else {
						// Fetch which driver # this is for by constructing the column name
						Id taskDriverColId(driverNames[i] + SUFFIX_DRIVERID);
						int taskDriverId = Get(row, taskDriverColId);
						// Insert a row to support the new value that will link back into the crosstab
						String addRepl = Format("INSERT INTO TaskMacroDriverReplacement(TaskMacroId, ReplaceWith, TaskDriverId) "
								"VALUES(%s, %s, %s)", ToSQL(taskMacroId), ToSQL(newReplValue), ToSQL(taskDriverId));

						if (!connection->SendChangeDataScript(addRepl)) {
							Exclamation("Failed to add a driver replacement value");
							
						} else {
							int newTaskMacroDriverReplacementId = connection->GetInsertedId("taskmacrodriverreplacement", "tskmacdrvrepid");
							ASSERT_(newTaskMacroDriverReplacementId >= 0, "Insert did not return a key for newTaskMacroDriverReplacementId, check database");
							Set(row, taskMacroDriverReplColId, newTaskMacroDriverReplacementId);
						}
					}
				}
			}
		}
	}
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