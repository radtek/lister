#include "Lister.h"

GUI_APP_MAIN
{
	Lister lister; // Construct first.  Some serialization/xmilization doesn't take when in constructor. (i.e., grid column widths)

	String binConfigFile = GetHomeDirFile("lister.config.bin");
	String xmlConfigFile = GetHomeDirFile("lister.config.xml");
	
	// Call all class Serialize functions
	LoadFromFile(lister, binConfigFile); // Primarily only responsible for Serializing window placement.
	
	// Call all class Xmlize functions
	LoadFromXMLFile(lister, xmlConfigFile);

	lister.Run();

	// Store binary serialized values, usually only placement
	// Binary serialization files get flushed when any changes are made to their inputs.
	StoreToFile(lister, binConfigFile);

	// Store text xmlized values.  XML format means user customizations are retained over version releases.  
	StoreAsXMLFile(lister, NULL, xmlConfigFile);

}

