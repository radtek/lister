#ifndef _TestState_h_
#define _TestState_h_

#include <CtrlLib/DlgColor.h>

	enum EnumTestState {
		NOTEST_NEVER,
		NOTEST_UNDEF,
		TESTING_START,
		TESTING_TIMEOUT,
		TEST_PASS,
		TEST_FAIL,
		TEST_INDETERMINATE
	};

class TestState : public ColorPusher {
public:
	TestState() : ColorPusher() {
		//	rgbactrl <<= THISBACK(ColorChanged);
	}

	static Color ConvertStateToColor(EnumTestState enumTestState) {
		
		switch (enumTestState) {
			case NOTEST_NEVER: return Gray();
//			case NOCON_WASSUCC: return Green();
//			case NOCON_WASFAIL: return Red();
			case NOTEST_UNDEF: return LtGray();
//			case NOCON_MISCONFIG: return Magenta();
			case TESTING_START: return LtYellow();
//			case CONNECTING_YAWN: return Yellow();
//			case CONNECTING_2NDTRY: return LtMagenta();
//			case CONNECTING_3RDTRY: return White();
			case TESTING_TIMEOUT: return Cyan();
			case TEST_PASS: return LtGreen();
			case TEST_FAIL: return LtRed();
			case TEST_INDETERMINATE: return Black();
		}
		
		return Black();
	}

};

#endif
