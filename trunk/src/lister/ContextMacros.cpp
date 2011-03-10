#include "ContextMacros.h"
#include "MacroHandler.h"

void ContextMacros::UpdateAvailableMacros(DropGrid &macrosAvailableList, ContextMacros *activeContextMacros) {
	
	macrosAvailableList.Clear();
	if (!activeContextMacros) return;
	MacMap &taskMacros = activeContextMacros->taskMacros;
	for (int i = 0; i < taskMacros.GetCount(); i++) {
		MacPair macPair = taskMacros.Get(taskMacros.GetKey(i));
		macrosAvailableList.Add(taskMacros.GetKey(i), macPair.replaceWith, macPair.expansion);
	}

	UpdateMacroList(macrosAvailableList);
}

