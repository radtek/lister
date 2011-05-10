#include "Oracle8.h"
#include "OciCommon.h"
#pragma hdrstop

#include <CtrlLib/CtrlLib.h>
/*
	TODO:
		OCI_ATTR_PARSE_ERROR_OFFSET, OCI_ATTR_STMT_IS_RETURNING
		
	OCIReset() Resets the interrupted asynchronous operation and protocol. Must be called
if a OCIBreak call had been issued while a non-blocking operation was in
progress.

	
	
	TOCONSIDER:	
	OCI_ATTR_NUM_ROWS, OCI_ATTR_TRANS_TIMEOUT,
		OCI_ATTR_SERVER_STATUS, OCI_ATTR_DESCRIBE_NATIVE, OCI_ATTR_SERVER_BUSY,
		OCI_ATTR_ROWS_FETCHED, OCI_ATTR_ERRONEOUS_COLUMN, 
		
*/
NAMESPACE_UPP

#define LLOG(x) RLOG(x)

//#define DLLFILENAME "ora803.dll"
#define DLLFILENAME "oci.dll"
#define DLIMODULE   OCI8
#define DLIHEADER   <lister/Oracle/Oci8.dli>
#include <Core/dli_source.h>
//C:\FreeBase\Oracle\Client\instantclientwin3211.2.0.1\instantclient_11_2
//C:\Program Files\Oracle\product\10.2.0\client_1\bin
//In u++ first of all need to set library path explicit by using the method: OCI8SetDllPath(<full oci.dll/so address path>)
//or OCI7SetDllPath(<full oci.dll/so address path>) if you use oracle7.
//Not needed if they are in the same dir as .exe.
//
//You also need tnsnames.ora and sqlnet.ora config files in the same dir as .dlls.
//
//Mirek
//I did not know about tnsnames. my connection is dirrect to server like usser/pass@host:port/SID
//
//Add:
//I want to add that is possible to install Oracle Client Lite (only shared library files) by address:
//http://www.oracle.com/technology/software/tech/oci/instantcl ient/index.html
//According by documentation Oracle Call Interface 10.2 guide.	
//http://www.ultimatepp.org/srcdoc$Core$DLI$en-us.html

void OCI8SetDllPath(String oci8_path, T_OCI8& oci8)
{
	static String dflt_name;
	if(IsNull(dflt_name))
		dflt_name = oci8.GetLibName();
	if(oci8_path != oci8.GetLibName())
		oci8.SetLibName(Nvl(oci8_path, dflt_name));
}

static String OciError(T_OCI8& oci8, OCIError *errhp, int *code, bool utf8_session)
{
	if(code) *code = Null;
	if(!oci8) return t_("Error running OCI8 Oracle connection dynamic library.");
	if(!errhp) return t_("Unknown error.");
	OraText errbuf[512];
	strcpy((char *)errbuf, t_("(unknown error)"));
	sb4 errcode;
	oci8.OCIErrorGet(errhp, 1, NULL, &errcode, errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
	if(code) *code = errcode;
	String out = (const char *) errbuf;
	if(utf8_session)
		out = ToCharset(CHARSET_DEFAULT, out, CHARSET_UTF8);
	return out;
}

bool Oracle8::AllocOciHandle(void *hp, int type) {
	LLOG("AllocOciHandle(type " << type << "), envhp = " << FormatIntHex(envhp));
	*(dvoid **)hp = NULL;
	return oci8 && !oci8.OCIHandleAlloc(envhp, (dvoid **)hp, type, 0, NULL);
}

void Oracle8::FreeOciHandle(void *hp, int type) {
	LLOG("FreeOciHandle(" << FormatIntHex(hp) << ", type " << type << ")");
	if(oci8 && hp) oci8.OCIHandleFree(hp, type);
}

void Oracle8::SetOciError(String text, OCIError *from_errhp)
{
	int errcode;
	String msg = OciError(oci8, from_errhp, &errcode, utf8_session);
	SetError(msg, text, errcode, NULL, OciErrorClass(errcode));
}

class OCI8Connection : public Link<OCI8Connection>, public OciSqlConnection {
public:
	int                 positionofparseerror; // Set by Execute if error, -1 if no error
protected:
	virtual void        SetParam(int i, const Value& r);
	virtual void        SetParam(int i, OracleRef r);
	virtual bool        Execute(); // Implements abstract function from Sqls.h SqlConnection
	virtual int         GetRowsProcessed() const;
	virtual int         GetParseErrorPosition() const;
	virtual bool        Fetch();
	virtual void        GetColumn(int i, Ref f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual String      GetUser() const;
	virtual String      ToString() const;
	
	virtual            ~OCI8Connection();
	
	bool                BulkExecute(const char *stmt, const Vector< Vector<Value> >& param_rows);

	struct Item {
		T_OCI8&        oci8;
		int            type;
		int            total_len;
		Buffer<ub2>    len;
		Buffer<sb2>    ind;
//		ub2            rl;
		ub2            rc;
		Vector<Value>  dynamic;
		bool           is_dynamic;
		bool           dyna_full;
		int            dyna_vtype;
		ub4            dyna_width;
		ub4            dyna_len;
		int            array_count;
		OCILobLocator *lob;
		OCIBind       *bind;
		OCIDefine     *define;
		OCI8Connection *refcursor;
		union {
			byte          *ptr;
			byte           buffer[8];
		};

		byte *Data()                 { return total_len > sizeof(buffer) ? ptr : buffer; }
		const byte *Data() const     { return total_len > sizeof(buffer) ? ptr : buffer; }
		bool  IsNull() const         { return ind[0] < 0; }
		bool  Alloc(int count, OCIEnv *envhp, int type, int len, int res = 0);
		void  Clear();
		void  DynaFlush();

		static sb4 DynaIn(dvoid *ictxp, OCIBind *bindp,
			ub4 iter, ub4 index, dvoid **bufpp, ub4 *alenp, ub1 *piecep, dvoid **indp) {
			Item *self = reinterpret_cast<Item *>(ictxp);
			ASSERT(self -> bind == bindp);
			*bufpp = NULL;
			*alenp = 0;
			*indp = NULL;
			*piecep = OCI_ONE_PIECE;
			return OCI_CONTINUE;
		}

		static sb4 DynaOut(dvoid *octxp, OCIBind *bindp,
			ub4 iter, ub4 index, dvoid **bufpp, ub4 **alenp, ub1 *piecep, dvoid **indp, ub2 **rcodep) {
			Item *self = reinterpret_cast<Item *>(octxp);
			ASSERT(self -> bind == bindp);
			return self -> Out(iter, index, bufpp, alenp, piecep, indp, rcodep);
		}

		sb4 Out(ub4 iter, ub4 index, dvoid **bufpp, ub4 **alenp, ub1 *piecep, dvoid **indp, ub2 **rcodep);

		Item(T_OCI8& oci8);
		~Item();
	};


	Oracle8           *session;
	T_OCI8&            oci8;
	OCIStmt           *stmthp;
	OCIError          *errhp;
	Array<Item>        param;
	Array<Item>        column;
	String             parsed_cmd;
	Vector<int>        dynamic_param;
	int                dynamic_pos;
	int                dynamic_rows;
	int                fetched;
	int                fetchtime;
	bool               refcursor;

	OCISvcCtx *SvcCtx() const        { ASSERT(session); return session->svchp; }

	Item&     PrepareParam(int i, int type, int len, int reserve, int dynamic_vtype);
	void      SetParam(int i, const String& s);
	void      SetParam(int i, const WString& s);
	void      SetParam(int i, int integer);
	void      SetParam(int i, double d);
	void      SetParam(int i, Date d);
	void      SetParam(int i, Time d);
	void      SetParam(int i, Sql& refcursor);
	void      SetRawParam(int i, const String& s);

	void      AddColumn(int type, int len);
	void      GetColumn(int i, String& s) const;
	void      GetColumn(int i, WString& s) const;
	void      GetColumn(int i, int& n) const;
	void      GetColumn(int i, double& d) const;
	void      GetColumn(int i, Date& d) const;
	void      GetColumn(int i, Time& t) const;

	void      SetError();
	bool      GetColumnInfo();

	void      Clear();

	OCI8Connection(Oracle8& s);

	friend class Oracle8;
};

void OCI8Connection::Item::Clear() {
	ind.Clear();
	len.Clear();
	if(type == SQLT_BLOB || type == SQLT_CLOB)
		oci8.OCIDescriptorFree((dvoid *)lob, OCI_DTYPE_LOB);
	else
	if(total_len > sizeof(buffer))
		delete[] ptr;
	total_len = 0;
	lob = NULL;
	dynamic.Clear();
	dyna_full = false;
	dyna_vtype = VOID_V;
	dyna_width = 0;
}

OCI8Connection::Item::Item(T_OCI8& oci8_)
: oci8(oci8_)
{
	lob = NULL;
	bind = NULL;
	define = NULL;
	total_len = 0;
	refcursor = 0;
	dyna_full = false;
	is_dynamic = false;
}

OCI8Connection::Item::~Item() {
	Clear();
}

bool OCI8Connection::Item::Alloc(int _count, OCIEnv *envhp, int _type, int _len, int res) {
	if(_type == type && total_len >= _len && array_count >= _count) return false;
	Clear();
	type = _type;
	total_len = _len + res;
	array_count = _count;
	if(type == SQLT_BLOB || type == SQLT_CLOB)
		oci8.OCIDescriptorAlloc(envhp, (dvoid **) &lob, OCI_DTYPE_LOB, 0, NULL);
	if(total_len > sizeof(buffer))
		ptr = new byte[total_len];
	ind.Alloc(_count);
	ind[0] = -1;
	len.Alloc(_count);
	len[0] = _len;
	return true;
}

void OCI8Connection::Item::DynaFlush() {
	if(dyna_full) {
		dyna_full = false;
		Value v;
		if(ind[0] == 0) {
			if(dyna_len > dyna_width)
				dyna_width = dyna_len;
			const byte *p = Data();
			switch(type) {
			case SQLT_INT:
				v = *(const int *)p;
				break;

			case SQLT_FLT:
				v = *(const double *)p;
				break;

			case SQLT_STR:
				v = String((const char *)p, dyna_len);
				break;

			case SQLT_DAT:
				v = OciDecodeTime(p);
				break;

			case SQLT_CLOB:
			case SQLT_BLOB:
			default:
				NEVER();
				break;
			}
		}
		dynamic.Add(v);
	}
}

sb4 OCI8Connection::Item::Out(ub4 iter, ub4 index, dvoid **bufpp, ub4 **alenp, ub1 *piecep, dvoid **indp, ub2 **rcodep) {
	DynaFlush();
	*bufpp = Data();
	*alenp = &dyna_len;
	dyna_len = total_len;
	*piecep = OCI_ONE_PIECE;
	ind.Alloc(1);
	ind[0] = 0;
	*indp = ind;
	rc = 0;
	*rcodep = &rc;
	dyna_full = true;
	return OCI_CONTINUE;
}

OCI8Connection::Item& OCI8Connection::PrepareParam(int i, int type, int len, int res, int dynamic_vtype) {
	while(param.GetCount() <= i)
		param.Add(new Item(oci8));
	Item& p = param[i];
	if(p.Alloc(1, session -> envhp, type, len, res))
		parse = true;
	p.dyna_vtype = dynamic_vtype;
	p.is_dynamic = (dynamic_vtype != VOID_V);
	return p;
}

void OCI8Connection::SetParam(int i, const String& s) {
	String rs = (session->utf8_session ? ToCharset(CHARSET_UTF8, s) : s);
	int l = rs.GetLength();
	Item& p = PrepareParam(i, SQLT_STR, l + 1, 100, VOID_V);
	memcpy(p.Data(), rs, l + 1);
	p.ind[0] = l ? 0 : -1;
}

void OCI8Connection::SetParam(int i, const WString& s) {
	String rs = (session->utf8_session ? ToUtf8(s) : s.ToString());
	int l = rs.GetLength();
	Item& p = PrepareParam(i, SQLT_STR, l + 1, 100, VOID_V);
	memcpy(p.Data(), rs, l + 1);
	p.ind[0] = l ? 0 : -1;
}

void OCI8Connection::SetParam(int i, int integer) {
	Item& p = PrepareParam(i, SQLT_INT, sizeof(int), 0, VOID_V);
	*(int *) p.Data() = integer;
	p.ind[0] = IsNull(integer) ? -1 : 0;
}

void OCI8Connection::SetParam(int i, double d) {
	Item& p = PrepareParam(i, SQLT_FLT, sizeof(double), 0, VOID_V);
	*(double *) p.Data() = d;
	p.ind[0] = IsNull(d) ? -1 : 0;
}

void OCI8Connection::SetParam(int i, Date d) {
	Item& w = PrepareParam(i, SQLT_DAT, 7, 0, VOID_V);
	w.ind[0] = (OciEncodeDate(w.Data(), d) ? 0 : -1);
}

void OCI8Connection::SetParam(int i, Time t) {
	Item& w = PrepareParam(i, SQLT_DAT, 7, 0, VOID_V);
	w.ind[0] = (OciEncodeTime(w.Data(), t) ? 0 : -1);
}

void OCI8Connection::SetParam(int i, OracleRef r) {
	PrepareParam(i, r.GetOraType(), r.GetMaxLen(), 0, r.GetType());
}

void OCI8Connection::SetRawParam(int i, const String& s) {
	int l = s.GetLength();
	Item& p = PrepareParam(i, SQLT_LBI, l, 0, VOID_V);
	memcpy(p.Data(), s, l);
	p.ind[0] = l ? 0 : -1;
}

class Oracle8RefCursorStub : public SqlSource {
public:
	Oracle8RefCursorStub(SqlConnection *cn) : cn(cn) {}
	virtual SqlConnection *CreateConnection() { return cn; }

private:
	SqlConnection *cn;
};

void OCI8Connection::SetParam(int i, Sql& rc) {
	Item& w = PrepareParam(i, SQLT_RSET, -1, 0, VOID_V);
	w.refcursor = new OCI8Connection(*session);
	w.refcursor -> refcursor = true;
	*(OCIStmt **)w.Data() = w.refcursor -> stmthp;
	w.ind[0] = 0;
	Oracle8RefCursorStub stub(w.refcursor);
	rc = stub;
}

void  OCI8Connection::SetParam(int i, const Value& q) {
	if(q.IsNull())
		SetParam(i, String());
	else
		switch(q.GetType()) {
			case SQLRAW_V:
				SetRawParam(i, SqlRaw(q));
				break;
			case STRING_V:
				SetParam(i, String(q));
				break;
			case WSTRING_V:
				SetParam(i, WString(q));
				break;
			case BOOL_V:
			case INT_V:
				SetParam(i, int(q));
				break;
			case INT64_V:
			case DOUBLE_V:
				SetParam(i, double(q));
				break;
			case DATE_V:
				SetParam(i, Date(q));
				break;
			case TIME_V:
				SetParam(i, (Time)q);
				break;
			case UNKNOWN_V:
				if(IsTypeRaw<Sql *>(q)) {
					SetParam(i, *ValueTo<Sql *>(q));
					break;
				}
				if(OracleRef::IsValue(q)) {
					SetParam(i, OracleRef(q));
					break;
				}
			default:
				NEVER();
		}
}

void OCI8Connection::AddColumn(int type, int len) {
	// buffer allocated here! Point of 1406 error (Truncation)
	column.Add(new Item(oci8)).Alloc(1, session -> envhp, type, len);
}

bool OCI8Connection::BulkExecute(const char *stmt, const Vector< Vector<Value> >& param_rows)
{
	ASSERT(session);

	int time = msecs();
	int args = 0;
	String cvt_stmt = ToCharset(session->utf8_session
		? CHARSET_UTF8 : CHARSET_DEFAULT, stmt, CHARSET_DEFAULT);
	if((args = OciParse(cvt_stmt, parsed_cmd, this, session)) < 0)
		return false;

	session->statement = parsed_cmd;
	int nrows = param_rows.GetCount();

	if(Stream *s = session->GetTrace()) {
		*s << "BulkExecute(#" << nrows << " rows)\n";
		for(int r = 0; r < nrows; r++) {
			const Vector<Value>& row = param_rows[r];
			*s << "[row #" << r << "] ";
			bool quotes = false;
			int argn = 0;
			for(const char *q = cvt_stmt; *q; q++) {
				if(*q== '\'' && q[1] != '\'')
					quotes = !quotes;
				if(!quotes && *q == '?') {
					if(argn < row.GetCount())
						*s << SqlCompile(ORACLE, SqlFormat(row[argn++]));
					else
						*s << t_("<not supplied>");
				}
				else
					*s << *q;
			}
			*s << "\n";
		}
		*s << "//BulkExecute\n";
	}

	if(oci8.OCIStmtPrepare(stmthp, errhp, (byte *)~parsed_cmd, parsed_cmd.GetLength(), OCI_NTV_SYNTAX,
		OCI_DEFAULT)) {
		SetError();
		return false;
	}

	for(int a = 0; a < args; a++) {
		int max_row_len = 1;
		int sql_type = 0;
		for(int r = 0; r < nrows; r++) {
			Value v = (a < param_rows[r].GetCount() ? param_rows[r][a] : Value());
			int len = 0;
			if(!IsNull(v)) {
				if(IsNumber(v)) {
					if((v.GetType() == INT_V || v.GetType() == BOOL_V)
					&& (!sql_type || sql_type == SQLT_INT)) {
						sql_type = SQLT_INT;
						len = sizeof(int);
					}
					else if(!sql_type || sql_type == SQLT_INT || sql_type == SQLT_FLT) {
						sql_type = SQLT_FLT;
						len = sizeof(double);
					}
					else {
						RLOG("invalid type combination in BulkExecute: " << sql_type << " <- number");
					}
				}
				else if(IsDateTime(v)) {
					if(!sql_type || sql_type == SQLT_DAT) {
						sql_type = SQLT_DAT;
						len = 7;
					}
					else {
						RLOG("invalid type combination in BulkExecute: " << sql_type << " <- date/time");
					}
				}
				else if(IsString(v)) {
					if(!sql_type || sql_type == SQLT_STR) {
						sql_type = SQLT_STR;
						if(session->utf8_session) {
							WString wstr(v);
							len = 1 + lenAsUtf8(wstr, wstr.GetLength());
						}
						else
							len = 1 + String(v).GetLength();
					}
					else {
						RLOG("invalid type combination in BulkExecute: " << sql_type << " <- string");
					}
				}
				else {
					RLOG("invalid data type: " << v.GetType());
				}
			}
			if(len > max_row_len)
				max_row_len = len;
		}

		if(sql_type == 0)
			sql_type = SQLT_STR;
		int sum_len = nrows * max_row_len;
		Item& p = param.Add(new Item(oci8));
		p.Alloc(nrows, session->envhp, sql_type, sum_len, 0);
		p.dyna_vtype = VOID_V;
		p.is_dynamic = false;
		sb2 *indp = p.ind;
		ub2 *lenp = p.len;

		switch(sql_type) {
			case SQLT_INT: {
				ASSERT(sum_len >= nrows * (int)sizeof(int));
				int *datp = (int *)p.Data();
				for(int r = 0; r < nrows; r++) {
					int i = (param_rows[r].GetCount() > a ? (int)param_rows[r][a] : (int)Null);
					*datp++ = i;
					*indp++ = IsNull(i) ? -1 : 0;
					*lenp++ = sizeof(int);
				}
				break;
			}
			case SQLT_FLT: {
				ASSERT(sum_len >= nrows * (int)sizeof(double));
				double *datp = (double *)p.Data();
				for(int r = 0; r < nrows; r++) {
					double d = (param_rows[r].GetCount() > a ? (double)param_rows[r][a] : (double)Null);
					*datp++ = d;
					*indp++ = IsNull(d) ? -1 : 0;
					*lenp++ = sizeof(double);
				}
				break;
			}
			case SQLT_DAT: {
				ASSERT(sum_len >= nrows * (int)7u);
				byte *datp = p.Data();
				for(int r = 0; r < nrows; r++) {
					Time d = (param_rows[r].GetCount() > a ? (Time)param_rows[r][a] : (Time)Null);
					*indp++ = OciEncodeTime(datp, d) ? 0 : -1;
					datp += 7;
					*lenp++ = 7;
				}
				break;
			}
			case SQLT_STR: {
				byte *datp = p.Data();
				for(int r = 0; r < nrows; r++) {
					String s;
					if(session->utf8_session)
						s = ToUtf8(param_rows[r].GetCount() > a ? (WString)param_rows[r][a] : WString());
					else
						s = (param_rows[r].GetCount() > a ? (String)param_rows[r][a] : String());
					*indp++ = (IsNull(s) ? -1 : 0);
					int rawlen = s.GetLength() + 1;
					*lenp++ = rawlen;
					memcpy(datp, s, rawlen);
					datp += max_row_len;
				}
				ASSERT((unsigned)(datp - (byte *)p.Data()) <= (unsigned)sum_len);
				break;
			}
			default: {
				RLOG("unsupported SQL type: " << sql_type);
			}
		}

		if(oci8.OCIBindByPos(stmthp, &p.bind, errhp, a + 1, p.Data(), max_row_len, p.type,
			p.ind, p.len, NULL, nrows, NULL, OCI_DEFAULT)) {
			SetError();
			return false;
		}
	}

	/* set non-blocking on */ 
	sword retval;
	
	if (retval = oci8.OCIAttrSet(SvcCtx(), (ub4) OCI_HTYPE_SERVER, (dvoid *) 0, (ub4) 0, (ub4) OCI_ATTR_NONBLOCKING_MODE, errhp)) { 
	    SetError();
	    session->PostError(); 
	    return false;
	} 

	// Is nrows a limit?
	
	while ((retval = oci8.OCIStmtExecute(SvcCtx(), stmthp, errhp, nrows, 0, (OCISnapshot *) 0, (OCISnapshot *)0 
	,	session->StdMode() && session->level == 0 ? OCI_COMMIT_ON_SUCCESS : OCI_DEFAULT)) == OCI_STILL_EXECUTING) {
		// ProcessEvents();
		;
	}
	
	if (retval != OCI_SUCCESS || retval != OCI_SUCCESS_WITH_INFO) {
		SetError();
		session->PostError();
		return false;
	}

	if(Stream *s = session->GetTrace()) {
		if(session->IsTraceTime())
			*s << Format("----- exec %d ms\n", msecs(time));
	}
	return true;
}

bool Oracle8::BulkExecute(const SqlStatement& stmt, const Vector< Vector<Value> >& param_rows)
{
	if(param_rows.IsEmpty())
		return true;
	return BulkExecute(stmt.Get(ORACLE), param_rows);
}

// Impl of Sqls.h abstract function Upp::SqlConnection::Execute(void)
bool OCI8Connection::Execute() {
	ASSERT(session);
	int time = msecs();
	int args = 0;
	sword retval;
	positionofparseerror = -1;
	
	if (parse) {

		String cvt_stmt = ToCharset(session->utf8_session ? CHARSET_UTF8 : CHARSET_DEFAULT, statement, CHARSET_DEFAULT);
			
		if ((args = OciParse(cvt_stmt, parsed_cmd, this, session)) < 0) {
			// What does this command do?
			// Error is not set?
			return false;
		}

		ub4 fr = fetchrows;

		oci8.OCIAttrSet(stmthp, OCI_HTYPE_STMT, &fr, 0, OCI_ATTR_PREFETCH_ROWS, errhp); // TODO: Analyze value of PREFETCH in our latent environment
				
		if (oci8.OCIStmtPrepare(stmthp, errhp, (byte *)~parsed_cmd, parsed_cmd.GetLength(), OCI_NTV_SYNTAX, OCI_DEFAULT)) {
			// TODO: Get location of parse failure
			SetError();
			//OCI_ATTR_PARSE_ERROR_OFFSET or OCI_ATTR_ERRONEOUS_COLUMN
			return false;
		}

		while (param.GetCount() < args)
			SetParam(param.GetCount(), String());
		
		param.Trim(args);
		dynamic_param.Clear();
		
		for (int i = 0; i < args; i++) {
			Item& p = param[i];

			if (oci8.OCIBindByPos(
						stmthp
					,	&p.bind
					,	errhp
					,	i + 1
					,	p.Data()
					,	p.total_len
					,	p.type
					,	p.ind
					,	NULL
					,	NULL
					,	0
					,	NULL
					,	p.is_dynamic ? OCI_DATA_AT_EXEC : OCI_DEFAULT
					)
				) {
				SetError();
				return false;
			}
			if (p.is_dynamic) {
				dynamic_param.Add(i);
				if (oci8.OCIBindDynamic(p.bind, errhp, &p, &Item::DynaIn, &p, &Item::DynaOut)) {
					SetError();
					return false;
				}
			}
		}
	}

	/* set non-blocking on (Jeff) for this call only (StmtExecute) */ 
	ub1 nonblockingflag = 1;
	
	// OCI_ATTR_NONBLOCKING_MODE: You must set this attribute only after OCISessionBegin() or OCILogon2() has been called.
	retval = oci8.OCIAttrSet(session->srvhp, OCI_HTYPE_SERVER, 0, 0, OCI_ATTR_NONBLOCKING_MODE, errhp);

	if (retval != OCI_SUCCESS && retval != OCI_SUCCESS_WITH_INFO) {
		SetError();
		session->PostError();
		return false;
	}

	// Determine what the command type is, not sure why this is passed in.  Control Fetch/Prefetch?
	
	ub2 type;
	if (
			oci8.OCIAttrGet(
				stmthp
			,	OCI_HTYPE_STMT
			,	&type
			,	NULL
			,	OCI_ATTR_STMT_TYPE
			,	errhp
			)
		)
	{
		SetError();
		return false;
	}

	// Asynchronously execute the statement until complete or OCICancel is called elsewhere
	
	while (
			(
				retval = oci8.OCIStmtExecute(
					SvcCtx()
				,	stmthp
				,	errhp
				,	type != OCI_STMT_SELECT
				,	0
				,	(OCISnapshot *) 0
				,	(OCISnapshot *) 0
				,	session->StdMode() && session->level == 0 ? OCI_COMMIT_ON_SUCCESS : OCI_DEFAULT
				)
			) == OCI_STILL_EXECUTING
		)
	{
		// Let the GUI process user mouse and other events if anyone set the main window event loop.
		// Without this, the screen will go white and cease to Paint.

		if (WhenStillExecuting) {
			WhenStillExecuting();
		}
		
		Sleep(10); // Pause a bit, don't eat all the CPU
		// TODO: OCIReset()???  OCIBreak???

	}

	if (retval != OCI_SUCCESS && retval != OCI_SUCCESS_WITH_INFO) {
		// Trap User cancel "Error" Do OCI Reset?
		SetError();
		session->PostError();
		int errcode = session->GetErrorCode();
		if (errcode == 1013) {
			oci8.OCIReset(SvcCtx(), errhp); // This is required if an OCIBreak has been issued.
			session->ClearError(); 
		} else { // #918 = column ambiguously defined
			ub2 lpositionofparseerror;
			// Try to fetch the position of the error if there was one returned
			if (
					oci8.OCIAttrGet(
						stmthp
					,	OCI_HTYPE_STMT
					,	&lpositionofparseerror
					,	NULL
					,	OCI_ATTR_PARSE_ERROR_OFFSET
					,	errhp
					)
				)
			{
				return false;
			} else {
				// return position to caller
				positionofparseerror = lpositionofparseerror;
			}
		}
		return false;
	}
	
	// Turn server non-blocking off to prevent crashes on non-DML commands that may follow
	
	nonblockingflag = 0; // zero = false = turn off async(non-blocking) mode
	
	retval = oci8.OCIAttrSet(session->srvhp, OCI_HTYPE_SERVER, &nonblockingflag, sizeof(nonblockingflag), OCI_ATTR_NONBLOCKING_MODE, errhp);

	if (retval != OCI_SUCCESS && retval != OCI_SUCCESS_WITH_INFO) {
		SetError();
		session->PostError();
		return false;
	}

	if(!dynamic_param.IsEmpty()) {
		dynamic_pos = -1;
		for(int i = 0; i < dynamic_param.GetCount(); i++)
			param[dynamic_param[i]].DynaFlush();
		dynamic_rows = param[dynamic_param[0]].dynamic.GetCount();
	}

	if(parse) {
		if(!GetColumnInfo())
			return false;
		for(int i = 0; i < param.GetCount(); i++)
			if(param[i].refcursor && !param[i].refcursor -> GetColumnInfo())
				return false;
	}
	fetched = 0;
	fetchtime = 0;
	if(Stream *s = session->GetTrace()) {
		*s << ToString() << '\n';
		if(session->IsTraceTime())
			*s << Format("----- exec %d ms\n", msecs(time));
	}
	return true;
}

bool OCI8Connection::GetColumnInfo() {
	info.Clear();
	column.Clear();
	ub4 argcount;
	if(oci8.OCIAttrGet(stmthp, OCI_HTYPE_STMT, &argcount, 0, OCI_ATTR_PARAM_COUNT, errhp) != OCI_SUCCESS) {
		SetError();
		return false;
	}
	if(!dynamic_param.IsEmpty()) {
		for(int i = 0; i < dynamic_param.GetCount(); i++) {
			const Item& p = param[dynamic_param[i]];
			SqlColumnInfo& ci = info.Add();
			ci.name = Format("#%d", i + 1);
			ci.valuetype = p.dyna_vtype;
			ci.width = p.dyna_width;
			ci.precision = Null;
			ci.scale = Null;
		}
		parse = false;
		return true;
	}
	for(ub4 i = 1; i <= argcount; i++) {
		OCIParam *pd;
		if(oci8.OCIParamGet(stmthp, OCI_HTYPE_STMT, errhp, (dvoid **)&pd, i) != OCI_SUCCESS) {
			SetError();
			return false;
		}
		char *name;
		ub4 name_len;
		ub2 type, width;
		ub2 prec;
		sb1 scale;
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &type, NULL,  OCI_ATTR_DATA_TYPE, errhp);
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &width, NULL, OCI_ATTR_DATA_SIZE, errhp);
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &name, &name_len, OCI_ATTR_NAME, errhp);
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &prec, NULL,  OCI_ATTR_PRECISION, errhp);
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &scale, NULL,  OCI_ATTR_SCALE, errhp);
		SqlColumnInfo& ii = info.Add();

		ii.width           = width;
		ii.precision       = prec;
		ii.scale           = scale;
		ii.name            = ToUpper(TrimRight(String(name, name_len)));
		
		bool blob = false;
		switch(type) {
		case SQLT_NUM:
			ii.valuetype = DOUBLE_V;
			AddColumn(SQLT_FLT, sizeof(double));
			break;
		case SQLT_DAT:
			ii.valuetype = TIME_V;
			AddColumn(SQLT_DAT, 7);
			break;
		case SQLT_BLOB:
			ii.valuetype = ORA_BLOB_V;
			AddColumn(SQLT_BLOB, sizeof(OCILobLocator *));
			blob = true;
			break;
		case SQLT_CLOB:
			ii.valuetype = ORA_CLOB_V;
			AddColumn(SQLT_CLOB, sizeof(OCILobLocator *));
			blob = true;
			break;
		case SQLT_RDD:
			ii.valuetype = STRING_V;
			AddColumn(SQLT_STR, 64);
			break;
		case SQLT_TIMESTAMP: // This was found in CSDR database, TRADE table
			//AddColumn(SQLT_TIMESTAMP, sizeof(OCIDateTime *));
			ii.valuetype = STRING_V;
			// ii.width for type 187 (TIMESTAMP) is 11, but that is not the string size, but byte size.
			// The size as a string comes out according to what your default timestamp format is.
			// For me, <03-DEC-10 12.00.00.000000 AM >is the default value, 30 characters.  A
			// 4-character year would be 32.
			AddColumn(SQLT_STR, 32); 
			break;
		case SQLT_TIMESTAMP_TZ: // TODO: Customize
			//ASSERT_(1==0, "SQLT_TIMESTAMP_TZ in GetColumnInfo not supported yet.  Please code.");
			Exclamation("SQLT_TIMESTAMP_TZ in GetColumnInfo not supported yet.  Please code.");
			return false;
			break;
		case SQLT_TIMESTAMP_LTZ: // TODO: Customize
			//ASSERT_(1==0, "SQLT_TIMESTAMP_LTZ in GetColumnInfo not supported yet.  Please code.");
			Exclamation("SQLT_TIMESTAMP_LTZ in GetColumnInfo not supported yet.  Please code.");
			return false;
			break;
		default:
			ii.valuetype = STRING_V;
			AddColumn(SQLT_STR, ii.width ? ii.width + 1 : longsize);
			break;
		}
		Item& c = column.Top();
		oci8.OCIDefineByPos(stmthp, &c.define, errhp, i,
			blob ? (void *)&c.lob : (void *)c.Data(), blob ? -1 : c.total_len,
			c.type, c.ind, c.len, NULL, OCI_DEFAULT);
	}
	parse = false;
	return true;
}

int OCI8Connection::GetRowsProcessed() const {
	if(!dynamic_param.IsEmpty())
		return dynamic_pos + 1;
	ub4 rp = 0;
	oci8.OCIAttrGet(stmthp, OCI_HTYPE_STMT, &rp, NULL, OCI_ATTR_ROW_COUNT, errhp);
	return rp;
}

int OCI8Connection::GetParseErrorPosition() const {
	return positionofparseerror;
}

bool OCI8Connection::Fetch() {
	ASSERT(!parse);
	if(parse) return false;
	if(!dynamic_param.IsEmpty()) // dynamic pseudo-fetch
		return (dynamic_pos < dynamic_rows && ++dynamic_pos < dynamic_rows);
	int fstart = msecs();

	/* set non-blocking on (Jeff) for this call only (OCIStmtFetch) */ 
	ub1 nonblockingflag = 1;
	
	sword retval;
	
	// OCI_ATTR_NONBLOCKING_MODE: You must set this attribute only after OCISessionBegin() or OCILogon2() has been called.
	// Passed parameters (0, 0) are ignored.
	retval = oci8.OCIAttrSet(session->srvhp, OCI_HTYPE_SERVER, 0, 0, OCI_ATTR_NONBLOCKING_MODE, errhp);

	if (retval != OCI_SUCCESS && retval != OCI_SUCCESS_WITH_INFO) {
		SetError();
		session->PostError();
		return false;
	}

	sword status;
	
	while (
			(
				status = oci8.OCIStmtFetch(
					stmthp
				,	errhp
				,	1
				,	OCI_FETCH_NEXT
				,	OCI_DEFAULT
				)
			) == OCI_STILL_EXECUTING // Only returned if OCI_ATTR_NONBLOCKING_MODE has been set
		)
	{
		// Let the GUI process user mouse and other events if anyone set the main window event loop.
		// Without this, the screen will go white and cease to Paint.

		if (WhenStillExecuting) {
			WhenStillExecuting();
		}
		
		Sleep(10); // Pause a bit, don't eat all the CPU
		
		// TODO: OCIReset()???
	}
		
	bool ok = false;

	if(status == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
		fetchtime += msecs(fstart);
		++fetched;
		ok = true;
	}
	else if(status != OCI_NO_DATA)
		SetError();
	
	// Turn server non-blocking off to prevent crashes on non-DML commands that may follow
	
	nonblockingflag = 0; // zero = false = turn off async(non-blocking) mode
	
	retval = oci8.OCIAttrSet(session->srvhp, OCI_HTYPE_SERVER, &nonblockingflag, sizeof(nonblockingflag), OCI_ATTR_NONBLOCKING_MODE, errhp);

	if (retval != OCI_SUCCESS && retval != OCI_SUCCESS_WITH_INFO) {
		SetError();
		session->PostError();
		return false;
	}

	if(Stream *s = session->GetTrace()) {
		if(!ok || fetched % 100 == 0 && fetchtime)
			*s << NFormat("----- fetch(%d) in %d ms, %8n ms/rec, %2n rec/s\n",
				fetched, fetchtime,
				fetchtime / max<double>(fetched, 1),
				fetched * 1000.0 / max<double>(fetchtime, 1));
	}
	
	return ok;
}

void OCI8Connection::GetColumn(int i, String& s) const {
	if(!dynamic_param.IsEmpty()) {
		s = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Item& c = column[i];
	if(c.type == SQLT_BLOB || c.type == SQLT_CLOB) {
		int handle;
		GetColumn(i, handle);
		if(!IsNull(handle)) {
			OracleBlob blob(*session, handle);
			s = LoadStream(blob);
			if(session->utf8_session && c.type == SQLT_CLOB)
				s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
		}
		else
			s = Null;
		return;
	}
	ASSERT(c.type == SQLT_STR);
	if(c.ind[0] < 0)
		s = Null;
	else {
		s = (char *) c.Data();
		if(session->utf8_session)
			s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
	}
}

void OCI8Connection::GetColumn(int i, WString& ws) const {
	if(!dynamic_param.IsEmpty()) {
		ws = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Item& c = column[i];
	if(c.type == SQLT_BLOB || c.type == SQLT_CLOB) {
		int handle;
		GetColumn(i, handle);
		if(!IsNull(handle)) {
			OracleBlob blob(*session, handle);
			String s = LoadStream(blob);
			if(session->utf8_session && c.type == SQLT_CLOB)
				ws = FromUtf8(s);
			else
				ws = s.ToWString();
		}
		else
			ws = Null;
		return;
	}
	ASSERT(c.type == SQLT_STR);
	String s;
	if(c.ind[0] < 0)
		s = Null;
	else
		s = (char *) c.Data();
	if(session->utf8_session)
		ws = FromUtf8(s);
	else
		ws = s.ToWString();
}

void OCI8Connection::GetColumn(int i, double& n) const {
	if(!dynamic_param.IsEmpty()) {
		n = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Item& c = column[i];
	ASSERT(c.type == SQLT_FLT || c.type == SQLT_BLOB || c.type == SQLT_CLOB);
	if(c.ind[0] < 0)
		n = DOUBLE_NULL;
	else
		n = c.type == SQLT_BLOB || c.type == SQLT_CLOB ? (int)(uintptr_t)c.lob : *(double *) c.Data();
}

void OCI8Connection::GetColumn(int i, int& n) const {
	double d;
	GetColumn(i, d);
	n = IsNull(d) ? (int) Null : (int) d;
}

void OCI8Connection::GetColumn(int i, Date& d) const {
	if(!dynamic_param.IsEmpty()) {
		d = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Item& c = column[i];
	const byte *data = c.Data();
	ASSERT(c.type == SQLT_DAT);
	if(c.ind[0] < 0)
		d = Null; // d.year = d.month = d.day = 0;
	else
		d = OciDecodeDate(data);
}

void OCI8Connection::GetColumn(int i, Time& t) const {
	if(!dynamic_param.IsEmpty()) {
		t = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Item& c = column[i];
	const byte *data = c.Data();
	ASSERT(c.type == SQLT_DAT);
	if(c.ind[0] < 0)
		t = Null; // t.year = t.month = t.day = 0;
	else
		t = OciDecodeTime(data);
}

void  OCI8Connection::GetColumn(int i, Ref f) const {
	if(!dynamic_param.IsEmpty()) {
		f.SetValue(param[dynamic_param[i]].dynamic[dynamic_pos]);
		return;
	}
	switch(f.GetType()) {
		case WSTRING_V: {
			WString ws;
			GetColumn(i, ws);
			f.SetValue(ws);
			break;
		}
		case STRING_V: {
			String s;
			GetColumn(i, s);
			f.SetValue(s);
			break;
		}
		case BOOL_V:
		case INT_V: {
			int d;
			GetColumn(i, d);
			f.SetValue(d);
			break;
		}
		case INT64_V:
		case DOUBLE_V: {
			double d;
			GetColumn(i, d);
			f.SetValue(d);
			break;
		}
		case DATE_V: {
			Date d;
			GetColumn(i, d);
			f.SetValue(d);
			break;
		}
		case TIME_V: {
			Time t;
			GetColumn(i, t);
			f.SetValue(t);
			break;
		}
		case VALUE_V: {
			switch(column[i].type) {
				case SQLT_STR:
				/*case SQLT_RDD:*/ {
					String s;
					GetColumn(i, s);
					f.SetValue(s);
					break;
				}
				case SQLT_BLOB:
				case SQLT_CLOB:
				case SQLT_FLT: {
					double d;
					GetColumn(i, d);
					f.SetValue(d);
					break;
				}
				case SQLT_DAT: {
					Time m;
					GetColumn(i, m);
					if(m.hour || m.minute || m.second)
						f = Value(m);
					else
						f = Value(Date(m));
					break;
				}
				default: {
					NEVER();
				}
			}
			break;
		}
		default: {
			NEVER();
		}
	}
}

void OCI8Connection::Cancel() {
	oci8.OCIBreak(SvcCtx(), errhp);
	parse = true;
}

void OCI8Connection::SetError() {
	if(session)
		session->SetOciError(statement, errhp);
	parse = true;
}

SqlSession& OCI8Connection::GetSession() const {
	ASSERT(session);
	return *session;
}

String OCI8Connection::GetUser() const {
	ASSERT(session);
	return session->user;
}

String OCI8Connection::ToString() const {
	String lg;
	bool quotes = false;
	int argn = 0;
	for(const char *q = statement; *q; q++) {
		if(*q== '\'' && q[1] != '\'')
			quotes = !quotes;
		if(!quotes && *q == '?') {
			if(argn < param.GetCount()) {
				const Item& m = param[argn++];
				if(m.IsNull())
					lg << "Null";
				else
				switch(m.type) {
				case SQLT_STR:
					lg.Cat('\'');
					lg += (const char *) m.Data();
					lg.Cat('\'');
					break;
				case SQLT_FLT:
					lg << *(const double *) m.Data();
					break;
				case SQLT_DAT:
					lg << OciDecodeTime(m.Data());
//					const byte *data = m.Data();
//					lg << (int)data[3] << '.' << (int)data[2] << '.' <<
//						  int(data[0] - 100) * 100 + data[1] - 100;
					break;
				}
			}
			else
				lg += t_("<not supplied>");
		}
		else
			lg += *q;
	}
	return lg;
}

static int conn_count = 0;

OCI8Connection::OCI8Connection(Oracle8& s)
: session(&s)
, oci8(s.oci8)
{
	LLOG("OCI8Connection construct, #" << ++conn_count << " total");
	refcursor = false;
	if(!session->AllocOciHandle(&stmthp, OCI_HTYPE_STMT)
	|| !session->AllocOciHandle(&errhp, OCI_HTYPE_ERROR))
		session->SetError(t_("Error initializing connection"), t_("OCI8 connection"));
	LinkAfter(&s.clink);
}

void OCI8Connection::Clear() {
	if(session) {
		if(refcursor) {
			OCIStmt *aux = 0;
			if(!session -> AllocOciHandle(&aux, OCI_HTYPE_STMT)) {
				int errcode;
				String err = OciError(oci8, errhp, &errcode, session->utf8_session);
				session->SetError(err, t_("Closing reference cursor"), errcode, NULL, OciErrorClass(errcode));
			}
			static char close[] = "begin close :1; end;";
			bool err = false;
			OCIBind *bind = 0;
			err = oci8.OCIStmtPrepare(aux, errhp, (OraText *)close, strlen(close), OCI_NTV_SYNTAX, OCI_DEFAULT)
				|| oci8.OCIBindByPos(aux, &bind, errhp, 1, &stmthp, 0, SQLT_RSET, 0, 0, 0, 0, 0, OCI_DEFAULT)
				|| oci8.OCIStmtExecute(SvcCtx(), aux, errhp, 1, 0, 0, 0, OCI_DEFAULT);
			if(err)
				SetError();
			session -> FreeOciHandle(aux, OCI_HTYPE_STMT);
		}
		session -> FreeOciHandle(stmthp, OCI_HTYPE_STMT);
		session -> FreeOciHandle(errhp, OCI_HTYPE_ERROR);
		session = NULL;
	}
}

OCI8Connection::~OCI8Connection() {
	Clear();
	LLOG("OCI8Connection destruct, #" << --conn_count << " left");
}

SqlConnection *Oracle8::CreateConnection() {
	return new OCI8Connection(*this);
}

bool   Oracle8::IsOpen() const {
	return svchp;
}

bool Oracle8::Open(const String& connect_string, bool use_objects, String *warn) {
	String name, pwd, server;
	const char *b = connect_string, *p = b;
	while(*p && *p != '/' && *p != '@')
		p++;
	name = String(b, p);
	if(*p == '/') {
		b = ++p;
		while(*p && *p != '@')
			p++;
		pwd = String(b, p);
	}
	if(*p == '@')
		server = ++p;
	return Login(name, pwd, server, use_objects, NULL, warn);
}

bool Oracle8::OpenWithNewPassword(const String& connect_string, String user, String oldPassword, String newPassword) {
	return false;
}

static void OCIInitError(Oracle8& ora, String infn)
{
	ora.Logoff();
	ora.SetError(NFormat(t_("Error initializing OCI8 library (%s)"), infn),
		t_("Connecting to Oracle database."), 0, NULL, Sql::CONNECTION_BROKEN);
}

//	OCI_LM_DEF = 0, // default login
//	OCI_LM_NBL = 1, // non-blocking logon

bool Oracle8::Login(const char *name, const char *pwd, const char *db, bool use_objects, const char *newpwd, String *warn) {
	LLOG("Oracle8::Login");
	level = 0;
	Logoff(); // Takes time?
	ClearError();
	user = ToUpper(String(name));
	LLOG("Loading OCI8 library");
	if(!oci8.Load()) {
		SetError(t_("Error loading OCI8 Oracle connection dynamic library."),
			t_("Connecting to Oracle database."), 0, NULL, Sql::CONNECTION_BROKEN);
		return false;
	}
	LLOG("OCI8 loaded -> OCIInitialize, OCIEnvInit");
	int accessmode = (use_objects ? OCI_OBJECT : 0)
#if defined(_MULTITHREADED) || defined(PLATFORM_POSIX)
		| OCI_THREADED
#endif
	;
	
	utf8_session = false;
	
	if(!disable_utf8_mode && oci8.OCIEnvNlsCreate) {
		if(oci8.OCIEnvNlsCreate(&envhp, accessmode, 0, 0, 0, 0, 0, 0, OCI_NLS_NCHARSET_ID_AL32UT8, OCI_NLS_NCHARSET_ID_AL32UT8)
		&& oci8.OCIEnvNlsCreate(&envhp, accessmode, 0, 0, 0, 0, 0, 0, OCI_NLS_NCHARSET_ID_UT8, OCI_NLS_NCHARSET_ID_UT8))
			LLOG("OCI8: error on initialization utf8 NLS");
		else
			utf8_session = true;
	}
	
	if(!utf8_session) {
		if(oci8.OCIEnvCreate) {
			if(oci8.OCIEnvCreate(&envhp, accessmode, 0, 0, 0, 0, 0, 0)) {
				OCIInitError(*this, "OCIEnvCreate");
				return false;
			}
		} else {
			if(oci8.OCIInitialize(accessmode, 0, 0, 0, 0)) {
				OCIInitError(*this, "OCIInitialize");
				return false;
			}
			if(oci8.OCIEnvInit(&envhp, OCI_DEFAULT, 0, 0)) {
				OCIInitError(*this, "OCIEnvInit");
				return false;
			}
		}
	}
	if(!AllocOciHandle(&errhp, OCI_HTYPE_ERROR)) {
		OCIInitError(*this, "OCI_HTYPE_ERROR");
		return false;
	}
	if(!AllocOciHandle(&svchp, OCI_HTYPE_SVCCTX)) {
		OCIInitError(*this, "OCI_HTYPE_SVCCTX");
		return false;
	}
	if(!AllocOciHandle(&srvhp, OCI_HTYPE_SERVER)) {
		OCIInitError(*this, "OCI_HTYPE_SERVER");
		return false;
	}
	if(!AllocOciHandle(&seshp, OCI_HTYPE_SESSION)) {
		OCIInitError(*this, "OCI_HTYPE_SESSION");
		return false;
	}

	sb4 retcode;
	
	LLOG("Attributes allocated -> OCIServerAttach");
	
//	mode (IN) - specifies the various modes of operation.  For release 8.0, pass as
//	OCI_DEFAULT - in this mode, calls made to the server on this server context 
//	are made in blocking mode. 
	
//When the Oracle 10 databases are up and running fine, OCILogon2() will connect immediately. When the databases are turned off or inaccessible due to network issues - it will fail immediately. However when our DBAs go into emergency maintenance and block incomming connections, it can take 5 to 10 minutes to timeout. This is problematic for me since I've found that OCILogin2 isn't thread safe and we can only use it serially - and I connect to quite a few Oracle DBs. 3 blocked servers X 5-10 minutes = 15 to 30 minutes of lockup time
//Does anyone know how to set the OCILogon2 connection timeout?
//[sqlnet.ora]
//SQLNET.OUTBOUND_CONNECT_TIMEOUT=60
//Without this parameter, a client connection request to the database server may block for the default TCP connect timeout duration (approximately 8 minutes on Linux) when the database server host system is unreachable.	

	if(
		oci8.OCIServerAttach( // See OCIServerDetach
			srvhp, errhp, (byte *)db, strlen(db)
			, OCI_DEFAULT // mode
			)
	) {
		SetOciError(NFormat(t_("Connecting to server '%s'"), db), errhp);
		Logoff();
		return false;
	}
	LLOG("Server attached -> OCIAttrSet, OCISessionBegin");
	in_server = true;

	ub1 attr_value;
	attr_value = 1;

	
	retcode = oci8.OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, srvhp, 0, OCI_ATTR_SERVER, errhp);

	retcode = oci8.OCIAttrSet(seshp, OCI_HTYPE_SESSION, (byte *)name, strlen(name), OCI_ATTR_USERNAME, errhp);
	
	retcode = oci8.OCIAttrSet(seshp, OCI_HTYPE_SESSION, (byte *)pwd, strlen(pwd), OCI_ATTR_PASSWORD, errhp);
	
	retcode = oci8.OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, seshp, 0, OCI_ATTR_SESSION, errhp);

	retcode = oci8.OCISessionBegin(svchp, errhp, seshp, OCI_CRED_RDBMS, OCI_DEFAULT);

	if (retcode != OCI_SUCCESS && retcode != OCI_SUCCESS_WITH_INFO) {
		if (newpwd) {
			int errcode;
			String error = OciError(oci8, errhp, &errcode, utf8_session);
			if (errcode == 28001) {
			//	OCIPasswordChange();
			}
		}
		SetOciError(t_("Connecting to Oracle database."), errhp);
		Logoff();
		return false;
	}

	if(retcode == OCI_SUCCESS_WITH_INFO && warn) {
		int errcode;
		*warn = OciError(oci8, errhp, &errcode, utf8_session);
	}

	LLOG("Session attached, user = " + GetUser());
	in_session = true;
	
	return true;
}

void Oracle8::Logoff() {
	LOG("Oracle8::Logoff, #" << conn_count << " connections pending");
	while(!clink.IsEmpty()) {
		clink.GetNext()->Clear();
		clink.GetNext()->Unlink();
		LOG("-> #" << conn_count << " connections left");
	}
	if(in_session)
	{
		in_session = false;
		LLOG("OCISessionEnd");
		oci8.OCISessionEnd(svchp, errhp, seshp, OCI_DEFAULT);
	}
	if(in_server)
	{
		in_server = false;
		LLOG("OCIServerDetach");
		oci8.OCIServerDetach(srvhp, errhp, OCI_DEFAULT);
	}
	FreeOciHandle(seshp, OCI_HTYPE_SESSION);
	seshp = NULL;
	FreeOciHandle(srvhp, OCI_HTYPE_SERVER);
	srvhp = NULL;
	FreeOciHandle(svchp, OCI_HTYPE_SVCCTX);
	svchp = NULL;
	FreeOciHandle(errhp, OCI_HTYPE_ERROR);
	errhp = NULL;
	if(envhp) {
		FreeOciHandle(envhp, OCI_HTYPE_ENV);
		envhp = NULL;
		if(!oci8.OCIEnvCreate)
			oci8.OCITerminate(OCI_DEFAULT);
	}
}

void   Oracle8::PostError() {
}

String Oracle8::Spn() {
	return Format("TRANSACTION_LEVEL_%d", level);
}

void   Oracle8::Begin() {
	if(Stream *s = GetTrace())
		*s << user << "(OCI8) -> StartTransaction(level " << level << ")\n";
	level++;
//	ClearError();
	if(level > 1)
		Sql(*this).Execute("savepoint " + Spn());
}

void   Oracle8::Commit() {
	int time = msecs();
	ASSERT(tmode == ORACLE || level > 0);
	if(level)
		level--;
//	else
//		ClearError();
	if(level == 0) {
		oci8.OCITransCommit(svchp, errhp, OCI_DEFAULT);
//		if(Stream *s = GetTrace())
//			*s << "%commit;\n";
	}
	if(Stream *s = GetTrace())
		*s << NFormat("----- %s (OCI8) -> Commit(level %d) %d ms\n", user, level, msecs(time));
}

void   Oracle8::Rollback() {
	ASSERT(tmode == ORACLE || level > 0);
	if(level > 1)
		Sql(*this).Execute("rollback to savepoint " + Spn());
	else {
		oci8.OCITransRollback(svchp, errhp, OCI_DEFAULT);
//		if(Stream *s = GetTrace())
//			*s << "%rollback;\n";
	}
	if(level)
		level--;
//	else
//		ClearError();
	if(Stream *s = GetTrace())
		*s << user << "(OCI8) -> Rollback(level " << level << ")\n";
}

String Oracle8::Savepoint() {
	static dword i;
	i = (i + 1) & 0x8fffffff;
	String s = Sprintf("SESSION_SAVEPOINT_%d", i);
	Sql(*this).Execute("savepoint " + s);
	return s;
}

void   Oracle8::RollbackTo(const String& savepoint) {
	Sql(*this).Execute("rollback to savepoint " + savepoint);
}

Oracle8::Oracle8(T_OCI8& oci8_)
: oci8(oci8_)
{
	level = 0;
	envhp = NULL;
	errhp = NULL;
	srvhp = NULL;
	seshp = NULL;
	svchp = NULL;
	tmode = NORMAL;
	disable_utf8_mode = false;
	in_session = in_server = utf8_session = false;
	Dialect(ORACLE);
}

Oracle8::~Oracle8() {
	Logoff();
}

Vector<String> Oracle8::EnumUsers()
{
	Sql cursor(*this);
	return OracleSchemaUsers(cursor);
}

Vector<String> Oracle8::EnumDatabases()
{
	Sql cursor(*this);
	return OracleSchemaUsers(cursor);
}

Vector<String> Oracle8::EnumTables(String database)
{
	Sql cursor(*this);
	return OracleSchemaTables(cursor, database);
}

Vector<String> Oracle8::EnumViews(String database)
{
	Sql cursor(*this);
	return OracleSchemaViews(cursor, database);
}

Vector<String> Oracle8::EnumSequences(String database)
{
	Sql cursor(*this);
	return OracleSchemaSequences(cursor, database);
}

Vector<String> Oracle8::EnumPrimaryKey(String database, String table)
{
	Sql cursor(*this);
	return OracleSchemaPrimaryKey(cursor, database, table);
}

String Oracle8::EnumRowID(String database, String table)
{
	Sql cursor(*this);
	return OracleSchemaRowID(cursor, database, table);
}

Vector<String> Oracle8::EnumReservedWords()
{
	return OracleSchemaReservedWords();
}

bool Oracle8::BulkExecute(const char *stmt, const Vector< Vector<Value> >& param_rows)
{
	if(!*stmt || param_rows.IsEmpty())
		return true;
	return OCI8Connection(*this).BulkExecute(stmt, param_rows);
}

void OracleBlob::SetStreamSize(int64 pos) {
	ASSERT(pos <= GetStreamSize());
	if(pos < (int)GetStreamSize())
		session->oci8.OCILobTrim(session->svchp, session->errhp, locp, (int)pos);
}

dword OracleBlob::Read(int64 at, void *ptr, dword size) {
	ASSERT(IsOpen() && (style & STRM_READ) && session);
	ASSERT(at == (dword)at);
	ub4 n = size;
	if(session->oci8.OCILobRead(session->svchp, session->errhp, locp, &n, (dword)at + 1, ptr, size,
		NULL, NULL, 0, SQLCS_IMPLICIT) != OCI_SUCCESS) return 0;
	return n;
}

void OracleBlob::Write(int64 at, const void *ptr, dword size) {
	ASSERT(IsOpen() && (style & STRM_WRITE) && session);
	ASSERT(at == (dword)at);
	ub4 n = size;
	if(session->oci8.OCILobWrite(session->svchp, session->errhp, locp, &n, (dword)at + 1, (void *)ptr, size,
		OCI_ONE_PIECE, NULL, NULL, 0, SQLCS_IMPLICIT) != OCI_SUCCESS || n != size)
	{
		SetError();
	}
}

void OracleBlob::Assign(Oracle8& s, int blob) {
	session = &s;
	locp = (OCILobLocator *)blob;
	ub4 n;
	OpenInit(READWRITE,
		session->oci8.OCILobGetLength(session->svchp, session->errhp, locp, &n) == OCI_SUCCESS ? n : 0);
}


void OracleBlob::Assign(const Sql& sql, int blob) {
	Oracle8 *session = dynamic_cast<Oracle8 *>(&sql.GetSession());
	ASSERT(session);
	Assign(*session, blob);
}

bool OracleBlob::IsOpen() const {
	return locp;
}

void OracleBlob::Close() {
	if(locp) Flush();
	locp = NULL;
}

OracleBlob::OracleBlob(const Sql& sql, int blob) {
	Assign(sql, blob);
}

OracleBlob::OracleBlob(Oracle8& session, int blob) {
	Assign(session, blob);
}

OracleBlob::OracleBlob() {
	locp = NULL;
	session = NULL;
}

OracleBlob::~OracleBlob() {
	Close();
}

END_UPP_NAMESPACE

/*
Runtime dynamic linking using .dli files

In Ultimate++ you can use .dli file to automatize generating .dll wrapper objects. To create such object you have to provide ".dli" file 
with content like (actual example is for Lotus Notes client .dll):
 
FN(WORD,   OSLoadString, (HMODULE hModule, STATUS StringCode, char *retBuffer, WORD BufferLength))
FN(WORD,   OSTranslate, (WORD TranslateMode, char far *In, WORD InLength, char far *Out, WORD OutLength))
FN(STATUS, NotesInitExtended, (int argc, char  **argv))
FN(STATUS, OSPathNetConstruct, (char *PortName, char *ServerName, char far *FileName, char *retPathName))
FN(STATUS, NSFDbOpen,  (char far *PathName, DBHANDLE far *rethDB))
FN(STATUS, NSFDbClose,  (DBHANDLE hDB))
...........
 
- basically, this is somewhat "reparsed" header file for .dll. Then place
 
#define DLLFILENAME "nnotes.dll"
#define DLIMODULE   NOTES
#define DLIHEADER   <notes/notes.dli>
#define DLLCALL     LNPUBLIC
#include <Core/dli_header.h>
 
to common header file and
 
#define DLLFILENAME "nnotes.dll"
#define DLIMODULE   NOTES
#define DLIHEADER   <notes/notes.dli>
#define DLLCALL     LNPUBLIC
#include <Core/dli_header.h>
 
to some .cpp file or, if you need that .dll just in single .cpp file, you can use
 
#define DLLFILENAME "nnotes.dll"
#define DLIMODULE   NOTES
#define DLIHEADER   <notes/notes.dli>
#define DLLCALL     LNPUBLIC
#include <Core/dli.h>
 
in .cpp (this is equivalent of placing both above variants to .cpp).
 
This creates global function NOTES() returning the object instance that has all .dll functions described in .dli file defined as its methods. Moreover, it has operator bool that can be used to test whether .dll is present:
 
char h[256];
if(NOTES())
    NOTES().OSLoadString(GetModuleHandle(NULL), ERR(nError), h, 255);


 
*/

/*
koldo wrote on Tue, 07 September 2010 16:45


I think in the .cpp file it has to appear:
#include <Core/dli.h>


instead of 
#include <Core/dli_header.h>


There should be
#include <Core/dli_source.h>

dli.h is if you want to use DLI in one file(.cpp) only.
*/

/*
Hello all

As it was hard to maintain avcodec.dli because the avcodec.dll has lots of functions, I have used some "ugly" linker options that work:

- MSC: /FORCE:MULTIPLE
- GCC/MinGW: -Wl,--allow-multiple-definition

With these options the linker uses the zlib functions included in plugin/z package, instead of the functions in avcodec.dll.
*/

/*
Symbols contain the mapping between the compiler-generated machine code and your source code. They help the debugger to "understand" the addresses of functions, parameters and variables and map them back to source code references. Without a proper symbol file, the debugger may present disassembly output which looks like this:
 
            call YourApp+0x11c0d (00411c0d)
 
With proper symbol resolution, the debugger can correctly interpret this machine code as the equivalent function name:
 
            call YourApp!CSampleDlg::OnBtnClicked
 
In the Visual Studio family of products, we use the PDB (Program Database) format for encoding the 

Public symbols normally just contain:
Mapping from addresses to function names
Frame pointer omission (FPO) records
Private symbol files are the default kind of files generated by the Visual Studio linkers. These files contain the following details in addition to what the public symbols contain:
Line numbers and source file information
Parameter names and data types
Local variable names and data types
Normally you would want private symbol files for any detailed debugging of your own application. However, it may not be a very good idea to share your private PDB files with your end customers, since some of the information in the private PDB can be viewed as intellectual property.
 
To generate stripped / public PDB files with Visual C++, use the information in http://msdn2.microsoft.com/en-US/library/y87kw2fd.aspx for steps.

In VC++ 6.0, a symbol file should automatically be created if you are compiling a debug build. If you are compiling a release build, then you will need to follow these steps to create symbols. Please perform the symbol generation steps for each and every module in your application. If it loads other DLLs then these steps need to be done for those DLLs also.

How to generate symbol files for Visual Basic 6.0
In VB6, symbols are not created by default. To create symbols for your VB6 applications, follow these steps:
Open the source code for your project.
Select "<projectname> Properties" from the Project menu.
In the Compile tab, check Create Symbolic Debug Info and select No Optimizations
Recompile. This will create a .PDB file (the symbols) in the same directory as your .DLL / .EXE / .OCX file.
Note: This does *not* break binary compatibility.

Using the SymChk.exe utility to download symbols
You can use the SymChk.exe utility to verify symbols and to build a local symbol cache in a convenient, noninvasive way. The SymChk.exe utility is included with the Debugging Tools for Windows package. SymChk.exe is a command-line tool. You may want to add the folder of the Debugging Tools for Windows package to the PATH environment variable on your system so that you can access this tool easily from any command prompt.

To use the SymChk.exe utility to download symbol files for all of the components in the Windows\System32 folder, use the following command-line command:
symchk /r c:\windows\system32 /s SRV*c:\symbols\*http://msdl.microsoft.com/download/symbols
In this example:
/r c:\windows\system32 finds all symbols for files in the System32 folder and any subfolders.
/s SRV*c:\symbols*http://msdl.microsoft.com/download/symbols specifies the symbol path to use for symbol resolution. In this case, c:\symbols is the local folder where the symbols will be copied from the symbol server.
To obtain more information about the command-line options for SymChk.exe, type symchk /? at a command prompt. Other options include the ability to specify the name or the process ID (PID) of an executable file that is running.

http://www.microsoft.com/whdc/devtools/ddk/default.mspx
http://www.microsoft.com/downloads/en/confirmation.aspx?FamilyID=6b6c21d2-2006-4afa-9702-529fa782d63b&displaylang=en
You can also read small memory dump files by using the WinDbg tool or the KD.exe tool. WinDbg and KD.exe are included with the latest version of the Debugging Tools for Windows package.

To load the dump file into a debugger, type one of the following commands, and then press ENTER:
windbg -y SymbolPath -i ImagePath -z DumpFilePath
kd -y SymbolPath -i ImagePath -z DumpFilePath
The following table explains the use of the placeholders that are used in these commands.

Placeholder	Explanation
SymbolPath	Either the local path where the symbol files have been downloaded or the symbol server path, including a cache folder. Because a small memory dump file contains limited information, the actual binary files must be loaded together with the symbols for the dump file to be correctly read.
There are several commands that you can use to gather information in the dump file, including the following commands:
The !analyze -show command displays the Stop error code and its parameters. The Stop error code is also known as the bug check code.
The !analyze -v command displays verbose output.
The lm N T command lists the specified loaded modules. The output includes the status and the path of the module.
Note: The !drivers extension command displays a list of all drivers that are loaded on the destination computer, together with summary information about their memory use. The !drivers extension is obsolete in Windows XP and later. To display information about loaded drivers and other modules, 

cd "c:\program files\debugging tools for windows"
kd -y srv*c:\symbols*http://msdl.microsoft.com/download/symbols -i c:\windows\i386 -z %1
When you want to examine a dump file, type the following command to pass the dump file path to the batch file:

dump c:\windows\minidump\minidump.dmp

http://www.microsoft.com/whdc/devtools/debugging/installx86.mspx
http://www.microsoft.com/whdc/devtools/debugging/install64bit.mspx

If you want an easy way out. Open kdexts.dll (based on the OS that you
are running) in a binary editor, search for IoApicCount and change it
to IOApicCount. i.e. it.

http://www.wintellect.com/CS/blogs/jrobbins/archive/2009/05/11/pdb-files-what-every-developer-must-know.aspx

As John Cunningham, the development manager for all things diagnostics on Visual Studio, said at the 2008 PDC, "Love, hold, and protect your PDBs."
The actual file format of a PDB file is a closely guarded secret but Microsoft provides APIs to return the data for debuggers. A native C++ PDB file contains quite a bit of information:
Public, private, and static function addresses
Global variable names and addresses
Parameter and local variable names and offsets where to find them on the stack
Type data consisting of class, structure, and data definitions
Frame Pointer Omission (FPO) data, which is the key to native stack walking on x86
Source file names and their lines
When you load a module into the process address space, the debugger uses two pieces of information to find the matching PDB file. The first is obviously the name of the file. If you load ZZZ.DLL, the debugger looks for ZZZ.PDB. The extremely important part is how the debugger knows this is the exact matching PDB file for this binary. That's done through a GUID that's embedded in both the PDB file and the binary. If the GUID does not match, you certainly won't debug the module at the source code level.

WinDbg can view source code, set breakpoints, view variables (including C++ objects), stack traces, and memory. It includes a command window to issue a wide variety of commands not available through the drop-down menus. For kernel-mode debugging, WinDbg typically requires two computers (the host machine and the target machine). It also allows you to remotely debug user-mode code.

Like static libraries, import libraries for DLLs are noted by the .lib file extension. For example, kernel32.dll, the primary dynamic library for Windows' base functions such as file creation and memory management, is linked via kernel32.lib.

Linking to dynamic libraries is usually handled by linking to an import library when building or linking to create an executable file. The created executable then contains an import address table (IAT) by which all DLL function calls are referenced (each referenced DLL function contains its own entry in the IAT). At run-time, the IAT is filled with appropriate addresses that point directly to a function in the separately-loaded DLL.

Symbol resolution and binding
Each function exported by a DLL is identified by a numeric ordinal and optionally a name. Likewise, functions can be imported from a DLL either by ordinal or by name. The ordinal represents the position of the function's address pointer in the DLL Export Address table. It is common for internal functions to be exported by ordinal only. For most Windows API functions only the names are preserved across different Windows releases; the ordinals are subject to change. Thus, one cannot reliably import Windows API functions by their ordinals.
Importing functions by ordinal provides only slightly better performance than importing them by name: export tables of DLLs are ordered by name, so a binary search can be used to find a function. The index of the found name is then used to look up the ordinal in the Export Ordinal table. In 16-bit Windows, the name table was not sorted, so the name lookup overhead was much more noticeable.

Explicit run-time linking
DLL files may be explicitly loaded at run-time, a process referred to simply as run-time dynamic linking by Microsoft, by using the LoadLibrary (or LoadLibraryEx) API function. The GetProcAddress API function is used to look up exported symbols by name, and FreeLibrary — to unload the DLL. These functions are analogous to dlopen, dlsym, and dlclose in the POSIX standard API.

The delay-loading mechanism also provides notification hooks, allowing the application to perform additional processing or error handling when the DLL is loaded and/or any DLL function is called.
[edit]

Besides specifying imported or exported functions using __declspec attributes, they may be listed in IMPORT or EXPORTS section of the DEF file used by the project. The DEF file is processed by the linker, rather than the compiler, and thus it is not specific to C++.
DLL compilation will produce both DLL and LIB files. The LIB file is used to link against a DLL at compile-time; it is not necessary for run-time linking. Unless your DLL is a COM server, the DLL file must be placed in one of the directories listed in the PATH environment variable, in the default system directory, or in the same directory as the program using it. COM server DLLs are registered using regsvr32.exe, which places the DLL's location and its globally unique ID (GUID) in the registry. Programs can then use the DLL by looking up its GUID in the registry to find its location.

http://en.wikipedia.org/wiki/Dependency_Walker
OCIMemoryAlloc
dllexport

http://www.winehq.org/docs/winedump

Run l2inca.exe with the following switches:

l2inca /M *.lib

l2inca.exe will extract information from the import libraries and create include files full of function prototypes.

Move those include files to your own include file folder. For MASM32 users, move them to MASM32\include folder.

http://www.woodmann.com/RCE-CD-SITES/Iczelion/

*/

// non blocking is not supported in OCCI. 