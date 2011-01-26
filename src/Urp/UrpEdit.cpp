#include "UrpEdit.h"

UrpEdit::UrpEdit() {
	RichEdit::RichEdit();
	zoomlevel = 8.0; // default zoom is about 9 pt on my screen
}

void UrpEdit::Layout() {
	RichEdit::Layout();
	long editor_cx = GetSize().cx;
	long adaptive_cx = (long)(editor_cx * zoomlevel); // Smaller the number, the bigger the text
	SetPage(Size(adaptive_cx, INT_MAX));
}
