#include "LogWin.h" 

//==============================================================================================
LogWin::LogWin() {
	// One exit button
	CtrlLayoutExit(*this, "Log");
	Sizeable().Zoomable();
	CenterScreen();
	activated = false;
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
void LogWin::writeLine(String line) {
	if (!activated) return;
	logText.SetSelection(INT_MAX);
 	logText.Paste(WString(TimeStamp() + "::" + line + '\n'));
	logText.SetSelection(INT_MAX);
}

//==============================================================================================
void LogWin::writeSep() {
	if (!activated) return;
	writeLine("=============================================================================");
}

LogWin globalLogWin;

void OpenLogWin() {
	globalLogWin.OpenIfNotOpen();
}

void LogLine(String line) {
	globalLogWin.writeLine(line);
}

void LogSep() {
	globalLogWin.writeSep();
}

void ActivateLogging() {
	globalLogWin.Clear();
	globalLogWin.activated = true;
}

void DeactivateLogging() {
	globalLogWin.activated = false; // Prevent all the various loggers from bogging up the window.
}
