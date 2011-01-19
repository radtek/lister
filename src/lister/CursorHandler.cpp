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
	int maxw = 18 * StdFont().Info().GetAveWidth();
	int maxx = outputGrid->GetSize().cx;
	int wx = 0;
	for(int i = 0; i < cursor->GetColumns(); i++) {
		int w = cw[i];
		outputGrid->SetColWidth(i + outputGrid->indicator, cw[i]);
	}
}

//==============================================================================================
// Run a script and push output to the grid, or to a table
bool CursorHandler::Run(Script &sob, bool log) {
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
	if (!connection->SendQueryDataScript(sob.script, true /* silent, we'll handle errors */, true /* expand macros */, log)) {
		// We make our own beep, so that it distintively identifies a "lister execution failure" to the user, not a generic windows failure
		Speak(EVS_EXECUTE_FAILED);
		Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), Format("Error: [* %s].", DeQtf(connection->GetLastError())), t_("OK"));
		return false;
	}

	Speak(EVS_EXECUTE_SUCCEEDED);
	int colCount = connection->GetColumns();
	
	cw.Clear();
	cw.SetCount(colCount);

	String outputTable = sob.targetName;
	
	int rowcount = 0;
	//__________________________________________________________________________________________
	if (sob.scriptTarget == SO_TABLE) {
		// Only popup up user didn't enter an output table name
		if (outputTable.IsEmpty()) {
			if (!UrpInputBox(outputTable, "Creating output table from script", "Enter unique name for table")) {
				return false;
			}
		}
		
		// Instead of truncating, we drop and create
		
		if (sob.fastFlushTarget) {
			RebuildTableFromConnection(outputTable, log);
		}
		
		rowcount = LoadIntoTableFromConnectionCOPY(outputTable, sob.rowLimit, log);
		
		String msg; msg << "Inserted " << rowcount << " rows into " << outputTable << ", took " << t.ToString();
		Exclamation(msg);
		if (log) LogLine(msg);
	
	//__________________________________________________________________________________________
	} else if (sob.scriptTarget == SO_SCREEN) { // load to screen grid

		rowcount = LoadIntoScreenGridFromConnection(sob.outputGrid, log);
	}
	
	if (connection->WasError()) {
		Speak(EVS_EXECUTE_FAILED);
		connection->HandleDbError(ACTNDB_EXECSEL, &sob.script, log);
		return false;
	}

	return true;
}

//==============================================================================================
int CursorHandler::LoadIntoTableFromConnectionCOPY(String outputTable, int rowLimit, bool log) {
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
	
	// Shell and execute.
	LocalProcess lp("psql --host localhost --dbname postgres --username postgres -f C:/MyApps/lister/totable.sql");
	
	while (lp.IsRunning()) {
		GuiSleep(0);
		ProcessEvents();		
	}
	
	return rowcount;
}

//==============================================================================================
// Assumes connection prepped with query already
/*protected: */
int CursorHandler::LoadIntoTableFromConnectionPREP(String outputTable, int rowLimit, bool log) {
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
void CursorHandler::RebuildTableFromConnection(String outputTable, bool log) {
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
int CursorHandler::LoadIntoScreenGridFromConnection(OutputGrid *outputGrid, bool log) {
	int colCount = connection->GetColumns();

	outputGrid->Ready(false);
	//outputGrid->Reset(); // Crashing when first column is sorted!  U++ bug.
	outputGrid->Clear(true /* Clear all columns.  Required or internal error hitems counts show all columns still, will crash. */);
	for (int i = colCount - 1; i >= 0; i--) {
		outputGrid->RemoveColumn(i);
	}

	// Get all the column definitions, especially the column name

	for (int i = 0; i < colCount; i++) {
		const SqlColumnInfo& ci = connection->GetColumnInfo(i);
		int w = GetTextSize(ci.name, StdFont()).cx + 14;
		outputGrid->AddColumn(ci.name).Width(w);
		cw[i] = w;
	}

	// Fetch data and display
	Progress pi;
	pi.SetText("Fetched %d line(s)");
	ColSize(outputGrid, connection);

	int rc = 0;		
	while(connection->Fetch()) {
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
		if (log) {
			String logLine = "";
			
			for (int i = 0; i < colCount; i++) {
				const SqlColumnInfo& ci = connection->GetColumnInfo(i);
				if (i) logLine << ", ";
				Value v = row.At(i); 
				logLine << ci.name << "=" << (v.IsNull()? "Null" : AsString(v));
			}
			LogLine(logLine);
		}
		
		if (pi.StepCanceled()) break;
	}
	
	outputGrid->Ready(true);

	ColSize(outputGrid, connection);
	
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
