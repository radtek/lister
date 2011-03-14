#include "ContextMacros.h"
#include "MacroHandler.h"

ContextMacros::ContextMacros() {
	allMacros.Add(&envMacros);
	allMacros.Add(&taskMacros);
}

//==============================================================================================
void ContextMacros::UpdateAvailableMacros(DropGrid &macrosAvailableList, ContextMacros *activeContextMacros) {
	
	macrosAvailableList.Clear();
	if (!activeContextMacros) return;
	
	for (int j = 0; j < activeContextMacros->allMacros.GetCount(); j++) {
		MacMap &macros = *(activeContextMacros->allMacros[j]);
		for (int i = 0; i < macros.GetCount(); i++) {
			MacPair macPair = macros.Get(macros.GetKey(i));
			macrosAvailableList.Add(macros.GetKey(i), macPair.replaceWith, macPair.expansion);
		}
	}
	
	UpdateMacroList(macrosAvailableList);
}

