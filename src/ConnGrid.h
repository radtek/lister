#ifndef _ConnGrid_h_
#define _ConnGrid_h_

#include "shared.h"
#include "MyGrid.h"
#include "ConnButton.h"
#include "ConnState.h"
#include "SqlUtil.h"
#include "NewInstanceWin.h"

#include <PostgreSQL/PostgreSQL.h> // Need to move to Connection manager

// THESE NAMES MUST BE UNIQUE IN ORDER TO LABEL COLUMN WIDTHS USER CUSTOMIZATIONS WHEN XMLIZING

Id IDConnState("ConnState");
Id IDConnId("ConnId"); 
Id IDConnName("ConnName"); 
Id IDLoginId("LoginId");
Id IDLoginStr("LoginStr"); 
Id IDLoginPwd("LoginPwd"); 
Id IDInstanceId("InstId");
Id IDInstanceName("InstanceName"); 
Id IDInstanceAddress("InstanceAddress"); 
Id IDInstTypId("InstTypID");                                            // 5) The Id of the server type; for the dropgrid and for updating the database
Id IDInstTypName("InstTypName");    
Id IDEnvId("EnvId"); 
Id IDEnvStdName("EnvStdName"); 
Id IDDUMMY("Dummy");
Id IDCONNECT("CONNECT");
Id IDConnNote("ConnNote"); 

//==========================================================================================	
void MakeButton(One<Ctrl>& ctrl) {
	ctrl.Create<ConnButton>();
	ctrl->WantFocus();
}

//==========================================================================================	
void MakeConnState(One<Ctrl>& ctrl) {
	ctrl.Create<ConnState>();
	ctrl->WantFocus();
}

//==========================================================================================	
class ConnGrid : public MyGrid {
public:
	PostgreSQLSession *connDb;
	
	EditInt fldConnId;
	EditStringNotNull fldConnName;               // 0) A meaningful name; too many connections to track without a name
	EditStringNotNull fldLoginStr;               // 1) The login string
	EditString fldLoginPwd;                      // 2) The password in plain text
	EditString fldInstNm;                // 3) The server connecting to, to display to user
	EditString fldInstanceAddress; // 4) The actual address code to pass to the connector function
	EditStringNotNull fldInstTypName;          // 6) String form of type of server: Oracle, MS SQL, etc, which connector we use
	EditString fldEnvStdName;                // 7) Environment: Dev, UAT, prod.  This is the color
	EditString fldConnNote;
	
	DropGrid instTypList;
	DropGrid instanceList;
	DropGrid envList;
	
	NewInstanceWin newInstanceWin;
		
	typedef ConnGrid CLASSNAME;

	// Everything you need to connect to a server
	GridCtrl *outputGrid;

	//==========================================================================================	
	ConnGrid() {
		connDb = NULL;
	}
	
	int GetConnId() { return Get(IDConnId); }
	void SetConnId(int pconnId) { Set(IDConnId, pconnId); }
	String GetConnName() { return TrimBoth(Get(IDConnName)); }
	String GetInstanceTypeName() { return TrimBoth(Get(IDInstTypName)); }
	int GetInstanceId() { return Get(IDInstanceId); }
	String GetInstanceName() { return TrimBoth(Get(IDInstanceName)); }
	String GetInstanceAddress() { return TrimBoth(Get(IDInstanceAddress)); }
	int GetInstTypId() { return Get(IDInstTypId); }
	int GetEnvId() { return Get(IDEnvId); }
	int GetLoginId() { return Get(IDLoginId); }
	String GetLoginStr() { return TrimBoth(Get(IDLoginStr)); }
	String GetLoginPwd() { return TrimBoth(Get(IDLoginPwd)); }
	ConnState *ConnGrid::GetConnState() { return (ConnState *)GetCtrl(GetCursor(), FindCol(IDConnState)); }

	//==========================================================================================	
	void NewConn() {
		GoTo(GetCurrentRow(), 2);
		StartEdit(); // Puts you into editmode so you can start typing the instance name right away.
	}
	
	//==========================================================================================	
	void Build() {
			Appending().Removing().Editing();
			EditsInNewRow().Accepting().Moving();
			MovingCols().MovingRows().Hiding().Duplicating().Canceling().AskRemove();
			Duplicating().Searching().SearchDisplay().SearchImmediate().SearchHideRows().SearchMoveCursor();
			Closing().Inserting().RowChanging().ResizingRows();
			LiveCursor();
			RejectNullRow(false);
			Clipboard();
			WhenNewRow = THISBACK(NewConn);
			Proportional();
			ColorRows(true);
			FixedPaste();
			instanceList.SearchHideRows().Resizeable().Width(200);
			instanceList.NotNull().AddPlus(THISBACK(NewInstance));
			
			// F3 moves the cursor to the next matched row. 
			// Hidden rows currently managed by serialization
			// Multi selecting works in the same way as in ArrayCtrl. One can select a row by holding CTRL and pressing LMB
						
			AddColumn(IDConnState, "").Ctrls(MakeConnState).Default(ConnState::ConvertStateToColor(NOCON_NEVER)).Fixed(16);
			AddColumn(IDConnId, "Id").Edit(fldConnId).Default(Null);
			AddColumn(IDConnName, "Name").Edit(fldConnName);
			AddColumn(IDLoginId, "Login Id");
			AddColumn(IDLoginStr, "Login").Edit(fldLoginStr);                                                             
			AddColumn(IDLoginPwd, "Pwd").Edit(fldLoginPwd);                                                               
			AddColumn(IDInstanceId, "Instance").Edit(instanceList).SetConvert(instanceList).Default(-1);
			AddColumn(IDInstanceName, "InstNm");
			AddColumn(IDInstTypId, "Database").Edit(instTypList).SetConvert(instTypList).Default(-1);
			AddColumn(IDInstTypName, "InstTypeNm");      
			AddColumn(IDInstanceAddress, "Address").Edit(fldInstanceAddress).Editable(false);
			AddColumn(IDEnvId, "Env").Edit(envList).SetConvert(envList).Default(-1);
			AddColumn(IDConnNote, "Note").Edit(fldConnNote);
			AddColumn(IDCONNECT, "").Ctrls(MakeButton).Fixed(20).SetImage(CtrlImg::go_forward());
			AddColumn(IDDUMMY, "").Fixed(1); // This is required due to bug in GridCtrl where image clones across all downstream cells if at end of visible chain.
			//Absolute(); // Warning: Hiding/moving columns will cause blackout drawing issues, but only way to get user sizing to retain across sessions (see xmlize)
	}
	
	//==========================================================================================	
	void NewInstance() {
		if (!newInstanceWin.IsOpen()) {
			newInstanceWin.Open();
		}
	}

	// Has to pass a connection that persists after this window closes
	//==========================================================================================	
	void Load(PostgreSQLSession *pconnDb, Sql &connSql) {
		connDb = pconnDb;
		// Populate the instance types
			
		if (!connSql.Execute("select insttypid, insttypname from insttyps")) {
			Exclamation(Format("Error: %s", DeQtf(connSql.GetLastError())));		
		} else {
			while(connSql.Fetch()) {
				instTypList.Add(atoi(connSql[0].ToString()), connSql[1].ToString());
				newInstanceWin.instTypList.Add(atoi(connSql[0].ToString()), connSql[1].ToString());
			}
		}
		
		// Populate the instance list
		
		if (!connSql.Execute("select i.instanceid, i.instancename, i.instanceaddress, i.note, it.insttypid, it.insttypname, i.envid from instances i left join insttyps it on i.insttypid = it.insttypid order by instancename")) {
			Exclamation(Format("Error: %s", DeQtf(connSql.GetLastError())));		
		} else {
	
			// Populate the Environment list
	
			while(connSql.Fetch()) {
				instanceList.Add(atoi(connSql[0].ToString()), connSql[1].ToString());
			}
		}
		
		if (!connSql.Execute("select e.envid, e.envstdname, e.envletter from environments e")) {
			Exclamation(Format("Error: %s", DeQtf(connSql.GetLastError())));		
		} else {
	
			// Populate the Connection List so user can select
	
			while(connSql.Fetch()) {
				envList.Add(atoi(connSql[0].ToString()), connSql[1].ToString());
				newInstanceWin.envList.Add(atoi(connSql[0].ToString()), connSql[1].ToString());
				newInstanceWin.envList.SetData(-1);
			}
		}
		
		if (!connSql.Execute("select ConnId, ConnName, LoginId, LoginStr, LoginPwd, InstanceId, InstanceName, InstanceAddress, InstTypID, InstTypName, EnvId, EnvStdName from v_conn order by ConnName")) {
			Exclamation(Format("Error: %s", DeQtf(connSql.GetLastError())));
			return;
		}
		while(connSql.Fetch()) {
			Add();
			Set(IDConnState, ConnState::ConvertStateToColor(NOCON_UNDEF));  // Show connection as red, not connected, should change to gray if unknown
			Set(IDConnId, connSql["CONNID"]);
			Set(IDConnName, connSql["CONNNAME"]);
			Set(IDLoginId, connSql["LOGINID"]);
			Set(IDLoginStr, connSql["LOGINSTR"]);
			Set(IDLoginPwd, connSql["LOGINPWD"]);
			Set(IDInstanceId, connSql["INSTANCEID"]);
			Set(IDInstanceName, connSql["INSTANCENAME"]);
			Set(IDInstanceAddress, connSql["INSTANCEADDRESS"]);
			Set(IDInstTypId, connSql["INSTTYPID"]);
			Set(IDInstTypName, connSql["INSTTYPNAME"]);
			Set(IDEnvId, connSql["ENVID"]);
		}
		
		// Create a blank row so user can just type new connection detail (not have to preset Insert)
	
		WhenAcceptedRow = THISBACK(AddedNewConnection);
	}
	
	//==========================================================================================	
	//  Let main interface set colors representing the state of the connection.
	void SetConnState(EnumConnState enumConnState) {
		Color &connColor = ConnState::ConvertStateToColor(enumConnState);
		Set(GetCursor(), IDConnState, connColor);
	}
	
	//==========================================================================================	
	void AddedNewConnection() {
		Sql connSql(*connDb);
		if (!MeaningfulDataChange()) return;
		if (IsNewRow()) {	
			
			// SQL allows INSTEAD OF VIEW code to search for the login/pwd combo and create a row if necessary
			// We do pass a valid instance #, though.
			String sql = Format(" \
			INSERT INTO v_conn(ConnName, LoginStr, LoginPwd, InstanceId) VALUES \
			                     ('%s'/*ConnName*/, '%s'/*LoginStr*/, '%s'/*LoginPwd*/, %d/*InstanceId*/)", 
			                   GetConnName(), GetLoginStr(), GetLoginPwd(), GetInstanceId());
			int rsp = PromptOKCancel(CAT << "Adding Connection: " << sql);
			if (rsp == 1) {
				
				if (!connSql.Execute(sql)) {
					Exclamation(Format("Error inserting v_conn: %s, %s", DeQtf(connSql.GetLastError()), sql));
					return;
				} else {
					int connId = GetInsertedId(connSql, "connections", "connid");
					SetConnId(connId);
					
					//Exclamation("Added Connection");
				}
				
				// Fetch generated key for connid
			}
		} else {
			String sql = Format(" \
			UPDATE v_conn set ConnName = '%s', LoginStr = '%s', LoginPwd = '%s', InstanceId = %d \
			     WHERE ConnId = %d", 
			     GetConnName(), GetLoginStr(), GetLoginPwd(), GetInstanceId(), GetConnId());
			int rsp = PromptOKCancel(CAT << "Updating Connection: " << sql);
			if (rsp == 1) {
				
				if (!connSql.Execute(sql)) {
					Exclamation(Format("Error updating v_conn: %s, %s", DeQtf(connSql.GetLastError()), sql));
					return;
				} else {
					
					//Exclamation("Updated Connection");
				}
			}
		}
	}
		
	//==========================================================================================	
	void HidePrivateColumns() {
		GetColumn(FindCol(IDConnId)).Hidden();
		GetColumn(FindCol(IDLoginId)).Hidden();
		GetColumn(FindCol(IDInstanceName)).Hidden();
		GetColumn(FindCol(IDInstanceAddress)).Hidden();
		GetColumn(FindCol(IDInstTypName)).Hidden();
	}
	
	//==========================================================================================	
	// Don't care if the connect button changed, or the color changed.
	bool MeaningfulDataChange() {
		if (!IsModifiedRow()) return false; // No change
		
		if (
			!IsModified(IDConnName) &&
			// Don't care, this is from a new record setting from GetInsertedId, !IsModified(IDConnId) &&
			!IsModified(IDLoginId) &&
			!IsModified(IDLoginStr) &&
			!IsModified(IDLoginPwd) &&
			!IsModified(IDInstanceId) &&
			!IsModified(IDConnNote)
			) return false;
	
		return true;
	}

	//==========================================================================================	
	//  The main interface calls this to position and select a specific connection, usually
	//  so it can trigger a connect event.
	bool FindConnName(String pconnName) {
		int rowno = Find(pconnName, IDConnName);
		if (rowno == -1) return false;
		GoTo(rowno);
		CenterCursor();
		ProcessEvents();
		return true;
	}

	//==========================================================================================	
	bool WasConnectionRequested() {
		int rowno = GetCursor();
		if (rowno == -1) return false;
		ConnButton *btn = (ConnButton *)GetCtrl(rowno, FindCol(IDCONNECT));
		if (!btn) return false;
		if (btn->wasClicked) {
			btn->wasClicked = false; // Clear the bit for next click
			return true;
		} else {
			return false;
		}
	}
};

#endif

/*
CREATE OR REPLACE RULE v_conn_upd AS
    ON UPDATE TO v_conn DO INSTEAD  
    (  
       UPDATE connections SET connname = new.connname, loginid = getnewlogin(new.loginid, new.loginstr, new.loginpwd), instanceid = new.instanceid, note = new.note WHERE connid = old.connid ;
       insert into actionlog(note) values('Updated connection');
     );
     
CREATE OR REPLACE RULE v_conn_ins AS
    ON INSERT TO v_conn DO INSTEAD  
    (  
       INSERT INTO connections (connid, connname, loginid, instanceid, lastsucceeddatetime, lastfaildatetime, note) 
       VALUES (DEFAULT, new.connname, getnewlogin(new.loginid, new.loginstr, new.loginpwd), new.instanceid, new.lastsucceeddatetime, new.lastfaildatetime, new.note);
       insert into actionlog(note) values('Added connection');
     );

CREATE OR REPLACE FUNCTION getnewlogin(ploginid INTEGER, ploginstr TEXT, ploginpwd TEXT) RETURNS INTEGER AS $newlogin$
DECLARE
   floginid INTEGER;
BEGIN
   IF ploginid IS NOT NULL THEN
     RETURN ploginid;
   END IF;
   
   SELECT loginid INTO floginid FROM logins WHERE loginstr = ploginstr and loginpwd = ploginpwd;
   IF FOUND THEN
     RETURN floginid;
   ELSE
     INSERT INTO logins(loginstr, loginpwd) VALUES(ploginstr, ploginpwd) RETURNING loginid INTO floginid;
     RETURN floginid;
   END IF;
     
   RETURN 0;
END;
$newlogin$ LANGUAGE plpgsql;
*/
