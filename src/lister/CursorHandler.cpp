#include "CursorHandler.h"
#include "SoundHandler.h"
#include "Connection.h"
#include "OutputGrid.h"
#include "OutputStat.h"
#include "SQLTypes.h"
#include <lister/Sql/Sql.h>

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
void CursorHandler::CursorOutputCancelled() {
	// Need access point added into Sql (virtual) and set in ODBC.  Sybase may have bug.
}

//==============================================================================================
// Run a script and push output to the grid, or to a table. Update the stats.
bool CursorHandler::Run(Script &sob, JobSpec &jobSpec, ContextMacros *contextMacros) {
	ASSERT(connection);
	int x = 0;
	WaitCursor wc;

	TimeStop t;
	t.Reset();
	
	if (connection->instanceTypeName == INSTTYPNM_ORACLE) {
		if (jobSpec.outputStat) {
			jobSpec.outputStat->SetStatus("Configuring instance");
		}
		if (!connection->SendChangeEnvScript("alter session set nls_date_format='DD-MON-RR'", contextMacros, true /* silent */)) {
			Speak(EVS_EXECUTE_FAILED);
			if (jobSpec.outputStat) {
				jobSpec.outputStat->SetStatus("Configuring instance failed.");
			}
			return false;
		}
	}
	
	if (jobSpec.outputStat) {
		jobSpec.outputStat->SetStatus("Sending script to instance for processing");
	}
	// We always send macros to our targets.  Only when we save scripts do we not expand the macros, elsewise we could never save them, huh?
	if (!connection->SendQueryDataScript(sob.scriptPlainText, contextMacros, true /* silent, we'll handle errors */, true /* expand macros */, jobSpec.log)) {
		// We make our own beep, so that it distintively identifies a "lister execution failure" to the user, not a generic windows failure
		if (jobSpec.outputStat) {
			jobSpec.outputStat->SetStatus("Script processing failed");
		}
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

	if (jobSpec.outputStat) {
		jobSpec.outputStat->SetStatus("Script processing succeeded");
	}
	Speak(EVS_EXECUTE_SUCCEEDED);
	int colCount = connection->GetColumns();
	if (jobSpec.outputStat) {
		jobSpec.outputStat->SetColumnCount(colCount);
	}
	
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
	
	bool failed = false;
	
	//__________________________________________________________________________________________
	if (sob.scriptTarget == Script::SO_TABLE) {
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
			if (jobSpec.outputStat) {
				jobSpec.outputStat->SetStatus("Rebuilding target");
			}
			LogLine(CAT << "Rebuilding target: " << outputTable);
			RebuildTableFromConnection(outputTable, jobSpec, contextMacros);
		}
		
		LogLine(CAT << "Beginning load into " << outputTable << " using Postgres COPY command");
		rowcount = LoadIntoTableFromConnectionCOPY(outputTable, adaptedRowLimit, jobSpec);
		
		String msg; 
		if (rowcount < 0) { 
			msg << "Target may be corrupted due to copy error";
			failed = true;
		} else {
			msg << "Inserted " << rowcount << " rows into " << outputTable << ", took " << t.ToString();
		}
		if (!jobSpec.batchMode) Exclamation(msg);
		if (jobSpec.log) LogLine(msg);
	
	//__________________________________________________________________________________________
	} else if (sob.scriptTarget == Script::SO_SCREEN) { // load to screen grid

		// Hmmmm, should support append to end of screen
		rowcount = LoadIntoScreenGridFromConnection(sob.outputGrid, jobSpec, sob);
		LogLine(CAT << "Output " << rowcount << " to screen.");
	}
	
	if (jobSpec.outputStat) {
		if (failed) {
			jobSpec.outputStat->SetStatus("Output operation failed");
		} else {
			jobSpec.outputStat->SetStatus("Completed fetch");
		}
		
		jobSpec.outputStat->SetRowCount(rowcount); // rowcount may be an error code in negative range
		Time stoppedWhen = GetSysTime();
		jobSpec.outputStat->SetStoppedWhen(stoppedWhen);
		Time startedWhen = jobSpec.outputStat->GetStartedWhen();
		Interval runtime(startedWhen, stoppedWhen);
		jobSpec.outputStat->SetFetchTime(runtime);
	}

	if (failed) {
		return false; // Allow any batches to cancel
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
	
	LogLine("COPY staging script complete.");
	Speak(EVS_FETCH_COMPLETED);
	// Shell and execute.
	String shellLine = "psql --host localhost --dbname postgres --username postgres -f C:/MyApps/lister/totable.sql";
	LogLine(CAT << "Shell: " << DeQtf(shellLine));
	LocalProcess lp(shellLine);
	GuiSleep(10);
	int e = lp.GetExitCode();
	String estr = lp.Get();
	bool errorincopy = false;
	bool isrunning = lp.IsRunning();
		
	if (!isrunning) {
		LogLine(CAT << "Never even started! e=" << e << ", stdout=" << estr);
	}
	
	while (isrunning) {
		GuiSleep(10);
		ProcessEvents();		
		estr = lp.Get();
		if (estr.Find("ERROR:") >= 0) {
			errorincopy = true;
			lp.Kill();
			LogLine(CAT <<"Copy error: " << estr);
			break;
		}
		
		e = lp.GetExitCode(); // Appears to be useless
		isrunning = lp.IsRunning();
	}

	if (errorincopy) {
		LOG(CAT << "Error in copy proc:" << estr);
		LogLine(CAT << "Error in copy proc:" << estr);
		Speak(EVS_INSERT_FAILED);
		return -1;
	} else {
		LogLine("Copy complete");
		Speak(EVS_INSERT_COMPLETED);
		return rowcount;
	}
	
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
void CursorHandler::RebuildTableFromConnection(String outputTable, JobSpec &jobSpec, ContextMacros *contextMacros) {
	String script = Format("drop table %s", outputTable);
	controlConnection->SendChangeStructureScript(script, contextMacros, RUN_SILENT); // ignore errors
	
	script = Format("create table %s(\n", outputTable);
	int colCount = connection->GetColumns();

	// Build column list
	for (int i = 0; i < colCount; i++) {
		const SqlColumnInfo& ci = connection->GetColumnInfo(i);
		if (i) script << ", ";
		
		int actualwidth = ci.width;
		
		// Due to some bugs in ODBC determination of size, we must trap negative widths
		
		if (actualwidth <= 0) actualwidth = 200;
		
		String datadef;

		// The data width in ci does not define the expanded string width needed to store these
		if (ci.valuetype == TIME_V) {
			datadef = "timestamp with time zone";
		} else if(ci.bindtype == BIND_INT8) {
			datadef = "smallint"; // No tinyint support on Postgresql!
		} else if(ci.bindtype == BIND_INT16) {
			datadef = "smallint"; // -32768 to +32767
		} else if(ci.bindtype == BIND_INT32) {
			datadef = "integer"; // 
		} else if(ci.bindtype == BIND_INT64) {
			datadef = "bigint";
		} else if(ci.valuetype == DATE_V) {
			datadef = "date";
		} else if(ci.valuetype == INT_V) {
			datadef = "integer";
		} else {
			datadef = Format("character varying(%d)", actualwidth);
		}
		
		script << ci.name << " " << datadef <<"\n";
	}
	script << ")";
	
	controlConnection->SendChangeStructureScript(script);
}

//==============================================================================================
int CursorHandler::LoadIntoScreenGridFromConnection(OutputGrid *outputGrid, JobSpec &jobSpec, Script &sob) {
	int colCount = connection->GetColumns();
	bool gridStyle = false;
	bool tabStyle = !gridStyle;
	outputGrid->outputSpec.Clear();
	
	outputGrid->Ready(false);
	//outputGrid->Reset(); // Crashing when first column is sorted!  U++ bug.
	outputGrid->Clear(true /* Clear all columns.  Required or internal error hitems counts show all columns still, will crash. */);
	for (int i = colCount - 1; i >= 0; i--) {
		outputGrid->RemoveColumn(i);
	}

	// Get all the column definitions, especially the column name

	String logLine;
	
	if (gridStyle) logLine = "{{1:2 "; // Send a QTF marker ahead

	if (tabStyle) {
		logLine = "[";
		for (int i = 0; i < colCount; i++) {
			logLine << "~" << i * 300;
		}
	}

	// Fetch all column definitions and create Grid columns to match
	
	int sepcolno = -1;
	
	for (int i = 0; i < colCount; i++) {
		const SqlColumnInfo& ci = connection->GetColumnInfo(i);
		int w = GetTextSize(ci.name, StdFont()).cx + 14;

		outputGrid->AddColumn(ci.name).Width(w);
		outputGrid->SetColWidth(i + outputGrid->indicator, w);

		// Attach column info to the result set
		
		OutputColumnDef outputColumnDef;
		outputColumnDef.name = ci.name;
		outputColumnDef.sqlType = ci.sqltype;
		outputColumnDef.sqlTypeName = SQLTypeName(ci.sqltype);
		outputColumnDef.visibleWidth = w;
		outputColumnDef.groupNo = -1;
			
		// If the user wants to separate groups of rows when a value changes, we need the column # to watch
		// so we convert the requested name to an index.
		if (sob.addSepToOutput && ci.name == sob.outFldSepWhenValChange) {
			sepcolno = i;
			outputColumnDef.groupNo = 1; // Group level one is all way support right now.
		}
		
		outputGrid->outputSpec.outputColumnDefList.Add(outputColumnDef);
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

	Progress pi;
	pi.SetText("Fetched %d line(s)");
	pi.WhenClose = THISBACK(CursorOutputCancelled);
	
	int rc = 0;		
	Value prevSepFldValue;
	
	// Fetch data and display

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

		// Test for change in grouping value, so we can sep out for copy to Excel		
		if (rc > 1 && sob.addSepToOutput && sepcolno >= 0) {
			Value curSepFldValue = row.At(sepcolno);
			// Compare values
			if (curSepFldValue != prevSepFldValue) {
				
				for (int j=0; j < sob.sepRowCount; j++) {
					outputGrid->Add(); // Add blank line
				}
			}
		}

		if (sepcolno >= 0) {		
			prevSepFldValue = row.At(sepcolno);
		}

		// Write entire line to grid
		
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

