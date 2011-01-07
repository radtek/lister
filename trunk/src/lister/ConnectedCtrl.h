#ifndef _lister_ConnectedCtrl_h_
#define _lister_ConnectedCtrl_h_

#include "shared.h"
#include "Connection.h"
#include "ConnState.h"

class ConnectedCtrl {
public:
	Connection *connection;
	ConnectedCtrl() {
		connection = NULL;
	}
	
	virtual void Load(Connection *pconnection) {
		ASSERT(pconnection);
		ASSERT(pconnection->enumConnState == CON_SUCCEED);
		connection = pconnection;
	}

};


#endif
