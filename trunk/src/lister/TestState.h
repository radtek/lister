#ifndef _TestState_h_
#define _TestState_h_

#include <CtrlLib/CtrlLib.h>
#include <CtrlLib/DlgColor.h>

using namespace Upp;

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
	TestState();

	static Color ConvertStateToColor(EnumTestState enumTestState);

};

#endif
