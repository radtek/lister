#ifndef _MyTool002_SqlUtil_h_
#define _MyTool002_SqlUtil_h_

#include "shared.h"
#include "Err.h"
#include "StringHandler.h"

#include <PostgreSQL/PostgreSQL.h>

//==========================================================================================	
int GetInsertedId(Sql &sql, String tableName, String columnName) {
	
	// PostgreSQL constructs a default sequence for serial columns
	String sequenceName = CAT << tableName << "_" << columnName << "_" << "seq";
	String cmd = CAT << "select currval('" << sequenceName << "')";
	if (!sql.Execute(cmd)) { // ERROR: currval of sequence "connections_connid_seq" is not yet defined in this session
		HandleDbError(ACTNDB_EXECSEL, sql, &cmd);
		return -1;
	}
	
	sql.Fetch();
	return atoi(sql[0].ToString());
}

//==========================================================================================	
String PGFormat(const String &inStr) {
	return ReplaceStr(ReplaceStr(inStr, "\\", "\\\\"), "'", "\\'");	
}

#endif
