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
void CursorHandler::ColSize(Sql *cursor) {
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
bool CursorHandler::Run(OutputGrid *poutputGrid, String sql, bool loadIntoTable) {
	ASSERT(connection);
	int x = 0;
	outputGrid = poutputGrid;
	WaitCursor wc;

	outputGrid->Ready(false);
	//outputGrid->Reset(); // Crashing when first column is sorted!  U++ bug.
	outputGrid->Clear(true /* Clear all columns.  Required or internal error hitems counts show all columns still, will crash. */);
	for (int i = outputGrid->GetColumnCount() - 1; i >= 0; i--) {
		outputGrid->RemoveColumn(i);
	}
	outputGrid->Ready(true);
	
	if (connection->instanceTypeName == INSTTYPNM_ORACLE) {
		if (!connection->SendChangeEnvScript("alter session set nls_date_format='DD-MON-RR'", true /* silent */)) {
			Speak(EVS_EXECUTE_FAILED);
			return false;
		}
	}
	
	if (!connection->SendQueryDataScript(sql, true /* silent */)) {
		// We make our own beep, so that it distintively identifies a "lister executino failure" to the user, not a generic windows failure
		Speak(EVS_EXECUTE_FAILED);
		Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), Format("Error: [* %s].", DeQtf(connection->GetLastError())), t_("OK"));
		return false;
	}

	Speak(EVS_EXECUTE_SUCCEEDED);
	int colCount = connection->GetColumns();
	
	cw.Clear();
	cw.SetCount(colCount);

	String outputTable;
	
	if (loadIntoTable) {
		if (!UrpInputBox(outputTable, "Creating output table from script", "Enter unique name for table")) {
			return false;
		}
		
		String script = Format("create table %s(\n", outputTable);
		
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
		
		// Generate table name or input from user
		// Create table on controlconnection, generate column list from our get column info
	}
	
	outputGrid->Ready(false);

	if (loadIntoTable) {
		//begin trans
		int rowcount = 0;
		controlConnection->Begin();

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
//			if (rowcount == 100) {
//				Exclamation(CAT << "Inserted " << rowcount << " rows into " << outputTable);
//				break;
//			}
		}
		
		if (!controlConnection->SendChangeEnvScript("DEALLOCATE preparedInsert")) {
			return false;
		}
		
		controlConnection->Commit();
		
		// end trans?
		
		Exclamation(CAT << "Inserted " << rowcount << " rows into " << outputTable);		
		
	} else { // load to screen

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
		ColSize(connection);
		wc.Show();
		
		int rc = 0;		
		while(connection->Fetch()) {
			rc++;
			Vector<Value> row = connection->GetRow();
			
			// Measure the column widths of the first 10 values, should be representative?
			if (rc < 10) {
				for (int j = 0; j < connection->GetColumns(); j++) {
					Value val = row[j];
					Upp::Size sz = GetTextSize(StdFormat(val), StdFont());
					int w = sz.cx;
					cw[j] = Upp::max(cw[j], (int)(GetTextSize(StdFormat(row[j]), StdFont()).cx + 14));
				}
			}
			outputGrid->Add(row);
			x++;
			if (pi.StepCanceled()) break;
		}
		
		outputGrid->Ready(true);
	
		ColSize(connection);
		
		if (outputGrid->GetCount() > 0)
			outputGrid->SetCursor(0);
		
		outputGrid->SetFocus();
	}
	
	if (connection->WasError()) {
		Speak(EVS_EXECUTE_FAILED);
		connection->HandleDbError(ACTNDB_EXECSEL, &sql);
		return false;
	}

	return true;
}
