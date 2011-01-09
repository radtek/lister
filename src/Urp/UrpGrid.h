#ifndef _Urp_UrpGrid_h_
#define _Urp_UrpGrid_h_

#include "UrpShared.h"

#define HIDDEN_COLUMN -2

//==========================================================================================	
class UrpGrid: public Upp::GridCtrl {
public:
	UrpGrid();
	// Obviously, U++ developer Uno was being a total dickwad when he refused to give a function
	// to extract column widths, so I've created it.
	int GetFloatingColumnWidth(int colno);
	// Series of corrective functions to deal with Lamo Uno's confusion about when and when not to adjust references for fixed columns
	int GetFloatingColumnCount();
	void UnhideFloatingColumnSilently(int i);
	void HideFloatingColumn(int i);
	void SetFloatingColumnWidth(int i, int w);
	Id GetFloatingColumnId(int n) const;
	GridCtrl::ItemRect& GetFloatingColumn(int n);
	//  GridCtrl is supposed to Xmlize, but I don't see it doing anything, so I've written my own.
	//  Have to save by name so that code changes that add/subtract columns will not cause confusion.
	//  Also, we save hidden state by name instead of position, which can really mess with a grid.
	void Xmlize(XmlIO xml);
};

#endif
