/***********************************************************************************************
*  lister - TestState.cpp
*  
*  Same as ConnState.  Matches state codes to colors.
*  Could be rolled up into generic State object that you stuff a keypair vector into, matching
*  enum codes with colors.
*  Colors could be generalized to FAIL, SUCCEED always being mapped to RED and GREEN.  That way
*  the rules could generalize so the same colors across many grids would mean the same thing.
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

#include "TestState.h"

//==============================================================================================
TestState::TestState() : ColorPusher() {
	//	rgbactrl <<= THISBACK(ColorChanged);
}

//==============================================================================================
/*static*/ Color TestState::ConvertStateToColor(EnumTestState enumTestState) {
	
	switch (enumTestState) {
		case NOTEST_NEVER: return Gray();
//		case NOCON_WASSUCC: return Green();
//		case NOCON_WASFAIL: return Red();
		case NOTEST_UNDEF: return LtGray();
//		case NOCON_MISCONFIG: return Magenta();
		case TESTING_START: return LtYellow();
//		case CONNECTING_YAWN: return Yellow();
//		case CONNECTING_2NDTRY: return LtMagenta();
//		case CONNECTING_3RDTRY: return White();
		case TESTING_TIMEOUT: return Cyan();
		case TEST_PASS: return LtGreen();
		case TEST_FAIL: return LtRed();
		case TEST_INDETERMINATE: return Black();
	}
	
	return Black();
}
