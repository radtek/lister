/***********************************************************************************************
*  lister - LogWin.cpp
*  
*  During multiple script execution, this window pops up and lists each step of the process.
*  Connections are made as needed and connection parameters and status are listed.
*
*  TODO:
*  - Get it to open up as a config'd window and remember its position, and be smaller.
*  - Could be partially transparent if lost focus, depin or pin.
*  - Auto-move if typing in editor
*
*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

#include "LogWin.h" 
#include <lister/Urp/UrpEdit.h>

#include "image_shared.h"
#include <Draw/iml_header.h>

// Only in lister.cpp
//#include "image_shared.h"
//#include <Draw/iml_source.h>

//==============================================================================================
LogWin::LogWin() {
	// One exit button
	CtrlLayoutExit(*this, "Log");
	Sizeable().Zoomable();
	Icon(MyImages::logwin16());
	TopMost(); // Force stayontop setting
	Transparent();
	//logText.Transparent();
	CenterScreen();
	activated = false;
	UrpEdit &re = logText;
	
	re.ShowCodes(Null); // Hide space/newline icon indicators http://www.ultimatepp.org/forum/index.php?t=msg&goto=2453&
	re.SetQTF("[%00-00 "); // Disable spell checker. http://www.ultimatepp.org/forum/index.php?t=msg&goto=2453&
}

String TimeStamp() {
	Time time = GetSysTime();
	return Format(Date(GetSysTime())) + (Format(" %02d:%02d:%02d", time.hour, time.minute, time.second));
}

//==============================================================================================
void LogWin::Clear() {
	logText.Clear();
}

//==============================================================================================
// writeLine treats all input as QTF data, so escape values properly before calling.
void LogWin::writeLine(String line, bool embellish/* = false */) {
	//http://www.ultimatepp.org/srcdoc$RichText$QTF$en-us.html
	if (!activated) return;
	UrpEdit &re = logText; // Bug: U++ editor not picking up RichEdit object for dev interface. Kludge.
	re.Select(INT_MAX, 0);
	// Take input that we know contains QTF codes and convert it to RichText. AsRichText treats incoming as escaped QTF (non-QTF)
	// @n = text color, \1 is the wrapper code for escaping text, & sign is new line, ` escapes a single character
 	if (embellish) {
 		re.PasteText(ParseQTF(CAT << "[@4 \1" << TimeStamp() << "\1] `- " << line << "&"));
 	} else {
 		re.PasteText(ParseQTF(CAT << line << "&"));
 	}
	re.Select(INT_MAX, 0);
}

//==============================================================================================
void LogWin::writeSep() {
	if (!activated) return;
	writeLine("\1=============================================================================\1");
}

/*
   Globally exposed functions instead of singleton crap.
   Logs only when activated.
*/

// U++ RichEdit cannot construct dynamically so we can't have static construct.
// We are stuck with RichEdit since we want color logging
LogWin *globalLogWin = NULL;

//==============================================================================================
void InitLogging() {
	ASSERT(!globalLogWin);
	globalLogWin = new LogWin();
}

//==============================================================================================
void TermLogging() {
	ASSERT(globalLogWin);
	delete globalLogWin;
}

//==============================================================================================
LogWin *GetLogWin() {
	return globalLogWin;
}

//==============================================================================================
void OpenLogWin() {
	if (!globalLogWin) return;
	globalLogWin->OpenIfNotOpen();
}

//==============================================================================================
void LogLine(String line) {
	if (!globalLogWin) return;
	globalLogWin->writeLine(line, true /* embellish with a timestamp */);
}

//==============================================================================================
void LogLineUnembellished(String line) {
	if (!globalLogWin) return;
	globalLogWin->writeLine(line, false);
}

//==============================================================================================
void LogSep() {
	if (!globalLogWin) return;
	globalLogWin->writeSep();
}

//==============================================================================================
void ActivateLogging() {
	if (!globalLogWin) return;
	globalLogWin->Clear();
	globalLogWin->activated = true;
}

//==============================================================================================
void DeactivateLogging() {
	if (!globalLogWin) return;
	globalLogWin->activated = false; // Prevent all the various loggers from bogging up the window.
}

//==============================================================================================
void LogGroupHeaderLine(String line) {
	if (!globalLogWin) return;
	// Attempt to use Qtf to create headerline (@r is dark red, [* is bold)
	globalLogWin->writeLine(CAT << "[* [@r " << line + "]]");
}

 	//re.SetQTF("Some line&Here comes the RichObject: @@mytype:1200*200`Hello world!`&Next line");
 	//re.PasteText(ParseQTF("Some line&Here comes the RichObject: @@mytype:1200*200`Hello world!`&Next line"));
 	//re.SetQTF(TimeStamp() + "\1::\0" + line + '\n');
