#ifndef _Urp_UrpSplitter_h_
#define _Urp_UrpSplitter_h_

#include "UrpShared.h"

class UrpSplitter: public Splitter {
public:
	int savedSplitterPosition;
	bool isExpanded; // Up to max temporarily
	bool isShrunk; // Down to zero temporarily
	
	UrpSplitter();
	void Xmlize(XmlIO xml);
};

#endif
