#include "RecordViewGrid.h"
#include <CtrlLib/CtrlLib.h>
#include "Connection.h"
#include "shared_db.h"

//==============================================================================================
Id IDColumnName("Name");
Id IDColumnValue("Value");
Id IDColumnDataType("DataType");
Id IDColumnWidth("Width");

using namespace Upp;

//==============================================================================================
RecordViewGrid::RecordViewGrid() {
	Indicator(); // Critical.  Sizing of columns will be distorted without this.
	MultiSelect();
	MultiSorting();
	ColorRows();
	Dragging();
	Absolute(); // Critical.  Columns will be scrunched to 2 bits without this.
	Clipboard();
	SearchDisplay();
	Searching();
	Moving();
	MovingCols();
	MovingRows();
	AskRemove(false);
	Hiding();
	SelectRow(false);
	FixedPaste();
	Navigating();
	LiveCursor(true);
}

//==============================================================================================
void RecordViewGrid::Build() {
	if (!built) {
		AddColumn(IDColumnName       , "Name"   , 100);
		AddColumn(IDColumnValue      , "Value"  , 100);
	}
	built = true;
}

//==============================================================================================
/*virtual*/ void RecordViewGrid::Load(Connection *pconnection) {
	Clear();
	// Nothing to do.  The MainGrid must add rows.
}

//==============================================================================================
void RecordViewGrid::Add(const String &columnName, const String &columnValue) {
	GridCtrl::Add();
	Set(IDColumnName, columnName);
	Set(IDColumnValue, columnValue);
	// Could add some jolly drilldowns and searches, eh?
	// Calendars, Calculators
	// Could drill to family tree: up to STRAW, TPR, BB flat file, BB Terminal!
	// or down to CSDR temp staging, staging, trade, order/exec, reference
	// Add Oracle comment, or postgre
	// Link to tickets/tasks related to this column or value or table or feed
}
