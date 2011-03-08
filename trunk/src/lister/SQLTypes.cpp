#include "SQLTypes.h"

// From C:\Program Files\Microsoft SDKs\Windows\v7.0\Include
#include "sqlext.h" 
// Also, C:\Program Files\Microsoft SDKs\Windows\v7.0\Include\sql.h contains SQL_VARCHAR
String SQLTypeName(int sqlType) {
	String typeName;
	switch (sqlType) {
		case SQL_C_CHAR: typeName = "CHAR"; break;
		case SQL_VARCHAR: typeName = "VARCHAR"; break;
		case SQL_C_LONG: typeName = "LONG"; break;
		case SQL_SMALLINT: typeName = "SMALLINT"; break;
		case SQL_REAL: typeName = "REAL"; break; // Same as SQL_C_FLOAT
		case SQL_FLOAT: typeName = "FLOAT"; break;
		case SQL_DOUBLE: typeName = "DOUBLE"; break;
		case SQL_DECIMAL: typeName = "DECIMAL"; break;
#if (ODBCVER >= 0x0300)
		case SQL_C_NUMERIC: typeName = "NUMERIC"; break;
#endif  /* ODBCVER >= 0x0300 */
		case SQL_C_DEFAULT: typeName = "DEFAULT"; break;
		case SQL_DATETIME: typeName = "DATETIME"; break; // Same as SQL_DATE
		case SQL_C_TIME: typeName = "TIME"; break;
		case SQL_C_TIMESTAMP: typeName = "TIMESTAMP"; break;
#if (ODBCVER >= 0x0300)
		case SQL_C_TYPE_DATE: typeName = "DATE"; break;
		case SQL_C_TYPE_TIME: typeName = "TIME"; break;
		case SQL_C_TYPE_TIMESTAMP:typeName = "TIMESTAMP"; break;
		case SQL_C_INTERVAL_YEAR: typeName = "INTERVAL_YEAR"; break;
		case SQL_C_INTERVAL_MONTH: typeName = "INTERVAL_MONTH"; break;
		case SQL_C_INTERVAL_DAY: typeName = "INTERVAL_DAY"; break;
		case SQL_C_INTERVAL_HOUR: typeName = "INTERVAL_HOUR"; break;
		case SQL_C_INTERVAL_MINUTE: typeName = "INTERVAL_MINUTE"; break;
		case SQL_C_INTERVAL_SECOND: typeName = "INTERVAL_SECOND"; break;
		case SQL_C_INTERVAL_YEAR_TO_MONTH: typeName = "INTERVAL_YEAR_TO_MONTH"; break;
		case SQL_C_INTERVAL_DAY_TO_HOUR: typeName = "INTERVAL_DAY_TO_HOUR"; break;
		case SQL_C_INTERVAL_DAY_TO_MINUTE: typeName = "INTERVAL_DAY_TO_MINUTE"; break;
		case SQL_C_INTERVAL_DAY_TO_SECOND: typeName = "INTERVAL_DAY_TO_SECOND"; break;
		case SQL_C_INTERVAL_HOUR_TO_MINUTE: typeName = "INTERVAL_HOUR_TO_MINUTE"; break;
		case SQL_C_INTERVAL_HOUR_TO_SECOND: typeName = "INTERVAL_HOUR_TO_SECOND"; break;
		case SQL_C_INTERVAL_MINUTE_TO_SECOND: typeName = "INTERVAL_MINUTE_TO_SECOND"; break;
#endif  /* ODBCVER >= 0x0300 */
		case SQL_C_BINARY: typeName = "BINARY"; break;
		case SQL_C_BIT: typeName = "BIT"; break;
#if (ODBCVER >= 0x0300)
		case SQL_C_SBIGINT: typeName = "SIGNED BIGINT"; break;
		case SQL_C_UBIGINT: typeName = "UNSIGNED BIGINT"; break;
#endif  /* ODBCVER >= 0x0300 */
		case SQL_C_TINYINT: typeName = "TINYINT"; break;
		case SQL_C_SLONG: typeName = "SIGNED_LONG"; break;
		case SQL_C_SSHORT: typeName = "SIGNED_SHORT"; break;
		case SQL_C_STINYINT: typeName = "SIGNED_TINYINT"; break;
		case SQL_C_ULONG: typeName = "UNSIGNED_LONG"; break;
		case SQL_C_USHORT: typeName = "UNSIGNED_SHORT"; break;
		case SQL_C_UTINYINT: typeName = "UNSIGNED_TINYINT"; break;
#if (ODBCVER >= 0x0350)
		case SQL_C_GUID: typeName = "GUID"; break;
#endif  /* ODBCVER >= 0x0350 */
		default: typeName = "Unknown"; break;
	}
	return typeName;
}

