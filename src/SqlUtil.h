#ifndef _SqlUtil_h_
#define _SqlUtil_h_

#include "shared.h"
#include "Err.h"
#include "StringHandler.h"

//==========================================================================================	
// Need to customize to include synonyms, views, selectable sps, tables, functions...
//Vector<String> GetTableNamesForSchema(Sql &sql, String &schemaName) {
//	if (conn.instanceTypeName == "Oracle") {
//		String cmd = CAT << "select  from all_objects where owner = '" << schemaName << "' order by ";
//		if (!SendQueryDataScript(sql, cmd)) { // ERROR: currval of sequence "connections_connid_seq" is not yet defined in this session
//			return -1;
//		}
//	
		Vector<String> list;
//		while (sql.Fetch()) {
//			list.Add(sql[0]);
//		}
//	}
//	return list;
//}

#endif
