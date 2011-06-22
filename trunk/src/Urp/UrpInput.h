#ifndef _Urp_UrpInput_h_
#define _Urp_UrpInput_h_

#include "UrpShared.h"

//==============================================================================================
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
bool UrpDateInputBox(String &inputText, const char *title, const char *label, VectorMap<String, Date> &returnDates);
void UrpMessageBox(const char *msg);
// It's annoying to deal with empty fields and get MIN_INT or some crap back.
int GetFieldInt(EditInt &fld, int valIfEmpty = -1);

// Tired of trying to remember what PromptYesNo returns
bool Ok(String question);

// Stolen from Option U++ object.  Needed something that returns a god damn bool!  WTF with all these ints!
class BoolOption : public Pusher {
public:
	virtual void   Paint(Draw& draw);
	virtual Size   GetMinSize() const;
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
	virtual void   MouseEnter(Point, dword);
	virtual void   MouseLeave();

protected:
	virtual void  RefreshPush();
	virtual void  RefreshFocus();
	virtual void  PerformAction();

protected:
	Image  edge, edged;
	bool   option;
	bool   switchimage;
	bool   threestate;
	bool   notnull;
	bool   blackedge;
	bool   showlabel;

public:
	BoolOption& Set(bool b);
	bool     Get() const                           { return option; }

	operator bool() const                          { return option; }
	void operator=(bool b)                         { Set(b); }

	BoolOption& BlackEdge(bool b = true)              { blackedge = b; Refresh(); return *this; }
	BoolOption& SwitchImage(bool b = true)            { switchimage = b; Refresh(); return *this; }
	BoolOption& ThreeState(bool b = true)             { threestate = b; notnull = false; return *this; }
	BoolOption& ShowLabel(bool b = true)              { showlabel = b; Refresh(); return *this; }
	BoolOption& NotNull(bool nn = true)               { notnull = nn; Refresh(); return *this; }
	BoolOption& NoNotNull()                           { return NotNull(false); }

	BoolOption();
	virtual ~BoolOption();
};

#endif
