#include "MacroHandler.h"
#include "lister/Urp/UrpString.h"

//==============================================================================================
// Convert [[]] codes to values.  Hacky for now.
String ExpandMacros(String inputText, ContextMacros *contextMacros) {
	MacMap &taskMacros = contextMacros->taskMacros;
	if (contextMacros) {
		for (int i = 0; i < taskMacros.GetCount(); i++) {
			String searchMacro;
			String searchFor(taskMacros.GetKey(i));
			searchMacro << "[[" << searchFor << "]]";
			MacPair macPair = taskMacros.Get(searchFor);
			inputText = UrpString::ReplaceInWhatWith(inputText, searchMacro, macPair.expansion);
		}
	}
	
	String macro;
	String expansion;
	Date curDate = GetSysDate();
	
	macro = "[[TPLUS1]]";
	if (inputText.Find(macro) >= 0) {
		if (DayName(DayOfWeek(curDate)) == "Sunday") {
			expansion = "2"; // Back two days to Friday
		} else if (DayName(DayOfWeek(curDate)) == "Monday") {
			expansion = "3";
		} else {
			expansion = "1";
		}
		inputText = UrpString::ReplaceInWhatWith(inputText, macro, expansion);
	}	

	macro = "[[TPLUS2]]";
	if (inputText.Find(macro) >= 0) {
		if (DayName(DayOfWeek(curDate)) == "Sunday") {
			expansion = "3"; // Back to Thursday
		} else if (DayName(DayOfWeek(curDate)) == "Monday") {
			expansion = "4"; // to Thursday
		} else if (DayName(DayOfWeek(curDate)) == "Tuesday") {
			expansion = "4"; // to Friday
		} else {
			expansion = "2";
		}
		inputText = UrpString::ReplaceInWhatWith(inputText, macro, expansion);
	}	

	macro = "[[TPLUS3]]";
	if (inputText.Find(macro) >= 0) {
		if (DayName(DayOfWeek(curDate)) == "Sunday") {
			expansion = "4"; // Back to Wednesday
		} else if (DayName(DayOfWeek(curDate)) == "Monday") {
			expansion = "5"; // Back to Wednesday
		} else if (DayName(DayOfWeek(curDate)) == "Tuesday") {
			expansion = "5"; // Back to Thursday
		} else if (DayName(DayOfWeek(curDate)) == "Wednesday") {
			expansion = "5"; // Back to Friday
		} else {
			expansion = "3";
		}
		inputText = UrpString::ReplaceInWhatWith(inputText, macro, expansion);
	}	

	macro = "[[TPLUS4]]";
	if (inputText.Find(macro) >= 0) {
		if (DayName(DayOfWeek(curDate)) == "Sunday") {
			expansion = "5"; // Back to Tuesday
		} else if (DayName(DayOfWeek(curDate)) == "Monday") {
			expansion = "6"; // Back to Tuesday
		} else if (DayName(DayOfWeek(curDate)) == "Tuesday") {
			expansion = "6"; // Back to Wednesday
		} else if (DayName(DayOfWeek(curDate)) == "Wednesday") {
			expansion = "6"; // Back to Thursday
		} else if (DayName(DayOfWeek(curDate)) == "Thursday") {
			expansion = "6"; // Back to Friday
		} else {
			expansion = "4";
		}
		inputText = UrpString::ReplaceInWhatWith(inputText, macro, expansion);
	}	

	return inputText;
}

