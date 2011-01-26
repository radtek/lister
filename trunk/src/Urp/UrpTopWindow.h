#ifndef _lister_Urp_UrpTopWindow_h_
#define _lister_Urp_UrpTopWindow_h_

#include "UrpShared.h"
#include "UrpWindow.h"

// Main first window. Created from main.cpp before call to UrpMain.

/*
   Sample use (your package class inherits from UrpTopWindow)
   
#include "<your package name here>.h"
#include "LogWin.h"

GUI_APP_MAIN
{
	// Bizarre bug fix: Eventually U++ will corrupt a package's StdFont permanently (any package) 
	// See UrpGrid column sizing for kludge to account for sizing error in GridCtrl (not present in ArrayCtrl)
	Draw::SetStdFont(Font(Font::ARIAL, 11));
	
	// Create LogWin instance for global availability
	InitLogging();
	// Construct first.  Some xmlization doesn't take affect when in constructor. 
	// (i.e., grid column widths)
	
	<Your Package class> <package name>; 

	UrpMain(<package name>, __FILE__);
	
	TermLogging();
}

*/

class UrpTopWindow : public UrpWindow {
public:
	UrpTopWindow();
	String		exeFilePath;
	String		connectAsUser;
	String		configFileFolder;
	EnvCode     envCode;
	const char *mainFilePath;
	
	void Init(const char *pmainFilePath);
	
	virtual void Run(bool appmodal = false);
};



#endif
