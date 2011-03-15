#include "ConnGrid.h"

#include "shared_db.h"
#include <lister/Sql/Sql.h>

// THESE NAMES MUST BE UNIQUE IN ORDER TO LABEL COLUMN WIDTHS USER CUSTOMIZATIONS WHEN XMLIZING

Id IDConnState       ("ConnState");
Id IDConnId          ("ConnId"); 
Id IDConnName        ("ConnName"); 
Id IDLoginId         ("LoginId");
Id IDLoginStr        ("LoginStr"); 
Id IDLoginPwd        ("LoginPwd"); 
Id IDInstanceId      ("InstId");
Id IDInstanceName    ("InstanceName"); 
Id IDInstanceAddress ("InstanceAddress"); 
Id IDPortNo          ("PortNo");
Id IDInstTypId       ("InstTypID");       // The Id of the server type; for the dropgrid and for updating the database
Id IDInstTypName     ("InstTypName");    
Id IDDatabaseName    ("Database");        // For connections to MS SQL and PostgreSQL, a database attribute should be passed in the connection
Id IDEnvId           ("EnvId"); 
Id IDEnvStdName      ("EnvStdName"); 
Id IDConnNote        ("ConnNote"); 
Id IDIsOSAuth        ("IsOSAuth");
Id IDCONNECT         ("CONNECT!");
Id IDDUMMY           ("Dummy");

//==============================================================================================
void MakeButton(One<Ctrl>& ctrl) {
	ctrl.Create<ConnButton>();
	ctrl->WantFocus();
}

//==============================================================================================
void MakeConnState(One<Ctrl>& ctrl) {
	ctrl.Create<ConnState>();
	ctrl->WantFocus();
}

//==============================================================================================
ConnGrid::ConnGrid() : UrpGrid() {

}

//==============================================================================================
int        ConnGrid::GetConnId          (int row)				   { return Get(row, IDConnId); }
void       ConnGrid::SetConnId          (int row, int pconnId)	   { Set(row, IDConnId, pconnId); }
String     ConnGrid::GetConnName        (int row)				   { return TrimBoth(Get(row, IDConnName)); }
String     ConnGrid::GetInstanceTypeName(int row)				   { return TrimBoth(Get(row, IDInstTypName)); }
int        ConnGrid::GetInstanceId      (int row)				   { return Get(row, IDInstanceId); }
void       ConnGrid::SetInstanceId      (int row, int pinstId)	   { Set(row, IDInstanceId, pinstId); }
String     ConnGrid::GetInstanceName    (int row)				   { return TrimBoth(Get(row, IDInstanceName)); }
void       ConnGrid::SetInstanceName    (int row, String pinstNm)  { Set(row, IDInstanceName, pinstNm); }
String     ConnGrid::GetInstanceAddress (int row)				   { return TrimBoth(Get(row, IDInstanceAddress)); }
void       ConnGrid::SetInstanceAddress (int row, String pinstAdr) { Set(row, IDInstanceAddress, pinstAdr); }
int        ConnGrid::GetInstTypId       (int row)			    { return Get(row, IDInstTypId); }
void       ConnGrid::SetInstTypId       (int row, int pinstTypId) { Set(row, IDInstTypId, pinstTypId); }
String     ConnGrid::GetDatabaseName    (int row)           	{ return Get(row, IDDatabaseName); }
String     ConnGrid::GetPortNo          (int row)           	{ return ((int)Get(row, IDPortNo) == -1 || IsNull((int)Get(row, IDPortNo)) ? "" : ToString(Get(row, IDPortNo))); }
void       ConnGrid::SetPortNo          (int row, int pportNo) 	{ Set(row, IDPortNo, pportNo); }
int        ConnGrid::GetEnvId           (int row)				{ return Get(row, IDEnvId); }
void       ConnGrid::SetEnvId           (int row, int penvId)   { Set(row, IDEnvId, penvId); }
int        ConnGrid::GetLoginId         (int row)				{ return Get(row, IDLoginId); }
String     ConnGrid::GetLoginStr        (int row)				{ return TrimBoth(Get(row, IDLoginStr)); }
String     ConnGrid::GetLoginPwd        (int row)				{ return TrimBoth(Get(row, IDLoginPwd)); }
ConnState *ConnGrid::GetConnState       (int row)       	    { return (ConnState *)GetCtrl(row, FindCol(IDConnState)); }
bool       ConnGrid::GetOSAuth          (int row)               { return SqlToBool(Get(row, IDIsOSAuth)); }


//==============================================================================================
void ConnGrid::NewConn() {
	GoTo(GetCurrentRow(), 2);
	StartEdit(); // Puts you into editmode so you can start typing the instance name right away.
}

//==============================================================================================
void ConnGrid::Build() {
	
	WhenNewRow = THISBACK(NewConn);
	
	instanceList.SearchHideRows().Resizeable().Width(200);
	instanceList.NotNull().AddPlus(THISBACK(NewInstance));
	
	// F3 moves the cursor to the next matched row. 
	// Hidden rows currently managed by serialization
	// Multi selecting works in the same way as in ArrayCtrl. One can select a row by holding CTRL and pressing LMB
				
	GridCtrl::ItemRect& x = AddColumn(IDConnState, "").Ctrls(MakeConnState).Default(ConnState::ConvertStateToColor(NOCON_NEVER)).Fixed(16);
	bool r = x.IsFixed(); // ismin ismax are not set in original GridCtrl, so I cloned it and fixed it, and made this function pubic.
//	bool rmax = x.IsMax();
//	bool rmin = x.IsMin();
	AddColumn( IDConnId         , "Id"          ).Edit(fldConnId         )                         .Default(Null);
	AddColumn( IDConnName       , "Name"        ).Edit(fldConnName       );
	AddColumn( IDLoginId        , "Login Id"    );
	AddColumn( IDLoginStr       , "Login"       ).Edit(fldLoginStr       );                                                             
	AddColumn( IDLoginPwd       , "Pwd"         ).Edit(fldLoginPwd       );                                                               
	AddColumn( IDIsOSAuth       , "OS Auth?"    ).Edit(osAuthList        ).SetConvert(osAuthList  ); // A Login property.
	AddColumn( IDInstanceId     , "Instance"    ).Edit(instanceList      ).SetConvert(instanceList).Default(-1);
	AddColumn( IDInstanceName   , "InstNm"      );
	AddColumn( IDInstTypId      , "InstTyp"     ).Edit(instTypList       ).SetConvert(instTypList ).Default(-1);
	AddColumn( IDInstTypName    , "InstTypeNm"  ); // Not really editable yet    
	AddColumn( IDDatabaseName   , "DbNm"        ).Edit(fldDbName         );
	AddColumn( IDPortNo         , "PrtNo"       ).Edit(fldPortNo         );
	AddColumn( IDInstanceAddress, "Address"     ).Edit(fldInstanceAddress).Editable(false);
	AddColumn( IDEnvId          , "Env"         ).Edit(envList           ).SetConvert(envList     ).Default(-1);
	AddColumn( IDConnNote       , "Note"        ).Edit(fldConnNote       );
	AddColumn( IDCONNECT        , ""            ).Ctrls(MakeButton).Fixed(20).SetImage(CtrlImg::go_forward());
	// Always last column, helps overcome bug in GridCtrl
	AddColumn( IDDUMMY          , ""            )                  .Fixed(1); // This is required due to bug in GridCtrl where image clones across all downstream cells if at end of visible chain.
	
	built = true;
}

//==============================================================================================
// Popup to let user enter new connection instance details
void ConnGrid::NewInstance() {
	int row = GetCurrentRow(); // Save our place
	
	switch(newInstanceWin.Run()) {
	case IDOK:
		Exclamation("Adding");
		{
			String newInstanceName = ToUpper(newInstanceWin.GetInstanceName());
		 	String instAddr        = newInstanceWin.instanceAddress.GetData().ToString();
		 	int instTypId          = newInstanceWin.instTypList.GetKey();
		 	int envId              = newInstanceWin.envList.GetKey();
		 	int portNo;
		 	Value vportNo          = newInstanceWin.portNo.GetData();
		 	if (vportNo == -1 || vportNo.IsNull()) { 
		 		portNo = -1;
		 	} else {
		 		portNo = vportNo;
		 	}

			SqlInsert q = ::Insert(INSTANCES);
			
			q(INSTANCENAME    , newInstanceName);
			q(INSTANCEADDRESS , instAddr);
			q(INSTTYPID       , instTypId);
			q(ENVID           , envId);
			q(PORTNO          , portNo);
			;

			String script = SqlStatement(q).Get(PGSQL);
			if (connection->SendAddDataScript(script)) {
				// Fetch id
				int id = connection->GetInsertedId("instances", "instanceid");
				EndEdit();
				instanceList.Add(id, newInstanceName);
				
				SetInstanceId      (row, id);
				SetInstanceName    (row, newInstanceName);
				SetEnvId           (row, envId);
				SetInstTypId       (row, instTypId);
				SetInstanceAddress (row, instAddr);
				SetPortNo          (row, portNo);
				
				instanceList.FindMove(newInstanceName);
			}
		}
		break;

	case IDCANCEL:
		break;
	}
	newInstanceWin.Close();
}

// Has to pass a connection that persists after this window closes
//==============================================================================================
/*virtual*/ void ConnGrid::Load(Connection *pconnection) {
	ConnectedCtrl::Load(pconnection);
	connection = pconnection;

	// Populate the instance types
		
	//if (connection->SendQueryDataScript("select insttypid, insttypname from insttyps")) {
	if (connection->SendQueryDataScript(SqlStatement(SqlSelect(INSTTYPID, INSTTYPNAME).From(INSTTYPS)).GetText())) {
		while(connection->Fetch()) {
			instTypList.Add(connection->Get(0), connection->Get(1));
			newInstanceWin.instTypList.Add(connection->Get(0), connection->Get(1));
		}
	}
	
	// Populate the instance list
	
	osAuthList.Clear();
	osAuthList.Add("0", "No");  //"Standard Authorization by password");
	osAuthList.Add("1", "Yes"); //"Set flag for OS authorization based on current NT login");

	if (connection->SendQueryDataScript("select i.instanceid, i.instancename, i.instanceaddress, i.note, it.insttypid, it.insttypname, i.envid from instances i left join insttyps it on i.insttypid = it.insttypid order by instancename")) {


		while(connection->Fetch()) {
			instanceList.Add(connection->Get(0), connection->Get(1));
		}
	}

	// Populate the Environment list
	
	if (connection->SendQueryDataScript("select e.envid, e.envstdname, e.envletter from environments e")) {


		while(connection->Fetch()) {
			envList.Add(connection->Get(0), connection->Get(1));
			newInstanceWin.envList.Add(connection->Get(0), connection->Get(1));
			newInstanceWin.envList.SetData(-1);
		}
	}

	// Populate the Connection List so user can select
	
	if (!connection->SendQueryDataScript("select ConnId, ConnName, LoginId, LoginStr, LoginPwd, InstanceId"
	", InstanceName, InstanceAddress, InstTypID, InstTypName, dbName, EnvId, EnvStdName, PortNo, isOSAuth "
	" from v_conn where ConnId <> -1 order by ConnName")) {
		return;
	}
	
	while(connection->Fetch()) {
		Add();
		Set(IDConnState       , ConnState::ConvertStateToColor(NOCON_UNDEF));  // Show connection as red, not connected, should change to gray if unknown
		Set(IDConnId          , connection->Get("CONNID"         ));
		Set(IDConnName        , connection->Get("CONNNAME"       ));
		Set(IDLoginId         , connection->Get("LOGINID"        ));
		Set(IDLoginStr        , connection->Get("LOGINSTR"       ));
		Set(IDLoginPwd        , connection->Get("LOGINPWD"       ));
		Set(IDIsOSAuth        , connection->Get( ISOSAUTH        ));
		Set(IDInstanceId      , connection->Get("INSTANCEID"     ));
		Set(IDInstanceName    , connection->Get("INSTANCENAME"   ));
		Set(IDInstanceAddress , connection->Get("INSTANCEADDRESS"));
		Set(IDInstTypId       , connection->Get("INSTTYPID"      ));
		Set(IDInstTypName     , connection->Get("INSTTYPNAME"    ));
		Set(IDDatabaseName    , connection->Get("DBNAME"         ));
		Set(IDPortNo          , connection->Get( PORTNO          ));
		Set(IDEnvId           , connection->Get("ENVID"          ));
	}
	
	// Create a blank row so user can just type new connection detail (not have to preset Insert)

	WhenAcceptedRow = THISBACK(AddedNewConnection);
}

//==============================================================================================
//  Let main interface set colors representing the state of the connection.
void ConnGrid::SetConnState(int row, EnumConnState enumConnState) {
	Color &connColor = ConnState::ConvertStateToColor(enumConnState);
	Set(row, IDConnState, connColor);
}

//==============================================================================================
void ConnGrid::AddedNewConnection() {
	ASSERT(connection);
	ASSERT(IsCursor());
	int row = GetCursor();
	
	// If no data changed that we need to update, ignore.
	
	if (!MeaningfulDataChange()) return;
	
	if (IsNewRow()) {	
		
		// SQL allows INSTEAD OF VIEW code to search for the login/pwd combo and create a row if necessary
		// We do pass a valid instance #, though.
		String script = Format(" \
		INSERT INTO v_conn(ConnName, LoginStr, LoginPwd, IsOSAuth, InstanceId, instanceName, instanceAddress, instTypId, dbName, portNo, EnvId) VALUES \
		                     ('%s'/*ConnName*/, '%s'/*LoginStr*/, '%s'/*LoginPwd*/, '%s' /*IsOSAuth*/, %d/*InstanceId*/, '%s'/*InstanceName*/, '%s'/*InstanceAddress*/, %d/*instTypId*/, '%s' /*dbName*/, %s /*portNo*/, %s/*EnvId*/)", 
		                   GetConnName(row), GetLoginStr(row), GetLoginPwd(row), GetOSAuth(row)? "1" : "0", GetInstanceId(row), GetInstanceName(row), GetInstanceAddress(row), GetInstTypId(row), GetDatabaseName(row), ToSQL(GetPortNo(row)), ToSQL(GetEnvId(row)));
		int rsp = PromptOKCancel(CAT << "Adding Connection: " << script);
		if (rsp == 1) {
			
			if (connection->SendAddDataScript(script)) {
				int connId = connection->GetInsertedId("connections", "connid");
				SetConnId(row, connId);
			}
		}
	} else {
		String script = Format(" \
		UPDATE v_conn set ConnName = '%s', LoginStr = '%s', LoginPwd = '%s', IsOSAuth = '%s', InstanceId = %d, dbName = '%s', portNo= %s, envId= %s \
		     WHERE ConnId = %d", 
		     GetConnName(row), GetLoginStr(row), GetLoginPwd(row), GetOSAuth(row)? "1" : "0", GetInstanceId(row), GetDatabaseName(row), ToSQL(GetPortNo(row)), ToSQL(GetEnvId(row)), GetConnId(row));
		int rsp = PromptOKCancel(CAT << "Updating Connection: " << script);
		if (rsp == 1) {
			connection->SendChangeDataScript(script);
		}
	}
}
	
//==============================================================================================
// Don't care if the connect button changed, or the color changed.
bool ConnGrid::MeaningfulDataChange() {
	if (!IsModifiedRow()) return false; // No change
	
	if (
		!IsModified(IDConnName        ) &&
		// Don't care, this is from a new record setting from GetInsertedId, !IsModified(IDConnId) &&
		!IsModified(IDLoginId         ) &&
		!IsModified(IDLoginStr        ) &&
		!IsModified(IDLoginPwd        ) &&
		!IsModified(IDInstanceId      ) &&
		!IsModified(IDConnNote        ) &&
		!IsModified(IDDatabaseName    ) &&
		!IsModified(IDPortNo          ) &&
		!IsModified(IDEnvId           ) &&
		!IsModified(IDInstTypId       ) &&
		!IsModified(IDConnNote        ) &&
		!IsModified(IDInstanceAddress ) &&
		!IsModified(IDInstanceName    ) &&
		!IsModified(IDIsOSAuth        )
		) return false;

	return true;
}

//==============================================================================================
// Programmatically select a connection, usually from a task script selection
bool ConnGrid::FindConnId(int pconnId, bool silent /*= false*/) {
	int rowno = Find(pconnId, IDConnId);
	if (rowno == -1) return false;
	GoTo(rowno);
	CenterCursor();
	ProcessEvents();
	return true;
}

//==============================================================================================
//  The main interface calls this to position and select a specific connection, usually
//  so it can trigger a connect event.
bool ConnGrid::FindConnName(String pconnName, bool silent /*= false*/) {
	int rowno = Find(pconnName, IDConnName);
	if (rowno == -1) return false;
	GoTo(rowno);
	CenterCursor();
	ProcessEvents();
	return true;
}

//==============================================================================================
bool ConnGrid::WasConnectionRequested() {
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
