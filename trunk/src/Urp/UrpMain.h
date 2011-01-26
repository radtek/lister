#ifndef _Urp_UrpMain_h_
#define _Urp_UrpMain_h_

#include "UrpTopWindow.h"
#include "UrpWindow.h"

#define CHILDWINDOWREQSTATE_OPEN    1
#define CHILDWINDOWREQSTATE_CLOSE   2
#define CHILDWINDOWREQSTATE_LOAD    3
#define CHILDWINDOWREQSTATE_SAVE    4

int UrpMain(UrpTopWindow &ctrlMain);
void UrpChild(UrpTopWindow &ctrlMain, UrpWindow &ctrlChild, String tagName, int requestedState);

#endif
