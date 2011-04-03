/***********************************************************************************************
*  lister - shared_layout.h
*  
*  U++ requires careful organization of layouts.  To try and reduce code generation time, I
*  only include this file in the .h files that construct a class via a 
*  CtrlLayoutOKCancel(*this, "") function call.
*
*  Author: Jeff Humphreys
*  
*  2011
*  http://code.google.com/p/lister/
*  http://lister.googlecode.com/svn/trunk/ lister-read-only
*  I used http://sourceforge.net/projects/win32svn/
*  I recommend http://tortoisesvn.tigris.org/ for SVN Client use from Windows Explorer
*
***********************************************************************************************/

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
