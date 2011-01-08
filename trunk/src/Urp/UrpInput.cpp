#include "UrpInput.h"

// Simple Input Box  (Hard to find something like this in U++) Sizeable.
//==========================================================================================	
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
