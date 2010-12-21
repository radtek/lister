#ifndef _Connection_h_
#define _Connection_h_

#include "shared.h"
#include "ConnState.h"

//==========================================================================================
class Connection : Moveable<Connection> {
public:
	EnumConnState enumConnState;  // Track the connection state
	One<SqlSession> session;
	int connId; // Needed to pass on to cmdScript and TestGrid so they can store the id with new objects
	String connName; // Needed so main thread can update the title of the TopWindow

	//==========================================================================================
	Connection(int pconnId, String pconnName) {
		connId = pconnId; // Unknown
		connName = pconnName;		
		enumConnState = NOCON_UNDEF;
		session = NULL;
	}
	
	//==========================================================================================
	~Connection() {
		// Still connected?
	}
};

#endif
