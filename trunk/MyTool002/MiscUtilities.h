#ifndef _lister_MyTool002_MiscUtilities_h_
#define _lister_MyTool002_MiscUtilities_h_

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

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4) {
	if (i == in1 || i == in2 || i == in3 || T == in4) return true;
	return false;
}

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5) {
	if (i == in1 || i == in2 || i == in3 || T == in4 || T == in5) return true;
	return false;
}

template <typename T>
bool In(T i, T in1, T in2, T in3, T in4, T in5, T in6) {
	if (i == in1 || i == in2 || i == in3 || T == in4 || T == in5 || T == in6) return true;
	return false;
}

#endif
