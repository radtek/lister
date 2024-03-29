#ifndef _PostgreSQL_PostgeSQL_h_
#define _PostgreSQL_PostgeSQL_h_

#ifndef flagNOPOSTGRESQL

#include <lister/Sql/Sql.h>
#ifdef PLATFORM_WIN32
#include <libpq-fe.h>
#elif defined(PLATFORM_POSIX)
#include <postgresql/libpq-fe.h>
#endif

NAMESPACE_UPP

// Postgre -> Value types
// Bytea_v values are stored as bytea data, but recived as string type
// These are scanners for closing dup of character at (s), ' or " only ones that work
const char *PostgreSQLReadString(const char *s, String& stmt);
// This is for /* */
const char *PostgreSQLReadStringComm(const char *s, String& stmt);

bool PostgreSQLPerformScript(const String& text,
#ifdef NOAPPSQL
	StatementExecutor& se,
#else
	StatementExecutor& se = SQLStatementExecutor(),
#endif
	Gate2<int, int> progress_canceled = false
);

String PostgreSQLTextType(int n);

class PostgreSQLConnection;

class PostgreSQLSession : public SqlSession {
public:
	virtual bool                  IsOpen() const                   { return conn; }
	virtual RunScript             GetRunScript() const             { return &PostgreSQLPerformScript; }

	virtual Vector<String>        EnumUsers();
	virtual Vector<String>        EnumDatabases();
	virtual Vector<String>        EnumTables(String database);
	virtual Vector<String>        EnumViews(String database);
	virtual Vector<String>        EnumSequences(String database);
	virtual Vector<SqlColumnInfo> EnumColumns(String database, String table);
	virtual Vector<String>        EnumPrimaryKey(String database, String table);
	virtual String                EnumRowID(String database, String table);
	virtual Vector<String>        EnumReservedWords();
	virtual SqlConnection *CreateConnection(); // Moved to public(!)

protected:

private:
	PGconn               *conn;
	PGresult             *result;
	
	String                conns;

	void                  ExecTrans(const char * statement);
	Vector<String>        EnumData(char type, const char *schema = NULL);
	String                ErrorMessage();
	String                ErrorCode();
	int                   level;
	byte                  charset;
	
	String                FromCharset(const String& s) const;
	String                ToCharset(const String& s) const;

	friend class PostgreSQLConnection;

public:
	Gate1<int>            WhenReconnect;

	bool                  Open(const char *connect);
	bool                  ConnectionOK();
	bool                  ReOpen();
	void                  Close();
	
	void                  SetCharset(byte chrset)         { charset = chrset; }

	String                GetUser()                       { return PQuser(conn); }
	operator PGconn *     ()                              { return conn; }
	
	virtual void          Begin();
	virtual void          Commit();
	virtual void          Rollback();
	virtual int           GetTransactionLevel() const;

	PostgreSQLSession()                                   { conn = NULL; Dialect(PGSQL); level = 0; }
	~PostgreSQLSession()                                  { Close(); }
	PGconn * GetPGConn()                                  { return conn; }
};

class PgSequence : public ValueGen {
	SqlId       ssq;
	SqlId&      seq;
	SqlSession *session;

public:
	virtual Value  Get();

	Value operator++()                                                  { return Get(); }

	void Set(SqlId id, SqlSession& s)                                   { ssq = id; session = &s; }

#ifndef NOAPPSQL
	void Set(SqlId id)                                                  { ssq = id; session = NULL; }
	PgSequence(const char *name) : ssq(name), seq(ssq)                  { session = NULL; }
	PgSequence(SqlId& seq) : seq(seq)                                   { session = NULL; }
#endif
	PgSequence(const char *name, SqlSession& s) : ssq(name), seq(ssq)   { session = &s; }
	PgSequence(SqlId& seq, SqlSession& s) : seq(seq)                    { session = &s; }
	PgSequence() : seq(ssq)                                             { session = NULL; }
};

END_UPP_NAMESPACE

#endif

#endif
