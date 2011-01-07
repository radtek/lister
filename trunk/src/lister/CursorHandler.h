#ifndef _CursorHandler_h_
#define _CursorHandler_h_

#include "shared.h"
#include "SoundHandler.h"
#include "Connection.h"


//==========================================================================================	
class CursorHandler : public TopWindow {
public:
	GridCtrl *outputGrid;
	Connection *connection;
	Vector<int> cw; // Column width calculator.  We add in heading widths and value widths as we load the data
	
	//==========================================================================================	
	CursorHandler(Connection *pconnection) {
		ASSERT(pconnection);
		connection = pconnection;
	}
	
	//==========================================================================================	
	void ColSize(Sql *cursor) {
		int maxw = 18 * StdFont().Info().GetAveWidth();
		int maxx = outputGrid->GetSize().cx;
		int wx = 0;
		for(int i = 0; i < cursor->GetColumns(); i++) {
			outputGrid->SetColWidth(i + outputGrid->indicator, cw[i]);
		}
	}
	
	//==========================================================================================	
	bool Run(GridCtrl *poutputGrid, String sql) {
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

};

#endif

/*
	colsopt.theme.Add(-1, "Flat");
	colsopt.theme.Add(0, "Cairo");		
	colsopt.theme.Add(1, "Gray");
	colsopt.theme.Add(2, "Plastik");		
	colsopt.theme.Add(3, "Lipstik");		
	colsopt.theme.Add(4, "Alloy");		
	colsopt.theme.Add(5, "Unknown 01");
	colsopt.theme.Add(6, "Default");

	case 0: grid->GetDisplay().SetTheme(~colsopt.theme);                  break;

	colsopt.odd_color.NoRampWheel();
	colsopt.even_color.NoRampWheel();
	colsopt.grid_color.NoRampWheel().NotNull();
	colsopt.focus_color.NoRampWheel();
	colsopt.live_color.NoRampWheel();

	WithEditOptionsLayout<ParentCtrl> editopt;
	
class NumbersOnRed : public Display
{
public:
  static bool numberString(String s);
	void PaintBackground(Draw& w, const Rect& r, const Value& q,
	                     Color ink, Color paper, dword style) const;
	
};

bool NumbersOnRed::numberString(String s)
{
  if(s.GetCount() == 0)
    return false;
  
  int i = 0; 
  if(s[0] == '+' || s[0] == '-')
    i++;  
  
  while(i < s.GetCount() && s[i] >= '0' && s[i] <= '9')
    i++;
  
  return i == s.GetCount();
}

void NumbersOnRed::PaintBackground(Draw& w, const Rect& r, const Value& q,
	                               Color ink, Color paper, dword style) const
{
	if(IsNumber(q) || (IsString(q) && numberString(AsString(q))))
	  paper = Color(255, 150, 150);
	
	Display::PaintBackground(w, r, q, ink, paper, style);
}

	
*/