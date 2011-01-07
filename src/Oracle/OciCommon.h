#ifndef __Oracle_OciCommon__
#define __Oracle_OciCommon__

// excerpt from OCI headers (OCI7 & OCI8)
NAMESPACE_UPP
struct OCIEnv;
struct OCIServer;
struct OCISession;
struct OCIStmt;
struct OCIBind;
struct OCIDefine;
struct OCISnapshot;
struct OCIParam;

// Added to support TIMESTAMP Column definition and allocation size calculation to avoid 1406 truncation errors.
// From oci.h

struct OCIDateTime;

END_UPP_NAMESPACE

/*---------------- Server Handle Attribute Values ---------------------------*/

/* OCI_ATTR_SERVER_STATUS */
#define OCI_SERVER_NOT_CONNECTED        0x0 
#define OCI_SERVER_NORMAL               0x1 

#define oci_boolean int
typedef void dvoid;
typedef int sword;
typedef int sb4;
typedef UPP::dword uword;
typedef UPP::dword ub4;
typedef short sb2;
typedef UPP::word ub2;
typedef signed char sb1;
typedef UPP::byte ub1;
typedef UPP::byte OraText;
enum
{
	// OCIInitialize modes
	OCI_THREADED   = 0x01,  // the application is in threaded environment
	OCI_OBJECT     = 0x02,  // the application is in object environment
	OCI_EVENTS     = 0x04,  // the application is enabled for events
	OCI_RESERVED1  = 0x08,  // Reserved for internal use
	OCI_SHARED     = 0x10,  // the application is in shared mode
	OCI_RESERVED2  = 0x20,  // Reserved for internal use
		// The following *TWO* are only valid for OCICreateEnvironment call
	OCI_NO_UCB     = 0x40,  // No user callback called during init
	OCI_NO_MUTEX   = 0x80,  // the environment handle will not be
		// NLS
	OCI_NLS_NCHARSET_ID_UT8 = 871,                   /* AL32UTF8 charset id */
	OCI_NLS_NCHARSET_ID_AL32UT8 = 873,               /* AL32UTF8 charset id */
		// protected by a mutex internally
	OCI_SHARED_EXT = 0x100, // Used for shared forms
	OCI_CACHE      = 0x200, // used by iCache

	OCI_DEFAULT      = 0x00,  // the default value for parameters and attributes
	OCI_DATA_AT_EXEC = 0x02,  // data at execute time

	OCI_HTYPE_FIRST                = 1, // start value of handle type
	OCI_HTYPE_ENV                  = 1, // environment handle
	OCI_HTYPE_ERROR                = 2, // error handle
	OCI_HTYPE_SVCCTX               = 3, // service handle
	OCI_HTYPE_STMT                 = 4, // statement handle
	OCI_HTYPE_BIND                 = 5, // bind handle
	OCI_HTYPE_DEFINE               = 6, // define handle
	OCI_HTYPE_DESCRIBE             = 7, // describe handle
	OCI_HTYPE_SERVER               = 8, // server handle
	OCI_HTYPE_SESSION              = 9, // authentication handle
	OCI_HTYPE_TRANS                = 10, // transaction handle
	OCI_HTYPE_COMPLEXOBJECT        = 11, // complex object retrieval handle
	OCI_HTYPE_SECURITY             = 12, // security handle
	OCI_HTYPE_SUBSCRIPTION         = 13, // subscription handle
	OCI_HTYPE_DIRPATH_CTX          = 14, // direct path context
	OCI_HTYPE_DIRPATH_COLUMN_ARRAY = 15, // direct path column array
	OCI_HTYPE_DIRPATH_STREAM       = 16, // direct path stream
	OCI_HTYPE_PROC                 = 17, // process handle
	OCI_HTYPE_LAST                 = 17, // last value of a handle type

	// input data types
	SQLT_CHR = 1, // (ORANET TYPE) character string
	SQLT_NUM = 2, // (ORANET TYPE) oracle numeric
	SQLT_INT = 3, // (ORANET TYPE) integer
	SQLT_FLT = 4, // (ORANET TYPE) Floating point number
	SQLT_STR = 5, // zero terminated string
	SQLT_VNU = 6, // NUM with preceding length byte
	SQLT_PDN = 7, // (ORANET TYPE) Packed Decimal Numeric
	SQLT_LNG = 8, // long
	SQLT_VCS = 9, // Variable character string
	SQLT_NON = 10, // Null/empty PCC Descriptor entry
	SQLT_RID = 11, // rowid
	SQLT_DAT = 12, // date in oracle format
	SQLT_VBI = 15, // binary in VCS format
	SQLT_BIN = 23, // binary data(DTYBIN)
	SQLT_LBI = 24, // long binary
	SQLT_UIN = 68, // unsigned integer
	SQLT_SLS = 91, // Display sign leading separate
	SQLT_LVC = 94, // Longer longs (char)
	SQLT_LVB = 95, // Longer long binary
	SQLT_AFC = 96, // Ansi fixed char
	SQLT_AVC = 97, // Ansi Var char
	SQLT_CUR = 102, // cursor type
	SQLT_RDD = 104, // rowid descriptor
	SQLT_LAB = 105, // label type
	SQLT_OSL = 106, // oslabel type

	SQLT_NTY = 108, // named object type
	SQLT_REF = 110, // ref type
	SQLT_CLOB = 112, // character lob
	SQLT_BLOB = 113, // binary lob
	SQLT_BFILEE = 114, // binary file lob
	SQLT_CFILEE = 115, // character file lob
	SQLT_RSET = 116, // result set type
	SQLT_NCO = 122, // named collection type (varray or nested table)
	SQLT_VST = 155, // OCIString type
	SQLT_ODT = 156, // OCIDate type

	// datetimes and intervals
	SQLT_DATE = 184, // ANSI Date
	SQLT_TIME = 185, // TIME
	SQLT_TIME_TZ = 186, // TIME WITH TIME ZONE
	SQLT_TIMESTAMP = 187, // TIMESTAMP
	SQLT_TIMESTAMP_TZ = 188, // TIMESTAMP WITH TIME ZONE
	SQLT_INTERVAL_YM = 189, // INTERVAL YEAR TO MONTH
	SQLT_INTERVAL_DS = 190, // INTERVAL DAY TO SECOND
	SQLT_TIMESTAMP_LTZ = 232, // TIMESTAMP WITH LOCAL TZ

	// cxcheng: this has been added for backward compatibility -
	// it needs to be here because ocidfn.h can get included ahead of sqldef.h
	SQLT_FILE = SQLT_BFILEE, // binary file lob
	SQLT_CFILE = SQLT_CFILEE,
	SQLT_BFILE = SQLT_BFILEE,

	OCI_DTYPE_FIRST = 50, // start value of descriptor type
	OCI_DTYPE_LOB = 50, // lob locator
	OCI_DTYPE_SNAP = 51, // snapshot descriptor
	OCI_DTYPE_RSET = 52, // result set descriptor
	OCI_DTYPE_PARAM = 53, // a parameter descriptor obtained from ocigparm
	OCI_DTYPE_ROWID = 54, // rowid descriptor
	OCI_DTYPE_COMPLEXOBJECTCOMP = 55,
	 // complex object retrieval descriptor
	OCI_DTYPE_FILE = 56, // File Lob locator
	OCI_DTYPE_AQENQ_OPTIONS = 57, // enqueue options
	OCI_DTYPE_AQDEQ_OPTIONS = 58, // dequeue options
	OCI_DTYPE_AQMSG_PROPERTIES = 59, // message properties
	OCI_DTYPE_AQAGENT = 60, // aq agent
	OCI_DTYPE_LOCATOR = 61, // LOB locator
	OCI_DTYPE_INTERVAL_YM = 62, // Interval year month
	OCI_DTYPE_INTERVAL_DS = 63, // Interval day second
	OCI_DTYPE_AQNFY_DESCRIPTOR = 64, // AQ notify descriptor
	OCI_DTYPE_DATE = 65, // Date
	OCI_DTYPE_TIME = 66, // Time
	OCI_DTYPE_TIME_TZ = 67, // Time with timezone
	OCI_DTYPE_TIMESTAMP = 68, // Timestamp
	OCI_DTYPE_TIMESTAMP_TZ = 69, // Timestamp with timezone
	OCI_DTYPE_TIMESTAMP_LTZ = 70, // Timestamp with local tz
	OCI_DTYPE_UCB = 71, // user callback descdriptor
	OCI_DTYPE_LAST = 71, // last value of a descriptor type

	OCI_V7_SYNTAX  = 2, // V815 language - for backwards compatibility
	OCI_V8_SYNTAX  = 3, // V815 language - for backwards compatibility
	OCI_NTV_SYNTAX = 1, // Use what so ever is the native lang of server

	OCI_STMT_SELECT  = 1, // select statement
	OCI_STMT_UPDATE  = 2, // update statement
	OCI_STMT_DELETE  = 3, // delete statement
	OCI_STMT_INSERT  = 4, // Insert Statement
	OCI_STMT_CREATE  = 5, // create statement
	OCI_STMT_DROP    = 6, // drop statement
	OCI_STMT_ALTER   = 7, // alter statement
	OCI_STMT_BEGIN   = 8, // begin ... (pl/sql statement)
	OCI_STMT_DECLARE = 9, // declare ... (pl/sql statement)
	OCI_STMT_CALL    = 10, //corresponds to kpu call
	OCI_BATCH_MODE             = 0x01, // batch the oci statement for execution
	OCI_EXACT_FETCH            = 0x02, // fetch the exact rows specified
	OCI_KEEP_FETCH_STATE       = 0x04, // unused
	OCI_SCROLLABLE_CURSOR      = 0x08, // cursor scrollable
	OCI_DESCRIBE_ONLY          = 0x10, // only describe the statement
	OCI_COMMIT_ON_SUCCESS      = 0x20, // commit, if successful execution
	OCI_NON_BLOCKING           = 0x40, // non-blocking
	OCI_BATCH_ERRORS           = 0x80, // batch errors in array dmls
	OCI_PARSE_ONLY             = 0x100, // only parse the statement
	OCI_EXACT_FETCH_RESERVED_1 = 0x200, // reserved for internal use
	OCI_SHOW_DML_WARNINGS      = 0x400, // return OCI_SUCCESS_WITH_INFO for delete/update w/no where clause
	OCI_RESULT_CACHE           = 0x00020000, // hint to use query caching
	OCI_NO_RESULT_CACHE        = 0x00040000, // hint to bypass query caching
	
	OCI_SUCCESS              = 0,      // maps to SQL_SUCCESS of SAG CLI
	OCI_SUCCESS_WITH_INFO    = 1,      // maps to SQL_SUCCESS_WITH_INFO
	OCI_RESERVED_FOR_INT_USE = 200,    // reserved for internal use
	OCI_NO_DATA              = 100,    // maps to SQL_NO_DATA
	OCI_ERROR                = -1,     // maps to SQL_ERROR
	OCI_INVALID_HANDLE       = -2,     // maps to SQL_INVALID_HANDLE
	OCI_NEED_DATA            = 99,     // maps to SQL_NEED_DATA
	OCI_STILL_EXECUTING      = -3123,  // OCI would block error
	OCI_CONTINUE             = -24200, // Continue with the body of the OCI function
	OCI_ROWCBK_DONE          = -24201, // done with user row callback
	
	SQLCS_IMPLICIT = 1, // for CHAR, VARCHAR2, CLOB w/o a specified set
	SQLCS_NCHAR    = 2, // for NCHAR, NCHAR VARYING, NCLOB
	SQLCS_EXPLICIT = 3, // for CHAR, etc, with "CHARACTER SET ..." syntax
	SQLCS_FLEXIBLE = 4, // for PL/SQL "flexible" parameters
	SQLCS_LIT_NULL = 5, // for typecheck of NULL and empty_clob() lits

	OCI_FETCH_NEXT       = 0x02, // next row
	OCI_FETCH_FIRST      = 0x04, // first row of the result set
	OCI_FETCH_LAST       = 0x08, // the last row of the result set
	OCI_FETCH_PRIOR      = 0x10, // the previous row relative to current
	OCI_FETCH_ABSOLUTE   = 0x20, // absolute offset from first
	OCI_FETCH_RELATIVE   = 0x40, // offset relative to current
	OCI_FETCH_RESERVED_1 = 0x80, // reserved for internal use

	OCI_ONE_PIECE    = 0, // one piece
	OCI_FIRST_PIECE  = 1, // the first piece
	OCI_NEXT_PIECE   = 2, // the next of many pieces
	OCI_LAST_PIECE   = 3, // the last piece

	OCI_ATTR_DATA_SIZE     = 1, // maximum size of the data
	OCI_ATTR_DATA_TYPE     = 2, // the SQL type of the column/argument
	OCI_ATTR_NONBLOCKING_MODE = 3,                    // non blocking mode (Jeff added)
	
	OCI_ATTR_NAME          = 4, // the name of the column/argument
	OCI_ATTR_PRECISION     = 5, // precision if number type
	OCI_ATTR_SCALE         = 6, // scale if number type
	OCI_ATTR_ROW_COUNT     = 9, // the rows processed so far
	OCI_ATTR_PREFETCH_ROWS = 11, // sets the number of rows to prefetch
	OCI_ATTR_PREFETCH_MEMORY = 13, // memory limit for rows fetched
	OCI_ATTR_PARAM_COUNT   = 18, // number of column in the select list
	OCI_ATTR_ROWID         = 19, // the rowid (JEff: Might be fast retrieval form)
	OCI_ATTR_STMT_TYPE     = 24, // statement type
	OCI_ATTR_ROWS_RETURNED = 42, // Number of rows returned in current iter - for Bind handles
	OCI_ATTR_NUM_DML_ERRORS = 73, // num of errs in array DML
	OCI_ATTR_AQ_NUM_ERRORS = 73,
	OCI_ATTR_NUM_ROWS      = 81, // number of rows in column array
	OCI_ATTR_ENV           = 5,  // the environment handle
	OCI_ATTR_SERVER        = 6,  // the server handle
	OCI_ATTR_SESSION       = 7,  // the user session handle
	OCI_ATTR_COL_COUNT     = 82, // columns of column array processed so far
	OCI_ATTR_USERNAME      = 22, // username attribute
	OCI_ATTR_PASSWORD      = 23, // password attribute
	OCI_ATTR_MIGSESSION    = 86, // Migratable session attribute
	OCI_CRED_RDBMS         = 1,  // database username/password
	OCI_ATTR_TIMESTAMP     = 119, // timestamp of object
	OCI_ATTR_PARSE_ERROR_OFFSET = 129, // !Cool
	OCI_ATTR_TRANS_TIMEOUT = 142,
	OCI_ATTR_SERVER_STATUS = 143,
	OCI_ATTR_STATEMENT = 144,
	OCI_ATTR_DESCRIBE_NATIVE = 189,
	OCI_ATTR_SERVER_BUSY = 193,
	OCI_ATTR_ROWS_FETCHED = 197, // rows fetched in last call
	OCI_ATTR_ERRONEOUS_COLUMN = 203, // position of erroneous col
	OCI_ATTR_ENV_UTF16 = 209, // Is env in utf16 mode?
	OCI_ATTR_STMT_IS_RETURNING = 218, // Useful! does stmt have returning clause
	OCI_ATTR_CURRENT_SCHEMA = 224,
	OCI_ATTR_BIND_ROWCBK = 301, // Callback for bind row
	OCI_ATTR_BIND_ROWCTX = 302,
	OCI_ATTR_SKIP_BUFFER = 303, // Skip buffer in array ops
	OCI_ATTR_CHNF_TABLENAMES = 401, // out: array of table names, DB Change Notification reg handle attributes
	OCI_ATTR_CHNF_ROWIDS = 402, // in: rowids needed
	OCI_ATTR_STMTCACHE_CBK = 421, // callback fn for stmtcache
	OCI_LM_DEF = 0, // default login
	OCI_LM_NBL = 1, // non-blocking logon

	OCI_STRING_MAXLEN = 4000, // default maximum length of a vstring
};

/*------------------DateTime and Interval check Error codes------------------*/

/* DateTime Error Codes used by OCIDateTimeCheck() */
#define   OCI_DT_INVALID_DAY         0x1                          /* Bad day */
#define   OCI_DT_DAY_BELOW_VALID     0x2      /* Bad DAy Low/high bit (1=low)*/
#define   OCI_DT_INVALID_MONTH       0x4                       /*  Bad MOnth */
#define   OCI_DT_MONTH_BELOW_VALID   0x8   /* Bad MOnth Low/high bit (1=low) */
#define   OCI_DT_INVALID_YEAR        0x10                        /* Bad YeaR */
#define   OCI_DT_YEAR_BELOW_VALID    0x20  /*  Bad YeaR Low/high bit (1=low) */
#define   OCI_DT_INVALID_HOUR        0x40                       /*  Bad HouR */
#define   OCI_DT_HOUR_BELOW_VALID    0x80   /* Bad HouR Low/high bit (1=low) */
#define   OCI_DT_INVALID_MINUTE      0x100                     /* Bad MiNute */
#define   OCI_DT_MINUTE_BELOW_VALID  0x200 /*Bad MiNute Low/high bit (1=low) */
#define   OCI_DT_INVALID_SECOND      0x400                    /*  Bad SeCond */
#define   OCI_DT_SECOND_BELOW_VALID  0x800  /*bad second Low/high bit (1=low)*/
#define   OCI_DT_DAY_MISSING_FROM_1582 0x1000     
                                 /*  Day is one of those "missing" from 1582 */
#define   OCI_DT_YEAR_ZERO           0x2000       /* Year may not equal zero */
#define   OCI_DT_INVALID_TIMEZONE    0x4000                 /*  Bad Timezone */
#define   OCI_DT_INVALID_FORMAT      0x8000         /* Bad date format input */


/* Interval Error Codes used by OCIInterCheck() */
#define   OCI_INTER_INVALID_DAY         0x1                       /* Bad day */
#define   OCI_INTER_DAY_BELOW_VALID     0x2  /* Bad DAy Low/high bit (1=low) */
#define   OCI_INTER_INVALID_MONTH       0x4                     /* Bad MOnth */
#define   OCI_INTER_MONTH_BELOW_VALID   0x8 /*Bad MOnth Low/high bit (1=low) */
#define   OCI_INTER_INVALID_YEAR        0x10                     /* Bad YeaR */
#define   OCI_INTER_YEAR_BELOW_VALID    0x20 /*Bad YeaR Low/high bit (1=low) */
#define   OCI_INTER_INVALID_HOUR        0x40                     /* Bad HouR */
#define   OCI_INTER_HOUR_BELOW_VALID    0x80 /*Bad HouR Low/high bit (1=low) */
#define   OCI_INTER_INVALID_MINUTE      0x100                  /* Bad MiNute */
#define   OCI_INTER_MINUTE_BELOW_VALID  0x200 
                                            /*Bad MiNute Low/high bit(1=low) */
#define   OCI_INTER_INVALID_SECOND      0x400                  /* Bad SeCond */
#define   OCI_INTER_SECOND_BELOW_VALID  0x800   
                                            /*bad second Low/high bit(1=low) */
#define   OCI_INTER_INVALID_FRACSEC     0x1000      /* Bad Fractional second */
#define   OCI_INTER_FRACSEC_BELOW_VALID 0x2000  
                                           /* Bad fractional second Low/High */

#if defined(SS_64BIT_SERVER) || defined(__64BIT__)
#define CDA_SIZE 88
#else
# define CDA_SIZE 64
#endif

struct cda_head {
    sb2          v2_rc;
    ub2          ft;
    ub4          rpc;
    ub2          peo;
    ub1          fc;
    ub1          rcs1;
    ub2          rc;
    ub1          wrn;
    ub1          rcs2;
    sword        rcs3;
    struct {
        struct {
           ub4    rcs4;
           ub2    rcs5;
           ub1    rcs6;
        } rd;
        ub4    rcs7;
        ub2    rcs8;
    } rid;
    sword        ose;
    ub1		 chk;
    dvoid        *rcsp;
};

struct cda_def {
    sb2          v2_rc;                                    /* V2 return code */
    ub2          ft;                                    /* SQL function type */
    ub4          rpc;                                /* rows processed count */
    ub2          peo;                                  /* parse error offset */
    ub1          fc;                                    /* OCI function code */
    ub1          rcs1;                                        /* filler area */
    ub2          rc;                                       /* V7 return code */
    ub1          wrn;                                       /* warning flags */
    ub1          rcs2;                                           /* reserved */
    sword        rcs3;                                           /* reserved */
    struct {                                              /* rowid structure */
        struct {
           ub4    rcs4;
           ub2    rcs5;
           ub1    rcs6;
        } rd;
        ub4    rcs7;
        ub2    rcs8;
    } rid;
    sword        ose;                                 /* OSD dependent error */
    ub1		 chk;
    dvoid        *rcsp;                          /* pointer to reserved area */
    ub1          rcs9[CDA_SIZE - sizeof (struct cda_head)];        /* filler */
};

typedef sb4 (*OCICallbackInBind)(dvoid *ictxp, UPP::OCIBind *bindp, ub4 iter,
	ub4 index, dvoid **bufpp, ub4 *alenp,
	ub1 *piecep, dvoid **indp);

typedef sb4 (*OCICallbackOutBind)(dvoid *octxp, UPP::OCIBind *bindp, ub4 iter,
	ub4 index, dvoid **bufpp, ub4 **alenp,
	ub1 *piecep, dvoid **indp,
	ub2 **rcodep);

// Added to support TIMESTAMP Column definition and allocation size calculation to avoid 1406 truncation errors.
// From oci.h

//typedef struct OCIDateTime OCIDateTime;           /* OCI DateTime descriptor */

#define OCI_SB2_IND_PTR       0x00000001                           /* unused */
#define OCI_DATA_AT_EXEC      0x00000002             /* data at execute time */
#define OCI_DYNAMIC_FETCH     0x00000002                /* fetch dynamically */
#define OCI_PIECEWISE         0x00000004          /* piecewise DMLs or fetch */
#define OCI_DEFINE_RESERVED_1 0x00000008                         /* reserved */
#define OCI_BIND_RESERVED_2   0x00000010                         /* reserved */
#define OCI_DEFINE_RESERVED_2 0x00000020                         /* reserved */
#define OCI_BIND_SOFT         0x00000040              /* soft bind or define */
#define OCI_DEFINE_SOFT       0x00000080              /* soft bind or define */
#define OCI_BIND_RESERVED_3   0x00000100                         /* reserved */
#define OCI_IOV               0x00000200   /* For scatter gather bind/define */

/* The following *TWO* are only valid for OCICreateEnvironment call */
#define OCI_NO_UCB          0x00000040 /* No user callback called during ini */
#define OCI_NO_MUTEX        0x00000080 /* the environment handle will not be */
                                         /*  protected by a mutex internally */
#define OCI_ALWAYS_BLOCKING 0x00000400    /* all connections always blocking */

#define OCI_NEW_LENGTH_SEMANTICS  0x00020000   /* adopt new length semantics */
       /* the new length semantics, always bytes, is used by OCIEnvNlsCreate */

#define OCI_NO_MUTEX_STMT   0x00040000           /* Do not mutex stmt handle */
#define OCI_MUTEX_ENV_ONLY  0x00080000  /* Mutex only the environment handle */
#define OCI_SUPPRESS_NLS_VALIDATION   0x00100000  /* suppress nls validation */
  /* nls validation suppression is on by default;
     use OCI_ENABLE_NLS_VALIDATION to disable it */
#define OCI_MUTEX_TRY                 0x00200000    /* try and acquire mutex */
#define OCI_NCHAR_LITERAL_REPLACE_ON  0x00400000 /* nchar literal replace on */
#define OCI_NCHAR_LITERAL_REPLACE_OFF 0x00800000 /* nchar literal replace off*/
#define OCI_ENABLE_NLS_VALIDATION     0x01000000    /* enable nls validation */

/*--------------------------------- OCILogon2 Modes -------------------------*/

#define OCI_LOGON2_SPOOL       0x0001     /* Use session pool */
#define OCI_LOGON2_CPOOL       OCI_CPOOL  /* Use connection pool */
#define OCI_LOGON2_STMTCACHE   0x0004     /* Use Stmt Caching */

/*------------------------- OCISessionPoolCreate Modes ----------------------*/

#define OCI_SPC_REINITIALIZE 0x0001   /* Reinitialize the session pool */
#define OCI_SPC_HOMOGENEOUS  0x0002   /* Session pool is homogeneneous */
#define OCI_SPC_STMTCACHE    0x0004   /* Session pool has stmt cache */
#define OCI_SPC_NO_RLB       0x0008 /* Do not enable Runtime load balancing. */ 

/*--------------------------- OCISessionGet Modes ---------------------------*/

#define OCI_SESSGET_SPOOL      0x0001     /* SessionGet called in SPOOL mode */
#define OCI_SESSGET_CPOOL      OCI_CPOOL  /* SessionGet called in CPOOL mode */
#define OCI_SESSGET_STMTCACHE  0x0004                 /* Use statement cache */
#define OCI_SESSGET_CREDPROXY  0x0008     /* SessionGet called in proxy mode */
#define OCI_SESSGET_CREDEXT    0x0010     
#define OCI_SESSGET_SPOOL_MATCHANY 0x0020
#define OCI_SESSGET_PURITY_NEW     0x0040 
#define OCI_SESSGET_PURITY_SELF    0x0080 
#define OCI_SESSGET_SYSDBA    0x0100    /* SessionGet with SYSDBA privileges */

/*----------------------------- Statement States ----------------------------*/

#define OCI_STMT_STATE_INITIALIZED  0x0001
#define OCI_STMT_STATE_EXECUTED     0x0002
#define OCI_STMT_STATE_END_OF_FETCH 0x0003

/*----------------------------- OCIEnvInit Modes ----------------------------*/
/* NOTE: NO NEW MODES SHOULD BE ADDED HERE BECAUSE THE RECOMMENDED METHOD 
 * IS TO USE THE NEW OCICreateEnvironment MODES.
 */
#define OCI_ENV_NO_UCB 0x01         /* A user callback will not be called in
                                       OCIEnvInit() */
#define OCI_ENV_NO_MUTEX 0x08 /* the environment handle will not be protected
/*------------------------ Prepare Modes ------------------------------------*/
#define OCI_NO_SHARING        0x01      /* turn off statement handle sharing */
#define OCI_PREP_RESERVED_1   0x02                               /* reserved */
#define OCI_PREP_AFC_PAD_ON   0x04          /* turn on blank padding for AFC */
#define OCI_PREP_AFC_PAD_OFF  0x08         /* turn off blank padding for AFC */

#define OCI_FASTPATH         0x0010              /* Attach in fast path mode */

/*------------------------Authentication Modes-------------------------------*/
#define OCI_MIGRATE         0x00000001            /* migratable auth context */
#define OCI_SYSDBA          0x00000002           /* for SYSDBA authorization */
#define OCI_SYSOPER         0x00000004          /* for SYSOPER authorization */
#define OCI_PRELIM_AUTH     0x00000008      /* for preliminary authorization */
#define OCIP_ICACHE         0x00000010             /* Private OCI cache mode */
#define OCI_AUTH_RESERVED_1 0x00000020                           /* reserved */
#define OCI_STMT_CACHE      0x00000040            /* enable OCI Stmt Caching */
#define OCI_STATELESS_CALL  0x00000080         /* stateless at call boundary */
#define OCI_STATELESS_TXN   0x00000100          /* stateless at txn boundary */
#define OCI_STATELESS_APP   0x00000200    /* stateless at user-specified pts */
#define OCI_AUTH_RESERVED_2 0x00000400                           /* reserved */
#define OCI_AUTH_RESERVED_3 0x00000800                           /* reserved */
#define OCI_AUTH_RESERVED_4 0x00001000                           /* reserved */
#define OCI_AUTH_RESERVED_5 0x00002000                           /* reserved */
#define OCI_SYSASM          0x00008000           /* for SYSASM authorization */
#define OCI_AUTH_RESERVED_6 0x00010000                           /* reserved */

#define OCI_ATCH_ENABLE_BEQ  0x4000        /* Allow bequeath connect strings */

/*---------------------OCIStmtPrepare2 Modes---------------------------------*/
#define OCI_PREP2_CACHE_SEARCHONLY    0x0010                  /* ONly Search */
#define OCI_PREP2_GET_PLSQL_WARNINGS  0x0020         /* Get PL/SQL warnings  */
#define OCI_PREP2_RESERVED_1          0x0040                     /* reserved */

/*---------------------OCIStmtRelease Modes----------------------------------*/
#define OCI_STRLS_CACHE_DELETE      0x0010              /* Delete from Cache */


/*------------------------Piece Information----------------------------------*/
#define OCI_PARAM_IN 0x01                                    /* in parameter */
#define OCI_PARAM_OUT 0x02                                  /* out parameter */

/*------------------------ Transaction Start Flags --------------------------*/
/* NOTE: OCI_TRANS_JOIN and OCI_TRANS_NOMIGRATE not supported in 8.0.X       */
#define OCI_TRANS_NEW          0x00000001 /* start a new local or global txn */
#define OCI_TRANS_JOIN         0x00000002     /* join an existing global txn */
#define OCI_TRANS_RESUME       0x00000004    /* resume the global txn branch */
#define OCI_TRANS_PROMOTE      0x00000008 /* promote the local txn to global */
#define OCI_TRANS_STARTMASK    0x000000ff  /* mask for start operation flags */

#define OCI_TRANS_READONLY     0x00000100            /* start a readonly txn */
#define OCI_TRANS_READWRITE    0x00000200          /* start a read-write txn */
#define OCI_TRANS_SERIALIZABLE 0x00000400        /* start a serializable txn */
#define OCI_TRANS_ISOLMASK     0x0000ff00  /* mask for start isolation flags */

#define OCI_TRANS_LOOSE        0x00010000        /* a loosely coupled branch */
#define OCI_TRANS_TIGHT        0x00020000        /* a tightly coupled branch */
#define OCI_TRANS_TYPEMASK     0x000f0000      /* mask for branch type flags */

#define OCI_TRANS_NOMIGRATE    0x00100000      /* non migratable transaction */
#define OCI_TRANS_SEPARABLE    0x00200000  /* separable transaction (8.1.6+) */
#define OCI_TRANS_OTSRESUME    0x00400000      /* OTS resuming a transaction */
#define OCI_TRANS_OTHRMASK     0xfff00000      /* mask for other start flags */


/*---------------------------------------------------------------------------*/

/*------------------------ Transaction End Flags ----------------------------*/
#define OCI_TRANS_TWOPHASE      0x01000000           /* use two phase commit */
#define OCI_TRANS_WRITEBATCH    0x00000001  /* force cmt-redo for local txns */
#define OCI_TRANS_WRITEIMMED    0x00000002              /* no force cmt-redo */
#define OCI_TRANS_WRITEWAIT     0x00000004               /* no sync cmt-redo */
#define OCI_TRANS_WRITENOWAIT   0x00000008   /* sync cmt-redo for local txns */
/*---------------------------------------------------------------------------*/

/* for schemas */
#define OCI_ATTR_LIST_OBJECTS              261  /* list of objects in schema */

#define OCI_ATTR_LIST_SCHEMAS              263            /* list of schemas */
#define OCI_ATTR_NOWAIT_SUPPORT            270             /* nowait support */
#define OCI_ATTR_EDITION                   288                /* ORA_EDITION */

#define OCI_ATTR_DISTINGUISHED_NAME        300        /* use DN as user name */

/*------------------------ Attributes for Rules objects ---------------------*/
#define OCI_ATTR_CONDITION                  342            /* rule condition */
#define OCI_ATTR_COMMENT                    343                   /* comment */
#define OCI_ATTR_VALUE                      344             /* Anydata value */
#define OCI_ATTR_EVAL_CONTEXT_OWNER         345        /* eval context owner */
#define OCI_ATTR_EVAL_CONTEXT_NAME          346         /* eval context name */
#define OCI_ATTR_EVALUATION_FUNCTION        347        /* eval function name */
#define OCI_ATTR_VAR_TYPE                   348             /* variable type */
#define OCI_ATTR_VAR_VALUE_FUNCTION         349   /* variable value function */
#define OCI_ATTR_VAR_METHOD_FUNCTION        350  /* variable method function */
#define OCI_ATTR_ACTION_CONTEXT             351            /* action context */
#define OCI_ATTR_LIST_TABLE_ALIASES         352     /* list of table aliases */
#define OCI_ATTR_LIST_VARIABLE_TYPES        353    /* list of variable types */
#define OCI_ATTR_TABLE_NAME                 356                /* table name */

/*----------------------- Attributes for End To End Tracing -----------------*/
#define OCI_ATTR_MODULE                     366        /* module for tracing */
#define OCI_ATTR_ACTION                     367        /* action for tracing */
#define OCI_ATTR_CLIENT_INFO                368               /* client info */
#define OCI_ATTR_COLLECT_CALL_TIME          369         /* collect call time */
#define OCI_ATTR_CALL_TIME                  370         /* extract call time */
#define OCI_ATTR_ECONTEXT_ID                371      /* execution-id context */
#define OCI_ATTR_ECONTEXT_SEQ               372  /*execution-id sequence num */

/*------------------------------ Session attributes -------------------------*/
#define OCI_ATTR_SESSION_STATE              373             /* session state */
#define OCI_SESSION_STATELESS  1                             /* valid states */
#define OCI_SESSION_STATEFUL   2

#define OCI_ATTR_SESSION_STATETYPE          374        /* session state type */
#define OCI_SESSION_STATELESS_DEF  0                    /* valid state types */
#define OCI_SESSION_STATELESS_CAL  1
#define OCI_SESSION_STATELESS_TXN  2
#define OCI_SESSION_STATELESS_APP  3

#define OCI_ATTR_SESSION_STATE_CLEARED      376     /* session state cleared */
#define OCI_ATTR_SESSION_MIGRATED           377       /* did session migrate */
#define OCI_ATTR_SESSION_PRESERVE_STATE     388    /* preserve session state */
#define OCI_ATTR_DRIVER_NAME                424               /* Driver Name */

/*----------------------- Attributes for End To End Tracing -----------------*/
/* -------------------------- HA Event Handle Attributes ------------------- */

#define OCI_ATTR_HOSTNAME         390                /* SYS_CONTEXT hostname */
#define OCI_ATTR_DBNAME           391                  /* SYS_CONTEXT dbname */
#define OCI_ATTR_INSTNAME         392           /* SYS_CONTEXT instance name */
#define OCI_ATTR_SERVICENAME      393            /* SYS_CONTEXT service name */
#define OCI_ATTR_INSTSTARTTIME    394      /* v$instance instance start time */
#define OCI_ATTR_HA_TIMESTAMP     395                          /* event time */
#define OCI_ATTR_RESERVED_22      396                            /* reserved */
#define OCI_ATTR_RESERVED_23      397                            /* reserved */
#define OCI_ATTR_RESERVED_24      398                            /* reserved */
#define OCI_ATTR_DBDOMAIN         399                           /* db domain */
#define OCI_ATTR_RESERVED_27      425                            /* reserved */

#define OCI_ATTR_EVENTTYPE        400                          /* event type */
#define OCI_EVENTTYPE_HA            0  /* valid value for OCI_ATTR_EVENTTYPE */

#define OCI_ATTR_HA_SOURCE        401
/* valid values for OCI_ATTR_HA_SOURCE */
#define OCI_HA_SOURCE_INSTANCE            0 
#define OCI_HA_SOURCE_DATABASE            1
#define OCI_HA_SOURCE_NODE                2
#define OCI_HA_SOURCE_SERVICE             3
#define OCI_HA_SOURCE_SERVICE_MEMBER      4
#define OCI_HA_SOURCE_ASM_INSTANCE        5
#define OCI_HA_SOURCE_SERVICE_PRECONNECT  6

/* ------------------------- Server Handle Attributes -----------------------*/

#define OCI_ATTR_TAF_ENABLED      405

/* Extra notification attributes */
#define OCI_ATTR_NFY_FLAGS        406 

#define OCI_ATTR_MSG_DELIVERY_MODE 407        /* msg delivery mode */
#define OCI_ATTR_DB_CHARSET_ID     416       /* database charset ID */
#define OCI_ATTR_DB_NCHARSET_ID    417       /* database ncharset ID */
#define OCI_ATTR_RESERVED_25       418                           /* reserved */

#define OCI_ATTR_FLOW_CONTROL_TIMEOUT       423  /* AQ: flow control timeout */

/*---------------- Describe Handle Parameter Attribute Values ---------------*/

/* OCI_ATTR_CURSOR_COMMIT_BEHAVIOR */
#define OCI_CURSOR_OPEN   0 
#define OCI_CURSOR_CLOSED 1

/* OCI_ATTR_SAVEPOINT_SUPPORT */
#define OCI_SP_SUPPORTED   0
#define OCI_SP_UNSUPPORTED 1

/* OCI_ATTR_NOWAIT_SUPPORT */
#define OCI_NW_SUPPORTED   0
#define OCI_NW_UNSUPPORTED 1

/* OCI_ATTR_AUTOCOMMIT_DDL */
#define OCI_AC_DDL    0
#define OCI_NO_AC_DDL 1

/* OCI_ATTR_LOCKING_MODE */
#define OCI_LOCK_IMMEDIATE 0
#define OCI_LOCK_DELAYED   1

/*---------------------------OCIPasswordChange-------------------------------*/
#define OCI_AUTH         0x08        /* Change the password but do not login */
#define OCI_ROWID_LEN             23
#define OCI_SQLSTATE_SIZE 5  

/*--------------------------- OCI Parameter Types ---------------------------*/
#define OCI_PTYPE_UNK                 0                         /* unknown   */
#define OCI_PTYPE_TABLE               1                         /* table     */
#define OCI_PTYPE_VIEW                2                         /* view      */
#define OCI_PTYPE_PROC                3                         /* procedure */
#define OCI_PTYPE_FUNC                4                         /* function  */
#define OCI_PTYPE_PKG                 5                         /* package   */
#define OCI_PTYPE_TYPE                6                 /* user-defined type */
#define OCI_PTYPE_SYN                 7                         /* synonym   */
#define OCI_PTYPE_SEQ                 8                         /* sequence  */
#define OCI_PTYPE_COL                 9                         /* column    */
#define OCI_PTYPE_ARG                 10                        /* argument  */
#define OCI_PTYPE_LIST                11                        /* list      */
#define OCI_PTYPE_TYPE_ATTR           12    /* user-defined type's attribute */
#define OCI_PTYPE_TYPE_COLL           13        /* collection type's element */
#define OCI_PTYPE_TYPE_METHOD         14       /* user-defined type's method */
#define OCI_PTYPE_TYPE_ARG            15   /* user-defined type method's arg */
#define OCI_PTYPE_TYPE_RESULT         16/* user-defined type method's result */
#define OCI_PTYPE_SCHEMA              17                           /* schema */
#define OCI_PTYPE_DATABASE            18                         /* database */
#define OCI_PTYPE_RULE                19                             /* rule */
#define OCI_PTYPE_RULE_SET            20                         /* rule set */
#define OCI_PTYPE_EVALUATION_CONTEXT  21               /* evaluation context */
#define OCI_PTYPE_TABLE_ALIAS         22                      /* table alias */
#define OCI_PTYPE_VARIABLE_TYPE       23                    /* variable type */
#define OCI_PTYPE_NAME_VALUE          24                  /* name value pair */

/*---------------------------------------------------------------------------*/

/*----------------------------- OCI List Types ------------------------------*/
#define OCI_LTYPE_UNK           0                               /* unknown   */
#define OCI_LTYPE_COLUMN        1                             /* column list */
#define OCI_LTYPE_ARG_PROC      2                 /* procedure argument list */
#define OCI_LTYPE_ARG_FUNC      3                  /* function argument list */
#define OCI_LTYPE_SUBPRG        4                         /* subprogram list */
#define OCI_LTYPE_TYPE_ATTR     5                          /* type attribute */
#define OCI_LTYPE_TYPE_METHOD   6                             /* type method */
#define OCI_LTYPE_TYPE_ARG_PROC 7    /* type method w/o result argument list */
#define OCI_LTYPE_TYPE_ARG_FUNC 8      /* type method w/result argument list */
#define OCI_LTYPE_SCH_OBJ       9                      /* schema object list */
#define OCI_LTYPE_DB_SCH        10                   /* database schema list */
#define OCI_LTYPE_TYPE_SUBTYPE  11                           /* subtype list */
#define OCI_LTYPE_TABLE_ALIAS   12                       /* table alias list */
#define OCI_LTYPE_VARIABLE_TYPE 13                     /* variable type list */
#define OCI_LTYPE_NAME_VALUE    14                        /* name value list */

/*--------------------------- User Callback Constants -----------------------*/
#define OCI_UCBTYPE_ENTRY       1                          /* entry callback */
#define OCI_UCBTYPE_EXIT        2                           /* exit callback */
#define OCI_UCBTYPE_REPLACE     3                    /* replacement callback */

/*--------------------- NLS service type and constance ----------------------*/
#define OCI_NLS_DAYNAME1      1                    /* Native name for Monday */
#define OCI_NLS_DAYNAME2      2                   /* Native name for Tuesday */
#define OCI_NLS_DAYNAME3      3                 /* Native name for Wednesday */
#define OCI_NLS_DAYNAME4      4                  /* Native name for Thursday */
#define OCI_NLS_DAYNAME5      5                    /* Native name for Friday */
#define OCI_NLS_DAYNAME6      6              /* Native name for for Saturday */
#define OCI_NLS_DAYNAME7      7                /* Native name for for Sunday */
#define OCI_NLS_ABDAYNAME1    8        /* Native abbreviated name for Monday */
#define OCI_NLS_ABDAYNAME2    9       /* Native abbreviated name for Tuesday */
#define OCI_NLS_ABDAYNAME3    10    /* Native abbreviated name for Wednesday */
#define OCI_NLS_ABDAYNAME4    11     /* Native abbreviated name for Thursday */
#define OCI_NLS_ABDAYNAME5    12       /* Native abbreviated name for Friday */
#define OCI_NLS_ABDAYNAME6    13 /* Native abbreviated name for for Saturday */
#define OCI_NLS_ABDAYNAME7    14   /* Native abbreviated name for for Sunday */
#define OCI_NLS_MONTHNAME1    15                  /* Native name for January */
#define OCI_NLS_MONTHNAME2    16                 /* Native name for February */
#define OCI_NLS_MONTHNAME3    17                    /* Native name for March */
#define OCI_NLS_MONTHNAME4    18                    /* Native name for April */
#define OCI_NLS_MONTHNAME5    19                      /* Native name for May */
#define OCI_NLS_MONTHNAME6    20                     /* Native name for June */
#define OCI_NLS_MONTHNAME7    21                     /* Native name for July */
#define OCI_NLS_MONTHNAME8    22                   /* Native name for August */
#define OCI_NLS_MONTHNAME9    23                /* Native name for September */
#define OCI_NLS_MONTHNAME10   24                  /* Native name for October */
#define OCI_NLS_MONTHNAME11   25                 /* Native name for November */
#define OCI_NLS_MONTHNAME12   26                 /* Native name for December */
#define OCI_NLS_ABMONTHNAME1  27      /* Native abbreviated name for January */
#define OCI_NLS_ABMONTHNAME2  28     /* Native abbreviated name for February */
#define OCI_NLS_ABMONTHNAME3  29        /* Native abbreviated name for March */
#define OCI_NLS_ABMONTHNAME4  30        /* Native abbreviated name for April */
#define OCI_NLS_ABMONTHNAME5  31          /* Native abbreviated name for May */
#define OCI_NLS_ABMONTHNAME6  32         /* Native abbreviated name for June */
#define OCI_NLS_ABMONTHNAME7  33         /* Native abbreviated name for July */
#define OCI_NLS_ABMONTHNAME8  34       /* Native abbreviated name for August */
#define OCI_NLS_ABMONTHNAME9  35    /* Native abbreviated name for September */
#define OCI_NLS_ABMONTHNAME10 36      /* Native abbreviated name for October */
#define OCI_NLS_ABMONTHNAME11 37     /* Native abbreviated name for November */
#define OCI_NLS_ABMONTHNAME12 38     /* Native abbreviated name for December */
#define OCI_NLS_YES           39   /* Native string for affirmative response */
#define OCI_NLS_NO            40                 /* Native negative response */
#define OCI_NLS_AM            41           /* Native equivalent string of AM */
#define OCI_NLS_PM            42           /* Native equivalent string of PM */
#define OCI_NLS_AD            43           /* Native equivalent string of AD */
#define OCI_NLS_BC            44           /* Native equivalent string of BC */
#define OCI_NLS_DECIMAL       45                        /* decimal character */
#define OCI_NLS_GROUP         46                          /* group separator */
#define OCI_NLS_DEBIT         47                   /* Native symbol of debit */
#define OCI_NLS_CREDIT        48                  /* Native sumbol of credit */
#define OCI_NLS_DATEFORMAT    49                       /* Oracle date format */
#define OCI_NLS_INT_CURRENCY  50            /* International currency symbol */
#define OCI_NLS_LOC_CURRENCY  51                   /* Locale currency symbol */
#define OCI_NLS_LANGUAGE      52                            /* Language name */
#define OCI_NLS_ABLANGUAGE    53           /* Abbreviation for language name */
#define OCI_NLS_TERRITORY     54                           /* Territory name */
#define OCI_NLS_CHARACTER_SET 55                       /* Character set name */
#define OCI_NLS_LINGUISTIC_NAME    56                     /* Linguistic name */
#define OCI_NLS_CALENDAR      57                            /* Calendar name */
#define OCI_NLS_DUAL_CURRENCY 78                     /* Dual currency symbol */
#define OCI_NLS_WRITINGDIR    79               /* Language writing direction */
#define OCI_NLS_ABTERRITORY   80                   /* Territory Abbreviation */
#define OCI_NLS_DDATEFORMAT   81               /* Oracle default date format */
#define OCI_NLS_DTIMEFORMAT   82               /* Oracle default time format */
#define OCI_NLS_SFDATEFORMAT  83       /* Local string formatted date format */
#define OCI_NLS_SFTIMEFORMAT  84       /* Local string formatted time format */
#define OCI_NLS_NUMGROUPING   85                   /* Number grouping fields */
#define OCI_NLS_LISTSEP       86                           /* List separator */
#define OCI_NLS_MONDECIMAL    87               /* Monetary decimal character */
#define OCI_NLS_MONGROUP      88                 /* Monetary group separator */
#define OCI_NLS_MONGROUPING   89                 /* Monetary grouping fields */
#define OCI_NLS_INT_CURRENCYSEP 90       /* International currency separator */
#define OCI_NLS_CHARSET_MAXBYTESZ 91     /* Maximum character byte size      */
#define OCI_NLS_CHARSET_FIXEDWIDTH 92    /* Fixed-width charset byte size    */
#define OCI_NLS_CHARSET_ID    93                         /* Character set id */
#define OCI_NLS_NCHARSET_ID   94                        /* NCharacter set id */

#define OCI_NLS_MAXBUFSZ   100 /* Max buffer size may need for OCINlsGetInfo */

#define OCI_NLS_BINARY            0x1           /* for the binary comparison */
#define OCI_NLS_LINGUISTIC        0x2           /* for linguistic comparison */
#define OCI_NLS_CASE_INSENSITIVE  0x10    /* for case-insensitive comparison */

#define OCI_NLS_UPPERCASE         0x20               /* convert to uppercase */
#define OCI_NLS_LOWERCASE         0x40               /* convert to lowercase */

/*------------------------- Version information -----------------------------*/
#define OCI_MAJOR_VERSION  11                       /* Major release version */
#define OCI_MINOR_VERSION  2                        /* Minor release version */

//#include <oci8dp.h>         /* interface definitions for the direct path api */

#endif//__Oracle_OciCommon__
