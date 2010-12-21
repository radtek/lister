#ifndef _MyTool002_ConnHandler_h_
#define _MyTool002_ConnHandler_h_

#include "shared.h"
#include "ConnHandler.h"
class ConnButton;
#include "ConnState.h"
#include "ConnGrid.h"
#include "Connection.h"

#include <Oracle/Oracle7.h>
#include <Oracle/Oracle8.h>
#include <MSSQL/MSSQL.h>
#include <PostgreSQL/PostgreSQL.h>

//==========================================================================================	
class ConnHandler {
public:
	
	GridCtrl *outputGrid;
	//==========================================================================================	
	ConnHandler() {
	}

	//==========================================================================================	
	~ConnHandler() {
	}
	
	// Connect to the database highlighted in the connection grid
	
	//==========================================================================================	
	void DbActive() {
		Exclamation("Active");
	}
	
	//==========================================================================================	
	EnumConnState Connect(String instanceTypeName, String loginStr, String loginPwd, String instanceAddress, Connection &conn) {
		// Determine what grid we are in by progressing back (Assumes we are clicking a button in a connection list grid)
		
		String connStr;
		
		if (instanceTypeName == "Oracle") {
			connStr << loginStr << "/" << loginPwd << "@" << instanceAddress;

			//One<Oracle8> oci = new Oracle8;
			One<Oracle7> oci = new Oracle7; // Needed to overcome ORA-1406 error on large column sets that fail on oci8 
			
			if (!oci->Open(connStr /* no objects (yet) */)) { // adjust for your Oracle server
				Exclamation(NFormat("Oracle connect failed, [* \1%s\1].", oci->GetLastError()));
				conn.enumConnState = CON_FAIL;
				conn.session = NULL;
				delete -oci;
				return conn.enumConnState;
			}

			// Make the connection generic so cursor can traverse any type of database			
			conn.session = -oci;
			// NLS_DATE_FORMAT (alter session dsenv? alter session set nls_date_format='DD-MON-YYYY HH24:MI:SS'
			//conn.session->WhenDatabaseActivity = THISBACK(DbActive);
			
			conn.enumConnState = CON_SUCCEED;

			return conn.enumConnState;
		} else if (instanceTypeName == "MS SQL") {
			//Microsoft SQL Server ODBC Driver Version 03.85.1132
			//SQL Native Client
			// iSeries Access ODBC Driver
			// Client Access ODBC Driver (32-bit)
			// Microsoft Excel Driver (*.xls)
			// Microsoft dBase Driver (*.dbf)
			// Microsoft Text Driver (*.txt; *.csv)
			
			
			//connStr << "Driver={SQL Server Native Client 10.0};"  // Doesn't work
			connStr << "Driver={SQL Server};"  // 
				<< "Server=" << instanceAddress << ";"
				<< "UID=" << loginStr << ";"
				<< "PWD=" << loginPwd << ";"
				// "Database=" << ?
				// Trusted_Connection=Yes;
				;
			One<MSSQLSession> mssql10 = new MSSQLSession;
			
			if (!mssql10->Connect(connStr)) {
				Exclamation(NFormat("MS SQL connect failed, [* \1%s\1].", mssql10->GetLastError()));
				conn.enumConnState = CON_FAIL;
				conn.session = NULL;
				delete -mssql10;
				return conn.enumConnState;
			}
			
			conn.session = -mssql10;

			conn.enumConnState = CON_SUCCEED;
			
			return conn.enumConnState;
		
		} else if (instanceTypeName == "FTP") {
			Exclamation(CAT << "Error: FTP not implemented");

			// FTP
			// C:\upp\uppsrc\plugin\ftp\ftp.cpp(44): if(!FtpConnect(host, &ftpconn, perror, &FtpClient::Callback, this, 200, idletimeout_secs)) {
		} else {
			Exclamation(CAT << "Error: Unrecognized instance type: " << instanceTypeName);
			conn.enumConnState = NOCON_MISCONFIG;
			conn.session = NULL;
			return conn.enumConnState;
		}
		
		return conn.enumConnState;
	}
};

#endif


/*

	Array<OleDBSession::Provider> providers = OleDBSession::EnumProviders();
	for(int i = 0; i < providers.GetCount(); i++)
		login.connection.Add("OLEDB:" + providers[i].name);



http://www.filibeto.org/sun/lib/nonsun/oracle/11.2.0.1.0/E11882_01_200909/appdev.112/e10646/oci22glb001.htm
This section describes the OCI locale functions

	Rect r = GetRect();
	r.Offset(200, 0);
	SetRect(r);

*/
