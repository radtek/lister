#include "MyTool002.h"

GUI_APP_MAIN
{
	
	MyTool002 myTool;
	LoadFromFile(myTool, ConfigFile("data.bin")); // Primarily only responsible for Serializing window placement.
	//LoadFromXML(myTool, ConfigFile("data.bin"));
	myTool.Run();
	StoreToFile(myTool, ConfigFile("data.bin"));
	//StoreAsXMLFile(myTool, "app", ConfigFile("data.bin"));
}

