#include "UrpInterval.h"
#include "UrpString.h"

//==============================================================================================
Interval::Interval() {
	day = month = hour = minute = second = 0;
	year = 0;
	int64 ti = 0l;
	Set(ti);
	isnull = true;
}

//==============================================================================================
Interval::Interval(Time startTime, Time stopTime) {
	day = month = hour = minute = second = 0;
	year = 0;
	int64 ti = (stopTime - startTime);
	Set(ti);
	isnull = false;
}


//==============================================================================================
/*virtual */String Interval::ToString() {
	
	if (isnull) return Null;
	
	String out;
	
	if (year > 0) {
		out << year << "y";
		out << month << "mo"; // Always set to 1 if year is zero, fyi
		out << day << "d";
	}
	if (hour > 0) {
		out << hour << "h";
	}
	
	if (minute > 0) {
		out << minute << "m";
	}
	
	if (second > 0) {
		out << UrpString::ToString(second) << "s";
	}

	return out;		
}


