#include "UrpInput.h"

//==============================================================================================
// Input Box for a transformation control string to be applied to a multiline input string
bool UrpInputBox(
		String& wrapperText
	,	String &scrubAllOcc // If inputed, all occurrence in each input line will be stripped.  No support for cross lines yet.  Hmmm.
	,	String &splitOnThisToMacros // If inputed, each line is split by this string into [[MACRO1]], [[MACRO2]], etc.
	,	bool &forceInputToUpper // Force input to upper, but not the wrapperText
	,	bool &convertSpacesToUnderscores // Only on input
	,	const char *title, const char *label) 
{
	WithMyEditStringLayout<TopWindow> dlg;
	dlg.Sizeable().Zoomable();
	CtrlLayoutOKCancel(dlg, title);
	dlg.lblInstructions = label;
	dlg.text = wrapperText.ToWString();
	if(dlg.Execute() == IDOK) {
		wrapperText = dlg.text;
		scrubAllOcc = dlg.fldScrubText;
		splitOnThisToMacros = dlg.fldSplitOn;
		forceInputToUpper = dlg.optForceInputToUpper;
		convertSpacesToUnderscores = dlg.optConvertSpacesToUnderscores;
		return true;
	}
	return false;
}

//==============================================================================================
// Simpler input box
bool UrpInputBox(String &inputText, const char *title, const char *label) {
	WithMyEditStringLayout<TopWindow> dlg;
	dlg.Sizeable().Zoomable();
	CtrlLayoutOKCancel(dlg, title);
	dlg.lblInstructions = label;
	dlg.text = inputText.ToWString();

	// Hide all the controls we don't support, from transformation action
	dlg.fldScrubText.Hide();
	dlg.fldSplitOn.Hide();
	dlg.optConvertSpacesToUnderscores.Hide();
	dlg.optForceInputToUpper.Hide();
	dlg.lblScrubText.Hide();
	dlg.lblSplitOn.Hide();
	
	if(dlg.Execute() == IDOK) {
		inputText = dlg.text;
		return true;
	}
	return false;
}

//==============================================================================================
// Date input box; will eventually support more options, like business date calendars, holiday
// marking and skipping, BDay counting back/forward.
bool UrpDateInputBox(String &inputText, const char *title, const char *label, VectorMap<String, Date> &returnDates) {
	WithDateEntryLayout<TopWindow> dlg;
	dlg.Sizeable().Zoomable();
	CtrlLayoutOKCancel(dlg, title);
	dlg.lblInstructions = label;
	
	if(dlg.Execute() == IDOK) {
		Value vdt = dlg.fldFromDate.GetData();
		
		if (!vdt.IsNull()) {
			Date dt = vdt;
			returnDates.Add("fromdate", dt);
		}
		
		vdt = dlg.fldToDate.GetData();
		
		if (!vdt.IsNull()) {
			Date dt = vdt;
			returnDates.Add("todate", dt);
		}
		
		return true;
	}
	return false;
}

//==============================================================================================
// It's annoying to deal with empty fields and get MIN_INT or some crap back.
int GetFieldInt(EditInt &fld, int valIfEmpty /*= -1*/) {
	Value f = fld.GetData();
	
	if (f.IsVoid() || f.IsNull()) {
		return valIfEmpty;
	} else {
		return f;
	}
}

//==============================================================================================
bool Ok(String question) {
	int response = PromptYesNo(question); // Returns 1 for yes
	
	if (In(response, 0, IDCANCEL, IDNO)) {
		return false;
	} else {
		return true;
	}
}
		
		
// -----------------

void  BoolOption::MouseEnter(Point, dword)
{
	RefreshPush();
}

void  BoolOption::MouseLeave()
{
	RefreshPush();
	Pusher::MouseLeave();
}

void BoolOption::RefreshFocus() {
	Refresh();
}

void BoolOption::RefreshPush() {
	if(showlabel)
		Refresh(0, 0, CtrlsImg::O0().GetSize().cx, GetSize().cy);
	else
		Pusher::RefreshPush();
}

Size BoolOption::GetMinSize() const {
	Size isz = CtrlsImg::O0().GetSize();
	return AddFrameSize(isz.cx + (GetSmartTextSize(label).cx + 4) * showlabel,
		                max(isz.cy, StdFont().Info().GetHeight()) + 2);
}

void BoolOption::Paint(Draw& w) {
	Size sz = GetSize();
	
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	
	Size isz = CtrlsImg::O0().GetSize();
	Size tsz;
	int ix = 0, iy = 0, ty = 0;
	
	if(showlabel) {
		tsz = GetSmartTextSize(label, font);
		ty = (sz.cy - tsz.cy) / 2;
		iy = (tsz.cy - isz.cy) / 2 + ty;
	} else {
		ix = (sz.cx - isz.cx) / 2;
		iy = (sz.cy - isz.cy) / 2;
	}
	
	int q = GetVisualState();
	int g = (!notnull || threestate) && IsNull(option) ? CtrlsImg::I_O2
	                                                   : option == 1 ? CtrlsImg::I_O1
	                                                                 : CtrlsImg::I_O0;
	if(switchimage)
		g = option ? CtrlsImg::I_S1 : CtrlsImg::I_S0;
	
	w.DrawImage(ix, iy, CtrlsImg::Get(g + q));
	
	if(showlabel) {
		bool ds = !IsShowEnabled();
		DrawSmartText(w, isz.cx + 4, ty, tsz.cx, label, font,
		              ds || IsReadOnly() ? SColorDisabled : SColorLabel, ////////
		              VisibleAccessKeys() ? accesskey : 0);
		if(HasFocus())
			DrawFocus(w, RectC(isz.cx + 2, ty - 1, tsz.cx + 3, tsz.cy + 2) & sz);
	}
}

void   BoolOption::SetData(const Value& data) {
	if (IsString(data)) {
		if (IsNull(data)) {
			Set(false);
		} else {
			Set(String(data) == "1");
		}
	} else if(IsNumber(data)) {
		Set((int64)data == 1);
	} else if (data.GetType() == BOOL_V) {
		Set((bool)data);
	}
}

Value  BoolOption::GetData() const {
	return (bool) option;
}

void  BoolOption::PerformAction() {
	if(option == 1)
		option = 0;
	else
		option = 1;
	UpdateAction();
	RefreshPush();
}

BoolOption& BoolOption::Set(bool b)
{
	if(b != option) {
		option = b;
		Update();
		RefreshPush();
	}
	return *this;
}

BoolOption::BoolOption() {
	option = false;
	notnull = true;
	switchimage = threestate = false;
	blackedge = false;
	showlabel = true;
	Transparent();
	font = StdFont();
}

BoolOption::~BoolOption() {}
