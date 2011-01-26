#include "UrpTopWindow.h"

UrpTopWindow::UrpTopWindow() {
	// These have to be here and called by default constructor, don't know why.
	exeFilePath = GetExeFilePath();
	connectAsUser = GetUserName();	
}

//==============================================================================================
// As the one and only main window, determine if we are in production or not.
// Eventually fix call to replace GUI_MAIN with URP_MAIN which includes __FILE__ macro
void UrpTopWindow::Init(const char *pmainFilePath) {
	mainFilePath = pmainFilePath;
	// If we are nested in the Program Files directory, we must be a proper user install and we need to use the users's configuration files

	if (exeFilePath.Find("Program Files") >= 0) {
		envCode = ENV_PROD;
		configFileFolder = GetHomeDirectory();
	} else {
		envCode = ENV_DEV;
		// We are probably in the dev env and running from the ide, and so we pull from our current directory
		configFileFolder = GetFileDirectory(mainFilePath);
	}
}

//==============================================================================================
// This must be present to extend the virtuality up to client classes, else a Run in the client 
// class would be ignored from a call to an abstracted class.
/*virtual*/ void UrpTopWindow::Run(bool appmodal/* = false*/) {};



