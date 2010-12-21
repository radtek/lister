#ifndef _ConnStatus_h_
#define _ConnStatus_h_

#include "shared.h"
#include <CtrlLib/DlgColor.h>

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
	ConnState() : ColorPusher() {
		//	rgbactrl <<= THISBACK(ColorChanged);
	}
	
	static Color ConvertStateToColor(EnumConnState enumConnState) {
		
		switch (enumConnState) {
			case NOCON_NEVER: return Gray();
			case NOCON_WASSUCC: return Green();
			case NOCON_WASFAIL: return Red();
			case NOCON_UNDEF: return LtGray();
			case NOCON_MISCONFIG: return Magenta();
			case CONNECTING_START: return LtYellow();
			case CONNECTING_YAWN: return Yellow();
			case CONNECTING_2NDTRY: return LtMagenta();
			case CONNECTING_3RDTRY: return White();
			case CONNECTING_TIMEOUT: return Cyan();
			case CON_SUCCEED: return LtGreen();
			case CON_FAIL: return LtRed();
			case CON_STALE: return LtBlue();
		}
		
		return Black();
	}
};

#endif
