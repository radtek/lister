#ifndef _lister_MyTool002_MySplitter_h_
#define _lister_MyTool002_MySplitter_h_

#include "shared.h"

#pragma warning(disable: 4355)

//==========================================================================================	
class MySplitter: public Splitter {
	public:

	//==========================================================================================	
	MySplitter() : Splitter() {
	}

	//==========================================================================================	
	void Xmlize(XmlIO xml) {
		int a;
		
		if(xml.IsLoading()) {
			xml("pos", a);
			if (a == 0) a = 20;
			SetPos(a);
		} else {
			// Storing
			a = GetPos();
			xml("pos", a);
		}
	}
	
	//==========================================================================================	
};


#endif
