#ifndef _Urp_UrpWindow_h_
#define _Urp_UrpWindow_h_

#include "UrpShared.h"

class UrpWindow : public TopWindow {
public:
	enum { MINIMIZED, MAXIMIZED, OVERLAPPED };
	Rect        overlapped;
	byte        state;

	UrpWindow();
	virtual void Run(bool appmodal = false);
	void SerializePlacement(Stream& s, bool reminimize = false);
};

class UrpTopWindow : public UrpWindow {
public:
	String		exeFilePath;
	String		connectAsUser;
	String		configFileFolder;
	String binConfigFile;
	String xmlConfigFile;
	
	UrpTopWindow();
	
	virtual void Run(bool appmodal = false);
};

#endif
