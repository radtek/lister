#ifndef _ConnStatus_h_
#define _ConnStatus_h_

#include <CtrlLib/CtrlLib.h>
#include <CtrlLib/DlgColor.h>

using namespace Upp;

	enum EnumConnState {
		NOCON_NEVER,
		NOCON_WASSUCC,
		NOCON_WASFAIL,
		NOCON_UNDEF,
		NOCON_MISCONFIG,
		CONNECTING_START,
		CONNECTING_YAWN,
		CONNECTING_2NDTRY,
		CONNECTING_3RDTRY,
		CONNECTING_TIMEOUT,
		CON_SUCCEED,
		CON_FAIL,
		CON_STALE
	};

class ConnState : public ColorPusher {
public:
	ConnState();
	
	static Color ConvertStateToColor(EnumConnState enumConnState);
};

#endif
