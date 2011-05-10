/***********************************************************************************************
*  lister - ContextMacros.cpp
*  
*  A Macro pack used to maintain the current context of compilable macros.  By passing this
*  around, the clients can be oblivious of the complexity and myriads of macro sources.
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
#include "ContextMacros.h"
//#include "MacroHandler.h"
#include "Connection.h"
// Necessary for RegExp use.  Add package plugin/pcre.
// Had to add folder: C:\Program Files\Microsoft Visual Studio 10.0\VC\include to find stdint.h
#include <plugin/pcre/Pcre.h>

//==============================================================================================
ContextMacros::ContextMacros() {
	allMacros.Add(&envMacros);
	allMacros.Add(&teamMacros);
	allMacros.Add(&taskMacros);
	rebuildTeamMacros = rebuildTaskMacros = true;
	taskId = UNKNOWN;
	
	// Build our team macros, which for now don't ever change during a session.
	
	teamMacros.Add("CSDRUSUFF", MacPair("cdw1_user.vw_", "cdw1_user.vw_"));
	teamMacros.Add("CSDRPSUFF", MacPair("cdw.", "cdw."));
	teamMacros.Add("CSDRDSUFF", MacPair("[[CSDRPSUFF]]", "[[CSDRPSUFF]]"));
	//teamMacros.Add("CSDRQSUFF", MacPair("[[CSDRPSUFF]]"));
	teamMacros.Add("CSDRSUFF", MacPair("[[CSDR[[ENV]]SUFF]]", "[[CSDR[[ENV]]SUFF]]"));
}

//==============================================================================================
void ContextMacros::UpdateAvailableMacros(DropGrid &macrosAvailableList, ContextMacros *activeContextMacros) {
	
	macrosAvailableList.Clear();
	if (!activeContextMacros) return;
	int macrosetcount = activeContextMacros->allMacros.GetCount();
	
	for (int j = 0; j < activeContextMacros->allMacros.GetCount(); j++) {
		MacMap &macros = *(activeContextMacros->allMacros[j]);
		for (int i = 0; i < macros.GetCount(); i++) {
			MacPair macPair = macros.Get(macros.GetKey(i));
			macrosAvailableList.Add(macros.GetKey(i), macPair.replaceWith, macPair.expansion);
		}
	}
	
	UpdateMacroList(macrosAvailableList);
}

//==============================================================================================	
void ContextMacros::RebuildMacros(
	  int            connId
	, int            driverId
	, ContextMacros *activeContextMacros
	, Connection    *controlConnection
	, Connection    *lastActiveConnection
	, DropGrid      *macrosAvailableList/*=NULL*/
) {
	String loadedEnvLetter;

	if (connId != UNKNOWN) {		
		Connection *connInfo = connectionFactory->FetchConnInfo(connId);
		loadedEnvLetter = connInfo->envLetter;
		delete connInfo;  // We created an object that is not on a stack anywhere, so we must destroy
	} else 
	if (lastActiveConnection) {
		loadedEnvLetter = lastActiveConnection->envLetter;
	} else {
		loadedEnvLetter = "XX"; // Default to a "Not an environment" value
	}

	// Only reload environment macros if there was a change in environment
	if (!loadedEnvLetter.IsEmpty() && loadedEnvLetter != envLetter) {
		envLetter = loadedEnvLetter;
		activeContextMacros->envMacros.Clear();
		activeContextMacros->envMacros.Add("ENV", MacPair(envLetter, envLetter));
		rebuildTeamMacros =
		rebuildTaskMacros = true; // Force rebuild of task macros since they probably reference the ENV macro
	}
	
	if (rebuildTeamMacros) {
		rebuildTeamMacros = false;
		for (int i = 0; i < activeContextMacros->teamMacros.GetCount(); i++) {
			// Just expand existing ones, don't reload
			
			String key = activeContextMacros->teamMacros.GetKey(i);
			MacPair &macPair = activeContextMacros->teamMacros.Get(key);
			macPair.expansion = ExpandMacros(macPair.replaceWith, activeContextMacros);
		}
	}
	
	// If we've stayed within the same task set of scripts, we don't need to rebuild tasks.
	if (rebuildTaskMacros) {
		rebuildTaskMacros = false; // Desensitize flag
		
		// Flush CurrentTaskMacroList; fetch from taskmacros.  Apply logic from CursorHandler macro function as each is loaded.
		// Move macro code out of CursorHandler and into MacroHandler.cpp
		// Alter macro function to rerun macros in CurrentTaskMacroList, then macros in current script.
		activeContextMacros->taskMacros.Clear();
		
		// Dup a connection for other lookups during the loop
		Connection driverConn(controlConnection);
		
		if (controlConnection->SendQueryDataScript(SqlStatement(SqlSelect(SEARCHFOR, REPLACEWITH, TASKMACID).From(TASKMACROS).Where(TASKID==taskId).OrderBy(PROCESSORDER)).GetText())) {
			while(controlConnection->Fetch()) {
				String searchFor = controlConnection->Get(SEARCHFOR);
				// Default replaceWith value (non-driver specific)
				String replaceWith = controlConnection->Get(REPLACEWITH);
				// If there's a custom driver for this task and a replaceWith string then it will override the default
				int taskMacroId = controlConnection->Get(TASKMACID);
				
				String driverReplaceWith = Null;
				
				// There are drivers for this task and one was selected for this task or this batch run or singular run
				if (!In(driverId, INT_NULL, UNKNOWN)) {
					// Check if there is a replacement
					String driverReplScript = Format(
						"SELECT replaceWith FROM TaskMacroDriverReplacement "
						"WHERE taskMacroId = %d "
						"AND taskDriverId = %d", taskMacroId, driverId);
					if (driverConn.SendQueryDataScript(driverReplScript)) {
						int driverMatchCount = driverConn.GetRowsProcessed();
						ASSERT_(driverMatchCount < 2, "Found more than 1 driver custom replaceWith value, which I can't handle here");
						
						if (driverMatchCount > 0) {
							driverConn.Fetch();
							driverReplaceWith = driverConn.Get(0);
						}
					}
				}
				
				// If there is a custom driver replacement, that overrides the default macros replacement
				if (!driverReplaceWith.IsEmpty()) {
					replaceWith = driverReplaceWith;
				}
				
				// Expand the output with any previous macros in task list, as well as the standard hardcoded ones
				String expansion = ExpandMacros(replaceWith, activeContextMacros);
				
				// Add it to the list and continue fetching and expanding
				
				activeContextMacros->taskMacros.Add(searchFor, MacPair(replaceWith, expansion));
			}
		}
		
		// Update the drop down so script-writer can see what macros are available

		if (macrosAvailableList) {		
			activeContextMacros->UpdateAvailableMacros(*macrosAvailableList, activeContextMacros);
		}
	}
}

//==============================================================================================
void ContextMacros::UpdateMacroListBuildLine(DropGrid &macrosAvailableList, const String &macroName) {
	// We don't pass a replaceWith since these are base macros, not nested macros
	// i.e., the replacewith value is the internal code that builds the value
	macrosAvailableList.Add(macroName + "{n}", "{code function}", ExpandMacros("[[" + macroName + "1]]"));
}
                                                                     
//==============================================================================================
// List the available internal macro subs available to the user so they don't have to remember them.
// If inserted from dropdown, a popup lets you fill in the days with a number of another macro
void ContextMacros::UpdateMacroList(DropGrid &macrosAvailableList) {
	UpdateMacroListBuildLine(macrosAvailableList, "TPLUS"); // Todo: needs a note to describe usage to user. Tag?  Edit DropGrid.
	//UpdateMacroListBuildLine(macrosAvailableList, "TMINUS");
	UpdateMacroListBuildLine(macrosAvailableList, "TPLUSORA");
	//UpdateMacroListBuildLine(macrosAvailableList, "TMINUSORA");
	UpdateMacroListBuildLine(macrosAvailableList, "TPLUSMSSQLUK");
	//UpdateMacroListBuildLine(macrosAvailableList, "TMINUSMSSQLUK");
	UpdateMacroListBuildLine(macrosAvailableList, "TPLUSSYBASE");
	//UpdateMacroListBuildLine(macrosAvailableList, "TMINUSSYBASE");
	UpdateMacroListBuildLine(macrosAvailableList, "TPLUSUS");
}

//==============================================================================================
// Convert [[]] codes to values.
String ContextMacros::ExpandMacros(String inputText, ContextMacros *contextMacros) {
	if (contextMacros) {
		int loopcnt = 0;
		
		while (true) {
			int totalReplacementCount = 0;
			
			int macrosetcount = contextMacros->allMacros.GetCount();
			for (int j = 0; j < contextMacros->allMacros.GetCount(); j++) {
				MacMap &macros = *(contextMacros->allMacros[j]);
	
				for (int i = 0; i < macros.GetCount(); i++) {
					String searchMacro;
					String searchFor(macros.GetKey(i));
					searchMacro << "[[" << searchFor << "]]";
					MacPair macPair = macros.Get(searchFor);
					int replacementCount;
					inputText = UrpString::ReplaceInWhatWith(inputText, searchMacro, macPair.expansion, &replacementCount);
					totalReplacementCount+= replacementCount;
				}
			}
			
			// If no work done then stop
			if (!totalReplacementCount) {
				break;
			}
			loopcnt++;
			
			// Don't look forever
			if (loopcnt > 30) {
				break;
			}
		}
	}

	bool expansionerror = false;	
	String macro;
	String expansion;
	
	Date curDate = GetSysDate();

	// Scan repeatedly until no more matches
	int loopcnt = 0;
	while (true) {
		
		// Search for macros in the form [[AAAANN]] to transform into values
		// http://www.regextester.com/pregsyntax.html
		// Matches can be TPLUS1, TPLUS5, TMINUS3
		
		
		String searchFor = "\\[\\[([a-zA-Z]+)(\\d+)\\]\\]"; // Parens force the digits to be inserted into r0[0], r0[1]
		RegExp r0(searchFor);
		String command;
		if (r0.Match(inputText)) {
			command = r0[0]; 
			String subcommand1, subcommand2;
			String arg1 = r0[1];
			macro = "[[" + command + arg1 + "]]";
			ToUpper(Trim(command));
			if (In(command, 
					"TPLUS"        , "TMINUS"        , 
					"TPLUSORA"     , "TMINUSORA"     , 
					"TPLUSMSSQLUK" , "TMINUSMSSQLUK" , 
					"TPLUSUS"      , "TMINUSUS"      , 
					"TPLUSSYBASE"  , "TMINUSSYBASE"
				)) {
				if (arg1 == "") {
					expansion << "{Missing numeric argument for command " << command << "}";
				} else {
					int arg1num = ToInt(arg1, 0);
						
					// Looking back
					if (command.StartsWith("TPLUS")) {
						arg1num *= -1;
						subcommand1 = RestOf(command, "TPLUS");
					// Looking foward
					} else if (command.StartsWith("TMINUS")) {
						subcommand1 = RestOf(command, "TMINUS");
					}
						
					Date endDate = GetDateFromDateBDays(curDate, arg1num);
					
					// The business days are now converted to physical days
					int actualphysicaldays = endDate - curDate;
	
					// They just want the number
					if (subcommand1 == "") {
						expansion = ToString(Abs(actualphysicaldays));
					} else if (subcommand1.StartsWith("ORA")) {
						// All oracles use dd-mon-yyyy as their default date format. 
						// Date.month must be converted to zero-base index
						expansion = Format("'%02d-%s-%d'", endDate.day, Upper(MonName(endDate.month-1)), endDate.year);
					} else if (subcommand1.StartsWith("SYBASE")) {
						// This is the default format that Sybase 12.5 ASE reads
						expansion = Format("'%4d-%02d-%02d'", endDate.year, endDate.month, endDate.day);
					} else if (subcommand1.StartsWith("MSSQLUK")) {
						// English format for London
						expansion = Format("'%02d-%02d-%04d'", endDate.day, endDate.month, endDate.year);
					} else if (subcommand1.StartsWith("US")) {
						// My general format, ok??
						expansion = Format("'%02d/%02d/%04d'", endDate.month, endDate.day, endDate.year);
					} else {
						expansion << "{unrecognized subcommand after TMINUS/TPLUS " << subcommand1 << ", looking for ORA, SYBASE, or MSSQLUK}";
						expansionerror = true;
					}
				}
			} else {
				expansion << "{Unsupported command " << command << "}";
				expansionerror = true;
			}

		} else {
			
			// Now look for [[AAAA::anything]]  This is a pattern that will allow us to
			// convert things like '03/09/2011' to '09/03/2011'.  It cannot be replaced by a
			// SQL function due to the limits of SQL Server or whatever tool is executing
			// For example, the following will not work: "set nocount on;exec api.sp_XTrades 'COMPLIANCE', convert(varchar, (convert('03/09/2011', 101), 103), [[TESTSOURCECSDR]]
			// It must be: "set nocount on;exec api.sp_XTrades 'COMPLIANCE', '09/03/2011', [[TESTSOURCECSDR]]
			// Which means that conversion must take place before passing to processor.
			// So, we have "[[TOUK::'03/09/2011']]" from "[[TOUK::TESTDATE]]".  Test date is demacroed, then here we are.
			
			searchFor = "\\[\\[([a-zA-Z]+)::([^\\]]+)\\]\\]";
			RegExp r2(searchFor);
			
			// Scan our input for the search pattern
			if (r2.Match(inputText)) {
				// Found it; break out the command and argument parts
				command = r2[0]; 
				
				String arg1 = r2[1];
				
				// Set our search string for the ReplaceInWhatWith simple search and replace func
				macro << "[[" << command << "::" << arg1 << "]]";
				ToUpper(Trim(command));
				
				// Search for commands we support
				// Convert 'mm/dd/yyyy' to 'dd/mm/yyyy' in place
				if (command == "TOUK") {
					// We expect and require that the date arg string (arg1) is 'mm/dd/yyyy', including the apostrophes
					int month, day, year;
					// Flip month and day
					sscanf(arg1, "'%02d/%02d/%04d'", &month, &day, &year);
					expansion = Format("'%02d/%02d/%04d'", day, month, year);
				// Convert 'mm/dd/yyyy' to yyyymmdd in place.  Apostophes expected and stripped
				} else if (command == "TONUM") {
					// We expect and require that the date arg string (arg1) is 'mm/dd/yyyy', including the apostrophes
					int month, day, year;
					// Flip month and day
					sscanf(arg1, "'%02d/%02d/%04d'", &month, &day, &year);
					expansion = Format("%04d%02d%02d", year, month, day);
				} else {
					expansion << "{Unsupported command " << command << "}";
					expansionerror = true;
				}
			// Command/arg format not found; Just isolate the macro if there and push error message out
			} else {
				searchFor = "\\[\\[(^\\]+)\\]\\]";
				RegExp r1(searchFor);
				if (r1.Match(inputText)) {
					String command = r1[0];
					macro << "[[" << command << "]]";
					expansion << "{Unrecognized macro format: " << command << ", must be [[AAAA::...]]}";
					expansionerror = true;
				}
			}
		}
		
		
		if (expansion.GetCount() > 0) {
			inputText = UrpString::ReplaceInWhatWith(inputText, macro, expansion);
			if (expansionerror) {
				break;
			}
		} else {
			break;
		}
		
		loopcnt++;
		
		if (loopcnt > 40) {
			Exclamation(CAT << "Max loop count reached: " << inputText << ", " << expansion);
			break;
		}
		
		// Truncate or it will prevent closure
		expansion = "";
	}
	
	return inputText;
}
