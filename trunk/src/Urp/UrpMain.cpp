#include "UrpInput.h"
#include "UrpWindow.h"

#include "UrpShared.h"
#include "UrpString.h"

// Minimum requirement is that we are dealing with an UrpTopWindow so we can reference configfileplacement, and expect Xmlize to be setup.

int UrpMain(UrpTopWindow &ctrlMain, const char *mainFilePath) {
	// When the dev version is run, it uses the config files in that folder so as to avoid
	// disturbing the useful user settings on the same computer for the app installed in 
	// "Program Files".
	
	ctrlMain.exeFilePath = GetExeFilePath();
	ctrlMain.connectAsUser = GetUserName();	

	// If we are nested in the Program Files directory, we must be a proper user install and we need to use the users's configuration files

	if (ctrlMain.exeFilePath.Find("Program Files") >= 0) {
		ctrlMain.configFileFolder = GetHomeDirectory();
	} else {
		// We are probably in the dev env and running from the ide, and so we pull from our current directory
		ctrlMain.configFileFolder = GetFileDirectory(mainFilePath);
	}
	
	String configFileNameBase = ToLower(UrpString::GetWord(ctrlMain.Name(), 1)); // Get 2nd word in string (index zero-based)
	
	ctrlMain.binConfigFile = AppendFileName(ctrlMain.configFileFolder, configFileNameBase + ".config.bin");
	ctrlMain.xmlConfigFile = AppendFileName(ctrlMain.configFileFolder, configFileNameBase + ".config.xml");
	
	// Call all class Serialize functions.  Each class has either a custom Serialize function
	// or defaults to base class Ctrl GetData/SetData virtual functions.
	// Primarily only responsible for Serializing window placement, since U++ SerializePlacement
	// function handles the problem fairly well.
	
	LoadFromFile(ctrlMain, ctrlMain.binConfigFile); 
	
	// Call all class Xmlize functions.  Again these are either custom or from Ctrl base.
	// Many of the U++ classes never bothered to implement this.
	
	LoadFromXMLFile(ctrlMain, ctrlMain.xmlConfigFile);

	ctrlMain.Run();

	// Store any changes user made to window positioning/sizing.
	// Binary serialization files get flushed when any changes are made to their input
	// attributes, which is why I only keep window placement here.
	
	StoreToFile(ctrlMain, ctrlMain.binConfigFile);  // Hack in Ctrl class to make Xmlize virtual

	// Store text xmlized values.  XML format means user customizations are retained over 
	// version releases, which is significant, and sad that U++ does not make this easier.
	
	StoreAsXMLFile(ctrlMain, NULL, ctrlMain.xmlConfigFile);
	
	return 0;  // Default exit code historically means success (MS-DOS)
};
