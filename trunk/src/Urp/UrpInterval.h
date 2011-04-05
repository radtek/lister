#ifndef _lister_Urp_UrpInterval_h_
#define _lister_Urp_UrpInterval_h_

#include "UrpShared.h"

class Interval : public Time {
public:
              bool isnull;
                   Interval();
                   Interval(Time startTime, Time stopTime);
	virtual String ToString();
	
};

#endif
