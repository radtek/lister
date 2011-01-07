#ifndef _Urp_UrpLineEdit_h_
#define _Urp_UrpLineEdit_h_

#include "UrpShared.h"

class UrpLineEdit : public LineEdit {
public:
	Callback          WhenKey;
	
	bool Key(dword key, int count) {
		bool k = LineEdit::Key(key, count);
		ProcessEvents();
		if (WhenKey) WhenKey();
		return k;
	}

	virtual void MouseMove(Point p, dword flags) {
		LineEdit::MouseMove(p, flags);
		if (WhenKey) WhenKey();
	}

};

#endif
