#ifndef _Urp_UrpWindow_h_
#define _Urp_UrpWindow_h_

#include "UrpShared.h"

//==============================================================================================
class UrpWindow : public TopWindow {
public:
	Vector<Ctrl *> ctrls;
	enum { MINIMIZED, MAXIMIZED, OVERLAPPED };
	Rect        overlapped;
	byte        state;
	String      configFileNameBase;
	String      binConfigFile;
	String      xmlConfigFile;
	Rect realPosition;

	UrpWindow();
	virtual void Run(bool appmodal = false);
	void AddCtrl(Ctrl *ctrl);
	void Serialize(Stream& s);
	void SerializePlacement(Stream& s, bool reminimize = false);
	virtual void Xmlize(XmlIO xml);
	virtual void Close();
	virtual void Open();
	virtual bool OpenIfNotOpen();
	virtual bool CloseIfNotClosed();
};

//==============================================================================================
class UrpTopWindow : public UrpWindow {
public:
	String		exeFilePath;
	String		connectAsUser;
	String		configFileFolder;
	
	UrpTopWindow();
	
	virtual void Run(bool appmodal = false);
};

//==============================================================================================
class UrpConfigWindow : public UrpWindow {
public:
	String                 configName;
	UrpTopWindow          *topWindow;

	// Construct a configurable window; take config path from topWindow
	UrpConfigWindow(UrpTopWindow *ptopWindow, String pconfigName);
	// Load configuration file based on 
	virtual void Open();
	virtual void Close();
};


//==============================================================================================
// Generator for dynamic creation of configurated windows
class UrpWindowFactory {
public:
	static VectorMap<String, UrpConfigWindow *>& Windows();
	UrpWindowFactory();
	~UrpWindowFactory();
	UrpConfigWindow *Open(UrpTopWindow *ptopWindow, String pwindowName); // Create and open window if not open, then show it
};

#endif

