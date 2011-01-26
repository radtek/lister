#ifndef _lister_Urp_UrpEdit_h_
#define _lister_Urp_UrpEdit_h_

#include "UrpShared.h"
#include <RichEdit/RichEdit.h>

class UrpEdit : public RichEdit {
public:
	double zoomlevel;
	
	UrpEdit();
	void Layout();
};


#endif
