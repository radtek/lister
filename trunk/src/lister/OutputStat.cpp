/***********************************************************************************************
*  lister - TestWin.cpp
*  
*  Simple panel that sits next to the OutputGrid (mainGrid) on the main app screen.  Displays
*  stats about the last run.  Also used in test runs to get row count and fetch time.
*  Sometimes Execute implementations don't set row counts.  So we track.
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

#include "OutputStat.h"
#include "lister/Urp/UrpString.h"
#include "shared.h"
#include "lister/Urp/UrpInterval.h"

//==============================================================================================
OutputStat::OutputStat() {
	CtrlLayout(*this);
	startedWhen = NULL;
	rowCount = -1;
}

//==============================================================================================
OutputStat::~OutputStat() {
	if (startedWhen)
		delete startedWhen;
}

//==============================================================================================
String OutputStat::PrepOutputValue(String in) {
	String prepOut;
	//1=8pt font, @m=magenta
	prepOut << "[1@m " << in << "]";
	return prepOut;
}

//==============================================================================================
void OutputStat::SetRowCount(int prowCount) {
	rowCount = prowCount;
	String srowcount;
	
	if (prowCount == 0) {
		srowcount = "ZERO";
	} else {
		srowcount = UrpString::ToString(prowCount);
	}
	
	dispRowCount.SetQTF(PrepOutputValue(srowcount));
}

//==============================================================================================
int OutputStat::GetRowCount() {
	return rowCount;
}

//==============================================================================================
void OutputStat::SetColumnCount(int pcolumnCount) {
	dispColumnCount.SetQTF(PrepOutputValue(UrpString::ToString(pcolumnCount)));
}

//==============================================================================================
void OutputStat::SetConnName(String pconnName) {
	dispConnName.SetQTF(PrepOutputValue(pconnName));
}

//==============================================================================================
void OutputStat::SetFetchTime(Interval ptime) {
	fetchTime = ptime;
	String stime = ptime.ToString();
	dispFetchTime.SetQTF(PrepOutputValue(stime));
}

//==============================================================================================
Interval OutputStat::GetFetchTime() {
	return fetchTime;
}

//==============================================================================================
void OutputStat::SetScriptId(int id) {
	String sid;
	if (id == UNKNOWN) {
		sid = "UNKNOWN";
	} else {
		sid = UrpString::ToString(id);
	}
	
	dispScriptId.SetQTF(PrepOutputValue(sid));
}

//==============================================================================================
void OutputStat::SetStartedWhen(Time pstartedWhen) {
	if (startedWhen) {
		delete startedWhen;
	}
	
	startedWhen = new Time(pstartedWhen);
	
	String stime = Format(pstartedWhen, true /*seconds*/);
	dispStartedWhen.SetQTF(PrepOutputValue(stime));
}

//==============================================================================================
Time OutputStat::GetStartedWhen() {
	if (startedWhen) {
		return *startedWhen;
	}
	
	return Time();
}

//==============================================================================================
void OutputStat::SetStoppedWhen(Time pstoppedWhen) {
	String stime = Format(pstoppedWhen, true /*seconds*/);
	dispStoppedWhen.SetQTF(PrepOutputValue(stime));
}

//==============================================================================================
void OutputStat::SetStatus(String pstatus) {
	dispStatus.SetQTF(PrepOutputValue(pstatus));
}

