#ifndef _Oracle_Telnet_h_
#define _Oracle_Telnet_h_

class Telnet {
	bool WriteHexString(String msg) {
		String out;
		for (int i = 0; i < msg.GetCount(); i+=2) {
			uint8 c = xtoi(msg.Mid(i, 2));
			UpdateLog('>', c);
			out.Clear();
			out.Cat(c);
			s.Write(out);
		}
		
		return true;
	}
};

#endif
