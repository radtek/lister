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
#include "MacroHandler.h"
#include "Connection.h"

//==============================================================================================
ContextMacros::ContextMacros() {
	allMacros.Add(&envMacros);
	allMacros.Add(&taskMacros);
	rebuildTaskMacros = true;
	taskId = UNKNOWN;
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
		rebuildTaskMacros = true; // Force rebuild of task macros since they probably reference the ENV macro
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

