#include "UrpInput.h"

//==========================================================================================	
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
	dlg.lbl = label;
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

//==========================================================================================	
// Simpler input box
bool UrpInputBox(String &inputText, const char *title, const char *label) {
	WithMyEditStringLayout<TopWindow> dlg;
	dlg.Sizeable().Zoomable();
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.text = inputText.ToWString();

	// Hide all the controls we don't support
	dlg.fldScrubText.Hide();
	dlg.fldSplitOn.Hide();
	dlg.optConvertSpacesToUnderscores.Hide();
	dlg.optForceInputToUpper.Hide();
	
	if(dlg.Execute() == IDOK) {
		inputText = dlg.text;
		return true;
	}
	return false;
}
