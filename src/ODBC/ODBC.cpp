#include "ODBC.h"
#include "../lister/shared.h"
#include <CtrlLib/CtrlLib.h>
#include <lister/Sql/Sql.h>
#include <lister/Urp/UrpShared.h>

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

// Set some tunable amount
#define ROW_FETCH_COUNT 100

bool In(SQLCHAR *i
	,	const char *in1
	) { 
	String x((char *)i);
	if (false
	||	x.IsEqual(in1)
	) return true; return false; }
bool In(SQLCHAR *i
	,	const char *in1
	,	const char *in2
	) { 
	String x((char *)i);
	if (false
	||	x.IsEqual(in1)
	||	x.IsEqual(in2)
	) return true; return false; }
bool In(SQLCHAR *i
	,	const char *in1
	,	const char *in2
	,	const char *in3
	) { 
	String x((char *)i);
	if (false
	||	x.IsEqual(in1)
	||	x.IsEqual(in2)
	||	x.IsEqual(in3)
	) return true; return false; }
bool In(SQLCHAR *i
	,	const char *in1
	,	const char *in2
	,	const char *in3
	,	const char *in4
	) { 
	String x((char *)i);
	if (false
	||	x.IsEqual(in1)
	||	x.IsEqual(in2)
	||	x.IsEqual(in3)
	||	x.IsEqual(in4)
	) return true; return false; }
bool In(SQLCHAR *i
	,	const char *in1
	,	const char *in2
	,	const char *in3
	,	const char *in4
	,	const char *in5
	) { 
	String x((char *)i);
	if (false
	||	x.IsEqual(in1)
	||	x.IsEqual(in2)
	||	x.IsEqual(in3)
	||	x.IsEqual(in4)
	||	x.IsEqual(in5)
	) return true; return false; }
bool In(SQLCHAR *i
	,	const char *in1
	,	const char *in2
	,	const char *in3
	,	const char *in4
	,	const char *in5
	,	const char *in6
	) { 
	String x((char *)i);
	if (false
	||	x.IsEqual(in1)
	||	x.IsEqual(in2)
	||	x.IsEqual(in3)
	||	x.IsEqual(in4)
	||	x.IsEqual(in5)
	||	x.IsEqual(in6)
	) return true; return false; }
bool In(SQLCHAR *i
	,	const char *in1
	,	const char *in2
	,	const char *in3
	,	const char *in4
	,	const char *in5
	,	const char *in6
	,	const char *in7
	) { 
	String x((char *)i);
	if (false
	||	x.IsEqual(in1)
	||	x.IsEqual(in2)
	||	x.IsEqual(in3)
	||	x.IsEqual(in4)
	||	x.IsEqual(in5)
	||	x.IsEqual(in6)
	||	x.IsEqual(in7)
	) return true; return false; }
bool In(SQLCHAR *i
	,	const char *in1
	,	const char *in2
	,	const char *in3
	,	const char *in4
	,	const char *in5
	,	const char *in6
	,	const char *in7
	,	const char *in8
	) { 
	String x((char *)i);
	if (false
	||	x.IsEqual(in1)
	||	x.IsEqual(in2)
	||	x.IsEqual(in3)
	||	x.IsEqual(in4)
	||	x.IsEqual(in5)
	||	x.IsEqual(in6)
	||	x.IsEqual(in7)
	||	x.IsEqual(in8)
	) return true; return false; }

class ODBCConnection : public SqlConnection
{
public:
	ODBCConnection(ODBCSession *session);
	virtual ~ODBCConnection();

	void                    Clear();

	virtual void            SetParam(int i, const Value& r);
	virtual bool            Execute();
	virtual int             GetRowsProcessed() const;
	virtual int             GetParseErrorPosition() const;
	virtual bool            Fetch();
	virtual void            GetColumn(int i, Ref r) const;
	virtual void            Cancel();
	virtual SqlSession&     GetSession() const { ASSERT(session); return *session; }
	virtual String          GetUser() const    { ASSERT(session); return session->user; }
	virtual String          ToString() const;
	virtual Value           GetInsertedId() const;
	Vector<MySqlColumnInfo>  myinfo;


private:
	friend class ODBCSession;

	ODBCSession            *session;
	struct Param {
		int    ctype;
		int    sqltype;
		String data;
		SQLLEN li;
	};
	Array<Param>             param, bparam;
	String                   last_insert_table;

	int                      rowsprocessed;
	Vector< Vector<double> > number;
	Vector< Vector<int64> >  num64;
	Vector< Vector<String> > text;
	Vector< Vector<Time> >   time;
	int                      rowcount;
	int                      rowi;
	Vector<Value>            fetchrow;
	Vector<bool>             binary;
	
	bool                     IsOk(SQLRETURN ret) const;
	void                     Flush();
	bool                     Fetch0();
	bool                     IsCurrent() const   { return session->current == this; }
	
	// How many rows fetched in last ExtendedFetch call
	SQLUINTEGER              rowsFetched;
	// Irrelevant row status since we are not altering data
	SQLUSMALLINT             rowStatus[ROW_FETCH_COUNT];
	// Have we fetched a batch?  If not, fetch another for cycling through.
	bool                     fetchedSet;
	// Pointer within last fetched set for returning to caller
	SQLSETPOSIROW            nextFetchSetRow;
	// 100 cols x 100 rows x 100 characters.  TODO: Convert to dynamic allocation or cache
	//SQLCHAR                  fetchedRows[MAX_COL_FETCH_COUNT][ROW_FETCH_COUNT][MAX_DATA_WIDTH_FETCH];
	// Tells us if the value was null
	//SQLLEN                   indicator[MAX_COL_FETCH_COUNT][ROW_FETCH_COUNT]; 
	SQLLEN                   *indicator;
	// Actual data created as each column is identified
	VectorMap<String, byte *> rowdata;
};

//==============================================================================================
bool ODBCSession::Connect(const char *cs) {
	if(henv && IsOk(SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc))) {
		// Attempt to improve speed by making all connections read-only: TODO: Parameterize
	// MSSQL	SQLSetConnectAttr(hdbc, SQL_ATTR_ACCESS_MODE, (SQLPOINTER)SQL_MODE_READ_ONLY, SQL_NTS);
	// MSSQL	SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
		//SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
		// For BA work, serialized data is not required.  TODO: Parameterize
	// MSSQL	SQLSetConnectAttr(hdbc, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)SQL_TRANSACTION_READ_UNCOMMITTED, SQL_NTS);

		if(IsOk(SQLDriverConnect(hdbc, NULL, (SQLCHAR *)cs, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT))) { // SQL_DRIVER_COMPLETE?
			SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
			// Reference: http://msdn.microsoft.com/en-us/library/ms713605(VS.85).aspx
//			//SQLSetConnectAttr(hdbc, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)SQL_TRANSACTION_SERIALIZABLE, SQL_NTS); // Took out, slowest connection
			// See also: SQL_ATTR_CONNECTION_DEAD
			return true;
		}

		// Connection failed if reached this point
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		// Error on this; taking out.
		hdbc = SQL_NULL_HANDLE;
	}
	return false;
}

//==============================================================================================
bool ODBCSession::IsOpen() const {
	return hdbc != SQL_NULL_HANDLE;
}

//==============================================================================================
void ODBCSession::Close() {
	if(hdbc != SQL_NULL_HANDLE) {
		current = NULL;
		FlushConnections();
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hdbc = SQL_NULL_HANDLE;
		hstmt = SQL_NULL_HANDLE;
		current = NULL;
	}
}

//==============================================================================================
void ODBCSession::FlushConnections() {
	LLOG("FlushConnections");
	if(current) {
		current->Flush();
		current = NULL;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

//==============================================================================================
bool ODBCSession::IsOk(SQLRETURN ret) {
	if(SQL_SUCCEEDED(ret))
		return true;
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   MsgLen;
	String        error;
	int i = 1;
	while(SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, i++, SqlState, &NativeError,
	                 Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) {
		if(error.GetCount())
			error << "\r\n";
		error << (char *)Msg;
	}
	SetError(error, statement);
	return false;
}

//==============================================================================================
ODBCSession::ODBCSession() {
	hdbc = SQL_NULL_HANDLE;
	hstmt = SQL_NULL_HANDLE;
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if(henv)
		SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
	tlevel = 0;
	Dialect(MSSQL);
	current = NULL;
}

//==============================================================================================
ODBCSession::~ODBCSession() {
	if(henv)
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void ODBCSession::Begin()
{
	if(tlevel == 0)
		SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);
	tlevel++;
}

void ODBCSession::Commit()
{
	tlevel--;
	ASSERT(tlevel >= 0);
	if(tlevel == 0) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
		SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
	}
}

void ODBCSession::Rollback()
{
	tlevel--;
	ASSERT(tlevel >= 0);
	if(tlevel == 0) {
		SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
		SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
	}
}

String ODBCSession::Savepoint()
{
	NEVER();
	return "";
}

void ODBCSession::RollbackTo(const String& savepoint)
{
	NEVER();
}

Vector<String> ODBCSession::EnumUsers()
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumDatabases()
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumTables(String database)
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumViews(String database)
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumSequences(String database)
{
	return Vector<String>();
}

Vector<String> ODBCSession::EnumPrimaryKeys(String database, String table)
{
	return Vector<String>();
}

String ODBCSession::EnumRowID(String database, String table)
{
	return "";
}

bool   ODBCPerformScript(const String& text, StatementExecutor& executor, Gate2<int, int> progress_canceled)
{
	const char *p = text;
	while(*p) {
		String cmd;
		while(*p && *p != ';')
			if(*p == '\'') {
				const char *s = p;
				while(*++p && (*p != '\'' || *++p == '\''))
					;
				cmd.Cat(s, int(p - s));
			}
			else {
				if(*p > ' ')
					cmd.Cat(*p);
				else if(!cmd.IsEmpty() && *cmd.Last() != ' ')
					cmd.Cat(' ');
				p++;
			}
		if(progress_canceled(int(p - text.Begin()), text.GetLength()))
			return false;
		if(!IsNull(cmd) && !executor.Execute(cmd))
			return false;
		if(*p == ';')
			p++;
	}
	return true;
}

//==============================================================================================
SqlConnection *ODBCSession::CreateConnection() {
	return new ODBCConnection(this);
}

//==============================================================================================
ODBCConnection::ODBCConnection(ODBCSession *session_) : session(session_) {
	LLOG("ODBCConnection " << (void *)this << " " << (void *)session);
	rowcount = rowi = 0;
}

//==============================================================================================
ODBCConnection::~ODBCConnection() {
	if(IsCurrent())
		session->current = NULL;
	LLOG("~ODBCConnection " << (void *)this << " " << (void *)session);
}

//==============================================================================================
bool ODBCConnection::IsOk(SQLRETURN ret) const {
	if(SQL_SUCCEEDED(ret) || ret == SQL_NO_DATA)
		return true;
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   MsgLen;
	String        error;
	int i = 1;
	while(SQLGetDiagRec(SQL_HANDLE_STMT, session->hstmt, i++, SqlState, &NativeError,
	                    Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) {
		if(error.GetCount())
			error << "\r\n";
		error << (char *)Msg;
	}
	session->SetError(error, statement);
	return false;
}

//==============================================================================================
void ODBCConnection::SetParam(int i, const Value& r) {
	Param& p = param.At(i);
	if(IsNumber(r)) {
		if(r.Is<int64>()) {
			int64 x = r;
			p.ctype = SQL_C_SBIGINT;
			p.sqltype = SQL_BIGINT;
			p.data = String((char *)&x, sizeof(x));
			p.li = sizeof(x);
		}
		else {
			double x = r;
			p.ctype = SQL_C_DOUBLE;
			p.sqltype = SQL_DOUBLE;
			p.data = String((char *)&x, sizeof(x));
			p.li = sizeof(x);
		}
	}
	if(IsString(r)) {
		p.ctype = SQL_C_CHAR;
		p.sqltype = SQL_LONGVARCHAR;
		p.data = r;
		p.li = p.data.GetLength();
	}
	if(IsDateTime(r)) {
		p.ctype = SQL_C_TYPE_TIMESTAMP;
		p.sqltype = SQL_TYPE_TIMESTAMP;
		Time t = r;
		SQL_TIMESTAMP_STRUCT tm;
		tm.year = t.year;
		tm.month = t.month;
		tm.day = t.day;
		tm.hour = t.hour;
		tm.minute = t.minute;
		tm.second = t.second;
		tm.fraction = 0;
		p.data = String((char *)&tm, sizeof(tm));
		p.li = sizeof(tm);
	}
	if(IsNull(r))
		p.li = SQL_NULL_DATA;
}

//==============================================================================================
// New version: Supports extendedfetch: results in 1000% increase in performance over latent connections.
bool ODBCConnection::Execute()
{
	LLOG("Execute " << (void *)this << " " << (void *)session);
	if(session->hstmt == SQL_NULL_HANDLE)
		return false;
	if(IsCurrent())
		session->current = NULL;
	session->FlushConnections();
	last_insert_table.Clear();
	number.Clear();
	text.Clear();
	time.Clear();
	CParser p(statement);
	if((p.Id("insert") || p.Id("INSERT")) && (p.Id("into") || p.Id("INTO")) && p.IsId())
		last_insert_table = p.ReadId();
	
	// Set up the call to ExtendedFetch.  Required SQLBindCol, cannot use SQLGetData.
	int rtnc = SQLSetStmtOption (session->hstmt, SQL_ROWSET_SIZE, ROW_FETCH_COUNT);
//	rtnc = SQLSetStmtOption (session->hstmt, SQL_CURSOR_TYPE, SQL_CURSOR_FORWARD_ONLY);

	// Required For SQL Server only
//	String setEnv = "SET NOCOUNT ON";
//	if(!IsOk(SQLPrepare(session->hstmt, (SQLCHAR *)~setEnv, setEnv.GetCount())))
//		return false;
//
//	if(!IsOk(SQLExecute(session->hstmt))) {
//		SQLFreeStmt(session->hstmt, SQL_CLOSE);
//		return false;
//	}

	fetchedSet = false;
	if(!IsOk(SQLPrepare(session->hstmt, (SQLCHAR *)~statement, statement.GetCount())))
		return false;
	parse = false;
	bparam = param;
	param.Clear();
	for(int i = 0; i < bparam.GetCount(); i++) {
		Param& p = bparam[i];
		SQLSMALLINT     DataType;
		SQLULEN         ParameterSize;
		SQLSMALLINT     DecimalDigits;
		SQLSMALLINT     Nullable;

		if(!IsOk(SQLDescribeParam(session->hstmt, i + 1, &DataType, &ParameterSize, &DecimalDigits, &Nullable)))
			return false;
		if(!IsOk(SQLBindParameter(session->hstmt, i + 1, SQL_PARAM_INPUT, p.ctype, DataType,
		                          ParameterSize, DecimalDigits, (SQLPOINTER)~p.data, p.data.GetLength(),
		                          &p.li)))
			return false;
	}

	SQLSMALLINT ncol;
	
	if(!IsOk(SQLExecute(session->hstmt))) {
		SQLFreeStmt(session->hstmt, SQL_CLOSE);
		return false;
	}

	if(!IsOk(SQLNumResultCols(session->hstmt, &ncol))) {
		SQLFreeStmt(session->hstmt, SQL_CLOSE);
		return false;
	}

	// Allocate a 2-dimensional array for the indicators col x row.
	indicator = (SQLLEN *)new SQLLEN[ncol * ROW_FETCH_COUNT];
	session->current = this;
	myinfo.Clear();
	info.Clear();
	binary.Clear();
	
	// Identify all properties of returned data and bind space for fetching

	for(int i = 0; i < myinfo.GetCount(); i++) {
		
		byte *b = rowdata.Get(myinfo[i].name);
		delete b;
	}
	rowdata.Clear();
	
	String collist; // Create a list of all columns for debugging

	for(int i = 1; i <= ncol; i++) {
		SQLCHAR      ColumnName[256];
		SQLSMALLINT  NameLength;
		SQLSMALLINT  DataType;
		SQLULEN      ColumnSize;
		SQLSMALLINT  DecimalDigits;
		SQLSMALLINT  Nullable;
		
		if(!IsOk(SQLDescribeCol(session->hstmt, i, ColumnName, 255, &NameLength, &DataType,
		                        &ColumnSize, &DecimalDigits, &Nullable)))
			return false;

		if (i > 1) collist << ", ";
		collist << "\"" << (char *)ColumnName << "\"";

//		if (!In(ColumnName, "source_system", "trade_id"//, "admin_id", "book", "branch", "bs_flag", "cpty", "cpty_entity", "ccy_pay_ml", "ccy_pay_cpty", "cob_date", "coper_id", "creation_date", "derivative_type", "end_date", "entity", "ex_venue", "int_book", "int_book_entity", "intra_company_trade", "latest_version", "notif_date", "notional_amt", "orig_trade_id", "orig_trade_status", "payment_amt", "price", "price_multiplier", "product_code", "product_subcode", "quantity", "rate", "region", "risk_entity_name", "settlement_date", "specialist_system_id", "start_date", "trade_date", "trade_status", "trade_time", "trade_ref", "trader_id", "trading_capacity", "update_count", "leg_parent_trade_id", "leg_type", "CUSIP", "ISIN", "u_ISIN", "MLInstrument", "sec_id_flag", "index_ticker", "security_description", "basket_name", "MIFID_reportable", "u_instrument_type", "cp_code", "system_reportable_flag", "risk_book", "bbg_agency_link_ref", "put_call", "strike_price", "option_premium", "spread", "option_premium_ccy", "coupon_rate", "accrued_interest", "ai_days", "coupon", "yield", "cancel_amend_reason", "cancel_amend_trader_name", "u_MLInstrument", "u_CUSIP", "rec_type", "description", "Desk_Name", "ticker", "Original_Source_System", "SubProductTypeName", "SubProductTypeDesc", "trade_type", "commodity_type", "coper_le_guid", "notional_1", "notional_ccy_1", "notional_2", "notional_ccy_2", "coverage", "deliverable", "contra_fwd_rate", "contra_spot_rate", "assignment_cpty", "broker_id", "option_premium_date", "option_style", "first_lower_barrier", "first_upper_barrier", "sourceSystemLegId", "cust_account_trader", "account_short_name", "mortgage_flag", "master_ticket_number", "slate_time", "ticket_time", "long_trade_time", "transaction_type", "usd_amount", "trans_no", "sec_id", "Note", "cancel_date", "capRate", "floorRate", "optionType", "GCINumber", "orig_trans_no", "quote_basis", "MTM", "MTM_USD", "MTM_CCY"

		//binary.Add(false);
		MySqlColumnInfo& f = myinfo.Add();
		SqlColumnInfo &flegacy = info.Add(); // Maintain old version ;)
		
		flegacy.nullable = f.nullable = Nullable != SQL_NO_NULLS;
		flegacy.precision = f.precision = DecimalDigits;
		flegacy.scale = f.scale = 0;
		flegacy.width = f.width = ColumnSize;
		flegacy.name = (char *)ColumnName;
		f.name = (char *)ColumnName;
		String nm((char *)ColumnName);
		 
		// My new property
		f.sqltype = DataType;
		f.isbinary = false;
		SQLLEN *indicatorptr = indicator + ((i-1) * ROW_FETCH_COUNT);
		
		switch(DataType) { // http://msdn.microsoft.com/en-us/library/ms710150(v=vs.85).aspx
		case SQL_INTEGER:
			{   // http://msdn.microsoft.com/en-us/library/ms714556(v=vs.85).aspx shows relationship between typedefs and identifiers
				SQLINTEGER *numdata = (SQLINTEGER *)new SQLINTEGER[ROW_FETCH_COUNT];
				rowdata.Add(nm, (byte *)numdata);
				f.type = INT_V;
				if (!IsOk(SQLBindCol(session->hstmt, i, SQL_C_SLONG, numdata, sizeof(SQLINTEGER), indicatorptr))) return false;
				break;
			}
			
		case SQL_TYPE_DATE:
			{
				SQL_DATE_STRUCT *datedata = (SQL_DATE_STRUCT *)new SQL_DATE_STRUCT[ROW_FETCH_COUNT];
				rowdata.Add(nm, (byte *)datedata);
				f.type = DATE_V;
				if (!IsOk(SQLBindCol(session->hstmt, i, SQL_C_TYPE_DATE, datedata, sizeof(SQL_DATE_STRUCT), indicatorptr))) return false;
				break;
			}
		case SQL_TYPE_TIMESTAMP:
			{
				SQL_TIMESTAMP_STRUCT *timedata = (SQL_TIMESTAMP_STRUCT *)new SQL_TIMESTAMP_STRUCT[ROW_FETCH_COUNT];
				rowdata.Add(nm, (byte *)timedata);
				f.type = TIME_V;
				if (!IsOk(SQLBindCol(session->hstmt, i, SQL_C_TYPE_TIMESTAMP, timedata, sizeof(SQL_TIMESTAMP_STRUCT), indicatorptr))) return false;
				break;
			}

		case SQL_VARCHAR: // Variable-length character string with a maximum string length n.
			{
				//DB2 Note: After a fetch for an SQL_VARCHAR column, DB2 for i5/OS CLI returns the bytes that are fetched in the first 2 bytes of the VARCHAR structure that is bound. DB2 for i5/OS CLI does not return the length in pcbValue as it does for SQL_CHAR.
				//http://publib.boulder.ibm.com/infocenter/iseries/v6r1m0/index.jsp?topic=/cli/rzadpfnbindc.htm
				SQLCHAR *strdata = (SQLCHAR *)new SQLCHAR[ROW_FETCH_COUNT * (ColumnSize + 1)]; 
				rowdata.Add(nm, (byte *)strdata);
				
				f.type = STRING_V;
				if (!IsOk(SQLBindCol(session->hstmt, i, SQL_C_CHAR, strdata, ColumnSize + 1, indicatorptr))) return false;
			}
			break;
		case SQL_CHAR:			
			{
				SQLCHAR *strdata = (SQLCHAR *)new SQLCHAR[ROW_FETCH_COUNT * (ColumnSize + 1)];
				rowdata.Add(nm, (byte *)strdata);
				f.type = STRING_V;
				if (!IsOk(SQLBindCol(session->hstmt, i, SQL_C_CHAR, strdata, ColumnSize + 1, indicatorptr))) return false;
				break;
			}
		case SQL_LONGVARCHAR: Exclamation("SQL_LONGVARCHAR not supported"); break;
		case SQL_WCHAR: // Unicode character string of fixed string length n
			Exclamation("SQL_WCHAR not supported"); break;
		case SQL_WVARCHAR: Exclamation("SQL_WVARCHAR not supported"); break;
		case SQL_WLONGVARCHAR: Exclamation("SQL_WLONGVARCHAR not supported"); break;
		case SQL_DOUBLE: // Signed, approximate, numeric value with a binary precision 53 (zero or absolute value 10[–308] to 10[308]).
		case SQL_REAL: // Signed, approximate, numeric value with a binary precision 24 (zero or absolute value 10[–38] to 10[38]).
		case SQL_FLOAT: // Signed, approximate, numeric value with a binary precision of at least p. (The maximum precision is driver-defined.)[5]
		case SQL_DECIMAL: // Signed, exact, numeric value with a precision of at least p and scale s. (The maximum precision is driver-defined.) (1 <= p <= 15; s <= p).[4]
		case SQL_NUMERIC: // Signed, exact, numeric value with a precision p and scale s  (1 <= p <= 15; s <= p).[4]
			{
				SQLDOUBLE *dbldata = (SQLDOUBLE *)new SQLDOUBLE[ROW_FETCH_COUNT];
				rowdata.Add(nm, (byte *)dbldata);
				f.type = DOUBLE_V;
				if (!IsOk(SQLBindCol(session->hstmt, i, SQL_C_DOUBLE, dbldata, sizeof(SQLDOUBLE), indicatorptr))) return false;
				break;
			}
			break;
		case SQL_SMALLINT: // Exact numeric value with precision 5 and scale 0 (signed: –32,768 <= n <= 32,767, unsigned: 0 <= n <= 65,535)[3].
			Exclamation("SQL_SMALLINT not supported"); break;
		case SQL_BIT: // Single bit binary
			{
				SQLCHAR *bitdata = (SQLCHAR *)new SQLCHAR[ROW_FETCH_COUNT];
				rowdata.Add(nm, (byte *)bitdata);
				f.type = BOOL_V;
				if (!IsOk(SQLBindCol(session->hstmt, i, SQL_C_BIT, bitdata, sizeof(SQLCHAR), indicatorptr))) return false;
				break;
			}
			break;
		case SQL_TINYINT: // Exact numeric value with precision 3 and scale 0 (signed: –128 <= n <= 127, unsigned: 0 <= n <= 255)[3].
			{
				SQLCHAR *intdata = (SQLCHAR *)new SQLCHAR[ROW_FETCH_COUNT];
				rowdata.Add(nm, (byte *)intdata);
				f.type = INT_V;
				if (!IsOk(SQLBindCol(session->hstmt, i, SQL_C_STINYINT, intdata, sizeof(SQLCHAR), indicatorptr))) return false;
				break;
			}
			break;
		case SQL_BIGINT: // Exact numeric value with precision 19 (if signed) or 20 (if unsigned) and scale 0 (signed: –2[63] <= n <= 2[63] – 1, unsigned: 0 <= n <= 2[64] – 1)[3],[9].
			{
				SQLBIGINT *intdata = (SQLBIGINT *)new SQLBIGINT[ROW_FETCH_COUNT];
				rowdata.Add(nm, (byte *)intdata);
				f.type = INT64_V;
				if (!IsOk(SQLBindCol(session->hstmt, i, SQL_C_SBIGINT, intdata, sizeof(SQLBIGINT), indicatorptr))) return false;
				break;
			}
		case SQL_BINARY: // Binary data of fixed length n.[9]
			Exclamation("SQL_BINARY not supported"); break;
		case SQL_VARBINARY: // Variable length binary data of maximum length n. The maximum is set by the user.[9]
			Exclamation("SQL_VARBINARY not supported"); break;
		case SQL_LONGVARBINARY: // Variable length binary data. Maximum length is data source–dependent.[9]
			Exclamation("SQL_LONGVARBINARY not supported"); break;
		case SQL_TYPE_TIME: // Hour, minute, and second fields, with valid values for hours of 00 to 23, valid values for minutes of 00 to 59, and valid values for seconds of 00 to 61. Precision p indicates the seconds precision.
			Exclamation("SQL_TYPE_TIME not supported"); break;
//		case SQL_TYPE_UTCDATETIME: // Year, month, day, hour, minute, second, utchour, and utcminute fields. The utchour and utcminute fields have 1/10 microsecond precision.
//			Exclamation("SQL_UTCDATETIME not supported"); break;
//		case SQL_TYPE_UTCTIME: // Hour, minute, second, utchour, and utcminute fields. The utchour and utcminute fields have 1/10 microsecond precision..
//			Exclamation("SQL_UTCTIME not supported"); break;
		case SQL_INTERVAL_MONTH:
			Exclamation("SQL_INTERVAL_MONTH not supported"); break;
		default:
			Exclamation("Unsupported type");
		}
		
		flegacy.type = f.type; // Copy to legacy
	}

	LOG(collist);
	SQLLEN rc;
	SQLRowCount(session->hstmt, &rc);
	rowsprocessed = rc;
	return true;
}

//==============================================================================================
int ODBCConnection::GetRowsProcessed() const {
	return rowsprocessed;
}

//==============================================================================================
int ODBCConnection::GetParseErrorPosition() const {
	return -1;
}

//==============================================================================================
// Extended support from U++ version: High-performance fetch
bool ODBCConnection::Fetch0() {
	LLOG("Fetch0 " << (void *)this << " " << (void *)session);
	int ret;
	// Fetch a batch set for reduced latency (huge difference)
	if (!fetchedSet) {
		ret = SQLExtendedFetch(session->hstmt, SQL_FETCH_NEXT, 0, &rowsFetched, rowStatus);
		if(ret == SQL_NO_DATA || !IsOk(ret)) {
			for(int i = 0; i < myinfo.GetCount(); i++) {
				if (rowdata.Find(myinfo[i].name) >= 0) {
					byte *b = rowdata.Get(myinfo[i].name);
					
					delete b;
				}
			}
			delete indicator;
			rowdata.Clear();
			return false;
		}
		fetchedSet = true; // So we don't run until we've returned all these
		nextFetchSetRow = 0; // SQLSetPos is 1-based, but return data is 0-based!
	}

	fetchrow.Clear();
//	double                 dbl;
//	int64                  n64;
	// Data Types Reference: http://msdn.microsoft.com/en-us/library/ms714556(v=vs.85).aspx
//	SQL_TIMESTAMP_STRUCT   tmstmp;
//	SQL_DATE_STRUCT        dt;
//	SQL_TIME_STRUCT        tm;
//	SQLLEN                 li;
//	SQLGUID                guid;
//	SQL_NUMERIC_STRUCT     n;
//	SQL_INTERVAL_STRUCT    iv;
//	SQLREAL                rl;    // Same as float
//	SQLFLOAT               flt;   // Same as double
//	SQLINTEGER             lint;  // long int
	
	// Walk through each column
	
	for(int i = 0; i < myinfo.GetCount(); i++) {
		int ColumnSize = myinfo[i].width;
		Value v = Null;
		SQLLEN ind = indicator[nextFetchSetRow + (ROW_FETCH_COUNT * i)];
		String nm = myinfo[i].name;
		
		if (ind != SQL_NULL_DATA) {
			byte *b = (byte *)rowdata.Get(nm);
			
			switch(myinfo[i].type) {

			case DOUBLE_V:
				{	
					SQLDOUBLE *x = (SQLDOUBLE *)&(b[(sizeof(SQLDOUBLE)) * nextFetchSetRow]);
					v = (double)*x;
				}
				break;
				
			case INT_V:
				{	
					SQLINTEGER *x = (SQLINTEGER *)&(b[(sizeof(SQLINTEGER)) * nextFetchSetRow]);
					v = (int64)*x;
				}
				break;
				
			case DATE_V:
				{	
					SQL_DATE_STRUCT *x = (SQL_DATE_STRUCT *)&(b[(sizeof(SQL_DATE_STRUCT)) * nextFetchSetRow]);
					Date d;
					d.year       = x->year;
					d.month      = (byte)x->month;
					d.day        = (byte)x->day;
					v = d;
				}
				break;
			
			case TIME_V: // Same for timestamps and date SQL Server types
				{	
					SQL_TIMESTAMP_STRUCT *x = (SQL_TIMESTAMP_STRUCT *)&(b[(sizeof(SQL_TIMESTAMP_STRUCT)) * nextFetchSetRow]);
					Time m;
					m.year       = x->year;
					m.month      = (byte)x->month;
					m.day        = (byte)x->day;
					m.hour       = (byte)x->hour;
					m.minute     = (byte)x->minute;
					m.second     = (byte)x->second;
					v = m;
				}
				break;

			case STRING_V:
				{
					SQLCHAR *x = (SQLCHAR *)&(b[(ColumnSize + 1) * nextFetchSetRow]);
					v = String((char *)(x));
					break;
				}

			case BOOL_V:
				{
					SQLCHAR *x = (SQLCHAR *)&(b[sizeof(SQLCHAR) * nextFetchSetRow]);
					v = *((bool *)(x));
					break;
				}
			default:
				v = "err";
			}
		}
		
		fetchrow.Add(v);
	}
	
	nextFetchSetRow++;
	
	if (nextFetchSetRow >= rowsFetched) {
		fetchedSet = false;
	}
	
	// When SQLFetch signals the end of the result set by returning SQL_NO_DATA, call SQLMoreResults to determine if another result set is available. 
	// To cancel processing a result set before SQLFetch returns SQL_NO_DATA, call SQLCloseCursor
	return true;
}

bool ODBCConnection::Fetch()
{
	if(IsCurrent())
		return Fetch0();
	if(rowi >= rowcount)
		return false;
	fetchrow.Clear();
	for(int i = 0; i < myinfo.GetCount(); i++) {
		Value v;
		switch(myinfo[i].type) {
		case DOUBLE_V:
			v = number[i][rowi];
			break;
		case INT64_V:
			v = num64[i][rowi];
			break;
		case TIME_V:
			v = time[i][rowi];
			break;
		default:
			v = text[i][rowi];
			break;
		}
		fetchrow.Add(v);
	}
	++rowi;
	return true;
}

void ODBCConnection::GetColumn(int i, Ref r) const
{
	LLOG("GetColumn " << (void *)this << " " << (void *)session);
	r.SetValue(fetchrow[i]);
}

void ODBCConnection::Flush()
{
	LLOG("Flush " << (void *)this);
	rowcount = 0;
	rowi = 0;
	number.Clear();
	number.SetCount(myinfo.GetCount());
	text.Clear();
	text.SetCount(myinfo.GetCount());
	time.Clear();
	time.SetCount(myinfo.GetCount());
	while(myinfo.GetCount() && Fetch0()) {
		rowcount++;
		for(int i = 0; i < myinfo.GetCount(); i++)
			switch(myinfo[i].type) {
			case DOUBLE_V:
				number[i].Add(fetchrow[i]);
				break;
			case INT64_V:
				num64[i].Add(fetchrow[i]);
				break;
			case STRING_V:
				text[i].Add(fetchrow[i]);
				break;
			case TIME_V:
				time[i].Add(fetchrow[i]);
				break;
			}
	}
	LLOG("Flush fetched " << rowcount);
}

void ODBCConnection::Cancel()
{
	param.Clear();
	bparam.Clear();
	number.Clear();
	text.Clear();
	time.Clear();
}

String ODBCConnection::ToString() const
{
	return statement;
}

Value ODBCConnection::GetInsertedId() const
{
	Sql sql(GetSession());
	return last_insert_table.GetCount() ? sql.Select("IDENT_CURRENT('" + last_insert_table + "')")
	                                    : sql.Select("@@IDENTITY");
}

END_UPP_NAMESPACE

/*
Copy #define ROWS 20
#define STATUS_LEN 6

SQLCHAR        szStatus[ROWS][STATUS_LEN], szReply[3];
SQLINTEGER     cbStatus[ROWS], cbOrderID;
SQLUSMALLINT   rgfRowStatus[ROWS];
SQLUINTEGER    sOrderID, crow = ROWS, irow;
SQLHSTMT       hstmtS, hstmtU;

SQLSetStmtAttr(hstmtS, SQL_ATTR_CONCURRENCY, (SQLPOINTER) SQL_CONCUR_ROWVER, 0);
SQLSetStmtAttr(hstmtS, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN, 0);
SQLSetStmtAttr(hstmtS, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER) ROWS, 0);
SQLSetStmtAttr(hstmtS, SQL_ATTR_ROW_STATUS_PTR, (SQLPOINTER) rgfRowStatus, 0);
SQLSetCursorName(hstmtS, "C1", SQL_NTS);
SQLExecDirect(hstmtS, "SELECT ORDERID, STATUS FROM ORDERS ", SQL_NTS);

SQLBindCol(hstmtS, 1, SQL_C_ULONG, &sOrderID, 0, &cbOrderID);
SQLBindCol(hstmtS, 2, SQL_C_CHAR, szStatus, STATUS_LEN, &cbStatus);

while ((retcode == SQLFetchScroll(hstmtS, SQL_FETCH_NEXT, 0)) != SQL_ERROR) {
   if (retcode == SQL_NO_DATA_FOUND)
      break;
   for (irow = 0; irow < crow; irow++) {
      if (rgfRowStatus[irow] != SQL_ROW_DELETED)
         printf("%2d %5d %*s\n", irow+1, sOrderID, NAME_LEN-1, szStatus[irow]);
   }
   while (TRUE) {
      printf("\nRow number to update?");
      gets_s(szReply, 3);
      irow = atoi(szReply);
      if (irow > 0 && irow <= crow) {
         printf("\nNew status?");
         gets_s(szStatus[irow-1], (ROWS * STATUS_LEN));
         SQLSetPos(hstmtS, irow, SQL_POSITION, SQL_LOCK_NO_CHANGE);
         SQLPrepare(hstmtU,
          "UPDATE ORDERS SET STATUS=? WHERE CURRENT OF C1", SQL_NTS);
         SQLBindParameter(hstmtU, 1, SQL_PARAM_INPUT,
            SQL_C_CHAR, SQL_CHAR,
            STATUS_LEN, 0, szStatus[irow], 0, NULL);
         SQLExecute(hstmtU);
      } else if (irow == 0) {
         break;
      }
   }
}
*/