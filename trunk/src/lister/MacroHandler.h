#ifndef _lister_lister_MacroHandler_h_
#define _lister_lister_MacroHandler_h_

#include "ContextMacros.h"

String ExpandMacros(String inputText, ContextMacros *contextMacros);
void UpdateMacroList(DropGrid &macrosAvailableList);

#endif
