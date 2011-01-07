#ifndef _Urp_UrpPaint_h_
#define _Urp_UrpPaint_h_

#include "UrpShared.h"

//==============================================================================================
struct UrpFxdWdthFntDisplay : GridDisplay
{
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
			           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
	{
		Font newfnt(FontZ(Font::COURIER, 11));
		GridDisplay::Paint(w, x, y, cx, cy, val, style, fg, bg, newfnt, found, fs, fe);
	}
};


#endif
