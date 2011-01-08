#include "ConnState.h"

	ConnState::ConnState() : ColorPusher() {
		//	rgbactrl <<= THISBACK(ColorChanged);
	}
	
	/*static*/ Color ConnState::ConvertStateToColor(EnumConnState enumConnState) {
		
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
