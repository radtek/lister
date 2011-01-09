#ifndef _Urp_UrpLineEdit_h_
#define _Urp_UrpLineEdit_h_

#include "UrpShared.h"

class UrpLineEdit : public LineEdit {
public:
	Callback          WhenKey;
	
	bool Key(dword key, int count);
	virtual void MouseMove(Point p, dword flags);
};

#endif
