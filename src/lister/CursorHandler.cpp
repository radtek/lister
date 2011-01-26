#include "CursorHandler.h"
#include "SoundHandler.h"
#include "Connection.h"
#include "OutputGrid.h"

//==============================================================================================
CursorHandler::CursorHandler(Connection *pcontrolConnection, Connection *pconnection) {
	ASSERT(pconnection);
	connection = pconnection;
	ASSERT(pcontrolConnection);
	controlConnection = pcontrolConnection;
}

//==============================================================================================
void CursorHandler::ColSize(OutputGrid *outputGrid, Sql *cursor) {
}

//==============================================================================================
// Run a script and push output to the grid, or to a table
bool CursorHandler::Run(Script &sob, JobSpec &jobSpec) {
	ASSERT(connection);
	int x = 0;
	WaitCursor wc;

	TimeStop t;
	t.Reset();
	
	if (connection->instanceTypeName == INSTTYPNM_ORACLE) {
		if (!connection->SendChangeEnvScript("alter session set nls_date_format='DD-MON-RR'", true /* silent */)) {
			Speak(EVS_EXECUTE_FAILED);
			return false;
		}
	}
	
	// We always send macros to our targets.  Only when we save scripts do we not expand the macros, elsewise we could never save them, huh?
	if (!connection->SendQueryDataScript(sob.scriptPlainText, true /* silent, we'll handle errors */, true /* expand macros */, jobSpec.log)) {
		// We make our own beep, so that it distintively identifies a "lister execution failure" to the user, not a generic windows failure
		Speak(EVS_EXECUTE_FAILED);
		if (!jobSpec.batchMode) {
			Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), Format("Error: [* %s].", DeQtf(connection->GetLastError())), t_("OK"));
		}
		LOG(connection->GetLastError());
		
		if (jobSpec.log) {
			LogLine(CAT << "Execution failed: " << DeQtf(connection->GetLastError()));
		}
		
		return false;
	}

	Speak(EVS_EXECUTE_SUCCEEDED);
	int colCount = connection->GetColumns();
	
	cw.Clear();
	cw.SetCount(colCount);

	String outputTable = sob.targetName;
	
	int rowcount = 0;
	
	int adaptedRowLimit;
	
	// Calculate rowlimit based on if this is a test mode
	if (jobSpec.testMode) {
		if (sob.rowLimit != -1 && sob.rowLimit < 100) {
			adaptedRowLimit = sob.rowLimit;
			if (jobSpec.log) LogLine(CAT << "Test mode`: Requested limit of \1" << sob.rowLimit << "\1 is actually lower than the test limit of 100`. Using requested limit.");
		} else {
			adaptedRowLimit = 100;
			if (jobSpec.log) LogLine(CAT << "Test mode`: Adapted limit from \1" << sob.rowLimit << "\1 down to " << adaptedRowLimit);
		}
	} else {
		adaptedRowLimit = sob.rowLimit;
	}
	

	//__________________________________________________________________________________________
	if (sob.scriptTarget == SO_TABLE) {
		// Only popup up user didn't enter an output table name
		if (outputTable.IsEmpty()) {
			if (jobSpec.batchMode) {
				throw new Exc("Batch mode: no output table name");
			}
			
			if (!UrpInputBox(outputTable, "Creating output table from script", "Enter unique name for table")) {
				return false;
			}
		}
		
		// Instead of truncating, we drop and create
		
		if (sob.fastFlushTarget) {
			RebuildTableFromConnection(outputTable, jobSpec);
		}
		
		rowcount = LoadIntoTableFromConnectionCOPY(outputTable, adaptedRowLimit, jobSpec);
		
		String msg; msg << "Inserted " << rowcount << " rows into " << outputTable << ", took " << t.ToString();
		if (!jobSpec.batchMode) Exclamation(msg);
		if (jobSpec.log) LogLine(msg);
	
	//__________________________________________________________________________________________
	} else if (sob.scriptTarget == SO_SCREEN) { // load to screen grid

		// Hmmmm, should support append to end of screen
		rowcount = LoadIntoScreenGridFromConnection(sob.outputGrid, jobSpec);
	}
	
	if (connection->WasError()) {
		Speak(EVS_EXECUTE_FAILED);
		connection->HandleDbError(ACTNDB_EXECSEL, &sob.scriptPlainText, jobSpec.log, jobSpec.batchMode);
		return false;
	}

	return true;
}

//==============================================================================================
int CursorHandler::LoadIntoTableFromConnectionCOPY(String outputTable, int rowLimit, JobSpec &jobSpec) {
	int rowcount = 0;
	
	// Generate table name or input from user
	// Create table on controlconnection, generate column list from our get column info

	//begin trans
	//controlConnection->Begin();
	int colCount = connection->GetColumns();
	String copyScript = "COPY "; // insert into %s values(", outputTable);
	copyScript << outputTable << " (";
	
	for (int i = 0; i < colCount; i++) {
		const SqlColumnInfo& ci = connection->GetColumnInfo(i);
		if (i) copyScript << ", ";
		copyScript << ci.name;
	}
	copyScript << ") FROM stdin;\n";

	while(connection->Fetch()) {
		Vector<Value> row = connection->GetRow();
		
		for (int i = 0; i < colCount; i++) {
			if (i) copyScript << "\t";
			String outputVal = row.At(i).ToString();
			if (outputVal.IsEmpty()) {
				copyScript << "\\N";
			} else {
				copyScript << connection->PrepForPostgresCopyFrom(row.At(i).ToString());
			}
		}
		copyScript << "\n";
		rowcount++;

		if (rowLimit >= 0 && rowcount == rowLimit) {
			break;
		}
	}
	
	copyScript << "\\.\n";
	
	
	FileOut fo("C:/MyApps/lister/totable.sql");
	
	fo.Put(copyScript);
	fo.Close();
	
	Speak(EVS_FETCH_COMPLETED);
	// Shell and execute.
	LocalProcess lp("psql --host localhost --dbname postgres --username postgres -f C:/MyApps/lister/totable.sql");
	
	while (lp.IsRunning()) {
		GuiSleep(0);
		ProcessEvents();		
	}

	Speak(EVS_INSERT_COMPLETED);
	
	return rowcount;
}

//==============================================================================================
// Assumes connection prepped with query already
/*protected: */
int CursorHandler::LoadIntoTableFromConnectionPREP(String outputTable, int rowLimit, JobSpec &jobSpec) {
	int rowcount = 0;
	
	// Generate table name or input from user
	// Create table on controlconnection, generate column list from our get column info

	//begin trans
	//controlConnection->Begin();
	int colCount = connection->GetColumns();
	String preparedScript = "PREPARE preparedInsert("; // insert into %s values(", outputTable);
	for (int i = 0; i < colCount; i++) {
		if (i) preparedScript << ", ";
		preparedScript << "varchar";
	}
	preparedScript << ") AS \nINSERT INTO " << outputTable << "(";
	
	for (int i = 0; i < colCount; i++) {
		const SqlColumnInfo& ci = connection->GetColumnInfo(i);
		if (i) preparedScript << ", ";
		preparedScript << ci.name;
	}
	preparedScript << ") VALUES(";
	for (int i = 0; i < colCount; i++) {
		const SqlColumnInfo& ci = connection->GetColumnInfo(i);
		if (i) preparedScript << ", ";
		preparedScript << "$" << i + 1;
	}
	preparedScript << ")";
	
	if (!controlConnection->SendChangeEnvScript(preparedScript)) {
		return false;
	}
	
	while(connection->Fetch()) {
		Vector<Value> row = connection->GetRow();
		String insertScript = "EXECUTE preparedInsert(";
		
		for (int i = 0; i < row.GetCount(); i++) {
			if (i) insertScript << ", ";
			String outputVal = row.At(i).ToString();
			if (outputVal.IsEmpty()) {
				insertScript << "NULL";
			} else {
				insertScript << "'" << row.At(i).ToString() << "'";
			}
		}
		insertScript << ")";
		// insert to control table
		if (!controlConnection->SendAddDataScript(insertScript)) {
			return false;
		}
		rowcount++;

		if (rowLimit >= 0 && rowcount == rowLimit) {
			break;
		}
	}
	
	if (!controlConnection->SendChangeEnvScript("DEALLOCATE preparedInsert")) {
		return false;
	}
	
	//controlConnection->Commit();
	return rowcount;	
}

//==============================================================================================
void CursorHandler::RebuildTableFromConnection(String outputTable, JobSpec &jobSpec) {
	String script = Format("drop table %s", outputTable);
	controlConnection->SendChangeStructureScript(script, RUN_SILENT); // ignore errors
	
	script = Format("create table %s(\n", outputTable);
	int colCount = connection->GetColumns();

	// Build column list
	for (int i = 0; i < colCount; i++) {
		const SqlColumnInfo& ci = connection->GetColumnInfo(i);
		if (i) script << ", ";
		
		int actualwidth = ci.width;
		
		// The data width in ci does not define the expanded string width needed to store these
		if (ci.type == TIME_V) {
			actualwidth = 32;
		} else if(ci.type == DATE_V) {
			actualwidth = 10;
		}
		
		script << ci.name << " character varying(" << actualwidth << ")\n";
	}
	script << ")";
	
	controlConnection->SendChangeStructureScript(script);
}

//==============================================================================================
int CursorHandler::LoadIntoScreenGridFromConnection(OutputGrid *outputGrid, JobSpec &jobSpec) {
	int colCount = connection->GetColumns();
	bool gridStyle = false;
	bool tabStyle = !gridStyle;
	
	outputGrid->Ready(false);
	//outputGrid->Reset(); // Crashing when first column is sorted!  U++ bug.
	outputGrid->Clear(true /* Clear all columns.  Required or internal error hitems counts show all columns still, will crash. */);
	for (int i = colCount - 1; i >= 0; i--) {
		outputGrid->RemoveColumn(i);
	}

	// Get all the column definitions, especially the column name

	String logLine;
	
	if (gridStyle) logLine = "{{1:2 ";

	if (tabStyle) {
		logLine = "[";
		for (int i = 0; i < colCount; i++) {
			logLine << "~" << i * 300;
		}
	}

	for (int i = 0; i < colCount; i++) {
		const SqlColumnInfo& ci = connection->GetColumnInfo(i);
		int w = GetTextSize(ci.name, StdFont()).cx + 14;
		outputGrid->AddColumn(ci.name).Width(w);
		outputGrid->SetColWidth(i + outputGrid->indicator, w);
		cw[i] = w;
		
		if (jobSpec.log) {
			if (gridStyle) if (i) logLine << ":: "; // Insert grid line
			if (tabStyle) logLine << " -|";
			// @m for dark Magenta, @R for bright red (blinking?) Really helps make the log viewer readable
			// Use deqtf so that underscores are not eaten
			if (gridStyle) logLine << DeQtf(ci.name);
			if (tabStyle) logLine << "[@m " << DeQtf(ci.name) << "]";
		}
	}
	
	// Write column headers to log, so user can save
	if (jobSpec.log && tabStyle) {
		logLine << "]";
		LogLineUnembellished(logLine);
	}

	// Fetch data and display
	Progress pi;
	pi.SetText("Fetched %d line(s)");

	int rc = 0;		

	while(connection->Fetch()) {
		if (tabStyle) {
			logLine = "[";
			for (int i = 0; i < colCount; i++) {
				logLine << "~" << i * 300;
			}
		}
		
		rc++;
		Vector<Value> row = connection->GetRow();
		
		// Measure the column widths of the first 10 values, should be representative?
		if (rc < 10) {
			for (int i = 0; i < colCount; i++) {
				Value val = row[i];
				Upp::Size sz = GetTextSize(StdFormat(val), StdFont());
				int w = sz.cx;
				cw[i] = Upp::max(cw[i], (int)(GetTextSize(StdFormat(row[i]), StdFont()).cx + 14));
			}
		}
		outputGrid->Add(row);
		
		// Log output columns
		
		if (jobSpec.log) {

			for (int i = 0; i < colCount; i++) {
				if (i && gridStyle) logLine << ":: ";
				if (tabStyle) logLine << " -|";
				Value v = row.At(i); 
				// @m for dark Magenta, @R for bright red (blinking?) Really helps make the log viewer readable
				//logLine << "[@m " << (v.IsNull()? "[@R Null]" : DeQtf(AsString(v))) << "]";
				logLine << "[@m " << (v.IsNull()? "[@R Null]" : DeQtf(AsString(v))) << "]";
			}
			if (jobSpec.log && tabStyle) {
				logLine << "]";
				LogLineUnembellished(logLine); // Leave off TimeStamp prefix so users can copy/paste
			}
		}
		
		if (pi.StepCanceled()) break;
	}

	if (gridStyle) logLine << "}}";
	
	if (jobSpec.log && gridStyle) LogLine(logLine);
	
	outputGrid->Ready(true);

	// Resize column based on width of first 10 rows
	for (int i = 0; i < colCount; i++) {
		outputGrid->SetColWidth(i + outputGrid->indicator, cw[i]);
	}

	if (outputGrid->GetCount() > 0)
		outputGrid->SetCursor(0);
	
	outputGrid->SetFocus();
	
	return outputGrid->GetCount();
}

//	if (loadIntoTable) {
//		//begin trans
//		int rowcount = 0;
//		controlConnection->Begin();
//		while(connection->Fetch()) {
//			
//			Vector<Value> row = connection->GetRow();
//			String insertScript = Format("insert into %s values(", outputTable);
//			for (int i = 0; i < row.GetCount(); i++) {
//				if (i) insertScript << ", ";
//				String outputVal = row.At(i).ToString();
//				if (outputVal.IsEmpty()) {
//					insertScript << "NULL";
//				} else {
//					insertScript << "'" << row.At(i).ToString() << "'";
//				}
//			}
//			insertScript << ")";
//			// insert to control table
//			if (!controlConnection->SendAddDataScript(insertScript)) {
//				return false;
//			}
//			rowcount++;
////			if (rowcount == 100) {
////				Exclamation(CAT << "Inserted " << rowcount << " rows into " << outputTable);
////				break;
////			}
//		}
//		
//		controlConnection->Commit();
//		
//		// end trans?
//		
//		Exclamation(CAT << "Inserted " << rowcount << " rows into " << outputTable);		
//		
//	} else { // load to screen
