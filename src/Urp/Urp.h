#ifndef _Urp_Urp_h
#define _Urp_Urp_h

// Wyatt Earp!!!!!

// This header is restricted to inclusion in applications that want to use Urp functions.
// Please do not put any code here.

// Added support to serialize window positions over multiple screens
#include "UrpWindow.h"

// Recursive String replaceall function that can handle things like extraneous string compression

#include "UrpString.h"

// Added Key trap so I could detect key position for a buffer editor
#include "UrpLineEdit.h" 

// Grid that remembers its position in XML files
#include "UrpGrid.h"

// Sql Grid built off ArrayCtrl  should merge into UrpGrid
#include "UrpSqlGrid.h"

// Snippets for Managing various Paint scenarios for coloring, font control
#include "UrpPaint.h"

// A sizeable input box with ok/cancel button and label.
#include "UrpInput.h"

// A main caller that takes care of serialization and xmlization
#include "UrpMain.h"

// Splitter remembers its position on screen
#include "UrpSplitter.h"

#endif

