#ifndef _Urp_UrpPaint_h_
#define _Urp_UrpPaint_h_

#include "UrpShared.h"

/*

grid.SetDisplay(Single<DispPM>()
or
AddColumn(..).SetDisplay

*/
//==============================================================================================
struct UrpFxdWdthFntDisplay : GridDisplay {
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
			           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe);
};

//==============================================================================================
struct TightFontDisplayForGridCtrl : GridDisplay {
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
			           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe);
};

//==============================================================================================
struct TightFontDisplayForArrayCtrl : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
};

//==============================================================================================
struct DispPM : GridDisplay {
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
			           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe);
};

class NumbersOnRed : public Display
{
public:
  static bool numberString(String s);
	void PaintBackground(Draw& w, const Rect& r, const Value& q,
	                     Color ink, Color paper, dword style) const;
	
};

#endif
