#ifndef _lister_dllhell_h_
#define _lister_dllhell_h_
/*

lister.exe from c:\MyApps\Oracle\depends.exe

Before any dynamic loaders:
- c:\program files\postgresql\9.0\bin\LIBPQ.DLL 10/03/2010 1:12p 160,256 0002B962 9.0.1.10275
-   c:\program files\postgresql\9.0\bin\LIBINTL-8.DLL
-   c:\winnt\winsxs\x86_microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4974_x-ww_d889290f\MSVCR90.DLL
-     c:\program files\postgresql\9.0\bin\LIBICONV-2.DLL
- c:\oracle\product\11.2.0.1\client_1\OCI.DLL version: is 04/02/2010 6:42a 987,136 F9CB6 11.2.0.1
- c:\oracle\product\11.2.0.1\client_1\ORAOCIEI11.DLL 04/02/2010 6:49a 126,205,952 11.2.0.1

After loading Oracle8:
DllMain(0x02640000, DLL_PROCESS_ATTACH, 0x00000000) in "OCI.DLL" called.
DllMain(0x02640000, DLL_PROCESS_ATTACH, 0x00000000) in "OCI.DLL" returned 1 (0x1).
LoadLibraryA("oci.dll") returned 0x02640000.
Connection failed, Service name not found!!!! I didn't really install it very well.
Path=
C:\FreeBase\Oracle\Client\instantclientwin3211.2.0.1\instantclient_11_2;        // Doesn't work well; not installed fully
C:\Program Files\oracle\product\10.2.0\client_1\bin;                            // Works?
C:\WINNT\system32;
C:\WINNT;
C:\WINNT\System32\Wbem;
C:\Program Files\PKWARE\pkzipc;
C:\Program Files\Rational\ClearCase\etc\utils;
C:\Program Files\Rational\common;
C:\Program Files\Rational\ClearCase\bin;
C:\Program Files\Microsoft SQL Server\80\Tools\Binn\;
C:\Program Files\IBM\Client Access\;
C:\Program Files\IBM\Client Access\Shared\;
C:\Program Files\Microsoft SQL Server\90\Tools\binn\;
C:\Program Files\Microsoft SQL Server\90\DTS\Binn\;
C:\Program Files\Microsoft SQL Server\90\Tools\Binn\VSShell\Common7\IDE\;
C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\PrivateAssemblies\;
C:\Program Files\Microsoft Visual Studio 9.0\VC\bin;
C:\Program Files\PostgreSQL\9.0\bin;
C:\Program Files\Subversion\bin;
C:\Program Files\TortoiseSVN\bin

Changed order:
Path=
C:\Program Files\oracle\product\10.2.0\client_1\bin;                            // Works?
C:\WINNT\system32;
C:\WINNT;
C:\WINNT\System32\Wbem;
C:\Program Files\PKWARE\pkzipc;
C:\Program Files\Rational\ClearCase\etc\utils;
C:\Program Files\Rational\common;
C:\Program Files\Rational\ClearCase\bin;
C:\Program Files\Microsoft SQL Server\80\Tools\Binn\;
C:\Program Files\IBM\Client Access\;
C:\Program Files\IBM\Client Access\Shared\;
C:\Program Files\Microsoft SQL Server\90\Tools\binn\;
C:\Program Files\Microsoft SQL Server\90\DTS\Binn\;
C:\Program Files\Microsoft SQL Server\90\Tools\Binn\VSShell\Common7\IDE\;
C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\PrivateAssemblies\;
C:\Program Files\Microsoft Visual Studio 9.0\VC\bin;
C:\Program Files\PostgreSQL\9.0\bin;
C:\Program Files\Subversion\bin;
C:\Program Files\TortoiseSVN\bin;
C:\FreeBase\Oracle\Client\instantclientwin3211.2.0.1\instantclient_11_2;        // Doesn't work well; not installed fully

Testing Oracle8 connection...
Still can't connect.  Can't find TNS names
ORACLE_HOME=c:\Program Files\oracle\product\10.2.0\client_1
Tried again from profiler...worked.
c:\program files\oracle\product\10.2.0\client_1\bin\OCI.DLL 09/30/2007 8:09a 352,256 10.2.0.1
I think U++ was holding the old environment.
Tried to open c:\program files\oracle\product\10.2.0\client_1\bin\ORAOCIEI10.DLL, failed
Tried to open c:\program files\oracle\product\10.2.0\client_1\bin\ORAOCIICUS10.DLL, failed
Opened c:\program files\oracle\product\10.2.0\client_1\bin\ORACLIENT10.DLL 11/26/2007 10:20a 2,408,448 10.2.0.3

Ok, trace for select t.SRCS_FD_TS from cdw1_user.vw_trade t where td_fd_src_nm = 'STRAW' and tdor_src_nm = 'BLOOMBERGBACNY' and cob_dt = '03-DEC-10'

GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIHandleAlloc") called from "OCI.DLL" at address 0x025727D1 and returned 0x025D5FCA.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIHandleAlloc") called from "OCI.DLL" at address 0x025727D1 and returned 0x025D5FCA.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrSet") called from "OCI.DLL" at address 0x025739E1 and returned 0x025D6D80.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIStmtPrepare") called from "OCI.DLL" at address 0x02572B9D and returned 0x025D651C.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrGet") called from "OCI.DLL" at address 0x02573999 and returned 0x025D6D5E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIStmtExecute") called from "OCI.DLL" at address 0x02572E1D and returned 0x025D68A4.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrGet") called from "OCI.DLL" at address 0x02573999 and returned 0x025D6D5E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrSet") called from "OCI.DLL" at address 0x025739E1 and returned 0x025D6D80.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIStmtPrepare") called from "OCI.DLL" at address 0x02572B9D and returned 0x025D651C.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrGet") called from "OCI.DLL" at address 0x02573999 and returned 0x025D6D5E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIStmtExecute") called from "OCI.DLL" at address 0x02572E1D and returned 0x025D68A4.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrGet") called from "OCI.DLL" at address 0x02573999 and returned 0x025D6D5E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIParamGet") called from "OCI.DLL" at address 0x02573089 and returned 0x025D6A2E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrGet") called from "OCI.DLL" at address 0x02573999 and returned 0x025D6D5E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrGet") called from "OCI.DLL" at address 0x02573999 and returned 0x025D6D5E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrGet") called from "OCI.DLL" at address 0x02573999 and returned 0x025D6D5E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrGet") called from "OCI.DLL" at address 0x02573999 and returned 0x025D6D5E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIAttrGet") called from "OCI.DLL" at address 0x02573999 and returned 0x025D6D5E.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIDefineByPos") called from "OCI.DLL" at address 0x02572E69 and returned 0x025D68CE.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIStmtFetch") called from "OCI.DLL" at address 0x02572F95 and returned 0x025D6962.
GetProcAddress(0x025D0000 [ORACLIENT10.DLL], "OCIErrorGet") called from "OCI.DLL" at address 0x02573295 and returned 0x025D6B1E.

Error is: Error: #=1406, ORA`-01406`: fetched column value was truncated
Datatype is timestamp.
Issue is header??


GetProcAddress(0x02640000 [OCI.DLL], "OCIInitialize") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BF338.
GetProcAddress(0x02640000 [OCI.DLL], "OCITerminate") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x02679D04.
GetProcAddress(0x02640000 [OCI.DLL], "OCIHandleAlloc") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BB87C.
GetProcAddress(0x02640000 [OCI.DLL], "OCIHandleFree") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BB6A8.
GetProcAddress(0x02640000 [OCI.DLL], "OCIDescriptorAlloc") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BB4CC.
GetProcAddress(0x02640000 [OCI.DLL], "OCIDescriptorFree") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BB2F8.
GetProcAddress(0x02640000 [OCI.DLL], "OCIEnvInit") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BF514.
GetProcAddress(0x02640000 [OCI.DLL], "OCIEnvNlsCreate") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x0266FEE4.
GetProcAddress(0x02640000 [OCI.DLL], "OCINlsEnvironmentVariableGet") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x0267248C.
GetProcAddress(0x02640000 [OCI.DLL], "OCINlsCharSetNameToId") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026720DC.
GetProcAddress(0x02640000 [OCI.DLL], "OCINlsCharSetIdToName") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x02671F00.
GetProcAddress(0x02640000 [OCI.DLL], "OCIEnvCreate") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x02676C80.
GetProcAddress(0x02640000 [OCI.DLL], "OCIServerAttach") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BAD68.
GetProcAddress(0x02640000 [OCI.DLL], "OCIServerDetach") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BAB90.
GetProcAddress(0x02640000 [OCI.DLL], "OCISessionBegin") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BA9B4.
GetProcAddress(0x02640000 [OCI.DLL], "OCISessionEnd") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BA7D8.
GetProcAddress(0x02640000 [OCI.DLL], "OCILogon") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BA5F0.
GetProcAddress(0x02640000 [OCI.DLL], "OCILogoff") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BA41C.
GetProcAddress(0x02640000 [OCI.DLL], "OCIPasswordChange") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BA234.
GetProcAddress(0x02640000 [OCI.DLL], "OCIStmtPrepare") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B9910.
GetProcAddress(0x02640000 [OCI.DLL], "OCIBindByPos") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B9718.
GetProcAddress(0x02640000 [OCI.DLL], "OCIBindDynamic") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B9158.
GetProcAddress(0x02640000 [OCI.DLL], "OCIStmtGetBindInfo") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B7E60.
GetProcAddress(0x02640000 [OCI.DLL], "OCIStmtExecute") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B89C0.
GetProcAddress(0x02640000 [OCI.DLL], "OCIDefineByPos") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B87D0.
GetProcAddress(0x02640000 [OCI.DLL], "OCIStmtFetch") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B8050.
GetProcAddress(0x02640000 [OCI.DLL], "OCIParamGet") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B7A98.
GetProcAddress(0x02640000 [OCI.DLL], "OCIParamSet") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B78B8.
GetProcAddress(0x02640000 [OCI.DLL], "OCITransStart") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B76DC.
GetProcAddress(0x02640000 [OCI.DLL], "OCITransDetach") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B7504.
GetProcAddress(0x02640000 [OCI.DLL], "OCITransCommit") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B732C.
GetProcAddress(0x02640000 [OCI.DLL], "OCITransRollback") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B7154.
GetProcAddress(0x02640000 [OCI.DLL], "OCITransPrepare") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B6F7C.
GetProcAddress(0x02640000 [OCI.DLL], "OCITransForget") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B6DA4.
GetProcAddress(0x02640000 [OCI.DLL], "OCIErrorGet") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BF6F0.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobAppend") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B6BC8.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobAssign") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B69EC.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobCharSetForm") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B6810.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobCharSetId") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B6634.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobCopy") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B6450.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobDisableBuffering") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B6278.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobEnableBuffering") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B60A0.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobErase") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B5EC4.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobFlushBuffer") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B4FDC.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobGetLength") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B4E00.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobIsEqual") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B4C24.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobLocatorIsInit") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026BF8D4.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobRead") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B484C.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobTrim") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B4670.
GetProcAddress(0x02640000 [OCI.DLL], "OCILobWrite") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B4478.
GetProcAddress(0x02640000 [OCI.DLL], "OCIBreak") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B393C.
GetProcAddress(0x02640000 [OCI.DLL], "OCIAttrGet") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B357C.
GetProcAddress(0x02640000 [OCI.DLL], "OCIAttrSet") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x026B339C.
LoadLibraryA("kernel32.dll") called from "OCI.DLL" at address 0x026C6030.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetDllDirectoryA") called from "OCI.DLL" at address 0x026C604C and returned 0x7C85FFAF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "SetDllDirectoryA") called from "OCI.DLL" at address 0x026C6088 and returned 0x7C85FDAF.
LoadLibraryA("c:\oracle\product\11.2.0.1\client_1\OraOCIEI11.dll") called from "OCI.DLL" at address 0x026C6233.
Loaded "ORAOCIEI11.DLL" at address 0x08380000.  Successfully hooked module.
Loaded "IPHLPAPI.DLL" at address 0x76D60000.  Successfully hooked module.
DllMain(0x08380000, DLL_PROCESS_ATTACH, 0x00000000) in "ORAOCIEI11.DLL" called.
LoadLibraryA("KERNEL32.DLL") called from "ORAOCIEI11.DLL" at address 0x083FA98B.
LoadLibraryA("KERNEL32.DLL") returned 0x7C800000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "TryEnterCriticalSection") called from "ORAOCIEI11.DLL" at address 0x083FA99C and returned 0x7C901118.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "TryEnterCriticalSection") called from "ORAOCIEI11.DLL" at address 0x083919C2 and returned 0x7C901118.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "accept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1040.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "bind") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4480.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "closesocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3E2B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "connect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4A07.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getpeername") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0B68.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3D10.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB46EA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ioctlsocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3F50.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_addr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_ntoa") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB45C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "listen") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8CD3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB676F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recvfrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2FF7.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "select") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB30A8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "send") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4C27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "sendto") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2F51.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "setsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4521.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "shutdown") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0BF6.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "socket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4211.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyaddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE491.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5355.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE18E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobynumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE0CC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE703.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyport") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE5B5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5449.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0991.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByAddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEA43.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE99D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByNumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE2C3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEAED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByPort") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE913.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE867.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelAsyncRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE347.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD4AC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAUnhookBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD508.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3CCE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A5E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelBlockingCall") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD40A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIsBlocking") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD459.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStartup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6A55.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACleanup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3FED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAccept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0DC1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACloseEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB65E8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAConnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0C81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACreateEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB655D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB9A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB0C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNetworkEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB657D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDC5F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB888C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEventSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB64D9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetOverlappedResult") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0D1B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetQOSByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF470.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3EC0.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAJoinLeaf") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0F70.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4CB5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF5EE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvFrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF66A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAResetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C80.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASend") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB68FA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0A22.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendTo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0AAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C91.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8B6A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB404E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAWaitForMultipleEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9462.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6979.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9765.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB281E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB562A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB35EF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5A8D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3181.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANSPIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5086.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceEnd") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB350E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC03C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFBC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARemoveServiceClass") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFCB4.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0440.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0159.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF9E1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFA41.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFD9F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFF81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC04D2.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0281.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCDeinstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC19C9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1665.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnumProtocols") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB7B61.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCGetProviderPath") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8E27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF14D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUnInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF319.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnableNSProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEE59.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WPUCompleteOverlappedRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1CBF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAProviderConfigChange") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8C20.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteProviderOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1531.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteNameSpaceOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF019.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUpdateProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1291.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetAddrInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2899.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getnameinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC689.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetNameInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC504.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "freeaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2B0B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "accept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1040.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "bind") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4480.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "closesocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3E2B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "connect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4A07.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getpeername") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0B68.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3D10.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB46EA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ioctlsocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3F50.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_addr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_ntoa") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB45C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "listen") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8CD3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB676F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recvfrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2FF7.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "select") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB30A8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "send") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4C27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "sendto") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2F51.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "setsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4521.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "shutdown") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0BF6.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "socket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4211.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyaddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE491.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5355.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE18E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobynumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE0CC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE703.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyport") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE5B5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5449.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0991.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByAddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEA43.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE99D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByNumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE2C3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEAED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByPort") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE913.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE867.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelAsyncRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE347.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD4AC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAUnhookBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD508.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3CCE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A5E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelBlockingCall") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD40A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIsBlocking") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD459.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStartup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6A55.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACleanup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3FED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAccept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0DC1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACloseEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB65E8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAConnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0C81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACreateEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB655D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB9A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB0C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNetworkEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB657D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDC5F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB888C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEventSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB64D9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetOverlappedResult") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0D1B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetQOSByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF470.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3EC0.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAJoinLeaf") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0F70.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4CB5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF5EE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvFrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF66A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAResetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C80.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASend") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB68FA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0A22.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendTo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0AAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C91.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8B6A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB404E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAWaitForMultipleEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9462.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6979.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9765.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB281E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB562A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB35EF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5A8D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3181.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANSPIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5086.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceEnd") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB350E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC03C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFBC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARemoveServiceClass") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFCB4.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0440.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0159.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF9E1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFA41.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFD9F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFF81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC04D2.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0281.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCDeinstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC19C9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1665.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnumProtocols") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB7B61.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCGetProviderPath") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8E27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF14D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUnInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF319.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnableNSProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEE59.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WPUCompleteOverlappedRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1CBF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAProviderConfigChange") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8C20.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteProviderOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1531.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteNameSpaceOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF019.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUpdateProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1291.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetAddrInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2899.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getnameinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC689.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetNameInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC504.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "freeaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2B0B.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CancelIo") called from "ORAOCIEI11.DLL" at address 0x09019855 and returned 0x7C8300E2.
DllMain(0x08380000, DLL_PROCESS_ATTACH, 0x00000000) in "ORAOCIEI11.DLL" returned 1 (0x1).
LoadLibraryA("c:\oracle\product\11.2.0.1\client_1\OraOCIEI11.dll") returned 0x08380000.
GetProcAddress(0x08380000 [ORAOCIEI11.DLL], "xaosw") called from "OCI.DLL" at address 0x026C63FE and returned 0x0F9F0C80.
GetProcAddress(0x08380000 [ORAOCIEI11.DLL], "xaoswd") called from "OCI.DLL" at address 0x026C6411 and returned 0x0F9F0CE0.
GetProcAddress(0x08380000 [ORAOCIEI11.DLL], "vsnnum") called from "OCI.DLL" at address 0x026C64B4 and returned 0x0F7958E0.
GetProcAddress(0x08380000 [ORAOCIEI11.DLL], "vsnsta") called from "OCI.DLL" at address 0x026C64D4 and returned 0x0F9D9844.
GetProcAddress(0x08380000 [ORAOCIEI11.DLL], "OCIEnvNlsCreate") called from "OCI.DLL" at address 0x0266FF4B and returned 0x08BC2F62.
LoadLibraryA("c:\oracle\product\11.2.0.1\client_1\Oraociei11.Dll") called from "ORAOCIEI11.DLL" at address 0x084061B3.
LoadLibraryA("c:\oracle\product\11.2.0.1\client_1\Oraociei11.Dll") returned 0x08380000.
GetProcAddress(0x08380000 [ORAOCIEI11.DLL], "ocieiInit") called from "ORAOCIEI11.DLL" at address 0x084061C9 and returned 0x08381130.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetVolumeNameForVolumeMountPointA") called from "ORAOCIEI11.DLL" at address 0x08A14D0A and returned 0x7C86B0A1.
LoadLibraryA("C:\WINNT\System32\winrnr.dll") called from "WS2_32.DLL" at address 0x71AB4EA0.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getaddrinfo") called from "ORAOCIEI11.DLL" at address 0x084D9850 and returned 0x71AB2A6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getaddrinfo") called from "ORAOCIEI11.DLL" at address 0x084D9870 and returned 0x71AB2A6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getnameinfo") called from "ORAOCIEI11.DLL" at address 0x084D9870 and returned 0x71ABC689.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "freeaddrinfo") called from "ORAOCIEI11.DLL" at address 0x084D9870 and returned 0x71AB2B0B.
GetProcAddress(0x08380000 [ORAOCIEI11.DLL], "OCIHandleAlloc") called from "OCI.DLL" at address 0x026BB8D0 and returned 0x08BC3088.
GetProcAddress(0x08380000 [ORAOCIEI11.DLL], "OCIServerAttach") called from "OCI.DLL" at address 0x026BADBC and returned 0x08BC2DB4.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "accept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1040.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "bind") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4480.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "closesocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3E2B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "connect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4A07.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getpeername") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0B68.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3D10.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB46EA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ioctlsocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3F50.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_addr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_ntoa") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB45C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "listen") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8CD3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB676F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recvfrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2FF7.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "select") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB30A8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "send") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4C27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "sendto") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2F51.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "setsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4521.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "shutdown") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0BF6.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "socket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4211.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyaddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE491.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5355.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE18E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobynumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE0CC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE703.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyport") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE5B5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5449.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0991.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByAddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEA43.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE99D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByNumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE2C3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEAED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByPort") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE913.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE867.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelAsyncRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE347.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD4AC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAUnhookBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD508.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3CCE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A5E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelBlockingCall") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD40A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIsBlocking") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD459.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStartup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6A55.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACleanup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3FED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAccept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0DC1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACloseEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB65E8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAConnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0C81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACreateEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB655D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB9A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB0C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNetworkEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB657D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDC5F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB888C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEventSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB64D9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetOverlappedResult") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0D1B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetQOSByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF470.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3EC0.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAJoinLeaf") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0F70.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4CB5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF5EE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvFrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF66A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAResetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C80.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASend") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB68FA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0A22.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendTo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0AAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C91.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8B6A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB404E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAWaitForMultipleEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9462.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6979.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9765.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB281E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB562A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB35EF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5A8D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3181.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANSPIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5086.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceEnd") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB350E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC03C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFBC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARemoveServiceClass") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFCB4.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0440.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0159.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF9E1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFA41.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFD9F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFF81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC04D2.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0281.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCDeinstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC19C9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1665.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnumProtocols") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB7B61.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCGetProviderPath") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8E27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF14D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUnInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF319.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnableNSProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEE59.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WPUCompleteOverlappedRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1CBF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAProviderConfigChange") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8C20.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteProviderOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1531.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteNameSpaceOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF019.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUpdateProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1291.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetAddrInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2899.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getnameinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC689.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetNameInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC504.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "freeaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2B0B.



Warning: At least one module has an unresolved import due to a missing export function in a delay-load dependent module.

--------------------------------------------------------------------------------
Starting profile on 12/23/2010 at 2:52:56 PM

Operating System: Microsoft Windows XP Professional (32-bit), version 5.01.2600 Service Pack 3
Program Executable: c:\upp\out\msc9.debug.debug_full.gui.mt\LISTER.EXE
Program Arguments: 
Starting Directory: C:\upp\out\MSC9.Debug.Debug_full.Gui.Mt\
Search Path: c:\oracle\product\11.2.0.1\client_1;C:\FreeBase\Oracle\Client\instantclientwin3211.2.0.1\instantclient_11_2;c:\Program Files\oracle\product\10.2.0\client_1\bin;C:\WINNT\system32;C:\WINNT;C:\WINNT\System32\Wbem;"C:\Program Files\PKWARE\pkzipc";C:\Program Files\Rational\ClearCase\etc\utils;C:\Program Files\Rational\common;C:\Program Files\Rational\ClearCase\bin;C:\Program Files\Microsoft SQL Server\80\Tools\Binn\;C:\Program Files\IBM\Client Access\;C:\Program Files\IBM\Client Access\Shared\;C:\Program Files\Microsoft SQL Server\90\Tools\binn\;C:\Program Files\Microsoft SQL Server\90\DTS\Binn\;C:\Program Files\Microsoft SQL Server\90\Tools\Binn\VSShell\Common7\IDE\;C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\PrivateAssemblies\;C:\Program Files\Microsoft Visual Studio 9.0\VC\bin;C:\Program Files\PostgreSQL\9.0\bin;C:\Program Files\Subversion\bin;C:\Program Files\TortoiseSVN\bin

Options Selected:
     Simulate ShellExecute by inserting any App Paths directories into the PATH environment variable.
     Log DllMain calls for process attach and process detach messages.
     Hook the process to gather more detailed dependency information.
     Log LoadLibrary function calls.
     Log GetProcAddress function calls.
     Log debug output messages.
     Automatically open and profile child processes.
--------------------------------------------------------------------------------

Started "LISTER.EXE" (process 0x1664) at address 0x00400000.  Successfully hooked module.
Loaded "NTDLL.DLL" at address 0x7C900000.  Successfully hooked module.
Loaded "KERNEL32.DLL" at address 0x7C800000.  Successfully hooked module.
DllMain(0x7C900000, DLL_PROCESS_ATTACH, 0x00000000) in "NTDLL.DLL" called.
DllMain(0x7C900000, DLL_PROCESS_ATTACH, 0x00000000) in "NTDLL.DLL" returned 1 (0x1).
DllMain(0x7C800000, DLL_PROCESS_ATTACH, 0x00000000) in "KERNEL32.DLL" called.
DllMain(0x7C800000, DLL_PROCESS_ATTACH, 0x00000000) in "KERNEL32.DLL" returned 1 (0x1).
Injected "DEPENDS.DLL" at address 0x08370000.
DllMain(0x08370000, DLL_PROCESS_ATTACH, 0x00000000) in "DEPENDS.DLL" called.
DllMain(0x08370000, DLL_PROCESS_ATTACH, 0x00000000) in "DEPENDS.DLL" returned 1 (0x1).
Loaded "LIBPQ.DLL" at address 0x10000000.  Successfully hooked module.
Loaded "SSLEAY32.DLL" at address 0x00340000.  Successfully hooked module.
Loaded "LIBEAY32.DLL" at address 0x00CD0000.  Successfully hooked module.
Loaded "WSOCK32.DLL" at address 0x71AD0000.  Successfully hooked module.
Loaded "WS2_32.DLL" at address 0x71AB0000.  Successfully hooked module.
Loaded "ADVAPI32.DLL" at address 0x77DD0000.  Successfully hooked module.
Loaded "RPCRT4.DLL" at address 0x77E70000.  Successfully hooked module.
Loaded "SECUR32.DLL" at address 0x77FE0000.  Successfully hooked module.
Loaded "MSVCRT.DLL" at address 0x77C10000.  Successfully hooked module.
Loaded "WS2HELP.DLL" at address 0x71AA0000.  Successfully hooked module.
Loaded "GDI32.DLL" at address 0x77F10000.  Successfully hooked module.
Loaded "USER32.DLL" at address 0x7E410000.  Successfully hooked module.
Loaded "LIBINTL-8.DLL" at address 0x61CC0000.  Successfully hooked module.
Loaded "LIBICONV-2.DLL" at address 0x66000000.  Successfully hooked module.
Loaded "WLDAP32.DLL" at address 0x76F60000.  Successfully hooked module.
Loaded "SHELL32.DLL" at address 0x7C9C0000.  Successfully hooked module.
Loaded "SHLWAPI.DLL" at address 0x77F60000.  Successfully hooked module.
Loaded "MSVCR90.DLL" at address 0x78520000.  Successfully hooked module.
Loaded "ODBC32.DLL" at address 0x74320000.  Successfully hooked module.
Loaded "COMCTL32.DLL" at address 0x5D090000.  Successfully hooked module.
Loaded "COMDLG32.DLL" at address 0x763B0000.  Successfully hooked module.
Loaded "OLE32.DLL" at address 0x774E0000.  Successfully hooked module.
Loaded "WINMM.DLL" at address 0x76B40000.  Successfully hooked module.
Loaded "MPR.DLL" at address 0x71B20000.  Successfully hooked module.
Loaded "OLEAUT32.DLL" at address 0x77120000.  Successfully hooked module.
Entrypoint reached. All implicit modules have been loaded.
DllMain(0x77FE0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "SECUR32.DLL" called.
DllMain(0x77FE0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "SECUR32.DLL" returned 1 (0x1).
DllMain(0x77E70000, DLL_PROCESS_ATTACH, 0x0012FD30) in "RPCRT4.DLL" called.
DllMain(0x77E70000, DLL_PROCESS_ATTACH, 0x0012FD30) in "RPCRT4.DLL" returned 1 (0x1).
DllMain(0x77DD0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "ADVAPI32.DLL" called.
DllMain(0x77DD0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "ADVAPI32.DLL" returned 1 (0x1).
DllMain(0x77C10000, DLL_PROCESS_ATTACH, 0x0012FD30) in "MSVCRT.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "InitializeCriticalSectionAndSpinCount") called from "MSVCRT.DLL" at address 0x77C379C2 and returned 0x7C80B8C9.
DllMain(0x77C10000, DLL_PROCESS_ATTACH, 0x0012FD30) in "MSVCRT.DLL" returned 1 (0x1).
DllMain(0x71AA0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WS2HELP.DLL" called.
DllMain(0x71AA0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WS2HELP.DLL" returned 1 (0x1).
DllMain(0x71AB0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WS2_32.DLL" called.
DllMain(0x71AB0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WS2_32.DLL" returned 1 (0x1).
DllMain(0x71AD0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WSOCK32.DLL" called.
DllMain(0x71AD0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WSOCK32.DLL" returned 1 (0x1).
DllMain(0x7E410000, DLL_PROCESS_ATTACH, 0x0012FD30) in "USER32.DLL" called.
LoadLibraryW("C:\WINNT\system32\IMM32.DLL") called from "USER32.DLL" at address 0x7E44DF6E.
Loaded "IMM32.DLL" at address 0x76390000.  Successfully hooked module.
DllMain(0x76390000, DLL_PROCESS_ATTACH, 0x00000000) in "IMM32.DLL" called.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmWINNLSEnableIME") called from "USER32.DLL" at address 0x7E43D476 and returned 0x7639E679.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmWINNLSGetEnableStatus") called from "USER32.DLL" at address 0x7E43D48B and returned 0x7639E6A0.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSendIMEMessageExW") called from "USER32.DLL" at address 0x7E43D4A0 and returned 0x7639EB34.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSendIMEMessageExA") called from "USER32.DLL" at address 0x7E43D4B5 and returned 0x7639EB4F.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmIMPGetIMEW") called from "USER32.DLL" at address 0x7E43D4CA and returned 0x7639E769.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmIMPGetIMEA") called from "USER32.DLL" at address 0x7E43D4DF and returned 0x7639E79D.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmIMPQueryIMEW") called from "USER32.DLL" at address 0x7E43D4F4 and returned 0x7639E804.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmIMPQueryIMEA") called from "USER32.DLL" at address 0x7E43D509 and returned 0x7639E90C.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmIMPSetIMEW") called from "USER32.DLL" at address 0x7E43D51E and returned 0x7639E995.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmIMPSetIMEA") called from "USER32.DLL" at address 0x7E43D533 and returned 0x7639EAB9.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmAssociateContext") called from "USER32.DLL" at address 0x7E43D548 and returned 0x76392378.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmEscapeA") called from "USER32.DLL" at address 0x7E43D55D and returned 0x76397C25.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmEscapeW") called from "USER32.DLL" at address 0x7E43D572 and returned 0x76397EA1.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetCompositionStringA") called from "USER32.DLL" at address 0x7E43D587 and returned 0x76395B62.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetCompositionStringW") called from "USER32.DLL" at address 0x7E43D59C and returned 0x7639548A.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetCompositionWindow") called from "USER32.DLL" at address 0x7E43D5B1 and returned 0x76393B48.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetContext") called from "USER32.DLL" at address 0x7E43D5C6 and returned 0x763923A1.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetDefaultIMEWnd") called from "USER32.DLL" at address 0x7E43D5DB and returned 0x763997BE.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmIsIME") called from "USER32.DLL" at address 0x7E43D5F0 and returned 0x76398C54.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmReleaseContext") called from "USER32.DLL" at address 0x7E43D605 and returned 0x763929D3.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmRegisterClient") called from "USER32.DLL" at address 0x7E43D61A and returned 0x763922B3.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetCompositionFontW") called from "USER32.DLL" at address 0x7E43D62F and returned 0x76394829.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetCompositionFontA") called from "USER32.DLL" at address 0x7E43D644 and returned 0x76394791.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetCompositionFontW") called from "USER32.DLL" at address 0x7E43D659 and returned 0x763949CE.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetCompositionFontA") called from "USER32.DLL" at address 0x7E43D66E and returned 0x763948C1.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetCompositionWindow") called from "USER32.DLL" at address 0x7E43D683 and returned 0x76394DD6.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmNotifyIME") called from "USER32.DLL" at address 0x7E43D698 and returned 0x76396FD8.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmLockIMC") called from "USER32.DLL" at address 0x7E43D6AD and returned 0x76399F2D.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmUnlockIMC") called from "USER32.DLL" at address 0x7E43D6C2 and returned 0x76399F45.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmLoadIME") called from "USER32.DLL" at address 0x7E43D6D7 and returned 0x7639776F.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetOpenStatus") called from "USER32.DLL" at address 0x7E43D6EC and returned 0x7639470B.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmFreeLayout") called from "USER32.DLL" at address 0x7E43D701 and returned 0x76396E43.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmActivateLayout") called from "USER32.DLL" at address 0x7E43D716 and returned 0x763977CD.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetCandidateWindow") called from "USER32.DLL" at address 0x7E43D72B and returned 0x76393B93.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetCandidateWindow") called from "USER32.DLL" at address 0x7E43D740 and returned 0x76394E54.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmConfigureIMEW") called from "USER32.DLL" at address 0x7E43D755 and returned 0x76397A7B.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetConversionStatus") called from "USER32.DLL" at address 0x7E43D76A and returned 0x76393A86.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetConversionStatus") called from "USER32.DLL" at address 0x7E43D77F and returned 0x763945F7.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetStatusWindowPos") called from "USER32.DLL" at address 0x7E43D794 and returned 0x76394D6E.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetImeInfoEx") called from "USER32.DLL" at address 0x7E43D7A9 and returned 0x76399C7F.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmLockImeDpi") called from "USER32.DLL" at address 0x7E43D7BE and returned 0x76399BAC.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmUnlockImeDpi") called from "USER32.DLL" at address 0x7E43D7D3 and returned 0x76399BFC.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetOpenStatus") called from "USER32.DLL" at address 0x7E43D7E8 and returned 0x76393AC3.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetActiveContext") called from "USER32.DLL" at address 0x7E43D7FD and returned 0x763929DE.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmTranslateMessage") called from "USER32.DLL" at address 0x7E43D812 and returned 0x7639DE65.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmLoadLayout") called from "USER32.DLL" at address 0x7E43D827 and returned 0x76398719.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmProcessKey") called from "USER32.DLL" at address 0x7E43D83C and returned 0x7639E0D3.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmPutImeMenuItemsIntoMappedFile") called from "USER32.DLL" at address 0x7E43D851 and returned 0x763A2E03.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetProperty") called from "USER32.DLL" at address 0x7E43D866 and returned 0x76398B8E.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetCompositionStringA") called from "USER32.DLL" at address 0x7E43D87B and returned 0x763967D5.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetCompositionStringW") called from "USER32.DLL" at address 0x7E43D890 and returned 0x763967FC.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmEnumInputContext") called from "USER32.DLL" at address 0x7E43D8A5 and returned 0x763937CA.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSystemHandler") called from "USER32.DLL" at address 0x7E43D8BA and returned 0x76399D74.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmTIMActivate") called from "USER32.DLL" at address 0x7E43D8CF and returned 0x763A3F0B.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmRestoreToolbarWnd") called from "USER32.DLL" at address 0x7E43D8E4 and returned 0x763A34DA.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmHideToolbarWnd") called from "USER32.DLL" at address 0x7E43D8F9 and returned 0x763A3511.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmDispatchDefImeMessage") called from "USER32.DLL" at address 0x7E43D90E and returned 0x763A4304.
DllMain(0x76390000, DLL_PROCESS_ATTACH, 0x00000000) in "IMM32.DLL" returned 1 (0x1).
LoadLibraryW("C:\WINNT\system32\IMM32.DLL") returned 0x76390000.
LoadLibraryW("LPK.DLL") called from "GDI32.DLL" at address 0x77F3DA06.
Loaded "LPK.DLL" at address 0x629C0000.  Successfully hooked module.
Loaded "USP10.DLL" at address 0x74D90000.  Successfully hooked module.
DllMain(0x74D90000, DLL_PROCESS_ATTACH, 0x00000000) in "USP10.DLL" called.
LoadLibraryA("gdi32.dll") called from "USP10.DLL" at address 0x74DAE20F.
LoadLibraryA("gdi32.dll") returned 0x77F10000.
GetProcAddress(0x77F10000 [GDI32.DLL], "GetCharABCWidthsI") called from "USP10.DLL" at address 0x74DAE22F and returned 0x77F3E682.
DllMain(0x74D90000, DLL_PROCESS_ATTACH, 0x00000000) in "USP10.DLL" returned 1 (0x1).
DllMain(0x629C0000, DLL_PROCESS_ATTACH, 0x00000000) in "LPK.DLL" called.
DllMain(0x629C0000, DLL_PROCESS_ATTACH, 0x00000000) in "LPK.DLL" returned 1 (0x1).
LoadLibraryW("LPK.DLL") returned 0x629C0000.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkInitialize") called from "GDI32.DLL" at address 0x77F3DA21 and returned 0x629C2EC8.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkExtTextOut") called from "GDI32.DLL" at address 0x77F3DA2C and returned 0x629C3203.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkGetCharacterPlacement") called from "GDI32.DLL" at address 0x77F3DA39 and returned 0x629C387A.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkGetTextExtentExPoint") called from "GDI32.DLL" at address 0x77F3DA46 and returned 0x629C3235.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkUseGDIWidthCache") called from "GDI32.DLL" at address 0x77F3DA53 and returned 0x629C3D92.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkTabbedTextOut") called from "GDI32.DLL" at address 0x77F3DA60 and returned 0x629C3E53.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkPSMTextOut") called from "GDI32.DLL" at address 0x77F3DA6B and returned 0x629C1282.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkDrawTextEx") called from "GDI32.DLL" at address 0x77F3DA76 and returned 0x629C1235.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkEditControl") called from "GDI32.DLL" at address 0x77F3DA81 and returned 0x629C6004.
DllMain(0x7E410000, DLL_PROCESS_ATTACH, 0x0012FD30) in "USER32.DLL" returned 1 (0x1).
DllMain(0x77F10000, DLL_PROCESS_ATTACH, 0x0012FD30) in "GDI32.DLL" called.
DllMain(0x77F10000, DLL_PROCESS_ATTACH, 0x0012FD30) in "GDI32.DLL" returned 1 (0x1).
DllMain(0x00CD0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "LIBEAY32.DLL" called.
DllMain(0x00CD0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "LIBEAY32.DLL" returned 1 (0x1).
DllMain(0x00340000, DLL_PROCESS_ATTACH, 0x0012FD30) in "SSLEAY32.DLL" called.
DllMain(0x00340000, DLL_PROCESS_ATTACH, 0x0012FD30) in "SSLEAY32.DLL" returned 1 (0x1).
DllMain(0x66000000, DLL_PROCESS_ATTACH, 0x0012FD30) in "LIBICONV-2.DLL" called.
DllMain(0x66000000, DLL_PROCESS_ATTACH, 0x0012FD30) in "LIBICONV-2.DLL" returned 1 (0x1).
DllMain(0x61CC0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "LIBINTL-8.DLL" called.
DllMain(0x61CC0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "LIBINTL-8.DLL" returned 1 (0x1).
DllMain(0x76F60000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WLDAP32.DLL" called.
DllMain(0x76F60000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WLDAP32.DLL" returned 1 (0x1).
DllMain(0x77F60000, DLL_PROCESS_ATTACH, 0x0012FD30) in "SHLWAPI.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CreateTimerQueue") called from "SHLWAPI.DLL" at address 0x77F65CF9 and returned 0x7C82BFE6.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DeleteTimerQueue") called from "SHLWAPI.DLL" at address 0x77F65D08 and returned 0x7C863F2B.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CreateTimerQueueTimer") called from "SHLWAPI.DLL" at address 0x77F65D17 and returned 0x7C82117D.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "ChangeTimerQueueTimer") called from "SHLWAPI.DLL" at address 0x77F65D27 and returned 0x7C8127C3.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DeleteTimerQueueTimer") called from "SHLWAPI.DLL" at address 0x77F65D35 and returned 0x7C821130.
DllMain(0x77F60000, DLL_PROCESS_ATTACH, 0x0012FD30) in "SHLWAPI.DLL" returned 1 (0x1).
DllMain(0x7C9C0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "SHELL32.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CreateActCtxW") called from "SHELL32.DLL" at address 0x7C9FAB8E and returned 0x7C8154FC.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "ActivateActCtx") called from "SHELL32.DLL" at address 0x7C9E7679 and returned 0x7C80A6E4.
LoadLibraryW("comctl32.dll") called from "SHELL32.DLL" at address 0x7C9FA7C4.
Loaded "COMCTL32.DLL" at address 0x773D0000.  Successfully hooked module.
DllMain(0x773D0000, DLL_PROCESS_ATTACH, 0x00000000) in "COMCTL32.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetSystemWindowsDirectoryW") called from "COMCTL32.DLL" at address 0x7745B749 and returned 0x7C80ADC9.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CreateActCtxW") called from "COMCTL32.DLL" at address 0x7745B5CC and returned 0x7C8154FC.
GetProcAddress(0x629C0000 [LPK.DLL], "LpkEditControl") called from "COMCTL32.DLL" at address 0x77422B6A and returned 0x629C6004.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "ActivateActCtx") called from "COMCTL32.DLL" at address 0x7745B65E and returned 0x7C80A6E4.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DeactivateActCtx") called from "COMCTL32.DLL" at address 0x7745B6B4 and returned 0x7C80A715.
DllMain(0x773D0000, DLL_PROCESS_ATTACH, 0x00000000) in "COMCTL32.DLL" returned 1 (0x1).
LoadLibraryW("comctl32.dll") returned 0x773D0000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DeactivateActCtx") called from "SHELL32.DLL" at address 0x7C9E76E4 and returned 0x7C80A715.
LoadLibraryW("comctl32.dll") called from "SHELL32.DLL" at address 0x7C9FA8F6.
LoadLibraryW("comctl32.dll") returned 0x5D090000.
GetProcAddress(0x5D090000 [COMCTL32.DLL], "InitCommonControlsEx") called from "SHELL32.DLL" at address 0x7C9FA906 and returned 0x5D093619.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CreateActCtxW") called from "COMCTL32.DLL" at address 0x5D0FF198 and returned 0x7C8154FC.
DllMain(0x7C9C0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "SHELL32.DLL" returned 1 (0x1).
DllMain(0x78520000, DLL_PROCESS_ATTACH, 0x0012FD30) in "MSVCR90.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsAlloc") called from "MSVCR90.DLL" at address 0x78543ACC and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsGetValue") called from "MSVCR90.DLL" at address 0x78543AD9 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsSetValue") called from "MSVCR90.DLL" at address 0x78543AE6 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsFree") called from "MSVCR90.DLL" at address 0x78543AF3 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x785435E2 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x785435E2 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x785435E2 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x785435E2 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x785435E2 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x785435E2 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x785435E2 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x78543667 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x78543667 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x7854379B and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x785437AB and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "IsProcessorFeaturePresent") called from "MSVCR90.DLL" at address 0x785A384B and returned 0x7C80AECA.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FindActCtxSectionStringW") called from "MSVCR90.DLL" at address 0x78542822 and returned 0x7C82FD54.
DllMain(0x78520000, DLL_PROCESS_ATTACH, 0x0012FD30) in "MSVCR90.DLL" returned 1 (0x1).
DllMain(0x10000000, DLL_PROCESS_ATTACH, 0x0012FD30) in "LIBPQ.DLL" called.
DllMain(0x10000000, DLL_PROCESS_ATTACH, 0x0012FD30) in "LIBPQ.DLL" returned 1 (0x1).
DllMain(0x5D090000, DLL_PROCESS_ATTACH, 0x0012FD30) in "COMCTL32.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "InitializeCriticalSectionAndSpinCount") called from "COMCTL32.DLL" at address 0x5D095338 and returned 0x7C80B8C9.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "ProcessIdToSessionId") called from "COMCTL32.DLL" at address 0x5D0954D8 and returned 0x7C813029.
GetProcAddress(0x7E410000 [USER32.DLL], "GetSystemMetrics") called from "COMCTL32.DLL" at address 0x5D0955CE and returned 0x7E418F9C.
GetProcAddress(0x7E410000 [USER32.DLL], "MonitorFromWindow") called from "COMCTL32.DLL" at address 0x5D0955E3 and returned 0x7E42A679.
GetProcAddress(0x7E410000 [USER32.DLL], "MonitorFromRect") called from "COMCTL32.DLL" at address 0x5D0955F8 and returned 0x7E42C713.
GetProcAddress(0x7E410000 [USER32.DLL], "MonitorFromPoint") called from "COMCTL32.DLL" at address 0x5D09560D and returned 0x7E42ABF5.
GetProcAddress(0x7E410000 [USER32.DLL], "EnumDisplayMonitors") called from "COMCTL32.DLL" at address 0x5D095622 and returned 0x7E42A77B.
GetProcAddress(0x7E410000 [USER32.DLL], "EnumDisplayDevicesW") called from "COMCTL32.DLL" at address 0x5D095637 and returned 0x7E41E03C.
GetProcAddress(0x7E410000 [USER32.DLL], "GetMonitorInfoW") called from "COMCTL32.DLL" at address 0x5D095654 and returned 0x7E42A6D9.
LoadLibraryW("imm32.dll") called from "COMCTL32.DLL" at address 0x5D0B6EB1.
LoadLibraryW("imm32.dll") returned 0x76390000.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmCreateContext") called from "COMCTL32.DLL" at address 0x5D0B6ED7 and returned 0x76392975.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmDestroyContext") called from "COMCTL32.DLL" at address 0x5D0B6EEC and returned 0x763936A8.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmNotifyIME") called from "COMCTL32.DLL" at address 0x5D0B6F01 and returned 0x76396FD8.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmAssociateContext") called from "COMCTL32.DLL" at address 0x5D0B6F16 and returned 0x76392378.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmReleaseContext") called from "COMCTL32.DLL" at address 0x5D0B6F27 and returned 0x763929D3.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetContext") called from "COMCTL32.DLL" at address 0x5D0B6F38 and returned 0x763923A1.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetCompositionStringA") called from "COMCTL32.DLL" at address 0x5D0B6F49 and returned 0x76395B62.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetCompositionStringA") called from "COMCTL32.DLL" at address 0x5D0B6F5A and returned 0x763967D5.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetCompositionStringW") called from "COMCTL32.DLL" at address 0x5D0B6F6B and returned 0x7639548A.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetCompositionStringW") called from "COMCTL32.DLL" at address 0x5D0B6F7C and returned 0x763967FC.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetCandidateWindow") called from "COMCTL32.DLL" at address 0x5D0B6F8D and returned 0x76394E54.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "ActivateActCtx") called from "COMCTL32.DLL" at address 0x5D0FF24A and returned 0x7C80A6E4.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DeactivateActCtx") called from "COMCTL32.DLL" at address 0x5D0FF2AF and returned 0x7C80A715.
DllMain(0x5D090000, DLL_PROCESS_ATTACH, 0x0012FD30) in "COMCTL32.DLL" returned 1 (0x1).
DllMain(0x763B0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "COMDLG32.DLL" called.
DllMain(0x763B0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "COMDLG32.DLL" returned 1 (0x1).
DllMain(0x74320000, DLL_PROCESS_ATTACH, 0x0012FD30) in "ODBC32.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "TryEnterCriticalSection") called from "ODBC32.DLL" at address 0x7432FBBB and returned 0x7C901118.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "TryEnterCriticalSection") called from "ODBC32.DLL" at address 0x7432EFDC and returned 0x7C901118.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "SetCriticalSectionSpinCount") called from "ODBC32.DLL" at address 0x7432EFE9 and returned 0x7C92A067.
LoadLibraryExA("C:\WINNT\system32\odbcint.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "ODBC32.DLL" at address 0x7432F42A.
Loaded "ODBCINT.DLL" at address 0x01330000.  Successfully hooked module.
LoadLibraryExA("C:\WINNT\system32\odbcint.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x01330000.
LoadLibraryExW("C:\WINNT\system32\odbctrac.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "ODBC32.DLL" at address 0x74352FCC.
Loaded "ODBCTRAC.DLL" at address 0x4EEC0000.  Successfully hooked module.
DllMain(0x4EEC0000, DLL_PROCESS_ATTACH, 0x00000000) in "ODBCTRAC.DLL" called.
LoadLibraryA("odbc32.dll") called from "ODBCTRAC.DLL" at address 0x4EEDD7AB.
LoadLibraryA("odbc32.dll") returned 0x74320000.
GetProcAddress(0x74320000 [ODBC32.DLL], "GetODBCSharedData") called from "ODBCTRAC.DLL" at address 0x4EEDD7C4 and returned 0x7434D0DF.
DllMain(0x4EEC0000, DLL_PROCESS_ATTACH, 0x00000000) in "ODBCTRAC.DLL" returned 1 (0x1).
LoadLibraryExW("C:\WINNT\system32\odbctrac.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x4EEC0000.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceOpenLogFile") called from "ODBC32.DLL" at address 0x7435304E and returned 0x4EEDBE3A.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceCloseLogFile") called from "ODBC32.DLL" at address 0x7435305E and returned 0x4EEDBFC4.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceReturn") called from "ODBC32.DLL" at address 0x7435306E and returned 0x4EEDD729.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceVersion") called from "ODBC32.DLL" at address 0x7435307E and returned 0x4EEDC2B9.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "FireVSDebugEvent") called from "ODBC32.DLL" at address 0x7435308E and returned 0x4EEDDF8E.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceVSControl") called from "ODBC32.DLL" at address 0x7435309D and returned 0x4EEDC2C4.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocConnect") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6375.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocConnectW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocEnv") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED63EC.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocEnvW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocStmt") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6458.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocStmtW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBindCol") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED64DB.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBindColW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLCancel") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED65BA.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLCancelW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLColAttributes") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6626.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLColAttributesW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EED9CA6.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLConnect") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6711.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLConnectW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EED9D98.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDescribeCol") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED67EF.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDescribeColW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EED9E7D.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDisconnect") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED68FB.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDisconnectW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLError") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6967.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLErrorW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EED9F90.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLExecDirect") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6A6B.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLExecDirectW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA097.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLExecute") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6B05.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLExecuteW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFetch") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6B71.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFetchW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFreeConnect") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6BDD.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFreeConnectW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFreeEnv") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6C49.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFreeEnvW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFreeStmt") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6CB0.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFreeStmtW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetCursorName") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6D34.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetCursorNameW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA138.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLNumResultCols") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6DE9.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLNumResultColsW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLPrepare") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6E6C.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLPrepareW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA1F4.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLRowCount") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6F06.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLRowCountW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetCursorName") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED6F89.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetCursorNameW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA295.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetParam") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7024.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetParamW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLTransact") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7124.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLTransactW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLColumns") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED71BF.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLColumnsW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA337.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDriverConnect") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED72BC.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDriverConnectW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA43B.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetConnectOption") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED73BD.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetConnectOptionW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA543.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetData") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7458.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetDataW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetFunctions") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7537.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetFunctionsW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetInfo") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED75D2.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetInfoW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA5E5.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetStmtOption") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED769F.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetStmtOptionW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetTypeInfo") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED773A.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetTypeInfoW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA6B9.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLParamData") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED77BE.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLParamDataW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLPutData") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7841.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLPutDataW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetConnectOption") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED78DB.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetConnectOptionW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA744.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetStmtOption") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7975.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetStmtOptionW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSpecialColumns") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7A10.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSpecialColumnsW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA7E5.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLStatistics") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7B33.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLStatisticsW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDA90F.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLTables") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7C39.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLTablesW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDAA1C.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBrowseConnect") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7D36.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBrowseConnectW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDAB20.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLColumnPrivileges") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7E0C.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLColumnPrivilegesW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDABFD.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDataSources") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED7F09.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDataSourcesW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDAD01.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDescribeParam") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED800C.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDescribeParamW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLExtendedFetch") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED80DA.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLExtendedFetchW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLForeignKeys") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED81A6.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLForeignKeysW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDAE07.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLMoreResults") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED82E6.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLMoreResultsW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLNativeSql") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8352.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLNativeSqlW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDAF52.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLNumParams") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8426.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLNumParamsW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLParamOptions") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED84A9.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLParamOptionsW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLPrimaryKeys") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8541.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLPrimaryKeysW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB02D.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLProcedureColumns") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED861F.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLProcedureColumnsW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB112.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLProcedures") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED871C.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLProceduresW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB216.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetPos") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED87FA.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetPosW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetScrollOptions") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED88B0.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetScrollOptionsW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLTablePrivileges") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED895E.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLTablePrivilegesW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB2FB.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDrivers") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8A3C.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLDriversW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB3E0.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBindParameter") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8B3F.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBindParameterW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocHandle") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8C65.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocHandleW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBindParam") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8D00.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBindParamW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLCloseCursor") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8DFC.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLCloseCursorW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLColAttribute") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8E68.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLColAttributeW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB4E6.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLCopyDesc") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8F4E.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLCopyDescW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLEndTran") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED8FC9.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLEndTranW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFetchScroll") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED905D.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFetchScrollW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFreeHandle") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED90F8.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLFreeHandleW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetConnectAttr") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED917C.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetConnectAttrW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB5D3.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetDescField") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED923F.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetDescFieldW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB69D.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetDescRec") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED931B.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetDescRecW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB780.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetDiagField") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED9440.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetDiagFieldW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB8AC.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetDiagRec") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED9525.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetDiagRecW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDB998.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetEnvAttr") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED9622.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetEnvAttrW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetStmtAttr") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED96E5.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLGetStmtAttrW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDBA9C.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetConnectAttr") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED97A8.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetConnectAttrW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDBB66.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetDescField") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED9854.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetDescFieldW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDBC19.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetDescRec") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED9919.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetDescRecW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetEnvAttr") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED9A2F.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetEnvAttrW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetStmtAttr") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED9ADB.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLSetStmtAttrW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDBCE5.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocHandleStd") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED9B87.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLAllocHandleStdW") called from "ODBC32.DLL" at address 0x743531D4 and returned 0x4EEDBD98.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBulkOperations") called from "ODBC32.DLL" at address 0x7435319F and returned 0x4EED9C22.
GetProcAddress(0x4EEC0000 [ODBCTRAC.DLL], "TraceSQLBulkOperationsW") called from "ODBC32.DLL" at address 0x743531D4 and returned NULL. Error: The specified procedure could not be found (127).
DllMain(0x74320000, DLL_PROCESS_ATTACH, 0x0012FD30) in "ODBC32.DLL" returned 1 (0x1).
DllMain(0x774E0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "OLE32.DLL" called.
DllMain(0x774E0000, DLL_PROCESS_ATTACH, 0x0012FD30) in "OLE32.DLL" returned 1 (0x1).
DllMain(0x76B40000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WINMM.DLL" called.
DllMain(0x76B40000, DLL_PROCESS_ATTACH, 0x0012FD30) in "WINMM.DLL" returned 1 (0x1).
DllMain(0x71B20000, DLL_PROCESS_ATTACH, 0x0012FD30) in "MPR.DLL" called.
DllMain(0x71B20000, DLL_PROCESS_ATTACH, 0x0012FD30) in "MPR.DLL" returned 1 (0x1).
DllMain(0x77120000, DLL_PROCESS_ATTACH, 0x0012FD30) in "OLEAUT32.DLL" called.
DllMain(0x77120000, DLL_PROCESS_ATTACH, 0x0012FD30) in "OLEAUT32.DLL" returned 1 (0x1).
Loaded "JASHOOK.DLL" at address 0x014D0000.  Successfully hooked module.
DllMain(0x014D0000, DLL_PROCESS_ATTACH, 0x00000000) in "JASHOOK.DLL" called.
LoadLibraryW("rpcrt4.dll") called from "RPCRT4.DLL" at address 0x77E8C735.
LoadLibraryW("rpcrt4.dll") returned 0x77E70000.
DllMain(0x014D0000, DLL_PROCESS_ATTACH, 0x00000000) in "JASHOOK.DLL" returned 1 (0x1).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsAlloc") called from "LISTER.EXE" at address 0x009209D9 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsGetValue") called from "LISTER.EXE" at address 0x009209ED and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsSetValue") called from "LISTER.EXE" at address 0x00920A01 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsFree") called from "LISTER.EXE" at address 0x00920A15 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "LISTER.EXE" at address 0x009207BC and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "LISTER.EXE" at address 0x009207BC and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "LISTER.EXE" at address 0x009207BC and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "LISTER.EXE" at address 0x009207BC and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "LISTER.EXE" at address 0x009207BC and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "LISTER.EXE" at address 0x009207BC and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "LISTER.EXE" at address 0x009207BC and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "LISTER.EXE" at address 0x009208AC and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "LISTER.EXE" at address 0x009208AC and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "LISTER.EXE" at address 0x00920C7B and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "LISTER.EXE" at address 0x00920C93 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "IsProcessorFeaturePresent") called from "LISTER.EXE" at address 0x0092426B and returned 0x7C80AECA.
LoadLibraryExW("C:\WINNT\system32\MSCTF.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "USER32.DLL" at address 0x7E428055.
Loaded "MSCTF.DLL" at address 0x74720000.  Successfully hooked module.
DllMain(0x74720000, DLL_PROCESS_ATTACH, 0x00000000) in "MSCTF.DLL" called.
GetProcAddress(0x7C900000 [NTDLL.DLL], "NtQueryInformationProcess") called from "MSCTF.DLL" at address 0x74723BC2 and returned 0x7C90D7FE.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmCoUninitialize") called from "MSCTF.DLL" at address 0x747227FF and returned 0x763A3E9F.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmLastEnabledWndDestroy") called from "MSCTF.DLL" at address 0x7472283C and returned 0x763A418A.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmSetCiceroStartInThread") called from "MSCTF.DLL" at address 0x7472287F and returned 0x763A37A6.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmIsCiceroStartedInThread") called from "MSCTF.DLL" at address 0x747228C4 and returned 0x763A378E.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmIsCiceroEnabled") called from "MSCTF.DLL" at address 0x74722909 and returned 0x763A3773.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmIsTextFrameServiceDisabled") called from "MSCTF.DLL" at address 0x74722945 and returned 0x763A38A1.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmEnterCoInitCountSkipMode") called from "MSCTF.DLL" at address 0x7472298C and returned 0x763A31F0.
GetProcAddress(0x76390000 [IMM32.DLL], "CtfImmLeaveCoInitCountSkipMode") called from "MSCTF.DLL" at address 0x747229D2 and returned 0x763A3206.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetDefaultIMEWnd") called from "MSCTF.DLL" at address 0x74722A16 and returned 0x763997BE.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmReleaseContext") called from "MSCTF.DLL" at address 0x74722A4F and returned 0x763929D3.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmNotifyIME") called from "MSCTF.DLL" at address 0x74722A89 and returned 0x76396FD8.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetConversionStatus") called from "MSCTF.DLL" at address 0x74722ABC and returned 0x763945F7.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetConversionStatus") called from "MSCTF.DLL" at address 0x74722AFA and returned 0x76393A86.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetProperty") called from "MSCTF.DLL" at address 0x74722B36 and returned 0x76398B8E.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetOpenStatus") called from "MSCTF.DLL" at address 0x74722B6A and returned 0x76393AC3.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetContext") called from "MSCTF.DLL" at address 0x74722BA0 and returned 0x763923A1.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetOpenStatus") called from "MSCTF.DLL" at address 0x74722BD5 and returned 0x7639470B.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmInstallIMEA") called from "MSCTF.DLL" at address 0x74722C0C and returned 0x763996DF.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetDescriptionA") called from "MSCTF.DLL" at address 0x74722C42 and returned 0x763989A2.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetDescriptionW") called from "MSCTF.DLL" at address 0x74722C7A and returned 0x763988F5.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetIMEFileNameA") called from "MSCTF.DLL" at address 0x74722CB2 and returned 0x76398AF0.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmGetIMEFileNameW") called from "MSCTF.DLL" at address 0x74722CEA and returned 0x76398A39.
GetProcAddress(0x76390000 [IMM32.DLL], "ImmSetHotKey") called from "MSCTF.DLL" at address 0x747231C8 and returned 0x7E453C33.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetUserDefaultUILanguage") called from "MSCTF.DLL" at address 0x74722D8B and returned 0x7C813110.
DllMain(0x74720000, DLL_PROCESS_ATTACH, 0x00000000) in "MSCTF.DLL" returned 1 (0x1).
LoadLibraryExW("C:\WINNT\system32\MSCTF.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x74720000.
LoadLibraryW("version.dll") called from "IMM32.DLL" at address 0x763985B6.
Loaded "VERSION.DLL" at address 0x77C00000.  Successfully hooked module.
DllMain(0x77C00000, DLL_PROCESS_ATTACH, 0x00000000) in "VERSION.DLL" called.
DllMain(0x77C00000, DLL_PROCESS_ATTACH, 0x00000000) in "VERSION.DLL" returned 1 (0x1).
LoadLibraryW("version.dll") returned 0x77C00000.
GetProcAddress(0x77C00000 [VERSION.DLL], "GetFileVersionInfoW") called from "IMM32.DLL" at address 0x763985D8 and returned 0x77C0166F.
GetProcAddress(0x77C00000 [VERSION.DLL], "GetFileVersionInfoSizeW") called from "IMM32.DLL" at address 0x763985ED and returned 0x77C0138C.
GetProcAddress(0x77C00000 [VERSION.DLL], "VerQueryValueW") called from "IMM32.DLL" at address 0x76398602 and returned 0x77C01805.
LoadLibraryExW("C:\WINNT\system32\msctfime.ime", 0x00000000, LOAD_LIBRARY_AS_DATAFILE) called from "VERSION.DLL" at address 0x77C013BD.
Mapped "MSCTFIME.IME" as a data file into memory at address 0x01930001.
LoadLibraryExW("C:\WINNT\system32\msctfime.ime", 0x00000000, LOAD_LIBRARY_AS_DATAFILE) returned 0x01930001.
LoadLibraryExW("C:\WINNT\system32\msctfime.ime", 0x00000000, LOAD_LIBRARY_AS_DATAFILE) called from "VERSION.DLL" at address 0x77C0169F.
Mapped "MSCTFIME.IME" as a data file into memory at address 0x01930001.
LoadLibraryExW("C:\WINNT\system32\msctfime.ime", 0x00000000, LOAD_LIBRARY_AS_DATAFILE) returned 0x01930001.
DllMain(0x77C00000, DLL_PROCESS_DETACH, 0x00000000) in "VERSION.DLL" called.
DllMain(0x77C00000, DLL_PROCESS_DETACH, 0x00000000) in "VERSION.DLL" returned 1 (0x1).
Unloaded "VERSION.DLL" at address 0x77C00000.
LoadLibraryW("C:\WINNT\system32\msctfime.ime") called from "IMM32.DLL" at address 0x763A3B26.
Loaded "MSCTFIME.IME" at address 0x755C0000.  Successfully hooked module.
DllMain(0x755C0000, DLL_PROCESS_ATTACH, 0x00000000) in "MSCTFIME.IME" called.
LoadLibraryA("C:\WINNT\system32\ole32.dll") called from "MSCTFIME.IME" at address 0x755DD50A.
LoadLibraryA("C:\WINNT\system32\ole32.dll") returned 0x774E0000.
GetProcAddress(0x774E0000 [OLE32.DLL], "CoCreateInstance") called from "MSCTFIME.IME" at address 0x755DA344 and returned 0x7750057E.
DllMain(0x755C0000, DLL_PROCESS_ATTACH, 0x00000000) in "MSCTFIME.IME" returned 1 (0x1).
LoadLibraryW("C:\WINNT\system32\msctfime.ime") returned 0x755C0000.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeCreateThreadMgr") called from "IMM32.DLL" at address 0x763A3B42 and returned 0x755D9590.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeDestroyThreadMgr") called from "IMM32.DLL" at address 0x763A3B5C and returned 0x755D961B.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeCreateInputContext") called from "IMM32.DLL" at address 0x763A3B76 and returned 0x755D9699.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeDestroyInputContext") called from "IMM32.DLL" at address 0x763A3B8C and returned 0x755D913A.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeSetActiveContextAlways") called from "IMM32.DLL" at address 0x763A3BA2 and returned 0x755D96C7.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeProcessCicHotkey") called from "IMM32.DLL" at address 0x763A3BB8 and returned 0x755D9979.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeDispatchDefImeMessage") called from "IMM32.DLL" at address 0x763A3BCE and returned 0x755D3818.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeIsIME") called from "IMM32.DLL" at address 0x763A3BE4 and returned 0x755D976B.
GetProcAddress(0x7C900000 [NTDLL.DLL], "RtlDllShutdownInProgress") called from "MSCTFIME.IME" at address 0x755D8AE1 and returned 0x7C9136D0.
LoadLibraryW("C:\WINNT\system32\msctfime.ime") called from "IMM32.DLL" at address 0x763971F4.
LoadLibraryW("C:\WINNT\system32\msctfime.ime") returned 0x755C0000.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeInquire") called from "IMM32.DLL" at address 0x7639720D and returned 0x755C76F9.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeConversionList") called from "IMM32.DLL" at address 0x76397222 and returned 0x755D90C5.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeRegisterWord") called from "IMM32.DLL" at address 0x76397237 and returned 0x755C76F9.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeUnregisterWord") called from "IMM32.DLL" at address 0x7639724C and returned 0x755C76F9.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeGetRegisterWordStyle") called from "IMM32.DLL" at address 0x76397261 and returned 0x755D9120.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeEnumRegisterWord") called from "IMM32.DLL" at address 0x76397276 and returned 0x755D90C5.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeConfigure") called from "IMM32.DLL" at address 0x7639728B and returned 0x755D940E.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeDestroy") called from "IMM32.DLL" at address 0x763972A0 and returned 0x755D90CF.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeEscape") called from "IMM32.DLL" at address 0x763972B5 and returned 0x755C76F9.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeProcessKey") called from "IMM32.DLL" at address 0x763972CA and returned 0x755D9899.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeSelect") called from "IMM32.DLL" at address 0x763972DF and returned 0x755D9120.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeSetActiveContext") called from "IMM32.DLL" at address 0x763972F4 and returned 0x755D9120.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeToAsciiEx") called from "IMM32.DLL" at address 0x7639730C and returned 0x755D94AF.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "NotifyIME") called from "IMM32.DLL" at address 0x76397324 and returned 0x755D9507.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeSetCompositionString") called from "IMM32.DLL" at address 0x7639733C and returned 0x755D946D.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "ImeGetImeMenuItems") called from "IMM32.DLL" at address 0x76397354 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeInquireExW") called from "IMM32.DLL" at address 0x76397394 and returned 0x755D9548.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeSelectEx") called from "IMM32.DLL" at address 0x763973A8 and returned 0x755D92C5.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeEscapeEx") called from "IMM32.DLL" at address 0x763973BC and returned 0x755D96FE.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeGetGuidAtom") called from "IMM32.DLL" at address 0x763973D0 and returned 0x755D9737.
GetProcAddress(0x755C0000 [MSCTFIME.IME], "CtfImeIsGuidMapEnable") called from "IMM32.DLL" at address 0x763973E4 and returned 0x755D931A.
LoadLibraryA("uxtheme.dll") called from "LISTER.EXE" at address 0x007FF610.
Loaded "UXTHEME.DLL" at address 0x5AD70000.  Successfully hooked module.
DllMain(0x5AD70000, DLL_PROCESS_ATTACH, 0x00000000) in "UXTHEME.DLL" called.
DllMain(0x5AD70000, DLL_PROCESS_ATTACH, 0x00000000) in "UXTHEME.DLL" returned 1 (0x1).
LoadLibraryA("uxtheme.dll") returned 0x5AD70000.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "OpenThemeData") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD773B8.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "CloseThemeData") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD74773.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "DrawThemeBackground") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD72BEF.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "GetThemePartSize") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD741A9.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "GetThemeColor") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD7459D.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "GetThemeInt") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD7459D.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "GetThemeBool") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD7BEA5.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "GetThemeBackgroundContentRect") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD73E8A.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "IsThemePartDefined") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD7A35A.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "GetThemeDocumentationProperty") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD9D3AF.
GetProcAddress(0x5AD70000 [UXTHEME.DLL], "GetCurrentThemeName") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x5AD7B76A.
LoadLibraryA("gdi32") called from "LISTER.EXE" at address 0x0073D97B.
LoadLibraryA("gdi32") returned 0x77F10000.
GetProcAddress(0x77F10000 [GDI32.DLL], "GetGlyphIndicesW") called from "LISTER.EXE" at address 0x0073D993 and returned 0x77F52604.
LoadLibraryA("Kernel32.dll") called from "LISTER.EXE" at address 0x007FF610.
LoadLibraryA("Kernel32.dll") returned 0x7C800000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetSystemDirectoryW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C831DEB.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetTempPathW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C830791.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetWindowsDirectoryW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C80AE1B.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetModuleFileNameW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C80B475.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetLocaleInfoW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C811602.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetCurrentDirectoryW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C80B917.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetFullPathNameW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C80B8F2.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FindFirstFileW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C80EF81.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FindNextFileW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C80EFDA.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetVolumeInformationA") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C821BA5.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CreateFileW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C810800.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CopyFileW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C82F87B.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "MoveFileW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C821261.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DeleteFileW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C831F63.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "RemoveDirectoryW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C836F8B.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CreateDirectoryW") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7C832402.
LoadLibraryA("ole32.dll") called from "SHELL32.DLL" at address 0x7CA0653F.
LoadLibraryA("ole32.dll") returned 0x774E0000.
GetProcAddress(0x774E0000 [OLE32.DLL], "CreateBindCtx") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x774FE54C.
GetProcAddress(0x773D0000 [COMCTL32.DLL], 0x000000EC) called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E1798.
GetProcAddress(0x774E0000 [OLE32.DLL], "CoGetMalloc") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x774FDD08.
LoadLibraryExW("C:\WINNT\system32\SHELL32.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\WINNT\system32\SHELL32.dll", 0x00000000, 0x00000000) returned 0x7C9C0000.
GetProcAddress(0x773D0000 [COMCTL32.DLL], 0x00000148) called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E1559.
LoadLibraryA("SETUPAPI.dll") called from "SHELL32.DLL" at address 0x7CA0653F.
Loaded "SETUPAPI.DLL" at address 0x77920000.  Successfully hooked module.
DllMain(0x77920000, DLL_PROCESS_ATTACH, 0x00000000) in "SETUPAPI.DLL" called.
DllMain(0x77920000, DLL_PROCESS_ATTACH, 0x00000000) in "SETUPAPI.DLL" returned 1 (0x1).
LoadLibraryA("SETUPAPI.dll") returned 0x77920000.
GetProcAddress(0x77920000 [SETUPAPI.DLL], "CM_Get_Device_Interface_List_Size_ExW") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x77929025.
GetProcAddress(0x77920000 [SETUPAPI.DLL], "CM_Get_Device_Interface_List_ExW") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x7792A15C.
GetProcAddress(0x773D0000 [COMCTL32.DLL], 0x0000014E) called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E0F5A.
GetProcAddress(0x773D0000 [COMCTL32.DLL], 0x0000014C) called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E0DF4.
LoadLibraryA("SHELL32.dll") called from "SHLWAPI.DLL" at address 0x77F717B4.
LoadLibraryA("SHELL32.dll") returned 0x7C9C0000.
GetProcAddress(0x7C9C0000 [SHELL32.DLL], 0x00000066) called from "SHLWAPI.DLL" at address 0x77F716EA and returned 0x7C9EB042.
LoadLibraryA("ole32.dll") called from "SHLWAPI.DLL" at address 0x77F717B4.
LoadLibraryA("ole32.dll") returned 0x774E0000.
GetProcAddress(0x774E0000 [OLE32.DLL], "CoTaskMemAlloc") called from "SHLWAPI.DLL" at address 0x77F716EA and returned 0x774FD060.
GetProcAddress(0x773D0000 [COMCTL32.DLL], 0x00000140) called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E0A75.
GetProcAddress(0x773D0000 [COMCTL32.DLL], 0x00000144) called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E0C22.
GetProcAddress(0x773D0000 [COMCTL32.DLL], 0x00000143) called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E0B17.
GetProcAddress(0x773D0000 [COMCTL32.DLL], 0x00000184) called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E1535.
GetProcAddress(0x774E0000 [OLE32.DLL], "CoTaskMemFree") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x774FD044.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "GetUserDefaultUILanguage") called from "SHELL32.DLL" at address 0x7CA09110 and returned 0x7C813110.
GetProcAddress(0x773D0000 [COMCTL32.DLL], "ImageList_Create") called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E935B.
GetProcAddress(0x773D0000 [COMCTL32.DLL], "ImageList_SetBkColor") called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E5264.
GetProcAddress(0x773D0000 [COMCTL32.DLL], "ImageList_GetImageCount") called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E5150.
GetProcAddress(0x773D0000 [COMCTL32.DLL], "ImageList_ReplaceIcon") called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E521D.
GetProcAddress(0x773D0000 [COMCTL32.DLL], "ImageList_SetOverlayImage") called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E52EB.
GetProcAddress(0x774E0000 [OLE32.DLL], "CoInitializeEx") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x774FEF7B.
LoadLibraryA("appHelp.dll") called from "SHLWAPI.DLL" at address 0x77F717B4.
Loaded "APPHELP.DLL" at address 0x77B40000.  Successfully hooked module.
DllMain(0x77B40000, DLL_PROCESS_ATTACH, 0x00000000) in "APPHELP.DLL" called.
DllMain(0x77B40000, DLL_PROCESS_ATTACH, 0x00000000) in "APPHELP.DLL" returned 1 (0x1).
LoadLibraryA("appHelp.dll") returned 0x77B40000.
GetProcAddress(0x77B40000 [APPHELP.DLL], "ApphelpCheckShellObject") called from "SHLWAPI.DLL" at address 0x77F716EA and returned 0x77B46906.
GetProcAddress(0x774E0000 [OLE32.DLL], "CoCreateInstance") called from "SHLWAPI.DLL" at address 0x77F716EA and returned 0x7750057E.
LoadLibraryA("CLBCATQ.DLL") called from "OLE32.DLL" at address 0x7751316D.
Loaded "CLBCATQ.DLL" at address 0x76FD0000.  Successfully hooked module.
Loaded "COMRES.DLL" at address 0x77050000.  Successfully hooked module.
Loaded "VERSION.DLL" at address 0x77C00000.  Successfully hooked module.
DllMain(0x77050000, DLL_PROCESS_ATTACH, 0x00000000) in "COMRES.DLL" called.
DllMain(0x77050000, DLL_PROCESS_ATTACH, 0x00000000) in "COMRES.DLL" returned 1 (0x1).
DllMain(0x77C00000, DLL_PROCESS_ATTACH, 0x00000000) in "VERSION.DLL" called.
DllMain(0x77C00000, DLL_PROCESS_ATTACH, 0x00000000) in "VERSION.DLL" returned 1 (0x1).
DllMain(0x76FD0000, DLL_PROCESS_ATTACH, 0x00000000) in "CLBCATQ.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "InitializeCriticalSectionAndSpinCount") called from "CLBCATQ.DLL" at address 0x76FD4BC2 and returned 0x7C80B8C9.
DllMain(0x76FD0000, DLL_PROCESS_ATTACH, 0x00000000) in "CLBCATQ.DLL" returned 1 (0x1).
LoadLibraryA("CLBCATQ.DLL") returned 0x76FD0000.
GetProcAddress(0x76FD0000 [CLBCATQ.DLL], "GetCatalogObject") called from "OLE32.DLL" at address 0x77513182 and returned 0x76FD3F78.
LoadLibraryA("CLBCATQ.DLL") called from "OLE32.DLL" at address 0x77513949.
LoadLibraryA("CLBCATQ.DLL") returned 0x76FD0000.
GetProcAddress(0x76FD0000 [CLBCATQ.DLL], "GetCatalogObject2") called from "OLE32.DLL" at address 0x7751395C and returned 0x76FD4017.
GetProcAddress(0x774E0000 [OLE32.DLL], "CLSIDFromOle1Class") called from "OLE32.DLL" at address 0x77513B05 and returned 0x775188B9.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "InitializeCriticalSectionAndSpinCount") called from "CLBCATQ.DLL" at address 0x76FD6665 and returned 0x7C80B8C9.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "OLE32.DLL" at address 0x77512485.
Loaded "TORTOISEOVERLAYS.DLL" at address 0x05960000.  Successfully hooked module.
DllMain(0x05960000, DLL_PROCESS_ATTACH, 0x00000000) in "TORTOISEOVERLAYS.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsAlloc") called from "TORTOISEOVERLAYS.DLL" at address 0x05965B1E and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsGetValue") called from "TORTOISEOVERLAYS.DLL" at address 0x05965B2B and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsSetValue") called from "TORTOISEOVERLAYS.DLL" at address 0x05965B38 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsFree") called from "TORTOISEOVERLAYS.DLL" at address 0x05965B45 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x059656CD and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x059656CD and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x059656CD and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x059656CD and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x059656CD and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x059656CD and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x059656CD and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x05965748 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x05965748 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x0596581C and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x0596582C and returned 0x7C913425.
DllMain(0x05960000, DLL_PROCESS_ATTACH, 0x00000000) in "TORTOISEOVERLAYS.DLL" returned 1 (0x1).
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05960000.
GetProcAddress(0x05960000 [TORTOISEOVERLAYS.DLL], "DllGetClassObject") called from "OLE32.DLL" at address 0x775124A0 and returned 0x0596BF00.
GetProcAddress(0x05960000 [TORTOISEOVERLAYS.DLL], "DllCanUnloadNow") called from "OLE32.DLL" at address 0x775124B6 and returned 0x0596BED0.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) returned 0x05960000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "QueryActCtxW") called from "TORTOISEOVERLAYS.DLL" at address 0x05963BD5 and returned 0x7C81637B.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "ActivateActCtx") called from "TORTOISEOVERLAYS.DLL" at address 0x05963BD5 and returned 0x7C80A6E4.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FindActCtxSectionStringW") called from "TORTOISEOVERLAYS.DLL" at address 0x05963BD5 and returned 0x7C82FD54.
LoadLibraryW("Comctl32.dll") called from "TORTOISEOVERLAYS.DLL" at address 0x059639AE.
LoadLibraryW("Comctl32.dll") returned 0x773D0000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DeactivateActCtx") called from "TORTOISEOVERLAYS.DLL" at address 0x05963BD5 and returned 0x7C80A715.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISEOVERLAYS.DLL" at address 0x05963D11.
Loaded "TORTOISESTUB.DLL" at address 0x05940000.  Successfully hooked module.
DllMain(0x05940000, DLL_PROCESS_ATTACH, 0x00000000) in "TORTOISESTUB.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsAlloc") called from "TORTOISESTUB.DLL" at address 0x05941814 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsGetValue") called from "TORTOISESTUB.DLL" at address 0x05941821 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsSetValue") called from "TORTOISESTUB.DLL" at address 0x0594182E and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FlsFree") called from "TORTOISESTUB.DLL" at address 0x0594183B and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x059413C3 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x059413C3 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x059413C3 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x059413C3 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x059413C3 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x059413C3 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x059413C3 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x0594143E and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x0594143E and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x05941512 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x05941522 and returned 0x7C913425.
DllMain(0x05940000, DLL_PROCESS_ATTACH, 0x00000000) in "TORTOISESTUB.DLL" returned 1 (0x1).
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05940000.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllGetClassObject") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCCA and returned 0x05947651.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllCanUnloadNow") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCF8 and returned 0x05947385.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseSVN.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISESTUB.DLL" at address 0x059475DA.
Loaded "TORTOISESVN.DLL" at address 0x01B60000.  Successfully hooked module.
Loaded "SHFOLDER.DLL" at address 0x76780000.  Successfully hooked module.
Loaded "WININET.DLL" at address 0x3D930000.  Successfully hooked module.
Loaded "NORMALIZ.DLL" at address 0x01C40000.  Successfully hooked module.
Loaded "IERTUTIL.DLL" at address 0x3DFD0000.  Successfully hooked module.
Loaded "LIBAPR_TSVN.DLL" at address 0x6EEC0000.  Successfully hooked module.
Loaded "MSWSOCK.DLL" at address 0x71A50000.  Successfully hooked module.
Loaded "LIBAPRUTIL_TSVN.DLL" at address 0x6EE60000.  Successfully hooked module.
Loaded "INTL3_TSVN.DLL" at address 0x01C70000.  Successfully hooked module.
Loaded "MSVCP90.DLL" at address 0x78480000.  Successfully hooked module.
Loaded "CRYPT32.DLL" at address 0x77A80000.  Successfully hooked module.
Loaded "MSASN1.DLL" at address 0x77B20000.  Successfully hooked module.
DllMain(0x76780000, DLL_PROCESS_ATTACH, 0x00000000) in "SHFOLDER.DLL" called.
DllMain(0x76780000, DLL_PROCESS_ATTACH, 0x00000000) in "SHFOLDER.DLL" returned 1 (0x1).
DllMain(0x01C40000, DLL_PROCESS_ATTACH, 0x00000000) in "NORMALIZ.DLL" called.
DllMain(0x01C40000, DLL_PROCESS_ATTACH, 0x00000000) in "NORMALIZ.DLL" returned 1 (0x1).
DllMain(0x3DFD0000, DLL_PROCESS_ATTACH, 0x00000000) in "IERTUTIL.DLL" called.
DllMain(0x3DFD0000, DLL_PROCESS_ATTACH, 0x00000000) in "IERTUTIL.DLL" returned 1 (0x1).
DllMain(0x3D930000, DLL_PROCESS_ATTACH, 0x00000000) in "WININET.DLL" called.
LoadLibraryW("comctl32.dll") called from "WININET.DLL" at address 0x3D95CD68.
LoadLibraryW("comctl32.dll") returned 0x773D0000.
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "EventRegister") called from "WININET.DLL" at address 0x3D95CA17 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "EventUnregister") called from "WININET.DLL" at address 0x3D95CA29 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "EventEnabled") called from "WININET.DLL" at address 0x3D95CA3B and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "EventWrite") called from "WININET.DLL" at address 0x3D95CA4D and returned NULL. Error: The specified procedure could not be found (127).
DllMain(0x3D930000, DLL_PROCESS_ATTACH, 0x00000000) in "WININET.DLL" returned 1 (0x1).
DllMain(0x71A50000, DLL_PROCESS_ATTACH, 0x00000000) in "MSWSOCK.DLL" called.
DllMain(0x71A50000, DLL_PROCESS_ATTACH, 0x00000000) in "MSWSOCK.DLL" returned 1 (0x1).
DllMain(0x6EEC0000, DLL_PROCESS_ATTACH, 0x00000000) in "LIBAPR_TSVN.DLL" called.
DllMain(0x6EEC0000, DLL_PROCESS_ATTACH, 0x00000000) in "LIBAPR_TSVN.DLL" returned 1 (0x1).
DllMain(0x6EE60000, DLL_PROCESS_ATTACH, 0x00000000) in "LIBAPRUTIL_TSVN.DLL" called.
DllMain(0x6EE60000, DLL_PROCESS_ATTACH, 0x00000000) in "LIBAPRUTIL_TSVN.DLL" returned 1 (0x1).
DllMain(0x01C70000, DLL_PROCESS_ATTACH, 0x00000000) in "INTL3_TSVN.DLL" called.
DllMain(0x01C70000, DLL_PROCESS_ATTACH, 0x00000000) in "INTL3_TSVN.DLL" returned 1 (0x1).
DllMain(0x78480000, DLL_PROCESS_ATTACH, 0x00000000) in "MSVCP90.DLL" called.
DllMain(0x78480000, DLL_PROCESS_ATTACH, 0x00000000) in "MSVCP90.DLL" returned 1 (0x1).
DllMain(0x77B20000, DLL_PROCESS_ATTACH, 0x00000000) in "MSASN1.DLL" called.
DllMain(0x77B20000, DLL_PROCESS_ATTACH, 0x00000000) in "MSASN1.DLL" returned 1 (0x1).
DllMain(0x77A80000, DLL_PROCESS_ATTACH, 0x00000000) in "CRYPT32.DLL" called.
LoadLibraryA("advapi32.dll") called from "CRYPT32.DLL" at address 0x77A86556.
LoadLibraryA("advapi32.dll") returned 0x77DD0000.
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "CryptAcquireContextW") called from "CRYPT32.DLL" at address 0x77A86569 and returned 0x77DE7F99.
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "CryptSignHashW") called from "CRYPT32.DLL" at address 0x77A8657B and returned 0x77E11FD1.
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "CryptVerifySignatureW") called from "CRYPT32.DLL" at address 0x77A8658D and returned 0x77DF3522.
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "CryptSetProviderW") called from "CRYPT32.DLL" at address 0x77A8659F and returned 0x77E12C61.
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "CryptEnumProvidersW") called from "CRYPT32.DLL" at address 0x77A865B1 and returned 0x77E12B09.
LoadLibraryA("kernel32.dll") called from "CRYPT32.DLL" at address 0x77A87972.
LoadLibraryA("kernel32.dll") returned 0x7C800000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "RegisterWaitForSingleObject") called from "CRYPT32.DLL" at address 0x77A8798D and returned 0x7C8211CD.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "UnregisterWaitEx") called from "CRYPT32.DLL" at address 0x77A879A7 and returned 0x7C83006A.
LoadLibraryA("advapi32.dll") called from "MSASN1.DLL" at address 0x77B235EA.
LoadLibraryA("advapi32.dll") returned 0x77DD0000.
GetProcAddress(0x77DD0000 [ADVAPI32.DLL], "RegOpenKeyExA") called from "MSASN1.DLL" at address 0x77B23600 and returned 0x77DD7852.
DllMain(0x77A80000, DLL_PROCESS_ATTACH, 0x00000000) in "CRYPT32.DLL" returned 1 (0x1).
DllMain(0x01B60000, DLL_PROCESS_ATTACH, 0x00000000) in "TORTOISESVN.DLL" called.
DllMain(0x01B60000, DLL_PROCESS_ATTACH, 0x00000000) in "TORTOISESVN.DLL" returned 1 (0x1).
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseSVN.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x01B60000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x78543667 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x78543667 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x7854379B and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x785437AB and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x05965748 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x05965748 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x0596581C and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x0596582C and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x0594143E and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x0594143E and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x05941512 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x05941522 and returned 0x7C913425.
GetProcAddress(0x01B60000 [TORTOISESVN.DLL], "DllGetClassObject") called from "TORTOISESTUB.DLL" at address 0x05947603 and returned 0x01BBCF40.
GetProcAddress(0x01B60000 [TORTOISESVN.DLL], "DllCanUnloadNow") called from "TORTOISESTUB.DLL" at address 0x05947619 and returned 0x01BBCF30.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "accept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1040.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "bind") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4480.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "closesocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3E2B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "connect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4A07.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getpeername") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0B68.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3D10.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB46EA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ioctlsocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3F50.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_addr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_ntoa") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB45C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "listen") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8CD3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB676F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recvfrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2FF7.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "select") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB30A8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "send") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4C27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "sendto") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2F51.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "setsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4521.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "shutdown") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0BF6.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "socket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4211.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyaddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE491.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5355.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE18E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobynumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE0CC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE703.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyport") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE5B5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5449.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0991.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByAddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEA43.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE99D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByNumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE2C3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEAED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByPort") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE913.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE867.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelAsyncRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE347.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD4AC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAUnhookBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD508.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3CCE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A5E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelBlockingCall") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD40A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIsBlocking") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD459.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStartup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6A55.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACleanup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3FED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAccept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0DC1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACloseEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB65E8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAConnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0C81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACreateEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB655D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB9A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB0C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNetworkEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB657D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDC5F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB888C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEventSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB64D9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetOverlappedResult") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0D1B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetQOSByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF470.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3EC0.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAJoinLeaf") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0F70.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4CB5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF5EE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvFrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF66A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAResetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C80.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASend") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB68FA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0A22.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendTo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0AAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C91.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8B6A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB404E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAWaitForMultipleEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9462.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6979.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9765.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB281E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB562A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB35EF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5A8D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3181.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANSPIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5086.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceEnd") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB350E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC03C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFBC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARemoveServiceClass") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFCB4.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0440.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0159.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF9E1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFA41.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFD9F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFF81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC04D2.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0281.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCDeinstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC19C9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1665.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnumProtocols") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB7B61.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCGetProviderPath") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8E27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF14D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUnInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF319.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnableNSProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEE59.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WPUCompleteOverlappedRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1CBF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAProviderConfigChange") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8C20.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteProviderOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1531.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteNameSpaceOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF019.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUpdateProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1291.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetAddrInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2899.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getnameinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC689.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetNameInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC504.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "freeaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2B0B.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "QueryActCtxW") called from "TORTOISESVN.DLL" at address 0x01B703F5 and returned 0x7C81637B.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "ActivateActCtx") called from "TORTOISESVN.DLL" at address 0x01B703F5 and returned 0x7C80A6E4.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "FindActCtxSectionStringW") called from "TORTOISESVN.DLL" at address 0x01B703F5 and returned 0x7C82FD54.
LoadLibraryW("Comctl32.dll") called from "TORTOISESVN.DLL" at address 0x01B700AE.
LoadLibraryW("Comctl32.dll") returned 0x773D0000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DeactivateActCtx") called from "TORTOISESVN.DLL" at address 0x01B703F5 and returned 0x7C80A715.
LoadLibraryW("Comctl32.dll") called from "TORTOISESVN.DLL" at address 0x01B7020D.
LoadLibraryW("Comctl32.dll") returned 0x773D0000.
GetProcAddress(0x773D0000 [COMCTL32.DLL], "InitCommonControlsEx") called from "TORTOISESVN.DLL" at address 0x01B703F5 and returned 0x773D4088.
LoadLibraryExW("C:\Program Files\TortoiseSVN\Languages\TortoiseProc1033.dll", 0x00000000, LOAD_LIBRARY_AS_DATAFILE) called from "VERSION.DLL" at address 0x77C013BD.
LoadLibraryExW("C:\Program Files\TortoiseSVN\Languages\TortoiseProc1033.dll", 0x00000000, LOAD_LIBRARY_AS_DATAFILE) returned NULL. Error: The system cannot find the file specified (2).
LoadLibraryW("shell32.dll") called from "VERSION.DLL" at address 0x77C01B47.
LoadLibraryW("shell32.dll") returned 0x7C9C0000.
GetProcAddress(0x7C9C0000 [SHELL32.DLL], "ExtractVersionResource16W") called from "VERSION.DLL" at address 0x77C01B59 and returned 0x7CA29681.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) returned 0x05960000.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISEOVERLAYS.DLL" at address 0x05963D11.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05940000.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllGetClassObject") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCCA and returned 0x05947651.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllCanUnloadNow") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCF8 and returned 0x05947385.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) returned 0x05960000.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISEOVERLAYS.DLL" at address 0x05963D11.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05940000.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllGetClassObject") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCCA and returned 0x05947651.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllCanUnloadNow") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCF8 and returned 0x05947385.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) returned 0x05960000.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISEOVERLAYS.DLL" at address 0x05963D11.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05940000.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllGetClassObject") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCCA and returned 0x05947651.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllCanUnloadNow") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCF8 and returned 0x05947385.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) returned 0x05960000.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISEOVERLAYS.DLL" at address 0x05963D11.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05940000.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllGetClassObject") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCCA and returned 0x05947651.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllCanUnloadNow") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCF8 and returned 0x05947385.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) returned 0x05960000.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISEOVERLAYS.DLL" at address 0x05963D11.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05940000.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllGetClassObject") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCCA and returned 0x05947651.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllCanUnloadNow") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCF8 and returned 0x05947385.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) returned 0x05960000.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISEOVERLAYS.DLL" at address 0x05963D11.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05940000.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllGetClassObject") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCCA and returned 0x05947651.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllCanUnloadNow") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCF8 and returned 0x05947385.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) returned 0x05960000.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISEOVERLAYS.DLL" at address 0x05963D11.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05940000.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllGetClassObject") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCCA and returned 0x05947651.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllCanUnloadNow") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCF8 and returned 0x05947385.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\Program Files\Common Files\TortoiseOverlays\TortoiseOverlays.dll", 0x00000000, 0x00000000) returned 0x05960000.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "TORTOISEOVERLAYS.DLL" at address 0x05963D11.
LoadLibraryExW("C:\Program Files\TortoiseSVN\bin\TortoiseStub.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x05940000.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllGetClassObject") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCCA and returned 0x05947651.
GetProcAddress(0x05940000 [TORTOISESTUB.DLL], "DllCanUnloadNow") called from "TORTOISEOVERLAYS.DLL" at address 0x0596CCF8 and returned 0x05947385.
LoadLibraryExW("C:\WINNT\System32\cscui.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "OLE32.DLL" at address 0x77512485.
Loaded "CSCUI.DLL" at address 0x77A20000.  Successfully hooked module.
Loaded "CSCDLL.DLL" at address 0x76600000.  Successfully hooked module.
DllMain(0x76600000, DLL_PROCESS_ATTACH, 0x00000000) in "CSCDLL.DLL" called.
DllMain(0x76600000, DLL_PROCESS_ATTACH, 0x00000000) in "CSCDLL.DLL" returned 1 (0x1).
DllMain(0x77A20000, DLL_PROCESS_ATTACH, 0x00000000) in "CSCUI.DLL" called.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "CreateActCtxW") called from "CSCUI.DLL" at address 0x77A2190F and returned 0x7C8154FC.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "ActivateActCtx") called from "CSCUI.DLL" at address 0x77A21B2F and returned 0x7C80A6E4.
LoadLibraryW("comctl32.dll") called from "CSCUI.DLL" at address 0x77A21A29.
LoadLibraryW("comctl32.dll") returned 0x773D0000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DeactivateActCtx") called from "CSCUI.DLL" at address 0x77A21AA3 and returned 0x7C80A715.
DllMain(0x77A20000, DLL_PROCESS_ATTACH, 0x00000000) in "CSCUI.DLL" returned 1 (0x1).
LoadLibraryExW("C:\WINNT\System32\cscui.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x77A20000.
GetProcAddress(0x77A20000 [CSCUI.DLL], "DllGetClassObject") called from "OLE32.DLL" at address 0x775124A0 and returned 0x77A21D1D.
GetProcAddress(0x77A20000 [CSCUI.DLL], "DllCanUnloadNow") called from "OLE32.DLL" at address 0x775124B6 and returned 0x77A2172D.
LoadLibraryExW("C:\WINNT\System32\cscui.dll", 0x00000000, 0x00000000) called from "SHLWAPI.DLL" at address 0x77F689EE.
LoadLibraryExW("C:\WINNT\System32\cscui.dll", 0x00000000, 0x00000000) returned 0x77A20000.
GetProcAddress(0x773D0000 [COMCTL32.DLL], 0x00000141) called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E0AA1.
GetProcAddress(0x774E0000 [OLE32.DLL], "CoUninitialize") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x774FEE46.
GetProcAddress(0x773D0000 [COMCTL32.DLL], "ImageList_GetIcon") called from "SHELL32.DLL" at address 0x7CA065DC and returned 0x773E55C7.
LoadLibraryA("USERENV.dll") called from "SHELL32.DLL" at address 0x7CA0653F.
Loaded "USERENV.DLL" at address 0x769C0000.  Successfully hooked module.
DllMain(0x769C0000, DLL_PROCESS_ATTACH, 0x00000000) in "USERENV.DLL" called.
DllMain(0x769C0000, DLL_PROCESS_ATTACH, 0x00000000) in "USERENV.DLL" returned 1 (0x1).
LoadLibraryA("USERENV.dll") returned 0x769C0000.
GetProcAddress(0x769C0000 [USERENV.DLL], "GetUserProfileDirectoryW") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x769C6357.
GetProcAddress(0x7C9C0000 [SHELL32.DLL], 0x00000042) called from "SHLWAPI.DLL" at address 0x77F716EA and returned 0x7C9F0618.
GetProcAddress(0x7C9C0000 [SHELL32.DLL], 0x00000064) called from "SHLWAPI.DLL" at address 0x77F716EA and returned 0x7C9EC509.
LoadLibraryA("shell32.dll") called from "LISTER.EXE" at address 0x0080071E.
LoadLibraryA("shell32.dll") returned 0x7C9C0000.
GetProcAddress(0x7C9C0000 [SHELL32.DLL], "SHGetKnownFolderPath") called from "LISTER.EXE" at address 0x00800735 and returned NULL. Error: The specified procedure could not be found (127).
LoadLibraryA("MPR.dll") called from "SHELL32.DLL" at address 0x7CA0653F.
LoadLibraryA("MPR.dll") returned 0x71B20000.
GetProcAddress(0x71B20000 [MPR.DLL], "WNetOpenEnumW") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x71B23910.
LoadLibraryExW("C:\WINNT\system32\ccasenp.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "MPR.DLL" at address 0x71B28A80.
Loaded "CCASENP.DLL" at address 0x23400000.  Successfully hooked module.
DllMain(0x23400000, DLL_PROCESS_ATTACH, 0x00000000) in "CCASENP.DLL" called.
DllMain(0x23400000, DLL_PROCESS_ATTACH, 0x00000000) in "CCASENP.DLL" returned 1 (0x1).
LoadLibraryExW("C:\WINNT\system32\ccasenp.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x23400000.
LoadLibraryExW("C:\WINNT\system32\ccasenp.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "MPR.DLL" at address 0x71B22B62.
LoadLibraryExW("C:\WINNT\system32\ccasenp.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x23400000.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPGetCaps") called from "MPR.DLL" at address 0x71B22BE5 and returned 0x234022E0.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPAddConnection") called from "MPR.DLL" at address 0x71B22C2E and returned 0x23402960.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPCancelConnection") called from "MPR.DLL" at address 0x71B22C74 and returned 0x23402830.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPGetConnection") called from "MPR.DLL" at address 0x71B22C86 and returned 0x234025E0.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPGetConnection3") called from "MPR.DLL" at address 0x71B22C93 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x23400000 [CCASENP.DLL], "NPGetUniversalName") called from "MPR.DLL" at address 0x71B22CA0 and returned 0x23404740.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPOpenEnum") called from "MPR.DLL" at address 0x71B22CC2 and returned 0x23402C70.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPEnumResource") called from "MPR.DLL" at address 0x71B22CCF and returned 0x23403BE0.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPCloseEnum") called from "MPR.DLL" at address 0x71B22CDC and returned 0x23403FE0.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPFormatNetworkName") called from "MPR.DLL" at address 0x71B22D1F and returned 0x234040F0.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPGetResourceParent") called from "MPR.DLL" at address 0x71B22D3D and returned 0x234045C0.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPGetResourceInformation") called from "MPR.DLL" at address 0x71B22D52 and returned 0x23404210.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPLogonNotify") called from "MPR.DLL" at address 0x71B28879 and returned 0x23401EF0.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPPasswordChangeNotify") called from "MPR.DLL" at address 0x71B28889 and returned 0x234022D0.
GetProcAddress(0x23400000 [CCASENP.DLL], "NPGetCaps") called from "MPR.DLL" at address 0x71B288A8 and returned 0x234022E0.
LoadLibraryExW("C:\WINNT\System32\drprov.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "MPR.DLL" at address 0x71B22B62.
Loaded "DRPROV.DLL" at address 0x75F60000.  Successfully hooked module.
DllMain(0x75F60000, DLL_PROCESS_ATTACH, 0x00000000) in "DRPROV.DLL" called.
DllMain(0x75F60000, DLL_PROCESS_ATTACH, 0x00000000) in "DRPROV.DLL" returned 1 (0x1).
LoadLibraryExW("C:\WINNT\System32\drprov.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x75F60000.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPGetCaps") called from "MPR.DLL" at address 0x71B22BE5 and returned 0x75F61410.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPGetUser") called from "MPR.DLL" at address 0x71B22C12 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPAddConnection") called from "MPR.DLL" at address 0x71B22C2E and returned 0x75F6293B.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPAddConnection3") called from "MPR.DLL" at address 0x71B22C40 and returned 0x75F618DB.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPGetReconnectFlags") called from "MPR.DLL" at address 0x71B22C4D and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPCancelConnection") called from "MPR.DLL" at address 0x71B22C74 and returned 0x75F61AB7.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPGetConnection") called from "MPR.DLL" at address 0x71B22C86 and returned 0x75F61670.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPGetConnection3") called from "MPR.DLL" at address 0x71B22C93 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPGetUniversalName") called from "MPR.DLL" at address 0x71B22CA0 and returned 0x75F622BF.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPOpenEnum") called from "MPR.DLL" at address 0x71B22CC2 and returned 0x75F6145D.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPEnumResource") called from "MPR.DLL" at address 0x71B22CCF and returned 0x75F62114.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPCloseEnum") called from "MPR.DLL" at address 0x71B22CDC and returned 0x75F6218E.
GetProcAddress(0x75F60000 [DRPROV.DLL], "NPGetResourceInformation") called from "MPR.DLL" at address 0x71B22D52 and returned 0x75F62534.
LoadLibraryExW("C:\WINNT\System32\ntlanman.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "MPR.DLL" at address 0x71B22B62.
Loaded "NTLANMAN.DLL" at address 0x71C10000.  Successfully hooked module.
Loaded "NETAPI32.DLL" at address 0x5B860000.  Successfully hooked module.
Loaded "NETUI0.DLL" at address 0x71CD0000.  Successfully hooked module.
Loaded "NETUI1.DLL" at address 0x71C90000.  Successfully hooked module.
Loaded "NETRAP.DLL" at address 0x71C80000.  Successfully hooked module.
Loaded "SAMLIB.DLL" at address 0x71BF0000.  Successfully hooked module.
DllMain(0x5B860000, DLL_PROCESS_ATTACH, 0x00000000) in "NETAPI32.DLL" called.
DllMain(0x5B860000, DLL_PROCESS_ATTACH, 0x00000000) in "NETAPI32.DLL" returned 1 (0x1).
DllMain(0x71CD0000, DLL_PROCESS_ATTACH, 0x00000000) in "NETUI0.DLL" called.
DllMain(0x71CD0000, DLL_PROCESS_ATTACH, 0x00000000) in "NETUI0.DLL" returned 1 (0x1).
DllMain(0x71C80000, DLL_PROCESS_ATTACH, 0x00000000) in "NETRAP.DLL" called.
DllMain(0x71C80000, DLL_PROCESS_ATTACH, 0x00000000) in "NETRAP.DLL" returned 1 (0x1).
DllMain(0x71BF0000, DLL_PROCESS_ATTACH, 0x00000000) in "SAMLIB.DLL" called.
DllMain(0x71BF0000, DLL_PROCESS_ATTACH, 0x00000000) in "SAMLIB.DLL" returned 1 (0x1).
DllMain(0x71C90000, DLL_PROCESS_ATTACH, 0x00000000) in "NETUI1.DLL" called.
DllMain(0x71C90000, DLL_PROCESS_ATTACH, 0x00000000) in "NETUI1.DLL" returned 1 (0x1).
DllMain(0x71C10000, DLL_PROCESS_ATTACH, 0x00000000) in "NTLANMAN.DLL" called.
DllMain(0x71C10000, DLL_PROCESS_ATTACH, 0x00000000) in "NTLANMAN.DLL" returned 1 (0x1).
LoadLibraryExW("C:\WINNT\System32\ntlanman.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x71C10000.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetCaps") called from "MPR.DLL" at address 0x71B22BE5 and returned 0x71C121A4.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetUser") called from "MPR.DLL" at address 0x71B22C12 and returned 0x71C13631.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPAddConnection") called from "MPR.DLL" at address 0x71B22C2E and returned 0x71C13478.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPAddConnection3") called from "MPR.DLL" at address 0x71B22C40 and returned 0x71C12E0A.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetReconnectFlags") called from "MPR.DLL" at address 0x71B22C4D and returned 0x71C135BE.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPCancelConnection") called from "MPR.DLL" at address 0x71B22C74 and returned 0x71C138DF.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetConnection") called from "MPR.DLL" at address 0x71B22C86 and returned 0x71C11AC4.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetConnection3") called from "MPR.DLL" at address 0x71B22C93 and returned 0x71C14005.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetUniversalName") called from "MPR.DLL" at address 0x71B22CA0 and returned 0x71C16E0F.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetConnectionPerformance") called from "MPR.DLL" at address 0x71B22E7C and returned 0x71C14198.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPOpenEnum") called from "MPR.DLL" at address 0x71B22CC2 and returned 0x71C1267A.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPEnumResource") called from "MPR.DLL" at address 0x71B22CCF and returned 0x71C123DC.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPCloseEnum") called from "MPR.DLL" at address 0x71B22CDC and returned 0x71C124E1.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetDirectoryType") called from "MPR.DLL" at address 0x71B22EAD and returned 0x71C17D71.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPDirectoryNotify") called from "MPR.DLL" at address 0x71B22ED5 and returned 0x71C174AA.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetPropertyText") called from "MPR.DLL" at address 0x71B22EFC and returned 0x71C174B9.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPPropertyDialog") called from "MPR.DLL" at address 0x71B22F09 and returned 0x71C1753B.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPFormatNetworkName") called from "MPR.DLL" at address 0x71B22D1F and returned 0x71C17FDF.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetResourceParent") called from "MPR.DLL" at address 0x71B22D3D and returned 0x71C18432.
GetProcAddress(0x71C10000 [NTLANMAN.DLL], "NPGetResourceInformation") called from "MPR.DLL" at address 0x71B22D52 and returned 0x71C18864.
LoadLibraryExW("C:\WINNT\System32\davclnt.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "MPR.DLL" at address 0x71B22B62.
Loaded "DAVCLNT.DLL" at address 0x75F70000.  Successfully hooked module.
DllMain(0x75F70000, DLL_PROCESS_ATTACH, 0x00000000) in "DAVCLNT.DLL" called.
DllMain(0x75F70000, DLL_PROCESS_ATTACH, 0x00000000) in "DAVCLNT.DLL" returned 1 (0x1).
LoadLibraryExW("C:\WINNT\System32\davclnt.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x75F70000.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPGetCaps") called from "MPR.DLL" at address 0x71B22BE5 and returned 0x75F716C0.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPGetUser") called from "MPR.DLL" at address 0x71B22C12 and returned 0x75F73F07.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPAddConnection") called from "MPR.DLL" at address 0x71B22C2E and returned 0x75F749A5.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPAddConnection3") called from "MPR.DLL" at address 0x71B22C40 and returned 0x75F742F3.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPGetReconnectFlags") called from "MPR.DLL" at address 0x71B22C4D and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPCancelConnection") called from "MPR.DLL" at address 0x71B22C74 and returned 0x75F723B9.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPGetConnection") called from "MPR.DLL" at address 0x71B22C86 and returned 0x75F740E9.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPGetConnection3") called from "MPR.DLL" at address 0x71B22C93 and returned NULL. Error: The specified procedure could not be found (127).
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPGetUniversalName") called from "MPR.DLL" at address 0x71B22CA0 and returned 0x75F74E72.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPOpenEnum") called from "MPR.DLL" at address 0x71B22CC2 and returned 0x75F71D59.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPEnumResource") called from "MPR.DLL" at address 0x71B22CCF and returned 0x75F71E7B.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPCloseEnum") called from "MPR.DLL" at address 0x71B22CDC and returned 0x75F71E32.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPFormatNetworkName") called from "MPR.DLL" at address 0x71B22D1F and returned 0x75F735E3.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPGetResourceParent") called from "MPR.DLL" at address 0x71B22D3D and returned 0x75F74BF4.
GetProcAddress(0x75F70000 [DAVCLNT.DLL], "NPGetResourceInformation") called from "MPR.DLL" at address 0x71B22D52 and returned 0x75F749C5.
LoadLibraryExW("C:\WINNT\system32\ccasenp.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "MPR.DLL" at address 0x71B23AC8.
LoadLibraryExW("C:\WINNT\system32\ccasenp.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x23400000.
LoadLibraryExW("C:\WINNT\System32\drprov.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "MPR.DLL" at address 0x71B23AC8.
LoadLibraryExW("C:\WINNT\System32\drprov.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x75F60000.
LoadLibraryExW("C:\WINNT\System32\ntlanman.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "MPR.DLL" at address 0x71B23AC8.
LoadLibraryExW("C:\WINNT\System32\ntlanman.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x71C10000.
LoadLibraryExW("C:\WINNT\System32\davclnt.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) called from "MPR.DLL" at address 0x71B23AC8.
LoadLibraryExW("C:\WINNT\System32\davclnt.dll", 0x00000000, LOAD_WITH_ALTERED_SEARCH_PATH) returned 0x75F70000.
GetProcAddress(0x71B20000 [MPR.DLL], "WNetEnumResourceW") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x71B23C8A.
GetProcAddress(0x71B20000 [MPR.DLL], "WNetCloseEnum") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x71B23E4F.
GetProcAddress(0x71B20000 [MPR.DLL], "WNetGetConnectionW") called from "SHELL32.DLL" at address 0x7CA064F6 and returned 0x71B21E09.
LoadLibraryA("Secur32.dll") called from "ADVAPI32.DLL" at address 0x77DE5439.
LoadLibraryA("Secur32.dll") returned 0x77FE0000.
GetProcAddress(0x77FE0000 [SECUR32.DLL], "GetUserNameExA") called from "ADVAPI32.DLL" at address 0x77DE548E and returned 0x77FE1DCA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "accept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1040.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "bind") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4480.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "closesocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3E2B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "connect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4A07.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getpeername") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0B68.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3D10.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB46EA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "htons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ioctlsocket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3F50.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_addr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "inet_ntoa") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB45C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "listen") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8CD3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2EAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "ntohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2E53.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB676F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "recvfrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2FF7.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "select") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB30A8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "send") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4C27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "sendto") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2F51.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "setsockopt") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4521.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "shutdown") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0BF6.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "socket") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4211.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyaddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE491.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5355.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE18E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getprotobynumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE0CC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE703.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getservbyport") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE5B5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "gethostname") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5449.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0991.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByAddr") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEA43.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetHostByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE99D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByNumber") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE2C3.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetProtoByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEAED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByPort") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE913.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAsyncGetServByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE867.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelAsyncRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABE347.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD4AC.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAUnhookBlockingHook") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD508.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3CCE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetLastError") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A5E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACancelBlockingCall") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD40A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIsBlocking") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABD459.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStartup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6A55.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACleanup") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3FED.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAccept") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0DC1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACloseEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB65E8.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAConnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0C81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSACreateEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB655D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB9A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSADuplicateSocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDB0C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNetworkEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB657D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABDC5F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumProtocolsW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB888C.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEventSelect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB64D9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetOverlappedResult") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0D1B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetQOSByName") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF470.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtonl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAHtons") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3EC0.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAJoinLeaf") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0F70.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBCE1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANtohs") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABBDC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecv") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB4CB5.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF5EE.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARecvFrom") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF66A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAResetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C80.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASend") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB68FA.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendDisconnect") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0A22.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASendTo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0AAD.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetEvent") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C91.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8B6A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASocketW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB404E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAWaitForMultipleEvents") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2C6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9462.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAAddressToStringW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB6979.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB9765.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAStringToAddressW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB281E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB562A.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceBeginW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB35EF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5A8D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceNextW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB3181.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSANSPIoctl") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB5086.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSALookupServiceEnd") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB350E.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC03C1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAInstallServiceClassW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFBC9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSARemoveServiceClass") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFCB4.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0440.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0159.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF9E1.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAEnumNameSpaceProvidersW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFA41.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFD9F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAGetServiceClassNameByClassIdW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABFF81.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceA") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC04D2.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSASetServiceW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC0281.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCDeinstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC19C9.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1665.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnumProtocols") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB7B61.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCGetProviderPath") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8E27.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF14D.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUnInstallNameSpace") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF319.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCEnableNSProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABEE59.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WPUCompleteOverlappedRequest") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1CBF.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSAProviderConfigChange") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB8C20.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteProviderOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1531.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCWriteNameSpaceOrder") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABF019.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "WSCUpdateProvider") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AC1291.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2A6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetAddrInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2899.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getnameinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC689.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "GetNameInfoW") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71ABC504.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "freeaddrinfo") called from "WS2_32.DLL" at address 0x71AB737E and returned 0x71AB2B0B.
LoadLibraryA("ws2_32") called from "LIBPQ.DLL" at address 0x10019A55.
LoadLibraryA("ws2_32") returned 0x71AB0000.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getaddrinfo") called from "LIBPQ.DLL" at address 0x10019A69 and returned 0x71AB2A6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getaddrinfo") called from "LIBPQ.DLL" at address 0x10019A89 and returned 0x71AB2A6F.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "freeaddrinfo") called from "LIBPQ.DLL" at address 0x10019A96 and returned 0x71AB2B0B.
GetProcAddress(0x71AB0000 [WS2_32.DLL], "getnameinfo") called from "LIBPQ.DLL" at address 0x10019AA3 and returned 0x71ABC689.
LoadLibraryA("C:\WINNT\System32\mswsock.dll") called from "WS2_32.DLL" at address 0x71AB4EA0.
LoadLibraryA("C:\WINNT\System32\mswsock.dll") returned 0x71A50000.
GetProcAddress(0x71A50000 [MSWSOCK.DLL], "NSPStartup") called from "WS2_32.DLL" at address 0x71AB4EB7 and returned 0x71A5A830.
LoadLibraryA("DNSAPI.dll") called from "MSWSOCK.DLL" at address 0x71A5C714.
Loaded "DNSAPI.DLL" at address 0x76F20000.  Successfully hooked module.
DllMain(0x76F20000, DLL_PROCESS_ATTACH, 0x00000000) in "DNSAPI.DLL" called.
DllMain(0x76F20000, DLL_PROCESS_ATTACH, 0x00000000) in "DNSAPI.DLL" returned 1 (0x1).
LoadLibraryA("DNSAPI.dll") returned 0x76F20000.
GetProcAddress(0x76F20000 [DNSAPI.DLL], "DnsQueryConfigAllocEx") called from "MSWSOCK.DLL" at address 0x71A5C6FD and returned 0x76F27A1D.
GetProcAddress(0x76F20000 [DNSAPI.DLL], "DnsApiFree") called from "MSWSOCK.DLL" at address 0x71A5C6FD and returned 0x76F23761.
GetProcAddress(0x76F20000 [DNSAPI.DLL], "DnsQuery_W") called from "MSWSOCK.DLL" at address 0x71A5C6FD and returned 0x76F22D94.
GetProcAddress(0x76F20000 [DNSAPI.DLL], "DnsRecordListFree") called from "MSWSOCK.DLL" at address 0x71A5C6FD and returned 0x76F25AD3.
GetProcAddress(0x76F20000 [DNSAPI.DLL], "DnsNameCompare_W") called from "MSWSOCK.DLL" at address 0x71A5C6FD and returned 0x76F2345A.
LoadLibraryA("rasadhlp.dll") called from "WS2_32.DLL" at address 0x71AB4FB8.
Loaded "RASADHLP.DLL" at address 0x76FC0000.  Successfully hooked module.
DllMain(0x76FC0000, DLL_PROCESS_ATTACH, 0x00000000) in "RASADHLP.DLL" called.
DllMain(0x76FC0000, DLL_PROCESS_ATTACH, 0x00000000) in "RASADHLP.DLL" returned 1 (0x1).
LoadLibraryA("rasadhlp.dll") returned 0x76FC0000.
GetProcAddress(0x76FC0000 [RASADHLP.DLL], "WSAttemptAutodialAddr") called from "WS2_32.DLL" at address 0x71AB4FCF and returned 0x76FC1AF3.
GetProcAddress(0x76FC0000 [RASADHLP.DLL], "WSAttemptAutodialName") called from "WS2_32.DLL" at address 0x71AB4FE1 and returned 0x76FC11A0.
GetProcAddress(0x76FC0000 [RASADHLP.DLL], "WSNoteSuccessfulHostentLookup") called from "WS2_32.DLL" at address 0x71AB4FF3 and returned 0x76FC13A0.
LoadLibraryA("C:\WINNT\system32\mswsock.dll") called from "WS2_32.DLL" at address 0x71AB78F1.
LoadLibraryA("C:\WINNT\system32\mswsock.dll") returned 0x71A50000.
GetProcAddress(0x71A50000 [MSWSOCK.DLL], "WSPStartup") called from "WS2_32.DLL" at address 0x71AB7908 and returned 0x71A5C29B.
LoadLibraryW("hnetcfg.dll") called from "MSWSOCK.DLL" at address 0x71A5C0C3.
Loaded "HNETCFG.DLL" at address 0x662B0000.  Successfully hooked module.
DllMain(0x662B0000, DLL_PROCESS_ATTACH, 0x00000000) in "HNETCFG.DLL" called.
DllMain(0x662B0000, DLL_PROCESS_ATTACH, 0x00000000) in "HNETCFG.DLL" returned 1 (0x1).
LoadLibraryW("hnetcfg.dll") returned 0x662B0000.
GetProcAddress(0x662B0000 [HNETCFG.DLL], "IcfOpenDynamicFwPort") called from "MSWSOCK.DLL" at address 0x71A5C0DB and returned 0x662DAF3F.
GetProcAddress(0x662B0000 [HNETCFG.DLL], "IcfDisconnect") called from "MSWSOCK.DLL" at address 0x71A5C0EA and returned 0x662DAD91.
GetProcAddress(0x662B0000 [HNETCFG.DLL], "IcfConnect") called from "MSWSOCK.DLL" at address 0x71A5C0F9 and returned 0x662DB384.
LoadLibraryA("C:\WINNT\system32\MSWSOCK.dll") called from "MSWSOCK.DLL" at address 0x71A5C359.
LoadLibraryA("C:\WINNT\system32\MSWSOCK.dll") returned 0x71A50000.
LoadLibraryW("C:\WINNT\System32\wshtcpip.dll") called from "MSWSOCK.DLL" at address 0x71A5BB83.
Loaded "WSHTCPIP.DLL" at address 0x71A90000.  Successfully hooked module.
DllMain(0x71A90000, DLL_PROCESS_ATTACH, 0x00000000) in "WSHTCPIP.DLL" called.
DllMain(0x71A90000, DLL_PROCESS_ATTACH, 0x00000000) in "WSHTCPIP.DLL" returned 1 (0x1).
LoadLibraryW("C:\WINNT\System32\wshtcpip.dll") returned 0x71A90000.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHOpenSocket") called from "MSWSOCK.DLL" at address 0x71A5BBBB and returned 0x71A94046.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHOpenSocket2") called from "MSWSOCK.DLL" at address 0x71A5BBCC and returned 0x71A91480.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHJoinLeaf") called from "MSWSOCK.DLL" at address 0x71A5BBE6 and returned 0x71A9406F.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHNotify") called from "MSWSOCK.DLL" at address 0x71A5BBF7 and returned 0x71A91304.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHGetSocketInformation") called from "MSWSOCK.DLL" at address 0x71A5BC10 and returned 0x71A91660.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHSetSocketInformation") called from "MSWSOCK.DLL" at address 0x71A5BC29 and returned 0x71A91741.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHGetSockaddrType") called from "MSWSOCK.DLL" at address 0x71A5BC42 and returned 0x71A9127D.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHGetWildcardSockaddr") called from "MSWSOCK.DLL" at address 0x71A5BC5D and returned 0x71A91C99.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHGetBroadcastSockaddr") called from "MSWSOCK.DLL" at address 0x71A5BC72 and returned 0x71A93554.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHAddressToString") called from "MSWSOCK.DLL" at address 0x71A5BC87 and returned 0x71A91C26.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHStringToAddress") called from "MSWSOCK.DLL" at address 0x71A5BC9C and returned 0x71A92152.
GetProcAddress(0x71A90000 [WSHTCPIP.DLL], "WSHIoctl") called from "MSWSOCK.DLL" at address 0x71A5BCB1 and returned 0x71A91A89.
LoadLibraryA("C:\WINNT\system32\MSWSOCK.dll") called from "MSWSOCK.DLL" at address 0x71A5D8A5.
LoadLibraryA("C:\WINNT\system32\MSWSOCK.dll") returned 0x71A50000.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x78543667 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x78543667 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x7854379B and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x785437AB and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x05965748 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x05965748 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x0596581C and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x0596582C and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x0594143E and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x0594143E and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x05941512 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x05941522 and returned 0x7C913425.
LoadLibraryA("User32.dll") called from "LISTER.EXE" at address 0x007FF610.
LoadLibraryA("User32.dll") returned 0x7E410000.
GetProcAddress(0x7E410000 [USER32.DLL], "MonitorFromWindow") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7E42A679.
GetProcAddress(0x7E410000 [USER32.DLL], "MonitorFromRect") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7E42C713.
GetProcAddress(0x7E410000 [USER32.DLL], "MonitorFromPoint") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7E42ABF5.
GetProcAddress(0x7E410000 [USER32.DLL], "GetMonitorInfoA") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7E42A84A.
GetProcAddress(0x7E410000 [USER32.DLL], "EnumDisplayMonitors") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7E42A77B.
GetProcAddress(0x7E410000 [USER32.DLL], "EnumDisplayDevicesA") called from "LISTER.EXE" at address 0x007FF6C8 and returned 0x7E428A74.
GetProcAddress(0x77F10000 [GDI32.DLL], "GetLayout") called from "USP10.DLL" at address 0x74DAC988 and returned 0x77F16B2E.
GetProcAddress(0x77F10000 [GDI32.DLL], "GdiRealizationInfo") called from "USP10.DLL" at address 0x74DA5ADD and returned 0x77F3EB44.
GetProcAddress(0x77F10000 [GDI32.DLL], "FontIsLinked") called from "USP10.DLL" at address 0x74DA76CD and returned 0x77F51FEC.
GetProcAddress(0x77F10000 [GDI32.DLL], "GetFontAssocStatus") called from "USP10.DLL" at address 0x74DAC9B9 and returned 0x77F46F92.
GetProcAddress(0x77F10000 [GDI32.DLL], "GetTextExtentExPointWPri") called from "USP10.DLL" at address 0x74DC45F3 and returned 0x77F3E486.
LoadLibraryA("msimg32.dll") called from "LISTER.EXE" at address 0x006F1BAB.
Loaded "MSIMG32.DLL" at address 0x76380000.  Successfully hooked module.
DllMain(0x76380000, DLL_PROCESS_ATTACH, 0x00000000) in "MSIMG32.DLL" called.
DllMain(0x76380000, DLL_PROCESS_ATTACH, 0x00000000) in "MSIMG32.DLL" returned 1 (0x1).
LoadLibraryA("msimg32.dll") returned 0x76380000.
GetProcAddress(0x76380000 [MSIMG32.DLL], "AlphaBlend") called from "LISTER.EXE" at address 0x006F1BC3 and returned 0x7638119B.
LoadLibraryW("C:\WINNT\system32\mslbui.dll") called from "MSCTF.DLL" at address 0x7475D037.
Loaded "MSLBUI.DLL" at address 0x605D0000.  Successfully hooked module.
DllMain(0x605D0000, DLL_PROCESS_ATTACH, 0x00000000) in "MSLBUI.DLL" called.
DllMain(0x605D0000, DLL_PROCESS_ATTACH, 0x00000000) in "MSLBUI.DLL" returned 1 (0x1).
LoadLibraryW("C:\WINNT\system32\mslbui.dll") returned 0x605D0000.
GetProcAddress(0x605D0000 [MSLBUI.DLL], "CTFGetLangBarAddIn") called from "MSCTF.DLL" at address 0x7475D070 and returned 0x605D2D5C.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x78543667 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x78543667 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "MSVCR90.DLL" at address 0x7854379B and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "MSVCR90.DLL" at address 0x785437AB and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x05965748 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x05965748 and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x0596581C and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISEOVERLAYS.DLL" at address 0x0596582C and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x0594143E and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x0594143E and returned 0x7C913425.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "EncodePointer") called from "TORTOISESTUB.DLL" at address 0x05941512 and returned 0x7C9133FF.
GetProcAddress(0x7C800000 [KERNEL32.DLL], "DecodePointer") called from "TORTOISESTUB.DLL" at address 0x05941522 and returned 0x7C913425.
LoadLibraryA("OLEAUT32.DLL") called from "MSCTF.DLL" at address 0x74760BC8.
LoadLibraryA("OLEAUT32.DLL") returned 0x77120000.
GetProcAddress(0x77120000 [OLEAUT32.DLL], "SysAllocString") called from "MSCTF.DLL" at address 0x74760BE7 and returned 0x77124BA2.
GetProcAddress(0x774E0000 [OLE32.DLL], "CoCreateInstanceEx") called from "OLEAUT32.DLL" at address 0x771256BE and returned 0x77500526.
LoadLibraryA("oleaut32.dll") called from "OLE32.DLL" at address 0x77502B4F.
LoadLibraryA("oleaut32.dll") returned 0x77120000.
GetProcAddress(0x77120000 [OLEAUT32.DLL], "SysStringByteLen") called from "MSCTF.DLL" at address 0x74760C48 and returned 0x77124C88.
GetProcAddress(0x77120000 [OLEAUT32.DLL], "SysFreeString") called from "MSCTF.DLL" at address 0x74760CAE and returned 0x77124880.
GetProcAddress(0x7C900000 [NTDLL.DLL], "RtlDllShutdownInProgress") called from "MSCTF.DLL" at address 0x7475FA60 and returned 0x7C9136D0.
LoadLibraryA("OLEAUT32.DLL") called from "MSLBUI.DLL" at address 0x605D4C9D.
LoadLibraryA("OLEAUT32.DLL") returned 0x77120000.
GetProcAddress(0x77120000 [OLEAUT32.DLL], "SysAllocString") called from "MSLBUI.DLL" at address 0x605D4CBC and returned 0x77124BA2.

*/
#endif
