#include "Connection.h"
#include "CursorHandler.h"
#include "Script.h"
#include "LogWin.h"
#include "MacroHandler.h"
#include "ContextMacros.h" // ExpandMacros

#include <lister/Oracle/Oracle7.h>
#include <lister/Oracle/Oracle8.h>
#include <lister/MSSQL/MSSQL.h>
#include <lister/PostgreSQL/PostgreSQL.h>
#include <lister/ODBC/ODBC.h>
#include <lister/Urp/UrpInput.h>

//==============================================================================================
Connection::Connection() {
	connId                      = UNKNOWN;
	connName                    = Null;	
	instanceTypeName            = Null;
	instanceType                = INSTTYP_UNDEF;	
	enumConnState               = NOCON_UNDEF;
	session                     = NULL;
	topWindow                   = NULL;
	cancelAnyActiveStatements   = 0; // Clear any requests
	connectErrorMessage         = "";
}

//==============================================================================================
// Called from within OCI8Connection in Oci8.cpp Execute() or Fetch() function during
// OCI_STILL_EXECUTING status.
void Connection::WhenStillExecuting() {
	if (topWindow) {
		topWindow->ProcessEvents();
		// No sleep function here; let Oci8.cpp sleep
		if (cancelAnyActiveStatements == 1) {
			Cancel();
		}
	}
	
	// TODO: Check if user requested to cancel this run.  if so OCIBreak/OCICancel
}

//==============================================================================================
bool Connection::Connect(TopWindow *ptopWindow) {
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

//==============================================================================================
// postcondition: Sets enumConnState, instType
// precondition: Connects strictly from ConnectFactory
// TopWindow is currently only supported by Oracle driver (I added :))
void Connection::ConnectThread(TopWindow *topWindow) {
	String connStr;
	bool connected = false;

	//__________________________________________________________________________________________
	if (instanceTypeName == INSTTYPNM_ORACLE) {
		instanceType = INSTTYP_ORACLE;
		
		connStr 
			<< loginStr << "/" 
			<< loginPwd << "@" 
			<< instanceAddress;

		One<Oracle8> attemptingsession = new Oracle8; // Altered to work with bug in 10.2.0.1 with TIMESTAMP size

		connected = attemptingsession->Open(connStr, NULL /* no objects (yet) */);
		session = -attemptingsession;
		
		if (!connected) {
			if (session->GetErrorCode() == 28001) {
				// Oracle password expired
				String newPassword;
				
				if (UrpInputBox(
					newPassword
				,	"Enter new password"
				,	CAT << "Password of " << loginPwd << " expired for user " << loginStr << ". Enter new password.")
				) 
				{
					// Call OCIPassworld
				}
			}
		}
		
		// Tell the Execute()/Fetch() to call the main event window every so often to prevent GUI freeze
//			((OCI8Connection *)this)->SetTopWindow(topWindow);

	//__________________________________________________________________________________________
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

	//__________________________________________________________________________________________
	} else if (instanceTypeName == INSTTYPNM_MSSQLSERVER) {
		instanceType = INSTTYP_MSSQLSERVER;
		connStr 
//			<< "Driver={SQL Server};" // Driver works
			<< "Driver="	<< "{SQL Native Client}"	<< ";"
//			<< "Driver="	<< "{SQL Server Native Client 10.0}"	<< ";"
			<< "Server=" 	<< instanceAddress			<< ";"
			<< "UID="		<< loginStr					<< ";"
			<< "PWD="		<< loginPwd					<< ";"
			<< "Database="	<< dbName					<< ";"
			// Trusted_Connection=Yes;
			;
			
		One<MSSQLSession> attemptingsession = new MSSQLSession;
		
		//SQLExecDirect(hstmt, "select * from authors; select * from titles", SQL_NTS);'
		//Excess use of SQLBindCol to bind a result set column to a program variable is expensive because SQLBindCol causes an ODBC driver to allocate memory
		//Applications can use SQLGetData to retrieve data on a column-by-column basis, instead of binding the result set columns using SQLBindCol. If a result set contains only a couple of rows, then using SQLGetData instead of SQLBindCol is faster, otherwise, SQLBindCol gives the best performance. If an application does not always put the data in the same set of variables, it should use SQLGetData instead of constantly rebinding. Applications can only use SQLGetData on columns that are in the select list after all columns are bound with SQLBindCol. The column must also appear after any columns on which the application has already used a SQLGetData.
		//All statements executed in a stored procedure, including SELECT statements, generate an "x rows affected" message. Issuing a SET NOCOUNT ON at the start of a large stored procedure can significantly reduce the network traffic between the server and client and improve performance by eliminating these messages. These messages are typically not needed by the application when it is executing a stored procedure.
		//the application can also use SQLExtendedFetch to retrieve multiple rows at a time from the network buffers.
		//In a static cursor, the complete result set is built when the cursor is opened.
		//When using server cursors, each call to SQLFetch, SQLExtendedFetch, or SQLSetPos causes a network roundtrip from the client to the server. All cursor statements must be transmitted to the server because the cursor is actually implemented on the server.
		
//		SQLAllocEnv(&henv):
//		SQLAllocConnect(henv, &hdbc);
//		SQLAllocStmt(hdbc, &hstmt1);
//		SQLAllocStmt(hdbc, &hstmt2);
//		SQLSetConnectOption(hdbc, SQL_CURSOR_TYPE, SQL_CURSOR_DYNAMIC);
//		SQLSetConnectOption(hdbc, SQL_ROWSET_SIZE, 5);
//		SQLExecDirect(hstmt1, "select * from authors", SQL_NTS);


		connected = attemptingsession->Connect(connStr);
		session = -attemptingsession;
	
	//__________________________________________________________________________________________
	} else if (instanceTypeName == INSTTYPNM_SYBASE) {
		instanceType = INSTTYP_SYBASE;
		connStr 
			<< "Driver="	<< "{Adaptive Server Enterprise}"	<< ";"
			<< "Server=" 	<< instanceAddress			<< ";"
			<< "UID="		<< loginStr					<< ";"
			<< "PWD="		<< loginPwd					<< ";"
			<< "Database="	<< dbName					<< ";"
			<< "Port="		<< portNo 					<< ";"
			// Trusted_Connection=Yes;
			;
			
		One<MSSQLSession> attemptingsession = new MSSQLSession;
		
		//SQLExecDirect(hstmt, "select * from authors; select * from titles", SQL_NTS);'
		//Excess use of SQLBindCol to bind a result set column to a program variable is expensive because SQLBindCol causes an ODBC driver to allocate memory
		//Applications can use SQLGetData to retrieve data on a column-by-column basis, instead of binding the result set columns using SQLBindCol. If a result set contains only a couple of rows, then using SQLGetData instead of SQLBindCol is faster, otherwise, SQLBindCol gives the best performance. If an application does not always put the data in the same set of variables, it should use SQLGetData instead of constantly rebinding. Applications can only use SQLGetData on columns that are in the select list after all columns are bound with SQLBindCol. The column must also appear after any columns on which the application has already used a SQLGetData.
		//All statements executed in a stored procedure, including SELECT statements, generate an "x rows affected" message. Issuing a SET NOCOUNT ON at the start of a large stored procedure can significantly reduce the network traffic between the server and client and improve performance by eliminating these messages. These messages are typically not needed by the application when it is executing a stored procedure.
		//the application can also use SQLExtendedFetch to retrieve multiple rows at a time from the network buffers.
		//In a static cursor, the complete result set is built when the cursor is opened.
		//When using server cursors, each call to SQLFetch, SQLExtendedFetch, or SQLSetPos causes a network roundtrip from the client to the server. All cursor statements must be transmitted to the server because the cursor is actually implemented on the server.
		
//		SQLAllocEnv(&henv):
//		SQLAllocConnect(henv, &hdbc);
//		SQLAllocStmt(hdbc, &hstmt1);
//		SQLAllocStmt(hdbc, &hstmt2);
//		SQLSetConnectOption(hdbc, SQL_CURSOR_TYPE, SQL_CURSOR_DYNAMIC);
//		SQLSetConnectOption(hdbc, SQL_ROWSET_SIZE, 5);
//		SQLExecDirect(hstmt1, "select * from authors", SQL_NTS);


		connected = attemptingsession->Connect(connStr);
		session = -attemptingsession;
	
	//__________________________________________________________________________________________
	} else if (instanceTypeName == INSTTYPNM_DB2) {
		instanceType = INSTTYP_DB2;
		connStr 
			<< "Driver="				<< "{iSeries Access ODBC Driver}"	<< ";" // Note: For iSeries Access for Windows, V5R2 or later installs, two ODBC drivers are registered. Both the Client Access ODBC Driver (32-bit) and the iSeries Access ODBC Driver names are registered, however, both of these registered names point to the same ODBC driver. These two registered names do not indicate that two different ODBC drivers are installed. The older name of Client Access ODBC Driver (32-bit) is registered to support backward compatibility.
			//<< "Provider="				<< "DB2OLEDB"						<< ";"
			//<< "Cache Authentication="	<< "False"							<< ";"
			<< "System="				<< instanceAddress					<< ";" // Requirement of iSeries driver
			<< "UserID="				<< loginStr							<< ";"
			<< "Password="				<< loginPwd							<< ";"
			//<<SIGNON= 0=windows user name
/*
			<< "Data Source=" 			<< instanceAddress					<< ";"
			<< "Network Address="		<< instanceAddress					<< ".bankofamerica.com;"
			//<< "Initial Catalog="		<< catalogName						<< ";"
			<< "Default Schema="		<< dbName							<< ";"
			//<< "Extended Properties="Default Isolation Level=NC;Auto Commit Mode=True;";
			<< "APPC Remote LU Alias=DRDADEMO;APPC Local LU Alias=LOCAL;APPC Mode Name=QPCSUPP;Network Transport Library=TCPIP;Host CCSID=37;PC Code Page=1252;"
			<< "Network Port=446;"
			<< "Package Collection=QGPL;"
			<< "Alternate TP Name=0X07F9F9F9;"
			<< "Process Binary as Character=False;"
			<< "Persist Security Info=True;"
*/
			;
		//Looks like you haven't set userID password for datasource using mqsisetdbparms
		One<ODBCSession> attemptingsession = new ODBCSession;
		//OLE DB Provider for DB2:strConnect = _T("Provider=DB2OLEDB;APPC Local LU Alias=MyLocalLUAlias;"        "APPC Remote LU Alias=MyRemoteLUAlias;Initial Catalog=MyCatalog;"        "Package Collection=MyPackageCollection;Default Schema=MySchema;"        "User ID=MyUsername;Password=MyPassword;");
		//OLE DB Provider for DB2:strConnect = _T("Provider=DB2OLEDB;Network Transport Library=TCPIP;"        "Network Address=130.120.110.001;"        "Initial Catalog=MyCatalog;Package Collection=MyPackageCollection;"        "Default Schema=MySchema;User ID=MyUsername;Password=MyPassword;");
		//OLE DB Provider for AS400strConnect = _T("Provider=IBMDA400;Data source=myAS400;User Id=myUsername;"     "Password=myPassword;");
		connected = attemptingsession->Connect(connStr);
		session = -attemptingsession;
	
	//Driver=Sybase ASE OLE DB Provider   Initial Catalog=irp;Connect Timeout=15;Server Name=CHIADVSPDB05;Network Protocol=Winsock;Server Port Address=2510;Optimize Prepare=Partial;Select Method=Direct;Raise Error Behavior=MS Compatible;Print Statement Behavior=MS Compatible;Extended ErrorInfo=FALSE;Stored Proc Row Count=Last Statement Only;Row Cache Size=50;Enable Quoted Identifiers=0;Packet Size=1;Default Length For Long Data=1024
	//__________________________________________________________________________________________
	} else if (instanceTypeName == INSTTYPNM_FTP) {
		instanceType = INSTTYP_FTP;
		
		Exclamation(CAT << "Error: FTP not implemented");
		session = NULL;
		
		// FTP
		// C:\upp\uppsrc\plugin\ftp\ftp.cpp(44): if(!FtpConnect(host, &ftpconn, perror, &FtpClient::Callback, this, 200, idletimeout_secs)) {
	
	//__________________________________________________________________________________________
	} else if (instanceTypeName == INSTTYPNM_TELNET) {
		instanceType = INSTTYP_TELNET;
		
		Exclamation(CAT << "Error: TELNET not implemented");
		session = NULL;

	//__________________________________________________________________________________________
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

	LOG(connStr);

	if (connected) {
		connectThreadStatus = CONNECTSTATUS_SUCCEED;
	} else {
		connectThreadStatus = CONNECTSTATUS_FAIL;
	}
	return;
}

//==============================================================================================
SqlSession &Connection::GetSession() {
	return *session;
}

//==============================================================================================
// Avoid breakage when the script is damaged by control characters in its data portion
// The Prep includes apostrophizing since the E has to wrap the apostrphoies
String Connection::PrepTextDataForSend(const String &textData) {
	switch (instanceType) {
		case INSTTYP_POSTGRESQL:
			return CAT << "E'" << UrpString::ReplaceInWhatWith(UrpString::ReplaceInWhatWith(textData, "\\", "\\\\'"), "'", "\\'") << "'";
			break;
		case INSTTYP_ORACLE:
			return CAT << "'" << UrpString::ReplaceInWhatWith(textData, "'", "''") << "'";
			break;
		default:
			return CAT << "'" << textData << "'";
	}
}

//==============================================================================================
// Oracle breaks with odd aposts in line comments
String Connection::PrepScriptForSend(const String &script, bool log /*=false*/) {
	switch (instanceType) {
		case INSTTYP_ORACLE:
			// Double wrap a script.  Embedded textdata in insert as values and constants will be double-wrapped from their call to PrepText.
			return PrepOracleScriptForSend(script, log);
			break;
		default:
			return script;
	}
}

//==============================================================================================
// This is a prep for actual execution by the receiving instance, not for insertion into 
// the control db as a saved script text.  Must pass by COPY or you will corrupt the original.
String Connection::PrepOracleScriptForSend(const String script, bool log /*=false*/) {
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
			if (log) {
				LogLine(CAT << "Found " << c << " at " << (s - first_s));
			}
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

//==============================================================================================
// The Postgres COPY tool (http://www.postgresql.org/docs/8.1/static/sql-copy.html)
// has expectations of the data it is inputing
String Connection::PrepForPostgresCopyFrom(const String scriptText) {
	if (scriptText.IsEqual("\\N")) return String("\\\\N");
	if (scriptText.IsEqual("\\.")) return String("\\\\.");
	const char *s = scriptText.Begin();
	String outText;
	for (int i = 0; i < scriptText.GetLength(); i++) {
		switch (s[i]) {
		case '\t':
			outText+= "\\T";
			break;
		case '\n':
			outText+= "\\N";
			break;
		case '\r':
			outText+= "\\R";
			break;
		case '\\': // Will break loader: psql:C:/MyApps/lister/totable.sql:47697: ERROR:  invalid byte sequence for encoding "UTF8": 0x82
			outText+= "\\\\";
			break;
		default:
			outText+= s[i];
		}
	}
	
	return outText;
}


//==============================================================================================
// Migrate logic from CursorHandler.  Traverse fetch and push out to screen or table.
// Must be here because we want to push around connection pointer, not connection and cursor pointer.
bool Connection::ProcessQueryDataScript(Script &sob, JobSpec &jspec, ContextMacros *contextMacros) {
	CursorHandler cursorHandler(controlConnection, this);
	bool ran = cursorHandler.Run(sob, jspec, contextMacros);

	return ran;
}

//==============================================================================================
// Wrap script method implementation and error handling.  Will allow reconnects and reexecutes.
// We don't want to expand macros normally if we are writing to the control db.
bool Connection::SendQueryDataScript(const char *scriptText, ContextMacros *contextMacros, bool silent /*= false*/, bool expandMacros /*= false*/, bool log /*= false*/) {
	ClearError();
	static bool retry = false;

	// Process macros if we are pushing to a target
	
	String expandedScript;
	
	if (expandMacros) {
		expandedScript = ExpandMacros(scriptText, contextMacros);
		if (log) {
			if (scriptText != expandedScript) {
				LogLine("Script altered by macros");
				LOG(scriptText);
			} else {
				LogLine("Script was not altered by macros");
			}
		}
	} else {
		expandedScript = scriptText;
	}

	LOG(expandedScript);	
retryQuery:

	// We have to prep some scripts where embedded codes break the processor

	LOG(expandedScript);	
	if (!Execute(PrepScriptForSend(expandedScript, log))) {
		int errcode = GetErrorCode();
		String errmsg = GetLastError(); // Note: SQL Server does not include the number in the message
		// Trap Oracle and MS SQLconnection timeout
		
		if (In(errcode, 12571, 233) || errmsg.Find("Communication link failure") > 0) {
			// 12571: Oracle TNS-Packet failure, happens after timeout
			// 233: SQLServer Error: 233, Communication link failure [SQLSTATE 08S01]
			// Should detect that we haven't run a script for a while and have probably timed out
			if (PromptYesNo("Detected connection timeout, reconnect?")) {
				delete -session;
				if (!Connect(topWindow)) {
					Exclamation(CAT << "Error reconnection: " << connectErrorMessage);
					return false;
				}
				if (!session) {
					Exclamation("Session not reset by reconnect");
					return false;
				}
				
				retry = true;
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
	} else {
		return true;
	}

	return true;
}

//==============================================================================================
bool Connection::SendAddDataScript(const char *sqlText, ContextMacros *contextMacros, bool silent /*= false*/, bool expandMacros /*= false*/) {
	return SendQueryDataScript(sqlText, contextMacros, silent, expandMacros);
}

//==============================================================================================
bool Connection::SendChangeDataScript(const char *sqlText, ContextMacros *contextMacros, bool silent /*= false*/, bool expandMacros /*= false*/) {
	return SendQueryDataScript(sqlText, contextMacros, silent, expandMacros);
}

//==============================================================================================
bool Connection::SendRemoveDataScript(const char *sqlText, ContextMacros *contextMacros, bool silent /*= false*/, bool expandMacros /*= false*/) {
	return SendQueryDataScript(sqlText, contextMacros, silent, expandMacros);
}

//==============================================================================================
bool Connection::SendChangeEnvScript(const char *sqlText, ContextMacros *contextMacros, bool silent /*= false*/, bool expandMacros /*= false*/) {
	return SendQueryDataScript(sqlText, contextMacros, silent, expandMacros);
}

//==============================================================================================
bool Connection::SendQueryEnvScript(const char *sqlText, ContextMacros *contextMacros, bool silent /*= false*/, bool expandMacros /*= false*/) {
	return SendQueryDataScript(sqlText, contextMacros, silent, expandMacros);
}

//==============================================================================================
bool Connection::SendChangeStructureScript(const char *sqlText, ContextMacros *contextMacros, bool silent /*= false*/, bool expandMacros /*= false*/) {
	return SendQueryDataScript(sqlText, contextMacros, silent, expandMacros);
}

//==============================================================================================
int Connection::GetInsertedId(String tableName, String columnName) {
	switch (instanceType) {
		case INSTTYP_POSTGRESQL:
			return GetPostgreSQLInsertedId(tableName, columnName);
			break;
		default:
			ASSERT(1==0);
	}
	
	return UNKNOWN;
}

//==============================================================================================
// Only works for PostgreSQL.
int Connection::GetPostgreSQLInsertedId(String tableName, String columnName) {
	String sequenceName = CAT << tableName << "_" << columnName << "_" << "seq";
	return GetPostgreSQLInsertedId(sequenceName);
}

//==============================================================================================
int Connection::GetPostgreSQLInsertedId(String sequenceName) {			
	// PostgreSQL constructs a default sequence for serial columns
	String script = CAT << "select currval('" << sequenceName << "')";
	if (!SendQueryDataScript(script)) { // ERROR: currval of sequence "connections_connid_seq" is not yet defined in this session
		return -1;
	}
	
	Fetch();
	return atoi((*this)[0].ToString());
}

//==============================================================================================
Value Connection::Get(SqlId i) const {
	Value v;
	Sql::GetColumn(i, v);
	return v;
}

//==============================================================================================
// PostgreSQL driver does not return the type of a boolean as boolean, but as string
bool Connection::GetBool(int i) const {
	Value v;
	Sql::GetColumn(i, v);

	if (v.GetType() == STRING_V) {
		return (v == "1");
	}
	
	return v;
}

//==============================================================================================
Value Connection::Get(int i) const {
	Value v;
	Sql::GetColumn(i, v);
	return v;
}

//==============================================================================================
bool Connection::HandleDbError(int actioncode, String *cmd/* = NULL*/, bool log, bool batchMode) {
	
	String errcode = GetErrorCodeString();
	int myerrno = GetErrorCode();
	Sql::ERRORCLASS ec = GetErrorClass();
	String es = GetErrorStatement();
	String msg;
	
	// CONNECTION_BROKEN?
	if (ec == Sql::CONNECTION_BROKEN) {
		msg = "Connection broken";
		//Reconnect, try again, but sql statement object will be broke.
		//Post a message to main window to reconnect.
	} else if (myerrno == 25408) {
		msg = "Connection broken (could not replay:Oracle)";
	} else {
		msg = Format("Error: #=%d, [* \1%s\1].", myerrno, DeQtf(GetLastError()));
	}
	
	if (!batchMode) Exclamation(msg);
	if (log) LogLine(msg);
	return false;
}

//==============================================================================================
/*static*/ VectorMap<String,Connection *>& ConnectionFactory::Connections() {
	static VectorMap<String,Connection *> connections;
	return connections;
}

//==============================================================================================
ConnectionFactory::ConnectionFactory(Connection *pcontrolConnection/* = NULL*/) {
	controlConnection = pcontrolConnection;
}

//==============================================================================================
ConnectionFactory::~ConnectionFactory() {
	// Release all the connection objects to prevent a memory leak
	for (int i = 0; i < Connections().GetCount(); i++) {
		Connection *conn = Connections()[i];
		delete conn;
	}
}

//==============================================================================================
/*static*/ String ConnectionFactory::ControlInstanceType() {
	return INSTTYPNM_POSTGRESQL;
}

//==============================================================================================
// Connections are just live connections, so no automatic connecting here.
Connection *ConnectionFactory::GetConnection(String connName) {
	if (connName.IsEmpty()) return NULL; // Empty for a new row, for instance
	ASSERT(!connName.IsEmpty());
	return Connections().Get(connName, (Connection *)NULL);
}

//==============================================================================================
Connection *ConnectionFactory::Connect(TopWindow *win, int connId, bool useIfFoundInPool /*=false*/, Connection *pcontrolConnection/* = NULL*/) {
	Connection *lcontrolConnection = NULL;
	
	// User passed a new control connection, we will use locally temporarily for this connection
	if (pcontrolConnection) lcontrolConnection = pcontrolConnection;
	
	// If they passed a new control connection and we don't have a shared controlconnection already established, we'll use the new one as a shared one
	if (lcontrolConnection && !controlConnection) controlConnection = lcontrolConnection;
	
	// If they didn't pass us a new control connection then they must want to use the shared one
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
		",  portNo"           // 13
		" from v_conn where ConnId = %d", connId);
		
	if (!lcontrolConnection->SendQueryDataScript(FetchConnDtlById)) {
		return NULL;
	}
	
	lcontrolConnection->Fetch();

	String connName			= lcontrolConnection->Get(1);
	String instanceTypeName = lcontrolConnection->Get(9);
	String loginStr 		= lcontrolConnection->Get(3);
	String loginPwd 		= lcontrolConnection->Get(4);
	String instanceAddress 	= lcontrolConnection->Get(7);
	String dbName 			= lcontrolConnection->Get(12);
	String portNo           = lcontrolConnection->Get(13);
	if (useIfFoundInPool) {
		Connection *conn = GetConnection(connName);
		if (conn != (Connection *)NULL) {
			return conn;
		}
	}
	
	return Connect(
			win
		,	connName
		,	instanceTypeName
		,	loginStr
		,	loginPwd
		,	instanceAddress
		,	dbName
		,   portNo
	);
	
}

//==============================================================================================
// Connection factory (Takes window for async connection spinning.
// Assumption: connName is unique per connection.  No support for multiple connections per connection definition
Connection *ConnectionFactory::Connect(TopWindow *win, String connName, String instanceTypeName
	, String loginStr, String loginPwd, String instanceAddress, String dbName/* = Null*/, String portNo/*=Null*/, bool log/*=false*/) {
	
	Connection *connection = Connections().Get(connName, (Connection *)NULL);
	if (!connection) {
		if (log) LogLine(CAT << "Connection " << connName << " not in Connections vector");
		connection = Connections().GetAdd(connName, new Connection());
	}

	if (log) {
			LogLine("Attempting To Establish Connection to " + connName + ", " + instanceTypeName 
			+ ", login=" + loginStr + ", addr=" + instanceAddress + ", db=" + dbName + ", port=" + portNo);
	}
	connection->instanceTypeName = instanceTypeName;
	connection->connName         =         connName;
	connection->loginStr         =         loginStr;
	connection->loginPwd         =         loginPwd; // For reconnecting, or changing password, you have to pass the old one
	connection->instanceAddress  =  instanceAddress;
	connection->dbName           =           dbName;
	connection->portNo           =           portNo;

	if (controlConnection) connection->controlConnection = controlConnection;
	
	// BUG: if connection information changed, it won't be representative of the actual connection
	
	if (!In(connection->enumConnState, CON_SUCCEED, CON_STALE)) {
		if (connection->Connect(win)) {
			if (log) LogLine("Connection Established.");
			Speak(EVS_CONNECT_SUCCEEDED);
		} else {
			if (log) LogLine("Failed to establish connection.");
			Speak(EVS_CONNECT_FAILED);
		}
	}

	if (connName == CONTROL_CONN_NAME) {
		controlConnection = Connections().Get(connName);
		LOG("Set control Connection to " + connName + ", " + instanceTypeName 
		+ ", login=" + loginStr + ", addr=" + instanceAddress + ", db=" + dbName + ", port=" + portNo);
	}
	
	// Cycle through hooks

	return Connections().Get(connName);
}

Connection *ConnectionFactory::controlConnection = NULL;
