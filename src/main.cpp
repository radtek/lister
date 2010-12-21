#include "Lister.h"

GUI_APP_MAIN
{
	
	Lister lister;
	LoadFromFile(lister, ConfigFile("lister.config.bin")); // Primarily only responsible for Serializing window placement.
	lister.Run();
	StoreToFile(lister, ConfigFile("lister.config.bin"));
	//StoreAsXMLFile(myTool, "app", ConfigFile("data.bin"));
}

