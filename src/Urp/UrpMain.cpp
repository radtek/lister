#include "UrpInput.h"
#include "UrpWindow.h"

#include "UrpShared.h"
#include "UrpString.h"
#include "UrpMain.h"

// Minimum requirement is that we are dealing with an UrpTopWindow so we can reference configfileplacement, and expect Xmlize to be setup.

// Hidden constants
#define BIN_TAIL ".config.bin"
#define XML_TAIL ".config.xml"

// Bit map over 2 bits
#define CONFIGACTION_LOAD                   1
#define CONFIGACTION_SAVE                   2
#define CONFIGACTION_LOAD_AND_SAVE          3

#define WINDOWINSTANCINGMETHOD_RUN          1
#define WINDOWINSTANCINGMETHOD_OPEN         2
#define WINDOWINSTANCINGMETHOD_NONE         3
#define WINDOWINSTANCINGMETHOD_CLOSE        4
#define WINDOWINSTANCINGMETHOD_HIDE         5

// May add "Show" if we want to play peek-a-boo with windows

//==============================================================================================
void RunWithConfig(UrpWindow &win, int configAction, int windowInstancingMethod, UrpTopWindow *parentWindow = NULL) {

	// Note that you must load config to open a window (Procedural dictator, that's me)
	if (configAction & CONFIGACTION_LOAD) {
		// Call all class Serialize functions.  Each class has either a custom Serialize function
		// or defaults to base class Ctrl GetData/SetData virtual functions.
		// Primarily only responsible for Serializing window placement, since U++ SerializePlacement
		// function handles the problem fairly well.
		
//		LoadFromFile(win, win.binConfigFile); 
		
		// Call all class Xmlize functions.  Again these are either custom or from Ctrl base.
		// Many of the U++ classes never bothered to implement this.
		
		LoadFromXMLFile(win, win.xmlConfigFile);

		switch (windowInstancingMethod) {
		case WINDOWINSTANCINGMETHOD_RUN:
			win.Run(); // Note that initial call from main will always be modal, following calls will be modeless
			break;
				
		case WINDOWINSTANCINGMETHOD_OPEN:
			win.Open(); // Modeless and retains position set previously in serialize, whereas run ignores position info
			break;

		case WINDOWINSTANCINGMETHOD_NONE:
			break;
		}
	}

	// ..AND you must save config to close, dammit!
	if (configAction & CONFIGACTION_SAVE) {	
	
		if (windowInstancingMethod == WINDOWINSTANCINGMETHOD_CLOSE) {
			if (win.IsOpen()) { // Probably not if it was modal
				win.Hide(); // For speed, we can hide it before we call the closer
				win.Close();
			}
		}
		
		// Store any changes user made to window positioning/sizing.
		// Binary serialization files get flushed when any changes are made to their input
		// attributes, which is why I only keep window placement here.
		
	//	StoreToFile(win, win.binConfigFile);  // Hack in Ctrl class to make Xmlize virtual
	
		// Store text xmlized values.  XML format means user customizations are retained over 
		// version releases, which is significant, and sad that U++ does not make this easier.
		
		StoreAsXMLFile(win, NULL, win.xmlConfigFile);
	}
}

//==============================================================================================
int UrpMain(UrpTopWindow &ctrlMain) {
	// When the dev version is run, it uses the config files in that folder so as to avoid
	// disturbing the useful user settings on the same computer for the app installed in 
	// "Program Files".
	
	ASSERT(&ctrlMain);
	ctrlMain.configFileNameBase = ToLower(UrpString::GetWord(ctrlMain.Name(), 1)); // Get 2nd word in string (index zero-based)
	
	ctrlMain.binConfigFile = AppendFileName(ctrlMain.configFileFolder, ctrlMain.configFileNameBase + BIN_TAIL);
	ctrlMain.xmlConfigFile = AppendFileName(ctrlMain.configFileFolder, ctrlMain.configFileNameBase + XML_TAIL);
	
	RunWithConfig(ctrlMain, CONFIGACTION_LOAD_AND_SAVE, WINDOWINSTANCINGMETHOD_RUN);
	
	return 0;  // Default exit code historically means success (MS-DOS)
};

//==============================================================================================
// Very complicated, but MS Windows is complicated!
void UrpChild(UrpTopWindow &ctrlMain, UrpWindow &ctrlChild, String tagName, int requestedState) {
	ASSERT(&ctrlMain);
	ASSERT(&ctrlChild);
	ASSERT(!tagName.IsEmpty());
	
	ctrlChild.configFileNameBase = ToLower(tagName);
	
	ctrlChild.binConfigFile = AppendFileName(ctrlMain.configFileFolder, ctrlMain.configFileNameBase 
		+ "." + ctrlChild.configFileNameBase + BIN_TAIL);
	ctrlChild.xmlConfigFile = AppendFileName(ctrlMain.configFileFolder, ctrlMain.configFileNameBase 
		+ "." + ctrlChild.configFileNameBase + XML_TAIL);
		
	if        (requestedState == CHILDWINDOWREQSTATE_OPEN) {
		RunWithConfig(ctrlChild, CONFIGACTION_LOAD, WINDOWINSTANCINGMETHOD_OPEN, &ctrlMain);
	} else if (requestedState == CHILDWINDOWREQSTATE_CLOSE) {
		RunWithConfig(ctrlChild, CONFIGACTION_SAVE, WINDOWINSTANCINGMETHOD_CLOSE, &ctrlMain);
	} else if (requestedState == CHILDWINDOWREQSTATE_LOAD) {
		RunWithConfig(ctrlChild, CONFIGACTION_LOAD, WINDOWINSTANCINGMETHOD_NONE, &ctrlMain);
	} else if (requestedState == CHILDWINDOWREQSTATE_SAVE) {
		RunWithConfig(ctrlChild, CONFIGACTION_SAVE, WINDOWINSTANCINGMETHOD_NONE, &ctrlMain);
	} else {
		// error
	}
}
