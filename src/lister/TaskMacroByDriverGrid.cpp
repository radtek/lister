#include "TaskMacroByDriverGrid.h"
#include "Connection.h"
#include "shared_db.h"
#include <lister/Urp/UrpString.h>

//==============================================================================================
TaskMacroByDriverGrid::TaskMacroByDriverGrid() {
	taskId = UNKNOWN; // Clear filter
	driverColumnOffset = 0;
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
		"SELECT tm.taskmacid, tm.taskid, tm.note, tm.searchfor, tm.replacewith, \n"
		"	replacewith_1, tskmacdrvrepid_1, driverid_1, \n"
		"	replacewith_2, tskmacdrvrepid_2, driverid_2 -- Construct in grid.\n"
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

	int driverColumnsPresent = GetFloatingColumnCount() - driverColumnOffset;
	ASSERT (driverColumnsPresent >= 0);
	
	// Remove all driver columns so we can start fresh
	RemoveColumn(driverColumnOffset, driverColumnsPresent);
	
	VectorMap<String, EditString *> editors;
	
	// Fetch our column headers (we already counted them) for all POSSIBLE drivers currently defined
	if (connection->SendQueryDataScript(Format("SELECT driverName FROM TaskDrivers WHERE taskId = %d ORDER BY processOrder", taskId))) {
		while (connection->Fetch()) {
			String newColumnName = connection->Get(0);
			editors.Add(newColumnName, new EditString());
			AddColumn(newColumnName).Edit(*(editors.Get(newColumnName)));
		}
	}
	
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
				Set(targetColumnNo++, replwithval);
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
	
	String deleteDependentsScript = Format("DELETE FROM TaskMacros WHERE taskMacroId = %d", id);
	String controlScript = Format("DELETE FROM TaskMacros WHERE taskMacroId = %d", id);
	
	int rsp = PromptOKCancel(CAT << "Remove Row from control database: " << controlScript);
	if (rsp == 1) {
		if (!connection->SendRemoveDataScript(controlScript)) {
			CancelRemove();
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
		if (
			IsModified(NOTE)
		||	IsModified(SEARCHFOR)
		||	IsModified(REPLACEWITH)
		) {
			int taskMacroId = Get(row, TASKMACID);
			String updateScript = Format("UPDATE TaskMacros SET NOTE = %s, SEARCHFOR = %s, REPLACEWITH = %s WHERE TASKMACID = %d"
							, ToSQL(Get(row, NOTE))
							, ToSQL(Get(row, SEARCHFOR))
							, ToSQL(Get(row, REPLACEWITH))
							, taskMacroId);
			if (connection->SendChangeDataScript(updateScript)) {
				Exclamation("Updated TaskMacro portion");
			}
		}
		
		// Check for any modified driver column values
		// If empty to non-empty, insert
		// If non-empty to empty, delete
		// If non-empty to non-empty, update
		;
	}
//	if (IsNewRow()) {
//		Set(PROCESSORDER, GetNextProcessOrder());
//		sts = SqlStatement(SqlInsert(TASKMACROSBYDRIVER)(THISBACK(FieldLayout), true /*nokey, let database generate from sequence */));
//		if (WhenToolBarNeedsUpdating) WhenToolBarNeedsUpdating(UrpGrid::USERADDEDROW);
//
//	// Updating an existing task macro, and possibly updating and/or deleting several existing drivers
//			
//	} else {
//		sts = SqlStatement(SqlUpdate(TESTS)(THISBACK(FieldLayout)).Where(TESTID == Get(TESTID)));
//		if (WhenToolBarNeedsUpdating) WhenToolBarNeedsUpdating(UrpGrid::USERCHANGEDDATA);
//	}
//	
//	// Wrap in a SqlStatement object, which will somehow either call the SqlUpdate or
//	// SqlInsert operator SqlStatement() const, which then constructs a string and forms
//	// the constructor "explicit SqlStatement(const String& s) : text(s) {}", and so
//	// sets the text member.  I don't know what the explicit command means.
//	
//	
//	// Transform to proper Sql; Sql will not be properly formed until the dialect of the 
//	// connection is determined, in this case PGSQL.  Do not use GetText().
//	// This calls SqlCompile(dialect, text) to finalize the "text" member contents.
//	String controlScript = sts.Get(connection->GetDialect());
//
//	LOG(controlScript);
//	int rsp = 1;
//	
//	if (prompt) {		
//		rsp = PromptOKCancel(CAT << "Script being applied: " << controlScript);
//	}		
//	
//	if (rsp == 1) {
//		if (!connection->SendAddDataScript(controlScript)) {
//			// Beep
//			return;
//		}
//		
//		if (IsNewRow()) {
//			if (connection->GetRowsProcessed() > 0) {		
//				int testid = connection->GetInsertedId("tests", "testid");
//				if (testid >= 0) {
//					SetTestId(row, testid);
//				}
//			}
//		}
//	}
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