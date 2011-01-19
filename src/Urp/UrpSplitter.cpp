#include "UrpSplitter.h"

//==========================================================================================	
UrpSplitter::UrpSplitter() : Splitter() {
	savedSplitterPosition = -1;
	isExpanded = isShrunk = false;
}

//==========================================================================================	
void UrpSplitter::Xmlize(XmlIO xml) {
	int a;

	xml
		("isexpanded", isExpanded)
		("isshrunk", isShrunk)
		("savedpos", savedSplitterPosition)
	;
	
	if (xml.IsLoading()) {
		xml
		    ("pos", a)
		;
		
		if (a <= 20) a = 200;
		SetPos(a);
	} else {
		// Storing
		a = GetPos();
		xml
			("pos", a)
		;
	}
}

