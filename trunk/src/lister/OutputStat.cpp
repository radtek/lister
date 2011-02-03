#include "OutputStat.h"
#include "lister/Urp/UrpString.h"
#include "shared.h"
#include "lister/Urp/UrpInterval.h"

//==============================================================================================
OutputStat::OutputStat() {
	CtrlLayout(*this);
	startedWhen = NULL;
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
	String srowcount;
	
	if (prowCount == 0) {
		srowcount = "ZERO";
	} else {
		srowcount = UrpString::ToString(prowCount);
	}
	
	dispRowCount.SetQTF(PrepOutputValue(srowcount));
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
	String stime = ptime.ToString();
	dispFetchTime.SetQTF(PrepOutputValue(stime));
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

