#ifndef _Connection_h_
#define _Connection_h_

#include "shared.h"
#include "ConnState.h"
#include "insttypnms.h"
#include "SoundHandler.h"
#include "MiscUtilities.h"
#include "StringHandler.h"

// List all supported connection headers

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
class Connection : Moveable<Connection> {
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
	// TODO: Put this in a stack, create struct of status, start time, ect.
	
	// Lock any connection information update or read using this mutex
	Mutex connectInfoUpdating;
		String connectErrorMessage;  // If connection fails, this is updated with the info (No access to GUI in thread) within Mutex
		
	volatile Atomic connectThreadStatus;
	enum ConnectThreadStatus {CONNECTSTATUS_UNDEF, CONNECTSTATUS_KILL, CONNECTSTATUS_NOTENOUGHINFO, CONNECTSTATUS_SUCCEED, CONNECTSTATUS_FAIL, CONNECTSTATUS_UNKNOWN };
	
protected:	
	//==========================================================================================
	// Use the ConnectionFactory.Connect
	Connection() {
		connId = UNKNOWN;
		connName = Null;	
		instanceTypeName = Null;
		instanceType = INSTTYP_UNDEF;	
		enumConnState = NOCON_UNDEF;
		session = NULL;
	}

	//==========================================================================================	
	bool Connect(TopWindow *win) {
		connectThreadStatus = CONNECTSTATUS_UNDEF;
		connectThread.Run(THISBACK(ConnectThread));
		while (connectThreadStatus == CONNECTSTATUS_UNDEF) {
			win->ProcessEvents();
			Sleep(10);
		}
		
		bool status = (connectThreadStatus == CONNECTSTATUS_SUCCEED);
		return status;
	}
	
	//==========================================================================================	
	// postcondition: Sets enumConnState, instType
	// precondition: Connects strictly from ConnectFactory
	void ConnectThread() {
		String connStr;
		bool connected = false;

		//———————————————————————————————————————————————————————————————————————————————————————— 
		if (instanceTypeName == INSTTYPNM_ORACLE) {
			instanceType = INSTTYP_ORACLE;
			
			connStr 
				<< loginStr << "/" << loginPwd << "@" << instanceAddress;

			One<Oracle8> attemptingsession = new Oracle8; // Altered to work with bug in 10.2.0.1 with TIMESTAMP size
			
			connected = attemptingsession->Open(connStr, NULL /* no objects (yet) */);
			session = -attemptingsession;

		//———————————————————————————————————————————————————————————————————————————————————————— 
		} else if (instanceTypeName == INSTTYPNM_POSTGRESQL) {
			instanceType = INSTTYP_POSTGRESQL;
			connStr 
				<< "host=" << instanceAddress 
				<< " dbname=" << dbName 
				<< " user=" << loginStr 
				<< " password=" << loginPwd;
				
			One<PostgreSQLSession> attemptingsession = new PostgreSQLSession; // Currently testing with 8.4 and 9.

			connected = attemptingsession->Open(connStr);
			session = -attemptingsession; // Pick?

		//———————————————————————————————————————————————————————————————————————————————————————— 
		} else if (instanceTypeName == INSTTYPNM_MSSQLSERVER) {
			instanceType = INSTTYP_MSSQLSERVER;
			connStr 
				<< "Driver={SQL Server};"  // 
				<< "Server=" << instanceAddress << ";"
				<< "UID=" << loginStr << ";"
				<< "PWD=" << loginPwd << ";"
				// "Database=" << ?
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
	// 	
	String PrepTextDataForSend(const String &textData) {
		switch (instanceType) {
			case INSTTYP_POSTGRESQL:
				return ReplaceStr(ReplaceStr(textData, "\\", "\\\\"), "'", "\\'");	
				break;
			case INSTTYP_ORACLE:
				return ReplaceStr(textData, "'", "''");	
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
		Vector<int> insertApostAt; // Will have to keep incrementing as we insert, or insert from end, yeah, that's better
		
		while(*s) {
			int c = *s;
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
						insertApostAt.Add(lastApostFoundAt+1);
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
		
		// Now patch up any odd commented aposts
		// Go backwards so as to avoid messing up indexes
		
		if (insertApostAt.GetCount()) {
			String newScript = script;
			for (int i = insertApostAt.GetCount() - 1;  i >= 0; i--) {
				newScript.Insert(i, '\'');
			}
			return newScript;
		}
		
		return script;
	}

	//==========================================================================================
	//  Wrap script method implementation and error handling.  Will allow reconnects and reexecutes.
	bool SendQueryDataScript(Sql &sql, const char *sqlText, bool silent = false) {
		
		// We have to prep some scripts where embedded codes break the processor
		if (!sql.Execute(PrepScriptForSend(sqlText))) {
			if (silent) {
				LOG(CAT << "SQL Error: " << DeQtf(sql.GetLastError()));
				return false;
			} else {
				Exclamation(Format("Error: [* \1%s\1].", DeQtf(sql.GetLastError())));
				return false;
			}
		}
		return true;
	}
	
	//==========================================================================================
	bool SendAddDataScript(Sql &sql, const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sql, sqlText, silent);
	}
	
	//==========================================================================================
	bool SendChangeDataScript(Sql &sql, const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sql, sqlText, silent);
	}
	
	//==========================================================================================
	bool SendDeleteDataScript(Sql &sql, const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sql, sqlText, silent);
	}
	
	//==========================================================================================
	bool SendChangeEnvScript(Sql &sql, const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sql, sqlText, silent);
	}
	
	//==========================================================================================
	bool SendQueryEnvScript(Sql &sql, const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sql, sqlText, silent);
	}
	
	//==========================================================================================
	bool SendChangeStructureScript(Sql &sql, const char *sqlText, bool silent = false) {
		return SendQueryDataScript(sql, sqlText, silent);
	}

	//==========================================================================================	
	int GetInsertedId(Sql &sql, String tableName, String columnName) {
		switch (instanceType) {
			case INSTTYP_POSTGRESQL:
				return GetPostgreSQLInsertedId(sql, tableName, columnName);
				break;
			default:
				ASSERT(1==0);
		}
		
		return UNKNOWN;
	}
	
	//==========================================================================================	
	// Only works for PostgreSQL.
	int GetPostgreSQLInsertedId(Sql &sql, String tableName, String columnName) {
				
		// PostgreSQL constructs a default sequence for serial columns
		String sequenceName = CAT << tableName << "_" << columnName << "_" << "seq";
		String cmd = CAT << "select currval('" << sequenceName << "')";
		if (!SendQueryDataScript(sql, cmd)) { // ERROR: currval of sequence "connections_connid_seq" is not yet defined in this session
			return -1;
		}
		
		sql.Fetch();
		return atoi(sql[0].ToString());
	}
};

//==========================================================================================	
//  Use a connection factory to get our connections.  VectorMap had to be function, not an
//  actual variable, got a link error, inorder to be static.  See http://www.ultimatepp.org/forum/index.php?t=msg&goto=26714&
//  I use pointers because I didn't want to write a Deep copy constructor for the connections.
//==========================================================================================	

class ConnectionFactory {
public:
	static VectorMap<String,Connection *>& Connections() {
		static VectorMap<String,Connection *> connections;
		return connections;
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
	// Connection factory (Takes window for async connection spinning.
	Connection *Connect(TopWindow *win, String connName, String instanceTypeName, String loginStr, String loginPwd, String instanceAddress, String dbName = Null) {
		
		Connection *connection = Connections().GetAdd(connName, new Connection());
		connection->instanceTypeName = instanceTypeName;
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

		// Cycle through hooks
	
	
		return Connections().Get(connName);
	}
};

#endif

			//Microsoft SQL Server ODBC Driver Version 03.85.1132
			//SQL Native Client
			// iSeries Access ODBC Driver
			// Client Access ODBC Driver (32-bit)
			// Microsoft Excel Driver (*.xls)
			// Microsoft dBase Driver (*.dbf)
			// Microsoft Text Driver (*.txt; *.csv)
