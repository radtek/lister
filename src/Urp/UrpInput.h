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

#endif
