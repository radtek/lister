#ifndef _Urp_UrpSplitter_h_
#define _Urp_UrpSplitter_h_

#include "UrpShared.h"

class UrpSplitter: public Splitter {
public:

	//==========================================================================================	
	void Xmlize(XmlIO xml) {
		int a;
		
		if(xml.IsLoading()) {
			xml("pos", a);
			if (a <= 20) a = 200;
			SetPos(a);
		} else {
			// Storing
			a = GetPos();
			xml("pos", a);
		}
	}
};

#endif
