#ifndef _Urp_UrpInput_h_
#define _Urp_UrpInput_h_

#include "UrpShared.h"

// Input Box for a transformation control string to be applied to a multiline input string
bool UrpInputBox(
		String& wrapperText
	,	String &scrubAllOcc // If inputed, all occurrence in each input line will be stripped.  No support for cross lines yet.  Hmmm.
	,	String &splitOnThisToMacros // If inputed, each line is split by this string into [[MACRO1]], [[MACRO2]], etc.
	,	bool &forceInputToUpper // Force input to upper, but not the wrapperText
	,	bool &convertSpacesToUnderscores // Only on input
	,	const char *title, const char *label);

// Simpler input box
bool UrpInputBox(String &inputText, const char *title, const char *label);
// It's annoying to deal with empty fields and get MIN_INT or some crap back.
int GetFieldInt(EditInt &fld, int valIfEmpty = -1);
// Tired of trying to remember what PromptYesNo returns
bool Ok(String question);
#endif
