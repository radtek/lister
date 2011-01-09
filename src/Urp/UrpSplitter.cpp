#include "UrpSplitter.h"

//==========================================================================================	
void UrpSplitter::Xmlize(XmlIO xml) {
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

