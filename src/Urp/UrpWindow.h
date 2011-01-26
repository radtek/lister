#ifndef _Urp_UrpWindow_h_
#define _Urp_UrpWindow_h_

#include "UrpShared.h"
#include "UrpGridCommon.h"

//==============================================================================================
class UrpWindow : public TopWindow, public UrpGridCommon {
public:
	enum EnvCode {
			ENV_DEV = 1
		,	ENV_PROD
	};
	
	String            configFileNameBase;
	String            binConfigFile;
	String            xmlConfigFile;
   
	                  UrpWindow();
	virtual void      Run(bool appmodal = false); // Called from main.cpp
	virtual void      Open();
	virtual void      Open(Ctrl *owner);
	virtual bool      OpenIfNotOpen(Ctrl *owner = NULL);
	virtual void      Close();
	virtual bool      CloseIfNotClosed();
	virtual void      Xmlize(XmlIO xml);
};


#endif

