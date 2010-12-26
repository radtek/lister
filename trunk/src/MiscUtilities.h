#ifndef _MiscUtilities_h_
#define _MiscUtilities_h_

//==========================================================================================	
bool MyEditText(String& s, const char *title, const char *label) {
	WithMyEditStringLayout<TopWindow> dlg;
	dlg.Sizeable().Zoomable();
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.text = s.ToWString();
//	dlg.text.SetFilter(f);
	if(dlg.Execute() == IDOK) {
		s = dlg.text;
		return true;
	}
	return false;
}

template <typename T>
bool In(T i, T in1, T in2) {
	if (i == in1 || i == in2) return true;
	return false;
}

template <typename T>
bool In(T i, T in1, T in2, T in3) {
	if (i == in1 || i == in2 || i == in3) return true;
	return false;
}

template <typename T, typename T2>
bool In(T i, T2 in1, T2 in2, T2 in3) {
	if (i == in1 || i == in2 || i == in3) return true;
	return false;
}

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4) {
	if (i == in1 || i == in2 || i == in3 || i == in4) return true;
	return false;
}

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5) {
	if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5) return true;
	return false;
}

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6) {
	if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6) return true;
	return false;
}

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7) {
	if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7) return true;
	return false;
}

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7, T in8) {
	if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7 || i == in8) return true;
	return false;
}

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6, T in7, T in8, T in9) {
	if (i == in1 || i == in2 || i == in3 || i == in4 || i == in5 || i == in6 || i == in7 || i == in8 || i == in9) return true;
	return false;
}

bool None(bool b1, bool b2) {
	return (!b1 && !b2);
}

bool None(bool b1, bool b2, bool b3) {
	return (!b1 && !b2 && !b3);
}

bool None(bool b1, bool b2, bool b3, bool b4) {
	return (!b1 && !b2 && !b3 && !b4);
}

bool None(bool b1, bool b2, bool b3, bool b4, bool b5) {
	return (!b1 && !b2 && !b3 && !b4 && !b5);
}

#endif
