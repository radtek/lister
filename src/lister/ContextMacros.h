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

class ContextMacros {
public:
	MacMap taskMacros;
	//User
	//Machine
	//Project
	//Environment (ex: PROD:"cdw1_user.vw_", UAT:"cdw.")
	//Branch (cdw1_user or cdw2_user view set)
	//Session (When you log in, as long as you are logged in)
	//Batch (across a set of scripts running when multi-selected)
	//Run (as long as the script runs, then flushed
	//Conditionals: if x macro not exists, then

	void UpdateAvailableMacros(DropGrid &macrosAvailableList, ContextMacros *activeContextMacros);

};


#endif
