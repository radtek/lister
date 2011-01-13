#ifndef _ConnGrid_h_
#define _ConnGrid_h_

#include "shared.h"
#include "ConnButton.h"
#include "Connection.h"
#include "ConnState.h"
#include "NewInstanceWin.h"
#include "ConnectedCtrl.h"

//==========================================================================================	
class ConnGrid : public UrpGrid, ConnectedCtrl {
public:
	
	EditInt fldConnId;
	EditStringNotNull fldConnName;               // 0) A meaningful name; too many connections to track without a name
	EditStringNotNull fldLoginStr;               // 1) The login string
	EditString fldLoginPwd;                      // 2) The password in plain text
	EditString fldInstNm;                // 3) The server connecting to, to display to user
	EditString fldInstanceAddress; // 4) The actual address code to pass to the connector function
	EditStringNotNull fldInstTypName;          // 6) String form of type of server: Oracle, MS SQL, etc, which connector we use
	EditString fldEnvStdName;                // 7) Environment: Dev, UAT, prod.  This is the color
	EditString fldConnNote;
	EditString fldDbName;	
	DropGrid instTypList;
	DropGrid instanceList;
	DropGrid envList;
	
	NewInstanceWin newInstanceWin;
		
	typedef ConnGrid CLASSNAME;
	
	// Everything you need to connect to a server
	//GridCtrl *outputGrid;

	ConnGrid();
	int GetConnId(int row);
	void SetConnId(int row, int pconnId);
	String GetConnName(int row);
	String GetInstanceTypeName(int row);
	int GetInstanceId(int row);
	String GetInstanceName(int row);
	String GetInstanceAddress(int row);
	int GetInstTypId(int row);
	String GetDatabaseName(int row);
	int GetEnvId(int row);
	int GetLoginId(int row);
	String GetLoginStr(int row);
	String GetLoginPwd(int row);
	ConnState *ConnGrid::GetConnState(int row);

	void NewConn();
	
	void Build();
	
	// Popup to let user enter new connection instance details
	void NewInstance();

	// Has to pass a connection that persists after this window closes
	virtual void Load(Connection *pconnection);
	
	//  Let main interface set colors representing the state of the connection.
	void SetConnState(int row, EnumConnState enumConnState);
	
	void AddedNewConnection();
		
	// Don't care if the connect button changed, or the color changed.
	bool MeaningfulDataChange();

	//  The main interface calls this to position and select a specific connection, usually
	//  so it can trigger a connect event.
	bool FindConnName(String pconnName, bool silent = false);

	bool WasConnectionRequested();
};

#endif
