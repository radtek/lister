#ifndef _lister_lister_shared_layout_h_
#define _lister_lister_shared_layout_h_

using namespace Upp;

// Referenced from TaskDefWin.cpp in layout
// lister.cpp populates mainwindow manually
#include "TaskMacroGrid.h" 
#include "TaskMacroByDriverGrid.h" // Replacing TaskMacro (slowly)
#include "ElementGrid.h"
#include "TaskGrid.h"
#include "ScriptGrid.h" 
#include "LinkGrid.h"
#include "TestGrid.h"   // Referenced from layout

// Referenced from MainGrid
#include "RecordViewGrid.h" 

#include <lister/Urp/UrpEdit.h> // My fix of U++ RichEdit

#define LAYOUTFILE <lister/lister/lister.lay>
#include <CtrlCore/lay.h>

#endif
