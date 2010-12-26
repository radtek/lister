#ifndef _Err_h_
#define _Err_h_

/*
Oracle:
 - 25408 - can not safely replay call - If trapped, connection expired, reconnect.
 Oracle connect failed, ORA-12560: TNS:protocol adapter error
 Oracle connect failed, ORA-01017: invalid username/password; logon denied
 Error: #=911, ORA`-00911`: invalid character (trailing semicolon)
 Error: #=1406, ORA`-01406`: fetched column value was truncated: column's program buffer area 
 OCIStmt           *stmthp;
*/

#include "shared.h"

#define ACTNDB_OPEN 1
#define ACTNDB_CLOSE 2
#define ACTNDB_EXECSEL 3
#define ACTNDB_CONFIG 4

#define RESPTOERR_MARKASCLOSED 1
#define RESPTOERR_RECONNECT 2
#define RESPTOERR_RERUN 3

//==========================================================================================	
// Register a function or method(?) that will cleanup a broken connection, return success/failure
// which will allow a reconnect and rerun.  A Fetch would require a reexecute.

bool RegisterReconnect(void *fnc()) {
	return false;	
}

//==========================================================================================	
bool HandleDbError(int actioncode, Sql &sql, String *cmd = NULL) {
	String errcode = sql.GetErrorCodeString();
	int myerrno = sql.GetErrorCode();
	Sql::ERRORCLASS ec = sql.GetErrorClass();
	String es = sql.GetErrorStatement();
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
	
	Exclamation(Format("Error: #=%d, [* \1%s\1].", myerrno, DeQtf(sql.GetLastError())));
	return false;
}

#endif
