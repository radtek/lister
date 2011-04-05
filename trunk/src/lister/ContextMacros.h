#ifndef _lister_lister_ContextMacros_h_
#define _lister_lister_ContextMacros_h_

#include "shared.h"
struct MacPair : Moveable<MacPair> {
	String replaceWith;
	String expansion;
	MacPair(String preplaceWith, String pexpansion) {
		replaceWith = preplaceWith;
		expansion = pexpansion;
	}
};

typedef VectorMap<String, MacPair> MacMap;

class ContextMacros;
class Connection;

class ContextMacros {
public:
	MacMap envMacros;
	MacMap taskMacros;
	String envLetter;
	bool rebuildTaskMacros;
	int taskId;
	
	//User
	//Machine
	//Project
	//Environment (ex: PROD:"cdw1_user.vw_", UAT:"cdw.") or ENV=P
	//Branch (cdw1_user or cdw2_user view set)
	//Session (When you log in, as long as you are logged in)
	//Batch (across a set of scripts running when multi-selected)
	//Run (as long as the script runs, then flushed
	//Conditionals: if x macro not exists, then
	Vector<MacMap *> allMacros;
	
	     ContextMacros(); 
	void UpdateAvailableMacros(DropGrid &macrosAvailableList, ContextMacros *activeContextMacros);
	void RebuildMacros(int connId, int driverId, ContextMacros *activeContextMacros, 
		Connection *controlConnection, Connection *lastActiveConnection, 
		DropGrid *macrosAvailableList = NULL);
};


#endif
