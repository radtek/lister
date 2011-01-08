#ifndef _Connection_h_
#define _Connection_h_

#include "shared.h"
#include "ConnState.h"
#include "SoundHandler.h"

#define INSTTYPNM_POSTGRESQL       "PostgreSQL"
#define INSTTYPNM_ORACLE           "Oracle"
#define INSTTYPNM_MSSQLSERVER      "MS SQL"
#define INSTTYPNM_FTP              "ftp"
#define INSTTYPNM_TELNET           "telnet"

#define CONTROL_CONN_NAME          "SESSIONCONTROL"

#define ACTNDB_EXECSEL 3

// List all supported connection headers
#include <CtrlLib/CtrlLib.h>
#include <Oracle/Oracle7.h>
#include <Oracle/Oracle8.h>
#include <MSSQL/MSSQL.h>
#include <PostgreSQL/PostgreSQL.h>

// Internal ids for speed of processing.  We don't use the control db key values since they
// are only for db relations.  There's no way to guarantee consistency across db<=>code constants
// except perhaps a db export to .DEF file that then is compiled in!

enum InstanceTypes { INSTTYP_UNDEF, INSTTYP_ORACLE, INSTTYP_POSTGRESQL, INSTTYP_MSSQLSERVER, INSTTYP_FTP
	, INSTTYP_TELNET, INSTTYP_TELFTP, INSTTYP_DB2, INSTTYP_SYBASE, INSTTYP_OUTLOOKMAIL
	, INSTTYP_OUTLOOKMEETING };
	
//==========================================================================================
class Connection : Moveable<Connection>, public Sql {
public:
	typedef Connection CLASSNAME;
	friend class ConnectionFactory;
	EnumConnState enumConnState;  // Track the connection state
	Vector<Callback> WhenChange;
	One<SqlSession> session; // The meat.
	int connId; // Needed to pass on to cmdScript and TestGrid so they can store the id with new objects
	String connName; // Needed so main thread can update the title of the TopWindow
	String instanceTypeName; // Needed to determine how to enumerate.  Oracle7 needs help.
	InstanceTypes instanceType; // Not same as insttypid in db.
	String loginStr; // Needed to reconnect or change password
	String loginPwd; // Needed to reconnect or change password
	String instanceAddress;
	String dbName; // Can be used complete SQL that doesn' set the default dtabase
	Thread connectThread;
	TopWindow *topWindow;

	// Set to 1 to cancel any activity against the connection that is cancellable.  Ignored
	// if nothing active.
	volatile Atomic cancelAnyActiveStatements;	
	// Lock any connection information update or read using this mutex
	Mutex connectInfoUpdating;
		String connectErrorMessage;  // If connection fails, this is updated with the info (No access to GUI in thread) within Mutex
		
	// Self-locking flag to interface between the ConnectThread and Connect method
	volatile Atomic connectThreadStatus;
	enum ConnectThreadStatus {CONNECTSTATUS_UNDEF, CONNECTSTATUS_KILL, CONNECTSTATUS_NOTENOUGHINFO, CONNECTSTATUS_SUCCEED, CONNECTSTATUS_FAIL, CONNECTSTATUS_UNKNOWN };
	
protected:	
	//==========================================================================================
	// Use the ConnectionFactory.Connect to create a new connection.
	Connection();

	//==========================================================================================	
	// Called from within OCI8Connection in Oci8.cpp Execute() or Fetch() function during
	// OCI_STILL_EXECUTING status.
	void WhenStillExecuting();
	
	//==========================================================================================	
	bool Connect(TopWindow *ptopWindow);
	
	//==========================================================================================	
	// postcondition: Sets enumConnState, instType
	// precondition: Connects strictly from ConnectFactory
	// TopWindow is currently only supported by Oracle driver (I added :))
	void ConnectThread(TopWindow *topWindow);

public:
	//==========================================================================================
	SqlSession &GetSession();
	
	//==========================================================================================
	String PrepTextDataForSend(const String &textData);

	//==========================================================================================
	// Oracle breaks with odd aposts in line comments
	String PrepScriptForSend(const String &script);

	//==========================================================================================
	// This is a prep for actual execution by the receiving instance, not for insertion into 
	// the control db as a saved script text.
	String PrepOracleScriptForSend(const String &script);
	//==========================================================================================
	//  Wrap script method implementation and error handling.  Will allow reconnects and reexecutes.
	bool SendQueryDataScript(const char *sqlText, bool silent = false);
	//==========================================================================================
	bool SendAddDataScript(const char *sqlText, bool silent = false);
	
	//==========================================================================================
	bool SendChangeDataScript(const char *sqlText, bool silent = false);
	
	//==========================================================================================
	bool SendRemoveDataScript(const char *sqlText, bool silent = false);
	
	//==========================================================================================
	bool SendChangeEnvScript(const char *sqlText, bool silent = false);
	
	//==========================================================================================
	bool SendQueryEnvScript(const char *sqlText, bool silent = false);
	//==========================================================================================
	bool SendChangeStructureScript(const char *sqlText, bool silent = false);
	//==========================================================================================	
	int GetInsertedId(String tableName, String columnName);
	
	//==========================================================================================	
	// Only works for PostgreSQL.
	int GetPostgreSQLInsertedId(String tableName, String columnName);

	Value Get(SqlId i) const;

	// PostgreSQL driver does not 
	bool GetBool(int i) const;
	
	Value Get(int i) const;

	bool HandleDbError(int actioncode, String *cmd = NULL);
};

//==========================================================================================	
//  Use a connection factory to get our connections.  VectorMap had to be function, not an
//  actual variable, got a link error, inorder to be static.  See http://www.ultimatepp.org/forum/index.php?t=msg&goto=26714&
//  I use pointers because I didn't want to write a Deep copy constructor for the connections.
//==========================================================================================	

class ConnectionFactory {
public:
	static Connection *controlConnection;
	static VectorMap<String,Connection *>& Connections();
	
	ConnectionFactory(Connection *pcontrolConnection = NULL);
	
	~ConnectionFactory();
	
	static String ControlInstanceType();
	Connection *Connect(TopWindow *win, int connId, Connection *pcontrolConnection = NULL);
	
	// Connection factory (Takes window for async connection spinning.
	// Assumption: connName is unique per connection.  No support for multiple connections per connection definition
	Connection *Connect(TopWindow *win, String connName, String instanceTypeName
		, String loginStr, String loginPwd, String instanceAddress, String dbName = Null);
};

#endif
