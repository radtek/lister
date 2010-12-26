#include "Lister.h"

#include "image_shared.h"
#include <Draw/iml_source.h>

GUI_APP_MAIN
{
	// Construct first.  Some serialization/xmilization doesn't take affect when in constructor. 
	// (i.e., grid column widths)
	
	Lister lister; 

	// When the dev version is run, it uses the config files in that folder so as to avoid
	// disturbing the useful user settings on the same computer for the app installed in 
	// "Program Files".
	
	String binConfigFile = AppendFileName(lister.configFileFolder, "lister.config.bin");
	String xmlConfigFile = AppendFileName(lister.configFileFolder, "lister.config.xml");
	
	// Call all class Serialize functions.  Each class has either a custom Serialize function
	// or defaults to base class Ctrl GetData/SetData virtual functions.
	// Primarily only responsible for Serializing window placement, since U++ SerializePlacement
	// function handles the problem fairly well.
	
	LoadFromFile(lister, binConfigFile); 
	
	// Call all class Xmlize functions.  Again these are either custom or from Ctrl base.
	// Many of the U++ classes never bothered to implement this.
	
	LoadFromXMLFile(lister, xmlConfigFile);

	lister.Run();

	// Store any changes user made to window positioning/sizing.
	// Binary serialization files get flushed when any changes are made to their input
	// attributes, which is why I only keep window placement here.
	
	StoreToFile(lister, binConfigFile);

	// Store text xmlized values.  XML format means user customizations are retained over 
	// version releases, which is significant, and sad that U++ does not make this easier.
	
	StoreAsXMLFile(lister, NULL, xmlConfigFile);
}

