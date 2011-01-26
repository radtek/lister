#include "lister.h"
#include "LogWin.h"

GUI_APP_MAIN
{
	// Bizarre bug fix: Eventually U++ will corrupt a package's StdFont permanently (any package) 
	Draw::SetStdFont(Font(Font::ARIAL, 11));
	
	// Create LogWin instance for global availability
	InitLogging();
	// Construct first.  Some xmlization doesn't take affect when in constructor. 
	// (i.e., grid column widths)
	
	Lister lister; 

	// replace with lister.Run
	UrpMain(lister);
	
	TermLogging();
}

