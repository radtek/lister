#include "SQLTypes.h"

#include "sqlext.h"

String SQLTypeName(int sqlType) {
	String typeName;
	switch (sqlType) {
		case SQL_C_CHAR:
			typeName = "CHAR";
		case SQL_C_LONG: typeName = "LONG";
		case SQL_SMALLINT: typeName = "SMALLINT";
		case SQL_C_FLOAT: typeName = "FLOAT";
		case SQL_DOUBLE: typeName = "DOUBLE";
#if (ODBCVER >= 0x0300)
		case SQL_C_NUMERIC: typeName = "NUMERIC";
#endif  /* ODBCVER >= 0x0300 */
		case SQL_C_DEFAULT: typeName = "DEFAULT";

		case SQL_C_DATE: typeName = "DATE";
		case SQL_C_TIME:
			typeName = "TIME";
		case SQL_C_TIMESTAMP: typeName = "TIMESTAMP";
#if (ODBCVER >= 0x0300)
		case SQL_C_TYPE_DATE:
			typeName = "DATE";
		case SQL_C_TYPE_TIME:
			typeName = "TIME";
		case SQL_C_TYPE_TIMESTAMP:
			typeName = "TIMESTAMP";
		case SQL_C_INTERVAL_YEAR: 
			typeName = "INTERVAL_YEAR";
		case SQL_C_INTERVAL_MONTH:
			typeName = "INTERVAL_MONTH";
		case SQL_C_INTERVAL_DAY:
			typeName = "INTERVAL_DAY";
		case SQL_C_INTERVAL_HOUR:
			typeName = "INTERVAL_HOUR";
		case SQL_C_INTERVAL_MINUTE:
			typeName = "INTERVAL_MINUTE";
		case SQL_C_INTERVAL_SECOND:
			typeName = "INTERVAL_SECOND";
		case SQL_C_INTERVAL_YEAR_TO_MONTH:
			typeName = "INTERVAL_YEAR_TO_MONTH";
		case SQL_C_INTERVAL_DAY_TO_HOUR:
			typeName = "INTERVAL_DAY_TO_HOUR";
		case SQL_C_INTERVAL_DAY_TO_MINUTE:
			typeName = "INTERVAL_DAY_TO_MINUTE";
		case SQL_C_INTERVAL_DAY_TO_SECOND:
			typeName = "INTERVAL_DAY_TO_SECOND";
		case SQL_C_INTERVAL_HOUR_TO_MINUTE:
			typeName = "INTERVAL_HOUR_TO_MINUTE";
		case SQL_C_INTERVAL_HOUR_TO_SECOND:
			typeName = "INTERVAL_HOUR_TO_SECOND";
		case SQL_C_INTERVAL_MINUTE_TO_SECOND:
			typeName = "INTERVAL_MINUTE_TO_SECOND";
#endif  /* ODBCVER >= 0x0300 */
		case SQL_C_BINARY:
			typeName = "BINARY";
		case SQL_C_BIT:
			typeName = "BIT";
#if (ODBCVER >= 0x0300)
		case SQL_C_SBIGINT:
			typeName = "SIGNED BIGINT";
		case SQL_C_UBIGINT:
			typeName = "UNSIGNED BIGINT";
#endif  /* ODBCVER >= 0x0300 */
		case SQL_C_TINYINT: typeName = "TINYINT";
		case SQL_C_SLONG: typeName = "SIGNED_LONG";
		case SQL_C_SSHORT: typeName = "SIGNED_SHORT";
		case SQL_C_STINYINT: typeName = "SIGNED_TINYINT";
		case SQL_C_ULONG: typeName = "UNSIGNED_LONG";
		case SQL_C_USHORT: typeName = "UNSIGNED_SHORT";
		case SQL_C_UTINYINT: typeName = "UNSIGNED_TINYINT";
#if (ODBCVER >= 0x0350)
		case SQL_C_GUID: typeName = "GUID";
#endif  /* ODBCVER >= 0x0350 */
		default: typeName = "Unknown";
	}
	return typeName;
}

