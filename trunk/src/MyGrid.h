#ifndef _MyGrid_h_
#define _MyGrid_h_

#include "shared.h"

//==========================================================================================	
class MyGrid: public GridCtrl {
public:
	//==========================================================================================	
	MyGrid() : GridCtrl() {
	}
	
	//==========================================================================================	
	//  GridCtrl is supposed to Xmlize, but I don't see it doing anything, so I've written my own.
	//  Have to save by name
	void Xmlize(XmlIO xml) {
		VectorMap<String, int> columnWidths;
		
		if (xml.IsLoading()) {
			
			xml("columnwidths", columnWidths);
			
			// Traverse all our saved user values.  If not here, they'll take their default width from the AddColumn of Build, during the Proportional setting
			for (int i = 0; i < columnWidths.GetCount(); i++) {
				String colName;
				int colWidth;
				colName = columnWidths.GetKey(i);
				int colNo = FindCol(colName); // See if the one saved still there (they come and go, don't ya know)
				if (colNo >= 0) {
					if (colName == "Dummy") {
						colWidth = 1; // A semi-invisible column to block bug in GridCtrl that flips out when last column has an image
					} else {
						colWidth = columnWidths.Get(colName);
					}
					SetColWidth(colNo, colWidth);
				}
			}
		} else {
			for (int i = 0; i < columnWidths.GetCount(); i++) {
				String colName;
				int colWidth;
				colName = columnWidths.GetKey(i);
				int colNo = FindCol(colName); // See if the one saved still there (they come and go, don't ya know)
				
				// Update the saved width				
				if (colNo >= 0) {
					columnWidths.Put(colName, colWidth);
					
				// If a saved labeled column width no longer in grid, drop it out of the listing (GetColumnWidths() can't do that)
				} else {
					columnWidths.RemoveKey(colName);
				}
			}
			
			// Check for new columns that have not been labeled in xmlizer before
			
			for (int i = 0; i < GetColumnCount(); i++) {
				ItemRect &ir = GetColumn(i);
				String colName = ir.GetName();
				if (columnWidths.Find(colName) < 0) {
					int colWidth = GetWidth(i); // GridCtrl API naming a little off :)  Should be part of ItemRect?
					columnWidths.Add(colName, colWidth);
				}
			}
			
			xml("columnwidths", columnWidths);
		}
	}
};

#endif
