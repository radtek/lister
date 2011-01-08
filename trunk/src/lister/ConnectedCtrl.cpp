#include "ConnectedCtrl.h"
#include "Connection.h"

#include "ConnState.h"

ConnectedCtrl::ConnectedCtrl() {
	connection = NULL;
}

/*virtual*/ void ConnectedCtrl::Load(Connection *pconnection) {
	ASSERT(pconnection);
	ASSERT(pconnection->enumConnState == CON_SUCCEED);
	connection = pconnection;
}
