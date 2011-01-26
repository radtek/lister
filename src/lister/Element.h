#ifndef _lister_lister_Element_h_
#define _lister_lister_Element_h_

#include "shared.h"

class Element {
public:
	int      elementId;
	String   elementName;
	String   status;
	String   note;
	int      taskId;
	
	Element();
};

#endif
