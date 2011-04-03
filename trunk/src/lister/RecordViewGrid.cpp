/***********************************************************************************************
*  lister - RecordViewGrid.cpp
*  
*  Simple popup from within RecordViewWin that supports record vertical view from an OutputGrid.
*  Useful for cut n paste straight to HTML paste for Outlook.  Can rapidly be search/filtered
*  for value or column name.  Sortable by value or column name.
*
*  TODO:
*  - Lookup metadata about a column.
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
// UrpGrid sets up all the grid behavior to be consistent across the app.
RecordViewGrid::RecordViewGrid() : UrpGrid() {
	Description("recordViewGrid");
}

//==============================================================================================
// Mandatory implementation
/*virtual=0*/void RecordViewGrid::Build(Connection *pconnection) {
	BuildBase(pconnection);
	
	if (!built) {
		AddColumn(IDColumnName       , "Name"   , 100);
		AddColumn(IDColumnValue      , "Value"  , 100);
	}
	BuildComplete();
}

//==============================================================================================
/*virtual=0*/ void RecordViewGrid::Load() {
	LoadBase();
	Clear();
	// Nothing to do.  The MainGrid must add rows.
	LoadComplete();
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
