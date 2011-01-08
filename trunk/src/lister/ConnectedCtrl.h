#ifndef _lister_ConnectedCtrl_h_
#define _lister_ConnectedCtrl_h_

class Connection;

class ConnectedCtrl {
public:
	Connection *connection;
	ConnectedCtrl();
	
	virtual void Load(Connection *pconnection);

};


#endif
