#ifndef _Oracle_nonblocking_h_
#define _Oracle_nonblocking_h_


ii. Non-blocking OCI
In non-blocking mode OCI calls return ORA-3123 if the call is still waiting for the server. As the OCI functions do not block in 
this mode it is possible to issue OCIBreak() if no response is received after a reasonable timeout. 
You may need to keep calling OCI functions after this until an error is received on the connection for non-delivery of the TCP packet.

A combination of these 2 approaches may be the best option such as using non-blocking mode along with 'ognfd()' and a 'select()' statement. This allows client code to wait for a response for a preset period of time without wasting CPU. If a response is received this can be processed otherwise the timer will expire and OCIBreak() can be sent.

Eg: Example of obtaining the file handle using ognfd() and monitoring
    it with "select()". (Error handling skipped for readability)

	Lda_Def lda;		/* An OCI7 LDA for temporary use */
   	int fd;			/* File handle SOCKET */
	fd_set maset;		/* File descriptors to monitor */
	fd_set fdSet;		/* File descriptors with events pending */
        struct timeval timer;	/* Timeout */

        timer.tv_sec=300;	/* 5 minute timeout */
        timer.tv_usec=0;

	...

	OCIServerAttach( ... ) /* or OCILogon() */ 
 	OCISessionBegin( ... ) 
	
	...

    	OCISvcCtxToLda( svchp, errhp, &lda );	/* Convert to V7 LDA */
    	ognfd(&lda,&fd);			/* Get socket handle */
    	OCILdaToSvcCtx( &svchp, errhp, &lda );	/* Back to V8 */
  	
	FD_ZERO(&maet);			/* ZERO handles to monitor */
	FD_SET(fd,&maset);			/* Save the file handle */

	fdSet=maset;				/* Set handles to monitor */
	do
	{ 
	  if (FD_ISSET(fd,&fdSet) { 
	    /* 1st time queue an OCI request, 2nd time poll for result */
	    ... non blocking OCI call goes here ...
	    ... Process errors , but fall through on ORA-3123 to the select ...
	    ... On success move on to the next OCI statement ...
	  }
	  fdset=maset;				/* Set handles to monitor */
	  printf( "Waiting for an event ... \n");
	} while (select(maxfd,&fdSet,0,0,&timer)

https://twiki.cern.ch/twiki/bin/viewfile/PSSGroup/OCIClientHangProtection?rev=1;filename=OCI_code_with_timeouts.txt

You can cancel a long-running OCI call by using the OCIBreak() function. After issuing an OCIBreak() while an OCI call is in progress, you must issue an OCIReset() call to reset the asynchronous operation and protocol.

If an application needs to maintain multiple user sessions on a database connection, the application requires a different set of calls to set up the sessions and connections. This includes specific calls to attach to the server and begin sessions:

OCIServerAttach() creates an access path to the data server for OCI operations.
OCISessionBegin() establishes a session for a user against a particular server. This call is required for the user to be able to execute any operation on the server.

Note: See "Non-Blocking Mode" for information about specifying a blocking or non-blocking connection in the OCIServerAttach() call.

Note: Using the OCI_COMMIT_ON_SUCCESS mode of the OCIExecute() call, the application can selectively commit transactions at the end of each statement execution, saving an extra roundtrip.

For output variables, indicator variables determine whether the value returned from Oracle is a NULL or a truncated value. 
In the case of a NULL fetch (on OCIStmtFetch()) or a truncation (on OCIStmtExecute() or OCIStmtFetch()), 
the OCI call returns OCI_SUCCESS_WITH_INFO. The corresponding indicator variable is set to the appropriate value, 
as listed in Table 2-8, "Output Indicator Values". If the application provided a return code variable in the corresponding 
OCIDefineByPos() call, the OCI assigns a value of ORA-01405 (for NULL fetch) or ORA-01406 (for truncation) to the return code variable.
when the fetch is performed, the ROWID attribute in the handle contains the row identifier of the SELECTed row. You can retrieve the ROWID into a buffer in your program by calling OCIAttrGet() as follows:

OCIRowid *rowid;   /* the rowid in opaque format */
/* allocate descriptor with OCIDescriptorAlloc() */
err = OCIAttrGet ((dvoid*) mystmtp, OCI_HTYPE_STMT, 
        (dvoid*) &rowid, (ub4 *) 0, OCI_ATTR_ROWID, (OCIError *) myerrhp);

You can then use the saved ROWID in a DELETE or UPDATE statement. For example, if MY_ROWID is the buffer in which the row identifier has been saved, you can later process a SQL statement such as

UPDATE emp SET sal = :1 WHERE rowid = :2
Actual NON-blocking of calls can potentially occur at all calls that talk to the server, which are calls that 
take a Server or Service context as an argument. I would
 recommend only setting OCI_ATTR_NONBLOCKING_MODE around calls, where you exlicitly want to check it, 
 which in real terms normally would be OCIExecuteStmt and OCIFetch only. Note, that OCIFetch does not actually take a Server or 
 Service context as argument, and due to the prefetch mechnism, not all actual OCIFetch calls will return the OCI_STILL_EXECUTING code.

	         
#endif
