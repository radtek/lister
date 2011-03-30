#ifndef _ConnGrid_h_
#define _ConnGrid_h_

#include "shared.h"
#include "ConnButton.h"
#include "Connection.h"
#include "ConnState.h"
#include "NewInstanceWin.h"
class Connection;

//==============================================================================================
class ConnGrid : public UrpGrid {
public:
	
	EditInt           fldConnId;
	EditStringNotNull fldConnName;          // 0) A meaningful name; too many connections to track without a name
	EditString        fldLoginStr;          // 1) The login string (Can be null if osAuth)
	EditString        fldLoginPwd;          // 2) The password in plain text
	EditString        fldInstNm;            // 3) The server connecting to, to display to user
	EditString        fldInstanceAddress;   // 4) The actual address code to pass to the connector function
	EditStringNotNull fldInstTypName;       // 6) String form of type of server: Oracle, MS SQL, etc, which connector we use
	EditString        fldEnvStdName;        // 7) Environment: Dev, UAT, prod.  This is the color
	EditString        fldConnNote;
	EditString        fldDbName;	
	EditInt           fldPortNo;
	DropGrid          instTypList;
	DropGrid          instanceList;
	DropGrid          envList;
	DropGrid          osAuthList;
	NewInstanceWin    newInstanceWin;
		
	typedef ConnGrid CLASSNAME;
	
	// Everything you need to connect to a server
	//GridCtrl *outputGrid;

	                  ConnGrid              ();
	int               GetConnId             (int row);
	void              SetConnId             (int row, int pconnId);
	String            GetConnName           (int row);
	String            GetInstanceTypeName   (int row);
	int               GetInstanceId         (int row);
	void              SetInstanceId         (int row, int pinstId);
	String            GetInstanceName       (int row);
	void              SetInstanceName       (int row, String pinstNm);
	String            GetInstanceAddress    (int row);
	void              SetInstanceAddress    (int row, String pinstAdr);
	int               GetInstTypId          (int row);
	void              SetInstTypId          (int row, int pinstTypId);
	String            GetDatabaseName       (int row);
	String            GetPortNo             (int row);
	void              SetPortNo             (int row, int pportNo);
	int               GetEnvId              (int row);
	void              SetEnvId              (int row, int penvId);
	int               GetLoginId            (int row);
	String            GetLoginStr           (int row);
	String            GetLoginPwd           (int row);
	ConnState *       GetConnState          (int row);
	void              SetConnState          (int row, EnumConnState enumConnState);
	bool              GetOSAuth             (int row);
	void              NewConn               ();
	virtual void      Build                 (Connection *pconnection);

	                  // Has to pass a connection that persists after this window closes
	virtual void      Load                  ();
	
	                  // Popup to let user enter new connection instance details
	void              NewInstance           ();

	                  // Let main interface set colors representing the state of the connection.
	void              AddedNewConnection    ();
	
	                  // Don't care if the connect button changed, or the color changed.
	bool              MeaningfulDataChange  ();
	
	                  // The main interface calls this to position and select a specific connection, usually
	                  // so it can trigger a connect event.
	bool              FindConnName          (String pconnName, bool silent = false);
	bool              FindConnId            (int pconnId, bool silent = false );
	bool              WasConnectionRequested();
};

#endif
