#ifndef _lister_lister_RecordViewWin_h_
#define _lister_lister_RecordViewWin_h_

#include "shared.h"
#include "shared_layout.h"

//==============================================================================================
class RecordViewWin : public WithRecordViewLayout<UrpWindow> {
public:
	typedef RecordViewWin CLASSNAME;
	
	RecordViewWin();
};

#endif
