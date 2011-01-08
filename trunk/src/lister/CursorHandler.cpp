#include "CursorHandler.h"

//==========================================================================================	
CursorHandler::CursorHandler(Connection *pconnection) {
	ASSERT(pconnection);
	connection = pconnection;
}

//==========================================================================================	
void CursorHandler::ColSize(Sql *cursor) {
	int maxw = 18 * StdFont().Info().GetAveWidth();
	int maxx = outputGrid->GetSize().cx;
	int wx = 0;
	for(int i = 0; i < cursor->GetColumns(); i++) {
		outputGrid->SetColWidth(i + outputGrid->indicator, cw[i]);
	}
}

//==========================================================================================	
bool CursorHandler::Run(GridCtrl *poutputGrid, String sql) {
	ASSERT(connection);
	int x = 0;
	outputGrid = poutputGrid;
	WaitCursor wc;

	outputGrid->Ready(false);
	//outputGrid->Reset(); // Crashing when first column is sorted!  U++ bug.
	outputGrid->Clear();
	for (int i = outputGrid->GetColumnCount() - 1; i >= 0; i--) {
		outputGrid->RemoveColumn(i);
	}
	outputGrid->Ready(true);

	if (!connection->SendChangeEnvScript("alter session set nls_date_format='DD-MON-RR'", true /* silent */)) {
		Speak(EVS_EXECUTE_FAILED);
		return false;
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
	
	// Get all the column definitions, especially the column name

	outputGrid->Indicator();
	outputGrid->MultiSelect();
	outputGrid->MultiSorting();
	outputGrid->ColorRows();
	outputGrid->Dragging();
	outputGrid->Absolute(); // Set BEFORE you set column sizes
	outputGrid->Clipboard();
	outputGrid->SearchDisplay();
	outputGrid->Searching();
	outputGrid->Moving().MovingCols().MovingRows().AskRemove(false).Hiding();
	outputGrid->SelectRow(false);
	outputGrid->FixedPaste();
	outputGrid->Navigating();

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
	
	outputGrid->Ready(false);

	int rc = 0;		
	while(connection->Fetch()) {
		rc++;
		Vector<Value> row = connection->GetRow();
		
		// Measure the column widths of the first 10 values, should be representative?
		if (rc < 10) {
			for (int j = 0; j < connection->GetColumns(); j++) {
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
	
	if (connection->WasError()) {
		Speak(EVS_EXECUTE_FAILED);
		connection->HandleDbError(ACTNDB_EXECSEL, &sql);
		return false;
	}

	return true;
}
