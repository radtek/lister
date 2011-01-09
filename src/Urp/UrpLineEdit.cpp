#include "UrpLineEdit.h"

bool UrpLineEdit::Key(dword key, int count) {
	bool k = LineEdit::Key(key, count);
	ProcessEvents();
	if (WhenKey) WhenKey();
	return k;
}

/*virtual*/ void UrpLineEdit::MouseMove(Point p, dword flags) {
	LineEdit::MouseMove(p, flags);
	if (WhenKey) WhenKey();
}
