#ifndef _Connection_h_
#define _Connection_h_

#include "shared.h"
#include "ConnState.h"
#include "ContextMacros.h"
#include "SoundHandler.h"
#include "LogWin.h"
#include "JobSpec.h"
#include "lister/ODBC/ODBC.h"

#define INSTTYPNM_POSTGRESQL       "PostgreSQL"
#define INSTTYPNM_ORACLE           "Oracle"
#define INSTTYPNM_MSSQLSERVER      "MS SQL"
#define INSTTYPNM_DB2              "DB2"
#define INSTTYPNM_SYBASE           "Sybase"
#define INSTTYPNM_FTP              "ftp"
#define INSTTYPNM_TELNET           "telnet"

#define CONTROL_CONN_NAME          "SESSIONCONTROL"

#define ACTNDB_EXECSEL 3

// List all supported connection headers
#include <CtrlLib/CtrlLib.h>
#include "Script.h"

#define RUN_SILENT  true
#define EXPAND_MACROS true

// http://www.sybase.com/ase_1500devel; downloaded ASE 15.5 Win x86 2/23/11

// Internal ids for speed of processing.  We don't use the control db key values since they
// are only for db relations.  There's no way to guarantee consistency across db<=>code constants
// except perhaps a db export to .DEF file that then is compiled in!

enum InstanceTypes { 
		INSTTYP_UNDEF, INSTTYP_ORACLE, INSTTYP_POSTGRESQL, INSTTYP_MSSQLSERVER, INSTTYP_FTP
	,	INSTTYP_TELNET, INSTTYP_TELFTP, INSTTYP_DB2, INSTTYP_SYBASE, INSTTYP_OUTLOOKMAIL
	,	INSTTYP_OUTLOOKMEETING 
	};
	
//==============================================================================================
class Connection : Moveable<Connection>, public Sql {
public:
	typedef Connection CLASSNAME;
	friend class ConnectionFactory;
	Connection                  *controlConnection;
	EnumConnState                enumConnState;  // Track the connection state
	Vector<Callback>             WhenChange;
	One<SqlSession>              session; // The meat.
	int                          connId; // Needed to pass on to scriptEditor and TestGrid so they can store the id with new objects
	String                       connName; // Needed so main thread can update the title of the TopWindow
	String                       instanceTypeName; // Needed to determine how to enumerate.  Oracle7 needs help.
	InstanceTypes                instanceType; // Not same as insttypid in db.
	String                       loginStr; // Needed to reconnect or change password
	String                       loginPwd; // Needed to reconnect or change password
	String                       instanceAddress;
	String                       dbName; // Can be used complete SQL that doesn' set the default dtabase
	String                       portNo;
	String                       envLetter; // Letter that gets stuffed in as [[ENV]] when active connection changes
	bool                         isOSAuth;
	bool                         isAClone;
	Connection                  *clonedFrom;
	Thread                       connectThread;
	TopWindow                   *topWindow;
    bool                         informationalOnly; // Not actually connected; just informational
	// Set to 1 to cancel any activity against the connection that is cancellable.  Ignored
	// if nothing active.
	volatile Atomic              cancelAnyActiveStatements;	
	// Lock any connection information update or read using this mutex
	Mutex                        connectInfoUpdating;
	// If connection fails, this is updated with the info (No access to GUI in thread) within Mutex
	String                       connectErrorMessage;  
		
	// Self-locking flag to interface between the ConnectThread and Connect method
	volatile Atomic              connectThreadStatus;
	enum ConnectThreadStatus {
			CONNECTSTATUS_UNDEF, CONNECTSTATUS_KILL, CONNECTSTATUS_NOTENOUGHINFO
		,	CONNECTSTATUS_SUCCEED, CONNECTSTATUS_FAIL, CONNECTSTATUS_UNKNOWN
	};
	
protected:	
	            // Use the ConnectionFactory.Connect to create a new connection. ConnectionFactoryCanCreate
	            Connection();
	            // Called from within OCI8Connection in Oci8.cpp Execute() or Fetch() function during
	            // OCI_STILL_EXECUTING status.
	void        WhenStillExecuting();
	bool        Connect(TopWindow *ptopWindow);
	            // postcondition: Sets enumConnState, instType
	            // precondition: Connects strictly from ConnectFactory
	            // TopWindow is currently only supported by Oracle driver (I added :))
	void        ConnectThread(TopWindow *topWindow);

public:
	            Connection(Connection *conn);
	SqlSession &GetSession();
	String      PrepTextDataForSend(const String &textData);
	            // Oracle breaks with odd aposts in line comments
	String      PrepScriptForSend(const String &script, bool log = false);
	            // This is a prep for actual execution by the receiving instance, not for insertion into 
	            // the control db as a saved script text.
	String      PrepOracleScriptForSend(const String script, bool log = false);
	String      PrepForPostgresCopyFrom(const String scriptText);
	            //  Wrap script method implementation and error handling.  Will allow reconnects and reexecutes.
	bool        ProcessQueryDataScript(Script &sob, JobSpec &jspec, ContextMacros *contextMacros);
	bool        SendQueryDataScript(const char *scriptText, ContextMacros *contextMacros = NULL, bool silent = false, bool expandMacros = false, bool log = false);
	bool        SendAddDataScript(const char *sqlText, ContextMacros *contextMacros = NULL, bool silent = false, bool expandMacros = false);
	bool        SendChangeDataScript(const char *sqlText, ContextMacros *contextMacros = NULL, bool silent = false, bool expandMacros = false);
	bool        SendRemoveDataScript(const char *sqlText, ContextMacros *contextMacros = NULL, bool silent = false, bool expandMacros = false);
	bool        SendChangeEnvScript(const char *sqlText, ContextMacros *contextMacros = NULL, bool silent = false, bool expandMacros = false);
	bool        SendQueryEnvScript(const char *sqlText, ContextMacros *contextMacros = NULL, bool silent = false, bool expandMacros = false);
	bool        SendChangeStructureScript(const char *sqlText, ContextMacros *contextMacros = NULL, bool silent = false, bool expandMacros = false);
	int         GetInsertedId(String tableName, String columnName);
	            // Only works for PostgreSQL.
	int         GetPostgreSQLInsertedId(String tableName, String columnName);
	int         GetPostgreSQLInsertedId(String sequenceName);
	Value       Get(SqlId i) const;
	            // PostgreSQL driver does not properly deal with bool
	bool        GetBool(int i) const;
	Value       Get(int i) const;
	bool        HandleDbError(int actioncode, String *cmd = NULL, bool log = false, bool batchMode = false);
};

//==============================================================================================
//  Use a connection factory to get our connections.  VectorMap had to be function, not an
//  actual variable, got a link error, inorder to be static.  See http://www.ultimatepp.org/forum/index.php?t=msg&goto=26714&
//  I use pointers because I didn't want to write a Deep copy constructor for the connections.
//==============================================================================================
class ConnectionFactory {
public:
	static Connection *controlConnection;

	static VectorMap<String,Connection *>& Connections();
	
	              ConnectionFactory(Connection *pcontrolConnection = NULL);
	             ~ConnectionFactory();
	static String ControlInstanceType();
	              // Warning: This does not store the connection in Connections pool since it is informational only
	              // and I don't want Connect searches to get confused with informationals.  Could get nasty.
	              // So remember to destroy.
	Connection   *FetchConnInfo(int connId, Connection *pcontrolConnection = NULL); // No connect; just lookup detail in database!
	Connection   *GetConnection(String connName); // Fetch a connection if it allready exists and is open, else return null
	Connection   *Connect(TopWindow *win, int connId, bool useIfFoundInPool = false, Connection *pcontrolConnection = NULL);
	              // Connection factory (Takes window for async connection spinning.
	              // Assumption: connName is unique per connection.  No support for multiple connections per connection definition
	Connection   *Connect(
		TopWindow *win, String connName, String instanceTypeName
	,	String loginStr, String loginPwd, String instanceAddress, String dbName = Null, String portNo = Null, bool isOSAuth = false, bool log = false);
};

#endif
