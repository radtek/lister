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
	Connection() {
		connId = UNKNOWN;
		connName = Null;	
		instanceTypeName = Null;
		instanceType = INSTTYP_UNDEF;	
		enumConnState = NOCON_UNDEF;
		session = NULL;
		topWindow = NULL;
		cancelAnyActiveStatements = 0; // Clear any requests
		connectErrorMessage = "";
	}

	//==========================================================================================	
	// Called from within OCI8Connection in Oci8.cpp Execute() or Fetch() function during
	// OCI_STILL_EXECUTING status.
	void WhenStillExecuting() {
		if (topWindow) {
			topWindow->ProcessEvents();
			// No sleep function here; let Oci8.cpp sleep
			if (cancelAnyActiveStatements == 1) {
				Cancel();
			}
		}
		
		// TODO: Check if user requested to cancel this run.  if so OCIBreak/OCICancel
	}
	
	//==========================================================================================	
	bool Connect(TopWindow *ptopWindow) {
		topWindow = ptopWindow;
		connectThreadStatus = CONNECTSTATUS_UNDEF;
		connectThread.Run(THISBACK1(ConnectThread, topWindow));
		while (connectThreadStatus == CONNECTSTATUS_UNDEF) {
			if (topWindow) {
				topWindow->ProcessEvents();
			}
			// TODO: Can connections be interrupted?
			Sleep(10);
		}
		
		bool status = (connectThreadStatus == CONNECTSTATUS_SUCCEED);

		if (status) {
			ASSERT(session);
			Assign(*session);  // FYI: Session = SqlSource, which has CreateConnection, which a sql object is really just a wrapper for "cn"
			this->SetWhenStillExecuting(THISBACK(WhenStillExecuting));
		}
		
		return status;
	}
	
	//==========================================================================================	
	// postcondition: Sets enumConnState, instType
	// precondition: Connects strictly from ConnectFactory
	// TopWindow is currently only supported by Oracle driver (I added :))
	void ConnectThread(TopWindow *topWindow) {
		String connStr;
		bool connected = false;

		//———————————————————————————————————————————————————————————————————————————————————————— 
		if (instanceTypeName == INSTTYPNM_ORACLE) {
			instanceType = INSTTYP_ORACLE;
			
			connStr 
				<< loginStr << "/" 
				<< loginPwd << "@" 
				<< instanceAddress;

			One<Oracle8> attemptingsession = new Oracle8; // Altered to work with bug in 10.2.0.1 with TIMESTAMP size

			connected = attemptingsession->Open(connStr, NULL /* no objects (yet) */);
			session = -attemptingsession;
			
			// Tell the Execute()/Fetch() to call the main event window every so often to prevent GUI freeze
//			((OCI8Connection *)this)->SetTopWindow(topWindow);

		//———————————————————————————————————————————————————————————————————————————————————————— 
		} else if (instanceTypeName == INSTTYPNM_POSTGRESQL) {
			instanceType = INSTTYP_POSTGRESQL;
			connStr 
				<< " host="			<< instanceAddress 
				<< " dbname="		<< dbName 
				<< " user="			<< loginStr 
				<< " password="		<< loginPwd;
				
			One<PostgreSQLSession> attemptingsession = new PostgreSQLSession; // Currently testing with 8.4 and 9.

			connected = attemptingsession->Open(connStr);
			session = -attemptingsession; // Pick?

		//———————————————————————————————————————————————————————————————————————————————————————— 
		} else if (instanceTypeName == INSTTYPNM_MSSQLSERVER) {
			instanceType = INSTTYP_MSSQLSERVER;
			connStr 
				<< "Driver={SQL Server};"
				<< "Server=" 	<< instanceAddress	<< ";"
				<< "UID="		<< loginStr			<< ";"
				<< "PWD="		<< loginPwd			<< ";"
				// "Database="	<< dbName			<< ";"
				// Trusted_Connection=Yes;
				;
				
			One<MSSQLSession> attemptingsession = new MSSQLSession;
			
			connected = attemptingsession->Connect(connStr);
			session = -attemptingsession;
		
		//———————————————————————————————————————————————————————————————————————————————————————— 
		} else if (instanceTypeName == INSTTYPNM_FTP) {
			instanceType = INSTTYP_FTP;
			
			Exclamation(CAT << "Error: FTP not implemented");
			session = NULL;
			
			// FTP
			// C:\upp\uppsrc\plugin\ftp\ftp.cpp(44): if(!FtpConnect(host, &ftpconn, perror, &FtpClient::Callback, this, 200, idletimeout_secs)) {
		
		//———————————————————————————————————————————————————————————————————————————————————————— 
		} else if (instanceTypeName == INSTTYPNM_TELNET) {
			instanceType = INSTTYP_TELNET;
			
			Exclamation(CAT << "Error: TELNET not implemented");
			session = NULL;

		//———————————————————————————————————————————————————————————————————————————————————————— 
		} else {
			Exclamation(CAT << "Error: Unrecognized instance type: " << instanceTypeName);
			enumConnState = NOCON_MISCONFIG;
			session = NULL;
			connectThreadStatus = CONNECTSTATUS_NOTENOUGHINFO;
			return;
		}

		if (!connected) {	
			connectInfoUpdating.Enter();
			connectErrorMessage = session->GetLastError();
			connectInfoUpdating.Leave();
			enumConnState = CON_FAIL;
			delete -session;
			session = NULL;
		} else {
			enumConnState = CON_SUCCEED;
		}

		if (connected) {
			connectThreadStatus = CONNECTSTATUS_SUCCEED;
		} else {
			connectThreadStatus = CONNECTSTATUS_FAIL;
		}
		return;
	}

public:
	//==========================================================================================
	SqlSession &GetSession() {
		return *session;
	}
	
	//==========================================================================================
	String PrepTextDataForSend(const String &textData) {
		switch (instanceType) {
			case INSTTYP_POSTGRESQL:
				return UrpString::ReplaceInWhatWith(UrpString::ReplaceInWhatWith(textData, "\\", "\\\\"), "'", "\\'");	
				break;
			case INSTTYP_ORACLE:
				return UrpString::ReplaceInWhatWith(textData, "'", "''");	
				break;
			default:
				return textData;
		}
	}

	//==========================================================================================
	// Oracle breaks with odd aposts in line comments
	String PrepScriptForSend(const String &script) {
		switch (instanceType) {
			case INSTTYP_ORACLE:
				return PrepOracleScriptForSend(script);
				break;
			default:
				return script;
		}
	}

	//==========================================================================================
	// This is a prep for actual execution by the receiving instance, not for insertion into 
	// the control db as a saved script text.
	String PrepOracleScriptForSend(const String &script) {
		ASSERT(instanceType == INSTTYP_ORACLE);
		
		// Odd number of apostrophes in a line comment before it terminates breaks Oracle's processor
		// ex:   " select * from x where John = '--married--' -- John's maiden name is Jake

		const char *s = script;
		const char *first_s = s;
		
		bool inLineComment = false;
		bool inTextDataSegment = false;
		bool inMultiLineComment = false;
		bool firstCharStartingBlockFound = false;
		bool oddApostFound = false;
		int lastApostFoundAt = 0;
		VectorMap<int, int> insertCharAt; // Will have to keep incrementing as we insert, or insert from end, yeah, that's better
		VectorMap<int, int> replaceCharAt;
		
		while(*s) {
			int c = *s;
			// ISO Latin-1 codes: 34=quotation mark, 39=apostrophe, 8216=left single curly quote, 8217=right single curly quote, 
			if (In(c, '`', 8216, 8217)) {
				replaceCharAt.Add((s - first_s), '\'');
			} else if (c < 0) {
				String unicode_c = FormatIntHex(c);
				// This character was detected in a pasted string, possibly a Qtf code?  Not sure.  Has to be stripped
				// or it will cause an error when OciParse is called on it.
				// Update:  These turned out to be some freaky unicode underline in "cr_staging". halfwidth underscore = U+005F aka underbar, understrike, low line, low dash
				if (In(unicode_c, "ffffffc2", "ffffffa0")) { // A0 = No-Break Space?? http://www.utf8-chartable.de/  These may be private characters
					replaceCharAt.Add((s - first_s), ' ');
				}
			}
			
			int nextc = *(s + 1);
			int prevc;
			if (s != first_s) {
				prevc = *(s - 1);
			} else {
				prevc = 0;
			}
			
			s++;
			
			if (None(inLineComment, inTextDataSegment, inMultiLineComment)) {
				
				if (prevc == '-' && nextc == '-') {
					inLineComment = true;
				} else 
				if (prevc == '/' && c == '*') {
					inMultiLineComment = true;
				} else
				if (prevc == '\'' && c != '\'') {
					inTextDataSegment = true;
				} else 
				if (prevc == '\'' && c == '\'') {
					inTextDataSegment = true;
				}
			} else
			if (inLineComment) {
				if (c == '\'') {
					oddApostFound = !oddApostFound;
					lastApostFoundAt = (s - first_s) - 1; // adjust for the autoinc up there
				} else 
				if (c == '\n' || nextc == 0) {
					inLineComment = false;
					if (oddApostFound) {
						insertCharAt.Add(lastApostFoundAt+1, '\'');
						oddApostFound = false;
					}
				}
			} else
			if (inTextDataSegment) {
				if (prevc != '\'' && c == '\'') {
					inTextDataSegment = false;
				}
			} else
			if (inMultiLineComment) {
				if (prevc == '*' && c == '/') {
					inMultiLineComment = false;
				}
			}
		}
		
		bool newScriptCreated = false;
		
		String newScript;

		// Replace any oddball grave accents, left/right apostrophes. 
				
		if (replaceCharAt.GetCount()) {
			if (!newScriptCreated) {
				newScript = script;
				newScriptCreated = true;
			}
			
			for (int i = replaceCharAt.GetCount() - 1;  i >= 0; i--) {
				int oldcharat = replaceCharAt.GetKey(i);
				int newchar = replaceCharAt[i]; // Doh!  Have to use key value, not index!!!
				newScript.Set(oldcharat, newchar);
			}
		}

		// Now patch up any odd commented aposts
		// Go backwards so as to avoid messing up indexes

		// This function hoses all saved positions
				    
		if (insertCharAt.GetCount()) {
			if (!newScriptCreated) {
				newScript = script;
				newScriptCreated = true;
			}
			
			for (int i = insertCharAt.GetCount() - 1;  i >= 0; i--) {
				int insertat = insertCharAt.GetKey(i);
				int newchar = insertCharAt.Get(i);
				newScript.Insert(insertat, newchar);
			}
		}

		if (newScriptCreated) {
			return newScript;
		} else {
			return script;
		}
	}

	//==========================================================================================
	//  Wrap script method implementation and error handling.  Will allow reconnects and reexecutes.
	bool SendQueryDataScript(const char *sqlText, bool silent = false) {
		ClearError();
		
	retryQuery:
	
		// We have to prep some scripts where embedded codes break the processor
		
		if (!Execute(PrepScriptForSend(sqlText))) {
			if (GetErrorCode() == 12571) {
				// Oracle TNS-Packet failure, happens after timeout
				// Should detect that we haven't run a script for a while and have probably timed out
				if (PromptYesNo("Detected connection timeout, reconnect?")) {
					delete -session;
					Connect(topWindow);
					goto retryQuery;
				} else {
					delete -session;
				}
				return false;
			} else {
				if (silent) {
					LOG(CAT << "SQL Error: " << DeQtf(GetLastError()));
					return false;
				} else {
					PromptOK(Format("Error: [* %s].", DeQtf(GetLastError())));
					return false;
				}
			}
		}

		return true;
	}
	
	//==========================================================================================
	bool SendAddDataScript(const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sqlText, silent);
	}
	
	//==========================================================================================
	bool SendChangeDataScript(const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sqlText, silent);
	}
	
	//==========================================================================================
	bool SendRemoveDataScript(const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sqlText, silent);
	}
	
	//==========================================================================================
	bool SendChangeEnvScript(const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sqlText, silent);
	}
	
	//==========================================================================================
	bool SendQueryEnvScript(const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sqlText, silent);
	}
	
	//==========================================================================================
	bool SendChangeStructureScript(const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sqlText, silent);
	}

	//==========================================================================================	
	int GetInsertedId(String tableName, String columnName) {
		switch (instanceType) {
			case INSTTYP_POSTGRESQL:
				return GetPostgreSQLInsertedId(tableName, columnName);
				break;
			default:
				ASSERT(1==0);
		}
		
		return UNKNOWN;
	}
	
	//==========================================================================================	
	// Only works for PostgreSQL.
	int GetPostgreSQLInsertedId(String tableName, String columnName) {
				
		// PostgreSQL constructs a default sequence for serial columns
		String sequenceName = CAT << tableName << "_" << columnName << "_" << "seq";
		String script = CAT << "select currval('" << sequenceName << "')";
		if (!SendQueryDataScript(script)) { // ERROR: currval of sequence "connections_connid_seq" is not yet defined in this session
			return -1;
		}
		
		Fetch();
		return atoi((*this)[0].ToString());
	}

	//==========================================================================================	
	Value Get(SqlId i) const {
		Value v;
		Sql::GetColumn(i, v);
		return v;
	}

	//==========================================================================================	
	// PostgreSQL driver does not 
	bool GetBool(int i) const {
		Value v;
		Sql::GetColumn(i, v);

		if (v.GetType() == STRING_V) {
			return (v == "1");
		}
		
		return v;
	}
	
	//==========================================================================================	
	Value Get(int i) const {
		Value v;
		Sql::GetColumn(i, v);
		return v;
	}

//	//==========================================================================================	
//	Value GetV(int i) const {
//		Value v;
//		Sql::GetColumn(i, v);
//		return v;
//	}

	//==========================================================================================	
	bool HandleDbError(int actioncode, String *cmd = NULL) {
		
		String errcode = GetErrorCodeString();
		int myerrno = GetErrorCode();
		Sql::ERRORCLASS ec = GetErrorClass();
		String es = GetErrorStatement();
		// CONNECTION_BROKEN?
		if (ec == Sql::CONNECTION_BROKEN) {
			Exclamation("Connection broken");
			return false;
			//Reconnect, try again, but sql statement object will be broke.
			//Post a message to main window to reconnect.
		}
		
		if (myerrno == 25408) {
			Exclamation("Connection broken (could not replay:Oracle)");
			return false;
		}
		
		Exclamation(Format("Error: #=%d, [* \1%s\1].", myerrno, DeQtf(GetLastError())));
		return false;
	}
};

//==========================================================================================	
//  Use a connection factory to get our connections.  VectorMap had to be function, not an
//  actual variable, got a link error, inorder to be static.  See http://www.ultimatepp.org/forum/index.php?t=msg&goto=26714&
//  I use pointers because I didn't want to write a Deep copy constructor for the connections.
//==========================================================================================	

class ConnectionFactory {
public:
	static Connection *controlConnection;
	static VectorMap<String,Connection *>& Connections() {
		static VectorMap<String,Connection *> connections;
		return connections;
	}
	
	//==========================================================================================	
	ConnectionFactory(Connection *pcontrolConnection = NULL) {
		controlConnection = pcontrolConnection;
	}
	
	//==========================================================================================	
	~ConnectionFactory() {
		// Release all the connection objects to prevent a memory leak
		for (int i = 0; i < Connections().GetCount(); i++) {
			Connection *conn = Connections()[i];
			delete conn;
		}
	}
	
	//==========================================================================================	
	static String ControlInstanceType() {
		return INSTTYPNM_POSTGRESQL;
	}

	Connection *Connect(TopWindow *win, int connId, Connection *pcontrolConnection = NULL) {
		Connection *lcontrolConnection = NULL;
		
		// User passed a new control connection, we will use locally temporarily for this connection
		if (pcontrolConnection) lcontrolConnection = pcontrolConnection;
		
		// If they passed a new control connection and we don't have a shared controlconnection already established, we'll use the new one as a shared one
		if (lcontrolConnection && !controlConnection) controlConnection = lcontrolConnection;
		
		// If they didn't pass us a anew control connection then they must want to use the shared one
		if (!lcontrolConnection) lcontrolConnection = controlConnection;
		
		// If there wasn't a shared control connection set up, then we can't look up any other connection information from a control database
		if (!lcontrolConnection) return NULL;
		
		String FetchConnDtlById = Format("select "
				"ConnId"          // 0
			",	ConnName"         // 1
			",  LoginId"          // 2
			",  LoginStr"         // 3
			",  LoginPwd"         // 4
			",  InstanceId"       // 5
			",  InstanceName"     // 6
			",  InstanceAddress"  // 7
			",  InstTypID"        // 8
			",  InstTypName"      // 9
			",  EnvId"            // 10
			",  EnvStdName"       // 11
			",  dbName"           // 12
			" from v_conn where ConnId = %d", connId);
			
		if (!lcontrolConnection->SendQueryDataScript(FetchConnDtlById)) {
			return NULL;
		}
		
		lcontrolConnection->Fetch();

		String connName = lcontrolConnection->Get(1);
		String instanceTypeName = lcontrolConnection->Get(9);
		String loginStr = lcontrolConnection->Get(3);
		String loginPwd = lcontrolConnection->Get(4);
		String instanceAddress = lcontrolConnection->Get(7);
		String dbName = lcontrolConnection->Get(12);
		
		return Connect(win, connName, instanceTypeName, loginStr, loginPwd, instanceAddress, dbName);
	}
	
	//==========================================================================================	
	// Connection factory (Takes window for async connection spinning.
	// Assumption: connName is unique per connection.  No support for multiple connections per connection definition
	Connection *Connect(TopWindow *win, String connName, String instanceTypeName
		, String loginStr, String loginPwd, String instanceAddress, String dbName = Null) {
		
		Connection *connection = Connections().GetAdd(connName, new Connection());
		connection->instanceTypeName = instanceTypeName;
		connection->connName = connName;
		connection->loginStr = loginStr;
		connection->loginPwd = loginPwd; // For reconnecting, or changing password, you have to pass the old one
		connection->instanceAddress = instanceAddress;
		connection->dbName = dbName;
		
		// BUG: if connection information changed, it won't be representative of the actual connection
		
		if (!In(connection->enumConnState, CON_SUCCEED, CON_STALE)) {
			if (connection->Connect(win)) {
				Speak(EVS_CONNECT_SUCCEEDED);
			} else {
				Speak(EVS_CONNECT_FAILED);
			}
		}

		if (connName == CONTROL_CONN_NAME) {
			controlConnection = Connections().Get(connName);
			LOG("Set control Connection to " + connName + ", " + instanceTypeName + ", login=" + loginStr + ", addr=" + instanceAddress + ", db=" + dbName);
		}
		
		// Cycle through hooks
	
		return Connections().Get(connName);
	}
};

Connection *ConnectionFactory::controlConnection = NULL;

#endif

			//Microsoft SQL Server ODBC Driver Version 03.85.1132
			//SQL Native Client
			// iSeries Access ODBC Driver
			// Client Access ODBC Driver (32-bit)
			// Microsoft Excel Driver (*.xls)
			// Microsoft dBase Driver (*.dbf)
			// Microsoft Text Driver (*.txt; *.csv)
