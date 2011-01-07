#ifndef _Oracle_Oci8_h_
#define _Oracle_Oci8_h_

#include "Oracle8.h"
#include "OciCommon.h"

#include <CtrlLib/CtrlLib.h>

class OCI8Connection : public Link<OCI8Connection>, public OciSqlConnection {
public:
	virtual void		SetTopWindow(TopWindow *ptopWindow); // Handle to allow async processing by calling back to ProcessEvents

protected:
	virtual void        SetParam(int i, const Value& r);
	virtual void        SetParam(int i, OracleRef r);
	virtual bool        Execute(); // Implements abstract function from Sqls.h SqlConnection
	TopWindow *topWindow;  // Set to null in Constructor.  I set this from my main client and Connection object so that Execute() and Fetch() can run in non-blocking mode and return control to the windows event loop intermittently.
	virtual int         GetRowsProcessed() const;
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

#endif
