#ifndef _Urp_UrpMain_h_
#define _Urp_UrpMain_h_

#define CHILDWINDOWREQSTATE_OPEN    1
#define CHILDWINDOWREQSTATE_CLOSE   2
#define CHILDWINDOWREQSTATE_LOAD    3
#define CHILDWINDOWREQSTATE_SAVE    4

int UrpMain(UrpTopWindow &ctrlMain, const char *mainFilePath);
void UrpChild(UrpTopWindow &ctrlMain, UrpWindow &ctrlChild, String tagName, int requestedState);

#endif
