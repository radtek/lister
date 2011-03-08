TABLE_(ENVIRONMENTS)
   INT_     (ENVID) PRIMARY_KEY
   STRING_  (ENVNAME, 200)
END_TABLE

TABLE_(COLUMNS)
   INT_     (COLID) PRIMARY_KEY
   STRING_  (COLNAME, 200) 
   STRING_  (SRCCOLCODE, 200)
   STRING_  (SRCDATATYPCODE, 10)
   INT_     (SRCDATATYPWDTH)
   INT_     (SRCDATATYPDEC)
   INT_     (SRCDATAFFSTARTPOS)
   INT_     (SRCDATAFFFINPOS)
   INT_     (TBID)
   INT_     (PHYSORD)
   INT_     (BESTFITWDTH)
   STRING_  (NOTE, 1000)
   BOOL_    (EXPWRAPINQUOTES)
END_TABLE

TABLE_(TABLES)
   INT      (TBID) PRIMARY_KEY
   STRING_  (TBNAME, 200) 
   STRING_  (TBADDRESS, 200)
   STRING   (NOTE, 1000)
END_TABLE

TABLE_(TASKS)
   INT_     (TASKID) PRIMARY_KEY
   STRING_  (TASKNAME, 100)
   STRING_  (SRCCODE, 100)
   STRING   (NOTE, 1000)
   INT_     (ASSIGNEDBYWHO)
   INT_     (ASSIGNEDTOWHO)
   STRING_  (PROJECTNAME, 100)
   DATE_    (PRIORITYASSIGNEDDATE)
   DATE_    (NEXTTARGETDATE)
   STRING_  (TARGETDATEEXPECTATION, 200)
   INT_     (PRIORITYWITHINPROJECT)
   STRING_  (TASKDESC, 1000)
   STRING_  (SOLUTIONDESC, 1000)
   INT_     (DEPONWHO)
   INT_     (DEPONWHO2)
   INT_     (DEPONWHO3)
   INT_     (DEPONWHO4)
   INT_     (DEPONWHO5)
   DATE_    (DEPASSIGNEDWHEN)
   DATE_    (DEPEXPECTSAMPWHEN)
   STRING_  (DEPASSIGNDESC, 400)
   STRING_  (DEPFEEDBACK, 1000)
   BOOL_    (HIDDEN)
END_TABLE

TABLE_(PROJECTS)
   INT_     (PROJECTID) PRIMARY_KEY
   STRING   (PROJECTNAME, 100)
   STRING   (NOTE, 1000)
   STRING_  (MASTERPROJECTNAME, 100)
END_TABLE

TABLE_(INSTTYPS)
   INT_     (INSTTYPID) PRIMARY_KEY
   STRING_  (INSTTYPNAME, 200)
END_TABLE

TABLE_(INSTANCES)
   INT_     (INSTANCEID) PRIMARY_KEY
   STRING_  (INSTANCENAME, 100)
   STRING_  (INSTANCEADDRESS, 100)
   STRING   (NOTE, 1000)
   INT      (INSTTYPID)
   INT      (ENVID)
   INT_     (PORT)
END_TABLE   

TABLE_(SCRIPTS)
   INT_     (SCRIPTID) PRIMARY_KEY
   STRING_  (SCRIPTNAME, 100)
   STRING_  (SCRIPTPLAINTEXT, 1000)
   STRING_  (SCRIPTRICHTEXT, 1000)
   STRING   (NOTE, 1000)
END_TABLE

TABLE_(TASKS_R)
   INT_     (RELID) PRIMARY_KEY
   INT      (TASKID)
   STRING   (TASKNAME, 100)
   STRING   (SRCCODE, 100)
   STRING_  (TASKNOTE, 1000)
   STRING_  (SCRIPTNOTE, 1000)
   INT      (ASSIGNEDBYWHO)
   INT      (ASSIGNEDTOWHO)
   STRING   (PROJECTNAME, 100)
   INT      (PRIORITYWITHINPROJECT)
   INT      (SCRIPTID)
   STRING   (SCRIPTPLAINTEXT, 1000)
   STRING   (SCRIPTRICHTEXT, 1000)
   STRING_  (WHY, 300)
   INT_     (RELCONNID)
   STRING_  (RELCONNNAME, 200)
   TIME_    (ADDTIMESTAMP)
   INT_     (SCRIPTTARGET)
   STRING_  (TARGETNAME, 200)
   STRING_  (FASTFLUSHTARGET, 10)
   INT_     (ROWLIMIT)
   INT_     (PROCESSORDER)
   STRING_  (CAUSE, 400)
END_TABLE

TABLE_(CONTACTS)
   INT_     (CONTACTID) PRIMARY_KEY
   STRING_  (CONTACTNAME, 100)
   STRING_  (EMAILADDRESS, 100)
   STRING_  (HOMEPHONENO, 50)
   STRING_  (WORKPHONENO, 50)
   STRING_  (CELLPHONENO, 50)
   STRING_  (NONWORKEMAILADDRESS, 100)
   STRING   (NOTE, 1000)
END_TABLE

TABLE_(QUESTIONS)
   INT_     (QUESTIONID) PRIMARY_KEY
   STRING_  (QUESTIONTEXT, 100)
   STRING_  (ANSWERTEXT, 100)
   STRING   (NOTE, 1000)
   INT      (TASKID)
   STRING_  (STATUS, 100)
END_TABLE

TABLE_(ELEMENTS)
   INT_     (ELEMENTID) PRIMARY_KEY
   STRING_  (ELEMENTNAME, 100)
   STRING_  (ELEMENTDESC, 1000)
   STRING   (STATUS, 100)
   STRING   (NOTE, 1000)
   INT      (TASKID)
END_TABLE

TABLE_(LINKS)
   INT_     (LINKID) PRIMARY_KEY
   STRING_  (LINKNAME, 100)
   STRING_  (LINKURL, 1200)
   STRING   (NOTE, 1000)
   INT      (TASKID)
END_TABLE

TABLE_(ANALYSISBYDATE)
   INT_     (ANALYSISID) PRIMARY_KEY
   STRING_  (ANALYSISNAME, 200)
   INT64_   (TOTALCOUNT)
   DATE_    (BYDATE)
   INT      (ENVID)
   INT      (TASKID)
   INT64_   (NOPROBCOUNT)
   INT64_   (UNRESOLVEDCOUNT)
   INT64_   (RESOLVABLECOUNT)
   INT64_   (UNRESOLVABLECOUNT)
   STRING   (NOTE, 1000)
END_TABLE

TABLE_(ANALYSISMETRIC)
   INT_     (ANALMETID) PRIMARY_KEY
   INT      (ANALYSISID)
   INT64_   (METRICVALUE)
   STRING_  (METRICDESC, 200)
   STRING   (NOTE, 1000)
END_TABLE

TABLE_(ASSIGNMENTS)
   INT_     (ASSIGNMENTID) PRIMARY_KEY
   INT_     (ASSIGNEDTOID)
   INT_     (REQUESTEDBYID)
   INT_     (ASSIGNEDBYID)
   STRING   (NOTE, 1000)
   INT_     (DEPFROMTASKID)
   DATE_    (ASSIGNEDWHEN)
   DATE_    (COMPLETEDWHEN)
   DATE_    (UATRELEASEWHEN)
   DATE_    (QASCHEDULEDWHEN)
   INT_     (ASSIGNEEPROXYID)
   INT_     (BENEFICIARYID)
   DATE_    (UNITTESTCOMPLETEWHEN)
   DATE_    (SAMPLEPROVIDEDWHEN)
   INT_     (FLOORMANAGERID)
   INT_     (BENEFICIARYREPMANAGERID)
   STRING_  (TICKET, 100)
   DATE_    (LASTUPDFROMASSIGNEEWHEN)
END_TABLE

TABLE_(V_CONN)
   INT_     (CONNID)
   STRING_  (CONNNAME, 100)
   INT_     (LOGINID)
   INT      (INSTANCEID)
   STRING   (NOTE, 1000)
   DATE_    (LASTSUCCEEDDATETIME)
   DATE_    (LASTFAILDATETIME)
   STRING_  (LOGINSTR, 40)
   STRING_  (LOGINPWD, 100)
   BOOL_    (LOGIN_ISLEGIT)
   STRING   (INSTANCENAME, 100)
   STRING   (INSTANCEADDRESS, 100)
   INT      (INSTTYPID)
   STRING   (INSTTYPNAME, 40)
   INT      (ENVID)
   STRING_  (ENVSTDNAME, 100)
   STRING_  (DBNAME, 200)
   INT_     (PORTNO)
   BOOL_    (ISOSAUTH)
END_TABLE

TABLE_(TASKMACROS)
   INT_     (TASKMACID) PRIMARY_KEY
   INT      (TASKID)
   STRING_  (SEARCHFOR, 200)
   STRING_  (REPLACEWITH, 200)
   INT      (PROCESSORDER)
   STRING   (NOTE, 1000)
END_TABLE