--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = public, pg_catalog;

--
-- Name: cpmove_relation(character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION cpmove_relation(param_tblname character varying, param_source_schema character varying, param_dest_schema character varying) RETURNS boolean
    LANGUAGE plpgsql
    AS $$
-- param_tblname is the table name
-- param_source_schema is the source schema
-- param_dest_schema is the destination schema
DECLARE
    new_schema_oid oid;
    old_schema_oid oid;
    tblname_oid oid;
BEGIN
    new_schema_oid := (SELECT oid FROM pg_catalog.pg_namespace
                WHERE nspname = param_dest_schema);
    old_schema_oid := (SELECT oid FROM pg_catalog.pg_namespace
                WHERE nspname = param_source_schema);
                
    tblname_oid := (SELECT oid FROM pg_catalog.pg_class
                WHERE relname = param_tblname AND relnamespace = old_schema_oid);
                
    IF new_schema_oid IS NULL or old_schema_oid IS NULL or tblname_oid IS NULL THEN
        RAISE NOTICE 'schema or table is invalid';
        RETURN false;
    ELSE
        EXECUTE('ALTER TABLE ' || param_source_schema || '.' || param_tblname || ' SET SCHEMA ' || param_dest_schema);
        
    
        /**Correct postgis geometry columns **/
        IF EXISTS(SELECT table_name 
                    FROM information_schema.tables 
                    WHERE table_name = 'geometry_columns' AND table_schema = 'public') THEN
            UPDATE public.geometry_columns SET f_table_schema = param_dest_schema
                WHERE f_table_schema = param_source_schema and f_table_name = param_tblname ;
            END IF;
                                                                                    
        RETURN TRUE;
    END IF;
END
$$;


ALTER FUNCTION public.cpmove_relation(param_tblname character varying, param_source_schema character varying, param_dest_schema character varying) OWNER TO postgres;

--
-- Name: getnewlogin(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION getnewlogin() RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
   floginid INTEGER;
BEGIN
   SELECT loginid INTO floginid FROM logins WHERE loginstr = new.loginstr and loginpwd = new.loginpwd;
   RETURN 0;
END;
$$;


ALTER FUNCTION public.getnewlogin() OWNER TO postgres;

--
-- Name: getnewlogin(integer, text, text); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION getnewlogin(ploginid integer, ploginstr text, ploginpwd text) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
   floginid INTEGER;
BEGIN
   SELECT loginid INTO floginid FROM logins WHERE loginstr = ploginstr and loginpwd = ploginpwd;
   IF FOUND THEN
     RETURN floginid;
   ELSE
     INSERT INTO logins(loginstr, loginpwd) VALUES(ploginstr, ploginpwd) RETURNING loginid INTO floginid;
     RETURN floginid;
   END IF;
     
   RETURN 0;
END;
$$;


ALTER FUNCTION public.getnewlogin(ploginid integer, ploginstr text, ploginpwd text) OWNER TO postgres;

--
-- Name: getnewlogin(integer, text, text, boolean); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION getnewlogin(ploginid integer, ploginstr text, ploginpwd text, pisosauth boolean) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
   floginid INTEGER;
BEGIN
   SELECT loginid INTO floginid FROM logins WHERE loginstr = ploginstr and loginpwd = ploginpwd;
   IF FOUND THEN
     UPDATE logins SET isosauth = pisosauth WHERE loginid = floginid;
     RETURN floginid;
   ELSE
     INSERT INTO logins(loginstr, loginpwd, isosauth) VALUES(ploginstr, ploginpwd, pisosauth) RETURNING loginid INTO floginid;
     RETURN floginid;
   END IF;
     
   RETURN 0;
END;
$$;


ALTER FUNCTION public.getnewlogin(ploginid integer, ploginstr text, ploginpwd text, pisosauth boolean) OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: actionlog; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE actionlog (
    actionid integer NOT NULL,
    note text NOT NULL,
    whenadded timestamp with time zone
);


ALTER TABLE public.actionlog OWNER TO postgres;

--
-- Name: actionlog_actionid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE actionlog_actionid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.actionlog_actionid_seq OWNER TO postgres;

--
-- Name: actionlog_actionid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE actionlog_actionid_seq OWNED BY actionlog.actionid;


--
-- Name: analysisbydate; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE analysisbydate (
    analysisid integer NOT NULL,
    analysisname character varying(200),
    totalcount bigint,
    bydate date,
    envid integer,
    taskid integer,
    noprobcount bigint,
    unresolvedcount bigint,
    resolvablecount bigint,
    unresolvablecount bigint,
    note text,
    scriptid integer
);


ALTER TABLE public.analysisbydate OWNER TO postgres;

--
-- Name: analysisbydate_analysisid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE analysisbydate_analysisid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.analysisbydate_analysisid_seq OWNER TO postgres;

--
-- Name: analysisbydate_analysisid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE analysisbydate_analysisid_seq OWNED BY analysisbydate.analysisid;


--
-- Name: analysismetric; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE analysismetric (
    analmetid integer NOT NULL,
    analysisid integer NOT NULL,
    metricvalue bigint,
    metricdesc character varying(300),
    note text,
    scriptid integer
);


ALTER TABLE public.analysismetric OWNER TO postgres;

--
-- Name: analysismetric_analmetid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE analysismetric_analmetid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.analysismetric_analmetid_seq OWNER TO postgres;

--
-- Name: analysismetric_analmetid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE analysismetric_analmetid_seq OWNED BY analysismetric.analmetid;


--
-- Name: assignments; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE assignments (
    assignmentid integer NOT NULL,
    assignedtoid integer NOT NULL,
    requestedbyid integer,
    assignedbyid integer,
    note text,
    depfromtaskid integer,
    assignedwhen timestamp with time zone DEFAULT now(),
    completedwhen timestamp with time zone,
    uatreleasewhen date,
    qascheduledwhen date,
    assigneeproxyid integer,
    beneficiaryid integer,
    unittestcompletewhen date,
    sampleprovidedwhen date,
    floormanagerid integer,
    beneficiaryrepmanagerid integer,
    ticket character varying(100),
    lastupdfromassigneewhen date,
    assignmentname character varying(200)
);


ALTER TABLE public.assignments OWNER TO postgres;

--
-- Name: TABLE assignments; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE assignments IS 'Like a task, but this is something assigned to a person/group, may have a proxy (like Shaya), with a stream of upstream people to contact, a manager to harass
An assignment date, the request date from Nitin for me to reach out to contact
A log of contacts made: phone/email/phone meeting
Leveraged people, like Bob or Nitin, or the source manager.
Resistance level: They don''t want to do it.
Each response: When, who, who cc''d, level of commitment to fulfillment, request for more information
I fulfilled request for more information
Incorrect assumptions made
I detect an incorrect assumption I made
Request change definition from my side
Partial completion/commitment
Refusal to do part x of request
Redirect to another party
"Not my job" response
No response note with date
Re-request for "Is this your job", acceptance
Implicit acceptance identified
Known that this is recipients duty, no need for explict commitment
Tracking system: Nexus, etc
Assigned party disconnected from point in flow (vacation, new role, left company)
Reassigned worker from x to y (Hi, I''ll be your new contact)
Date commitment for fulfillment, commitment to sample provision
Environment requirements (Must go Dev->QA->UAT->Preprod->Prod
Expectations for involvment asserted by source
Test results of part of returned work: fail, indeterminate, too vague, incorrect solution, source solved incorrect problem
';


--
-- Name: COLUMN assignments.assignedtoid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.assignedtoid IS 'Contact assigned to by some method, hopefully email.';


--
-- Name: COLUMN assignments.requestedbyid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.requestedbyid IS 'Contact (aka Nitin or Rob) who requested that I contact such and such (STRAW) to do something.';


--
-- Name: COLUMN assignments.assignedbyid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.assignedbyid IS 'Probably assigned by me, but this could track heresay';


--
-- Name: COLUMN assignments.depfromtaskid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.depfromtaskid IS 'Task id that this assignment supports.  Assignments are only created from within a task, to simplify tracking.  Plus, an assignment doesn''t support more than one task, to keep tracking simple.';


--
-- Name: COLUMN assignments.assignedwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.assignedwhen IS 'When did I actually send the email that I felt assigned it, clearly.';


--
-- Name: COLUMN assignments.completedwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.completedwhen IS 'If set, then its completed.  Not when tested, though.  I do a unit test, who cares about QA testing.';


--
-- Name: COLUMN assignments.uatreleasewhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.uatreleasewhen IS 'When is it scheduled for release to uat?  This changes at the project level, so I probably shouldn''t put it here.';


--
-- Name: COLUMN assignments.qascheduledwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.qascheduledwhen IS 'When is QA scheduled to look at it?  Are they?';


--
-- Name: COLUMN assignments.assigneeproxyid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.assigneeproxyid IS 'The contact (Shaya) who supposedly interacts and proxies between STRAW and CSDR.';


--
-- Name: COLUMN assignments.beneficiaryid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.beneficiaryid IS 'Gerry Curry is the ultimate user for which an assignment is made and completed';


--
-- Name: COLUMN assignments.unittestcompletewhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.unittestcompletewhen IS 'My unit testing on a sample provided from the source or data in dev/uat';


--
-- Name: COLUMN assignments.sampleprovidedwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.sampleprovidedwhen IS 'When did we get a sample from source?';


--
-- Name: COLUMN assignments.floormanagerid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.floormanagerid IS 'Rob, a project manager, but really does the day to day.  Nitin is just the high-up interface for the whole team with the Director. Always CCd.';


--
-- Name: COLUMN assignments.beneficiaryrepmanagerid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.beneficiaryrepmanagerid IS 'This would be Mike Sukovitch, who manages the developers and BAs for Gerry Curry.';


--
-- Name: COLUMN assignments.ticket; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.ticket IS 'A code I generate uniquely to track.  Attach to correspondence.';


--
-- Name: COLUMN assignments.lastupdfromassigneewhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.lastupdfromassigneewhen IS 'Last date that the assignee (STRAW) responded with a meaningful status.  Don''t date bogus statuses like "Oh yeah, forgot, I''ll get to it today" means "No change".';


--
-- Name: assignments_assignmentid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE assignments_assignmentid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.assignments_assignmentid_seq OWNER TO postgres;

--
-- Name: assignments_assignmentid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE assignments_assignmentid_seq OWNED BY assignments.assignmentid;


--
-- Name: blocks; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE blocks (
    blockid integer NOT NULL,
    blockdesc character varying(400),
    deponwho integer,
    waitstartedwhen date
);


ALTER TABLE public.blocks OWNER TO postgres;

--
-- Name: TABLE blocks; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE blocks IS 'Things blocking action or waiting for a response to a question, connectivity, install, upgrade, migration, ';


--
-- Name: blocks_blockid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE blocks_blockid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.blocks_blockid_seq OWNER TO postgres;

--
-- Name: blocks_blockid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE blocks_blockid_seq OWNED BY blocks.blockid;


--
-- Name: columns; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE columns (
    colid integer NOT NULL,
    colname character varying(200),
    srccolcode character varying(200),
    srcdatatypcode character varying(100),
    srcdatatypwdth integer,
    srcdataffstartpos integer,
    srcdatafffinpos integer,
    tbid integer,
    physord integer,
    note text,
    srcdatatypdec integer,
    expwrapinquotes boolean,
    bestfitwdth integer
);


ALTER TABLE public.columns OWNER TO postgres;

--
-- Name: TABLE columns; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE columns IS 'or field mappings for flat files (FF)';


--
-- Name: COLUMN columns.bestfitwdth; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN columns.bestfitwdth IS 'Most common say.... 4 sigma width';


--
-- Name: columns_colid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE columns_colid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.columns_colid_seq OWNER TO postgres;

--
-- Name: columns_colid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE columns_colid_seq OWNED BY columns.colid;


--
-- Name: comptyps; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE comptyps (
    comptypid integer NOT NULL,
    comptypname character varying(100) NOT NULL,
    note text
);


ALTER TABLE public.comptyps OWNER TO postgres;

--
-- Name: TABLE comptyps; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE comptyps IS 'Comparison Operation type in testing.
Each test has a test type, and each type involves invoking some code, which performs the Comparison Op listed here.

Comp Ops: (Not is a bit flag in the test table)
- Equals
- In List
- Between
- Series (Odds, Evens, Prime?, Evenly divisible by x)
- > than x
- < than x
- >= than x
- <= than x
- between x and y (inclusive)
- starts with
- starts with after trim
- ends with
- ends with after trim
- contains
- regex PCE
- integer
- digit
- letter
- all uppercase
- all lowercase
';


--
-- Name: comptyp_compid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE comptyp_compid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.comptyp_compid_seq OWNER TO postgres;

--
-- Name: comptyp_compid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE comptyp_compid_seq OWNED BY comptyps.comptypid;


--
-- Name: connections; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE connections (
    connid integer NOT NULL,
    loginid integer NOT NULL,
    instanceid integer NOT NULL,
    note text,
    lastsucceeddatetime timestamp with time zone,
    lastfaildatetime timestamp with time zone,
    connname character varying(100),
    dbname character varying(200),
    isosauth boolean
);


ALTER TABLE public.connections OWNER TO postgres;

--
-- Name: TABLE connections; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE connections IS 'Join logins to instances.  I can login (using TOAD) to D426 wth several logins, each one goes here.
Some work some don''t anymore.  They each have a last fail and last succeed date.
';


--
-- Name: COLUMN connections.connid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.connid IS 'Note that GetLastInsertedID function in code will try to build pk serial sequence name from table name and this field name, so be careful when changing.';


--
-- Name: COLUMN connections.loginid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.loginid IS 'login string and password pair are stored in separate table.';


--
-- Name: COLUMN connections.instanceid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.instanceid IS 'What instance this connection is to. Multiple instances can sit on the same server.  In DB, this may be the actual database name, as in BACKOFF400, though I believe there is only one per server.';


--
-- Name: COLUMN connections.connname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.connname IS 'Unique name of the connection.  I always lead with the connection environment chain or system name so its easier to read, for example, CSDR-Dev.  Or CSDR-Power, to show a powerful connection with maybe alter and drop table capabilities, not to be used lightly.';


--
-- Name: COLUMN connections.dbname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.dbname IS 'Connections to PostgreSQL provide the database name.  SQL server supports a default database to connect, which will greatly affect SQL that expects this and does not name the database in a 3-part query format.  Tests would break.
Should possibly be linking to the dbs table, but U++ dropgrid object may not support dynamic filtering when used in GridCtrl for an embedded Object Ctrl.  Another words, it would show all dbs across all instances!  Difficult to use.';


--
-- Name: connections_connid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE connections_connid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.connections_connid_seq OWNER TO postgres;

--
-- Name: connections_connid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE connections_connid_seq OWNED BY connections.connid;


--
-- Name: contactidentifiers; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE contactidentifiers (
    identid integer NOT NULL,
    identifier character varying(200) NOT NULL,
    contactid integer,
    identtypid integer
);


ALTER TABLE public.contactidentifiers OWNER TO postgres;

--
-- Name: TABLE contactidentifiers; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE contactidentifiers IS 'strings that identify a contact: For instance Bill MacSweeny, or William MacSwiney??? No one gets it right, so we put both here. I don''t even care which is right, just need a single key.

Also, sales rep identifiers can link here, logins, etc.  An FTS rapid search resolves to a set of contact ids, which we distinct on, and get hopefully one.';


--
-- Name: contactidentifiers_identid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE contactidentifiers_identid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contactidentifiers_identid_seq OWNER TO postgres;

--
-- Name: contactidentifiers_identid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE contactidentifiers_identid_seq OWNED BY contactidentifiers.identid;


--
-- Name: contacts; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE contacts (
    contactid integer NOT NULL,
    contactname character varying(200) NOT NULL,
    emailaddress character varying(200),
    note text,
    workphoneno character varying(30),
    homephoneno character varying(30),
    cellphoneno character varying(30),
    nonworkemailaddress character varying(100)
);


ALTER TABLE public.contacts OWNER TO postgres;

--
-- Name: TABLE contacts; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE contacts IS 'Includes users with syslogins and externals, like BB support people, their email, etc.';


--
-- Name: contacts_contactid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE contacts_contactid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contacts_contactid_seq OWNER TO postgres;

--
-- Name: contacts_contactid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE contacts_contactid_seq OWNED BY contacts.contactid;


--
-- Name: databases; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE databases (
    dbid integer NOT NULL,
    dbname character varying(100) NOT NULL,
    note text,
    dbaddress character varying(200)
);


ALTER TABLE public.databases OWNER TO postgres;

--
-- Name: TABLE databases; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE databases IS 'Mostly for sql server where perms are per database.  In Oracle, there is no schema level perm, but you can logically access a schema.';


--
-- Name: dbs_dbid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE dbs_dbid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.dbs_dbid_seq OWNER TO postgres;

--
-- Name: dbs_dbid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE dbs_dbid_seq OWNED BY databases.dbid;


--
-- Name: dual; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE dual (
    dummy character(1) NOT NULL
);


ALTER TABLE public.dual OWNER TO postgres;

--
-- Name: TABLE dual; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE dual IS 'dummy one-row table for uses.';


--
-- Name: elements; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE elements (
    elementid integer NOT NULL,
    elementname character varying(100),
    elementdesc text,
    status character varying(100),
    taskid integer,
    note text
);


ALTER TABLE public.elements OWNER TO postgres;

--
-- Name: TABLE elements; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE elements IS 'Point of a task requiring specific examination,
like "Target CUSIP incorrect", "Position 10 pt off"

These can each be either "Resolved", or "Pending Investigation"
or "Unable to correct at this time"';


--
-- Name: COLUMN elements.status; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN elements.status IS 'Resolved, Irrelevant, Rework, Cannot Resolve, Pending Time, Deprioritized';


--
-- Name: environments; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE environments (
    envid integer NOT NULL,
    envstdname character varying(100) NOT NULL,
    note text,
    envlngname character varying(100),
    envshrtname character varying(40),
    envletter character(1)
);


ALTER TABLE public.environments OWNER TO postgres;

--
-- Name: TABLE environments; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE environments IS 'environments that an instance or a database or server supports.

-3 = supports several environments, you need to look at the hosted objects.  Could be a bit map?  But then linking....
';


--
-- Name: COLUMN environments.envstdname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN environments.envstdname IS 'Standard name that people say on a phone, like "Is it in dev?", not is it in Development.';


--
-- Name: environments_envid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE environments_envid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.environments_envid_seq OWNER TO postgres;

--
-- Name: environments_envid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE environments_envid_seq OWNED BY environments.envid;


--
-- Name: facts; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE facts (
    factid integer NOT NULL,
    factname character varying(400) NOT NULL,
    factvalue character varying(400),
    factvalueasint integer,
    note text
);


ALTER TABLE public.facts OWNER TO postgres;

--
-- Name: TABLE facts; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE facts IS 'General information that may be of use';


--
-- Name: facts_factid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE facts_factid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.facts_factid_seq OWNER TO postgres;

--
-- Name: facts_factid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE facts_factid_seq OWNED BY facts.factid;


--
-- Name: instances; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE instances (
    instanceid integer NOT NULL,
    instancename character varying(100) NOT NULL,
    instanceaddress character varying(100) NOT NULL,
    note text,
    insttypid integer,
    envid integer,
    portno integer
);


ALTER TABLE public.instances OWNER TO postgres;

--
-- Name: TABLE instances; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE instances IS 'On oracle, D426, U426, the SID.
On SQL Server XXXX/DB, port';


--
-- Name: COLUMN instances.instancename; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.instancename IS 'For oracle, can be the SID or global name.';


--
-- Name: COLUMN instances.instanceaddress; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.instanceaddress IS 'computer address either an IP or a DNS resolvable name.  This is what would be TNSPING''d for Oracle.';


--
-- Name: COLUMN instances.insttypid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.insttypid IS 'What some call the database type or vendor, the brand.  SQL Server, Oracle, etc.  NOT the version, as in Oracle7, SQL Server 2000.  This is used in the Connection creation, and generally the same client connection code can be used across versions.  If not, another attribute in the instance row must dictate that specifically for this instance.';


--
-- Name: COLUMN instances.envid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.envid IS 'What business environment does this instance support?  There is -2 for when this is multiple environments, or -1 when not known.  The concept of a "devprod" may be created, since I develop in and use for real productive work connections from the same database (shock!)';


--
-- Name: COLUMN instances.portno; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.portno IS 'For cases when the port is not the default and is necessary for the connection, or for documentation.  If not necessary for connecting, there should be an attribute to say so, because these often are changed by production support, and if forcibly passed when not necessary, you create an unnecessary breakage point in the future.';


--
-- Name: instances_instanceid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE instances_instanceid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.instances_instanceid_seq OWNER TO postgres;

--
-- Name: instances_instanceid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE instances_instanceid_seq OWNED BY instances.instanceid;


--
-- Name: insttyps; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE insttyps (
    insttypid integer NOT NULL,
    insttypname character varying(40) NOT NULL,
    note text
);


ALTER TABLE public.insttyps OWNER TO postgres;

--
-- Name: TABLE insttyps; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE insttyps IS 'instances of server databases, what type they are?
SQL Server?
Not version.';


--
-- Name: insttyps_insttypid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE insttyps_insttypid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.insttyps_insttypid_seq OWNER TO postgres;

--
-- Name: insttyps_insttypid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE insttyps_insttypid_seq OWNED BY insttyps.insttypid;


--
-- Name: links; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE links (
    linkid integer NOT NULL,
    linkname character varying(400),
    linkurl character varying(1200),
    note text,
    taskid integer
);


ALTER TABLE public.links OWNER TO postgres;

--
-- Name: TABLE links; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE links IS 'url links to web sites, local files and sharepoint folders attached to a task that support that task.';


--
-- Name: links_linkid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE links_linkid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.links_linkid_seq OWNER TO postgres;

--
-- Name: links_linkid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE links_linkid_seq OWNED BY links.linkid;


--
-- Name: listertables; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE listertables (
    listertbid integer NOT NULL,
    listertbname character varying(200) NOT NULL,
    physicaltbname character varying(200) NOT NULL,
    shortphysicalname character varying NOT NULL,
    "nameunique?" boolean NOT NULL,
    note text
);


ALTER TABLE public.listertables OWNER TO postgres;

--
-- Name: TABLE listertables; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE listertables IS 'all tables listed here.  These are here so we can automagically generate all the little tables.

So its <Name<s/es> for the table name
pk is always <pk>singular short name
ak is always <ak>singular short name
<shortname<id> for pk field, always serial, and not null
<shortname<name> for name, and flag for must be unique
<note> always nullable
all timestamps with tz?
';


--
-- Name: listertables_listertableid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE listertables_listertableid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.listertables_listertableid_seq OWNER TO postgres;

--
-- Name: listertables_listertableid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE listertables_listertableid_seq OWNED BY listertables.listertbid;


--
-- Name: logins; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE logins (
    loginid integer NOT NULL,
    loginstr character varying(40),
    loginpwd character varying(100),
    islegit boolean,
    passwordrulelist character varying(200),
    prevpassword character varying(200),
    lastchangedwhen timestamp with time zone,
    isldap boolean,
    isosauth boolean,
    ispassencrypted boolean
);


ALTER TABLE public.logins OWNER TO postgres;

--
-- Name: TABLE logins; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE logins IS 'just a login paired with a password.';


--
-- Name: COLUMN logins.loginstr; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN logins.loginstr IS 'actual string entered into interface
Can be null (once!) to support the isosauth connection, and so it will have something to link to.';


--
-- Name: COLUMN logins.islegit; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN logins.islegit IS 'gained officially and available for safe, open use.';


--
-- Name: COLUMN logins.isosauth; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN logins.isosauth IS 'Is OS Authorization';


--
-- Name: COLUMN logins.ispassencrypted; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN logins.ispassencrypted IS 'Encrypted strings can be passed for Oracle and Sybase.';


--
-- Name: logins_loginid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE logins_loginid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.logins_loginid_seq OWNER TO postgres;

--
-- Name: logins_loginid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE logins_loginid_seq OWNED BY logins.loginid;


--
-- Name: macros; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE macros (
    macid integer NOT NULL,
    outputvalue character varying(400) NOT NULL,
    tagid integer,
    loopno integer
);


ALTER TABLE public.macros OWNER TO postgres;

--
-- Name: TABLE macros; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE macros IS 'macros that spit out an output value when the tag string (at the end of the tag id) is found in a script, as defined in the transformation guide.  A loop number must be present if the tag is looped, and must not be present if the tag is not looped, but instead shared for the whole transguide run.';


--
-- Name: macros_macid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE macros_macid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.macros_macid_seq OWNER TO postgres;

--
-- Name: macros_macid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE macros_macid_seq OWNED BY macros.macid;


--
-- Name: mappings; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE mappings (
    mapid integer NOT NULL,
    targettbname character varying(100),
    targetcolname character varying(100),
    basortorder integer,
    baversionofrule character varying(1000),
    designfieldid integer,
    ispk boolean,
    nullable boolean,
    datatype character varying(30),
    datawidth integer,
    scale integer,
    defaultvalue character varying(100),
    releasenote text,
    prevreqrule character varying(1000),
    etlversionofrule character varying(1000),
    dbcomment character varying(1000),
    waitingforsource boolean,
    deponmapid integer,
    nullcount integer,
    distinctcount integer,
    iscolinprod boolean,
    iscolinuat boolean,
    iscolindev boolean
);


ALTER TABLE public.mappings OWNER TO postgres;

--
-- Name: TABLE mappings; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE mappings IS 'ETL BA mappings!!';


--
-- Name: COLUMN mappings.baversionofrule; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.baversionofrule IS 'BA''s version of transformation rule; deployed to ETL team.';


--
-- Name: COLUMN mappings.prevreqrule; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.prevreqrule IS 'What did Shaya originally scribble in here?';


--
-- Name: COLUMN mappings.etlversionofrule; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.etlversionofrule IS 'What does Prasad "say" he deployed?';


--
-- Name: COLUMN mappings.dbcomment; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.dbcomment IS 'Comment sucked out of Oracle.';


--
-- Name: COLUMN mappings.waitingforsource; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.waitingforsource IS 'waiting for STRAW to do something.  Should be an Assignment/Task attached, with a date, followup.';


--
-- Name: COLUMN mappings.deponmapid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.deponmapid IS 'Dependent on previous mapping being completed first, such as when the IPS account # is constructed, we then can use it to fetch a DIM KEY.';


--
-- Name: COLUMN mappings.iscolinprod; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.iscolinprod IS 'Based on csdr[x]_columns extract from "Fetch metadata from Oracle for mappings", set to y if column found in physical database.';


--
-- Name: mappings_mapid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE mappings_mapid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.mappings_mapid_seq OWNER TO postgres;

--
-- Name: mappings_mapid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE mappings_mapid_seq OWNED BY mappings.mapid;


--
-- Name: operations; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE operations (
    opid integer NOT NULL,
    opname character varying(400) NOT NULL,
    note text
);


ALTER TABLE public.operations OWNER TO postgres;

--
-- Name: TABLE operations; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE operations IS 'BA actions/ops that help with classifying work or tasks, or singular steps in a task.';


--
-- Name: operations_opid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE operations_opid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.operations_opid_seq OWNER TO postgres;

--
-- Name: operations_opid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE operations_opid_seq OWNED BY operations.opid;


--
-- Name: oses; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE oses (
    osid integer NOT NULL,
    osname character varying(100),
    note text
);


ALTER TABLE public.oses OWNER TO postgres;

--
-- Name: TABLE oses; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE oses IS 'Operating Systems that servers sit on';


--
-- Name: oses_osid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE oses_osid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.oses_osid_seq OWNER TO postgres;

--
-- Name: oses_osid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE oses_osid_seq OWNED BY oses.osid;


--
-- Name: points_pointid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE points_pointid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.points_pointid_seq OWNER TO postgres;

--
-- Name: points_pointid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE points_pointid_seq OWNED BY elements.elementid;


--
-- Name: portable_executables; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE portable_executables (
    libid integer NOT NULL,
    libname character varying(200) NOT NULL,
    libpath character varying(1000),
    filesize integer,
    version character varying(100),
    note text,
    filetimestamp timestamp without time zone
);


ALTER TABLE public.portable_executables OWNER TO postgres;

--
-- Name: TABLE portable_executables; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE portable_executables IS 'dlls, sos so we can track what works.';


--
-- Name: portable_executables_libid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE portable_executables_libid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.portable_executables_libid_seq OWNER TO postgres;

--
-- Name: portable_executables_libid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE portable_executables_libid_seq OWNED BY portable_executables.libid;


--
-- Name: procurements; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE procurements (
    procurid integer NOT NULL,
    procurname character varying(300) NOT NULL,
    note text
);


ALTER TABLE public.procurements OWNER TO postgres;

--
-- Name: TABLE procurements; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE procurements IS 'Procurements';


--
-- Name: procurements_procurid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE procurements_procurid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.procurements_procurid_seq OWNER TO postgres;

--
-- Name: procurements_procurid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE procurements_procurid_seq OWNED BY procurements.procurid;


--
-- Name: projects; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE projects (
    projectname character varying(400),
    projectbriefname character varying(100),
    note text,
    projectid integer NOT NULL,
    masterprojectname character varying(400)
);


ALTER TABLE public.projects OWNER TO postgres;

--
-- Name: TABLE projects; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE projects IS 'Projects, not necessarily a big deal, just a way to group Tasks together for prioritization.';


--
-- Name: projects_projectid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE projects_projectid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.projects_projectid_seq OWNER TO postgres;

--
-- Name: projects_projectid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE projects_projectid_seq OWNED BY projects.projectid;


--
-- Name: questions; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE questions (
    questionid integer NOT NULL,
    questiontext character varying(400) NOT NULL,
    answertext character varying(400),
    note text,
    taskid integer NOT NULL,
    status character varying(100)
);


ALTER TABLE public.questions OWNER TO postgres;

--
-- Name: TABLE questions; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE questions IS 'Questions that arise around the scoping of a task, and the answers found.';


--
-- Name: questions_qid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE questions_qid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.questions_qid_seq OWNER TO postgres;

--
-- Name: questions_qid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE questions_qid_seq OWNED BY questions.questionid;


--
-- Name: relations; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE relations (
    relid integer NOT NULL,
    fromid integer NOT NULL,
    toid integer NOT NULL,
    why character varying(200),
    note text,
    fromtbid integer,
    totbid integer,
    connid integer,
    addtimestamp timestamp with time zone DEFAULT now(),
    scripttarget integer,
    targetname character varying(400),
    flushtarget boolean,
    fastflushtarget boolean,
    rowlimit integer DEFAULT (-1),
    processorder integer,
    outputtomacro character varying(300)
)
WITH (autovacuum_enabled=true);


ALTER TABLE public.relations OWNER TO postgres;

--
-- Name: TABLE relations; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE relations IS 'relation table describing relationship between anything with an id.

This is for from to relations.';


--
-- Name: COLUMN relations.why; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.why IS 'Critical documentation!  Code forces this to be populated at least to 10 characters, which may annoy some users, but without this there is no way to make sense of why a script was attached to a task.  What function does it provide?  Why did you attach it?  What was the output usually?  Is it really slow?';


--
-- Name: COLUMN relations.fromtbid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.fromtbid IS 'Just task ids for now.  A task can have tests and scripts attached.  Later will want to attach contacts and stuff.';


--
-- Name: COLUMN relations.totbid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.totbid IS 'For now, scripts or tests.  Currently scripts are being attached and tests aren''t as critical.  Tests are more for automation and will be cool when I can get to that level.';


--
-- Name: COLUMN relations.connid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.connid IS 'If a single connection supports this task relation (script usually), then we can store it here and connect automagically when user goes to execute the task script.';


--
-- Name: COLUMN relations.addtimestamp; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.addtimestamp IS 'Helpful for sorting (see tasks_r) so that attachments to a task are listed in order of creation, creating a process or flowchart of sorts.';


--
-- Name: COLUMN relations.scripttarget; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.scripttarget IS '0=undef, 1=screen, 2=table, in which case targettable must be defined.  No sense building this stuff into the script object since the actual script doesn''t care where its output and is more reusable without target detail embedded.';


--
-- Name: COLUMN relations.flushtarget; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.flushtarget IS 'A delete of all rows in target first.  In non-SQL sources, this would be a logged action if available.';


--
-- Name: COLUMN relations.fastflushtarget; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.fastflushtarget IS 'for sql targets, use truncate with no logging.  for non-sql, any non-logged delete of rows or clearing of target space.';


--
-- Name: COLUMN relations.rowlimit; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.rowlimit IS '-1 means no limit. 0 means run but don''t output.  Note that truncate may still take place!';


--
-- Name: COLUMN relations.processorder; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.processorder IS 'Put an arbitrary number here to control the order.  Nulls processed last.  Same numbers are unknown order of execution.';


--
-- Name: COLUMN relations.outputtomacro; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.outputtomacro IS 'Empty mean no assignment of 1/1 row/col value to a macro, otherwise a string is created and stuffed on the ContextMacro task script inprocess macros.  It is only active during a batch run, then flushed.';


--
-- Name: relations_relid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE relations_relid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.relations_relid_seq OWNER TO postgres;

--
-- Name: relations_relid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE relations_relid_seq OWNED BY relations.relid;


--
-- Name: roles; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE roles (
    roleid integer NOT NULL,
    rolename character varying(200) NOT NULL,
    note text
);


ALTER TABLE public.roles OWNER TO postgres;

--
-- Name: TABLE roles; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE roles IS 'What role does stakeholder play in this task?';


--
-- Name: roles_roleid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE roles_roleid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.roles_roleid_seq OWNER TO postgres;

--
-- Name: roles_roleid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE roles_roleid_seq OWNED BY roles.roleid;


--
-- Name: scripts; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE scripts (
    scriptid integer NOT NULL,
    scriptplaintext text NOT NULL,
    scriptname character varying(200),
    scriptrichtext text,
    note text,
    addtimestamp timestamp with time zone DEFAULT now()
);


ALTER TABLE public.scripts OWNER TO postgres;

--
-- Name: TABLE scripts; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE scripts IS 'scripts to execute against connections.';


--
-- Name: COLUMN scripts.scriptplaintext; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN scripts.scriptplaintext IS 'Plain Text version for executing.  No QTF codes embedded, or font information (RichText).';


--
-- Name: COLUMN scripts.scriptname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN scripts.scriptname IS 'I haven''t been populating this, and it may be dropped eventually, but we may want to run scripts by name in the future.';


--
-- Name: COLUMN scripts.scriptrichtext; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN scripts.scriptrichtext IS 'U++ supports a script language called QTF which is a series of embedded codes that bold or underline or create html tables, etc.  Saving the script in this format in parallel allows colorization and such to be saved.';


--
-- Name: scripts_scriptid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE scripts_scriptid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.scripts_scriptid_seq OWNER TO postgres;

--
-- Name: scripts_scriptid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE scripts_scriptid_seq OWNED BY scripts.scriptid;


--
-- Name: servers; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE servers (
    serverid integer NOT NULL,
    serveraddress character varying(40) NOT NULL,
    note text,
    osid integer
);


ALTER TABLE public.servers OWNER TO postgres;

--
-- Name: servers_serverid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE servers_serverid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.servers_serverid_seq OWNER TO postgres;

--
-- Name: servers_serverid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE servers_serverid_seq OWNED BY servers.serverid;


--
-- Name: stakeholders; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE stakeholders (
    holderid integer NOT NULL,
    contactid integer NOT NULL,
    taskid integer NOT NULL,
    why character varying(400),
    note text,
    roleid integer
);


ALTER TABLE public.stakeholders OWNER TO postgres;

--
-- Name: TABLE stakeholders; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE stakeholders IS 'People or groups involved';


--
-- Name: stakeholders_holderid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE stakeholders_holderid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.stakeholders_holderid_seq OWNER TO postgres;

--
-- Name: stakeholders_holderid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE stakeholders_holderid_seq OWNED BY stakeholders.holderid;


--
-- Name: statuses; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE statuses (
);


ALTER TABLE public.statuses OWNER TO postgres;

--
-- Name: TABLE statuses; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE statuses IS 'What is the status of a task, or a running script (job), or a script?  Is it runnable?  Running?
Is a task cancelled or active
urgent? behind? stuck waiting for dependency?';


--
-- Name: strawtestin1; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE strawtestin1 (
    trex_rf_id character varying(220),
    tdc_id character varying(100),
    straw_id character varying(220),
    tdor_sys_tk_no character varying(100),
    td_sta_cd character varying(40),
    tdor_sta_cd character varying(40),
    tdor_sys_td_sta_cd character varying(100),
    tdor_sys_rc_sbty_lkup_cd character varying(100),
    tdor_sys_rc_sbty_lkup_dim_ky character varying(22),
    lkup_src_cd character varying(220),
    lkup_nm character varying(1020),
    td_qy character varying(22),
    td_lcl_px character varying(22),
    td_cpt_br_id character varying(40),
    td_ico_td_in character varying(4),
    td_ytm character varying(22),
    td_cpn_rt character varying(22),
    td_vl character varying(22),
    cob_dt timestamp with time zone,
    td_dt timestamp with time zone,
    trex_ts timestamp with time zone,
    td_stl_dt timestamp with time zone,
    td_mat_dt timestamp with time zone,
    td_prpy_ac_in character varying(4),
    td_ac_in character varying(4),
    td_bysl_cd character varying(40),
    td_prm_bk_nm character varying(1020),
    td_cpt_bk_nm character varying(40),
    td_cli_coper_id character varying(22),
    td_cli_pty_dim_ky character varying(22),
    td_cpt_ml_lgey_id character varying(100),
    td_cpt_id character varying(220),
    td_cpt_ac_no character varying(220),
    td_cpt_ac_dim_ky character varying(22),
    td_gci_id character varying(100),
    td_cusip_insm_id character varying(40),
    td_isin_insm_id character varying(100),
    td_insm_dim_ky character varying(22),
    td_insm_hit_atr_vl character varying(100),
    td_sub_insm_hit_atr_vl character varying(1020),
    td_bb_tck_id character varying(220),
    trex_dsk_tx character varying(400),
    td_sc_ty_cd character varying(1020),
    td_srcs_tdr_id character varying(220),
    td_nt_tx character varying(1020),
    tdor_src_nm character varying(220),
    td_fd_src_nm character varying(220)
);


ALTER TABLE public.strawtestin1 OWNER TO postgres;

--
-- Name: tables; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tables (
    tbid integer NOT NULL,
    tbname character varying(100) NOT NULL,
    tbaddress character varying(300),
    note text
);


ALTER TABLE public.tables OWNER TO postgres;

--
-- Name: TABLE tables; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE tables IS 'or views, or mat views, or...';


--
-- Name: tables_tbid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tables_tbid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tables_tbid_seq OWNER TO postgres;

--
-- Name: tables_tbid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tables_tbid_seq OWNED BY tables.tbid;


--
-- Name: tags; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tags (
    tagid integer NOT NULL,
    tagname character varying(200),
    transguideid integer,
    looped boolean,
    required boolean DEFAULT true,
    defaultvalue character varying(200),
    colno integer
);


ALTER TABLE public.tags OWNER TO postgres;

--
-- Name: TABLE tags; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE tags IS 'Just the macro names for a transformation guide';


--
-- Name: COLUMN tags.looped; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tags.looped IS 'Whether we travers through the macros and spit out a new output for each loopno present there, or whether we create an output from a macro only once.';


--
-- Name: COLUMN tags.colno; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tags.colno IS 'To provide some structure so this can be turned into a grid for the user to enter across on.';


--
-- Name: tags_tagid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tags_tagid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tags_tagid_seq OWNER TO postgres;

--
-- Name: tags_tagid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tags_tagid_seq OWNED BY tags.tagid;


--
-- Name: taskmacros; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE taskmacros (
    taskmacid integer NOT NULL,
    taskid integer NOT NULL,
    searchfor character varying(200) NOT NULL,
    replacewith character varying(200) NOT NULL,
    processorder integer NOT NULL,
    note text,
    CONSTRAINT cktaskmacdiff CHECK (((searchfor)::text <> (replacewith)::text))
)
WITH (autovacuum_enabled=true);


ALTER TABLE public.taskmacros OWNER TO postgres;

--
-- Name: TABLE taskmacros; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE taskmacros IS 'Macro strings (very simple) attached to tasks, input string found in script, then converted to outputstring.  outputstring could be another macro, though.';


--
-- Name: COLUMN taskmacros.taskid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacros.taskid IS 'When this task is selected, these macros are loaded into an array that scripts scan.';


--
-- Name: COLUMN taskmacros.searchfor; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacros.searchfor IS 'String to search for, always bracketed thusly: [[STRING]]';


--
-- Name: COLUMN taskmacros.replacewith; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacros.replacewith IS 'String that replaces the found input string in the script being searched.';


--
-- Name: COLUMN taskmacros.processorder; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacros.processorder IS 'Order that a script reads through the macros assigned to a task.  This way the output of one can feed the input of another.';


--
-- Name: taskmacros_taskmacid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE taskmacros_taskmacid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.taskmacros_taskmacid_seq OWNER TO postgres;

--
-- Name: taskmacros_taskmacid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE taskmacros_taskmacid_seq OWNED BY taskmacros.taskmacid;


--
-- Name: tasks; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tasks (
    taskid integer NOT NULL,
    srccode character varying(100),
    note text,
    priorityassigneddate timestamp with time zone,
    projectname character varying(100),
    prioritywithinproject integer,
    taskname character varying(200),
    addtimestamp timestamp with time zone DEFAULT now(),
    taskdesc text,
    solutiondesc text,
    nexttargetdate date,
    targetdateexpectation character varying(200),
    depassignedwhen date,
    depexpectsampwhen date,
    depassigndesc character varying(400),
    depfeedback character varying(1000),
    enduser character varying(100),
    prodlivedate date,
    uatdate date,
    status character varying(200),
    cancelled bit(1),
    deponwho5 integer,
    assignedtowho integer,
    deponwho integer,
    deponwho2 integer,
    deponwho3 integer,
    deponwho4 integer,
    assignedbywho integer,
    cause text,
    isbug boolean,
    hidden boolean,
    processorder integer
);


ALTER TABLE public.tasks OWNER TO postgres;

--
-- Name: TABLE tasks; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE tasks IS 'List of QCs and tasks, linkage to QC system.  Some are not QCs.  Assigned from user or elsewhere, date assigned.';


--
-- Name: COLUMN tasks.srccode; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.srccode IS 'Currently used to hold the Ticket # in the Quality Center (Qual Assurance).  Allows me to keep linked to the web Mercury tool without having to go in there.';


--
-- Name: COLUMN tasks.priorityassigneddate; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.priorityassigneddate IS 'Assigned by user/manager as high priority on this date.  This changes (sarcastic harumph here).';


--
-- Name: COLUMN tasks.projectname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.projectname IS 'Use to group a set of tasks for prioritization. Unlike QCs, which have priorities across all time and space, making them meaningless.';


--
-- Name: COLUMN tasks.prioritywithinproject; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.prioritywithinproject IS 'Within same "projectname", 0 is top priority.';


--
-- Name: COLUMN tasks.targetdateexpectation; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.targetdateexpectation IS 'What is expected by this date?';


--
-- Name: COLUMN tasks.status; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.status IS 'Not a code.  A description of the current status, and when last updated the status.';


--
-- Name: COLUMN tasks.cause; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.cause IS 'Enter causes found for problem describe in the task, if it is a problem or bug.';


--
-- Name: COLUMN tasks.isbug; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.isbug IS 'Useful to discriminate from tasks that are just to do work, run scripts, generate output, maintenance.';


--
-- Name: COLUMN tasks.hidden; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.hidden IS 'Hidden from main task list on main screen.';


--
-- Name: tasks_r; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW tasks_r AS
    SELECT r.relid, t.taskid, t.taskname, t.srccode, t.note AS tasknote, t.priorityassigneddate, t.assignedbywho, t.assignedtowho, t.projectname, t.prioritywithinproject, s.scriptid, s.scriptname, s.scriptplaintext, s.note AS scriptnote, s.scriptrichtext, s.addtimestamp, r.why, r.connid AS relconnid, c.connname AS relconnname, r.scripttarget, r.targetname, r.flushtarget, r.fastflushtarget, r.rowlimit, r.processorder FROM (((tasks t JOIN relations r ON (((t.taskid = r.fromid) AND (r.fromtbid = (SELECT listertables.listertbid FROM listertables WHERE ((listertables.listertbname)::text = 'tasks'::text)))))) LEFT JOIN scripts s ON (((r.toid = s.scriptid) AND (r.totbid = (SELECT listertables.listertbid FROM listertables WHERE ((listertables.listertbname)::text = 'scripts'::text)))))) LEFT JOIN connections c ON ((r.connid = c.connid)));


ALTER TABLE public.tasks_r OWNER TO postgres;

--
-- Name: tasks_taskid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tasks_taskid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tasks_taskid_seq OWNER TO postgres;

--
-- Name: tasks_taskid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tasks_taskid_seq OWNED BY tasks.taskid;


--
-- Name: tests; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tests (
    testid integer NOT NULL,
    testname character varying(200),
    note text,
    relid integer NOT NULL,
    addtimestamp timestamp with time zone DEFAULT now() NOT NULL,
    connid integer,
    desiredoutcome character(1) DEFAULT 'P'::bpchar NOT NULL,
    actualoutcome character(1),
    outputvalue character varying(400),
    x character varying(400),
    y character varying(400),
    invertcomparison boolean DEFAULT false NOT NULL,
    comptypid integer,
    testtypid integer,
    taskid integer,
    processorder integer,
    assigntowho integer,
    firstassignedwhen date,
    firstacceptedwhen date,
    theirticket character varying(20),
    testmacros character varying(1000),
    lastrunwhen timestamp with time zone,
    stopbatchrunonfail boolean
);


ALTER TABLE public.tests OWNER TO postgres;

--
-- Name: TABLE tests; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE tests IS 'Single scripts that output a single (?) value at 0.
The test is either positive or negative.  If 0 rows returned, pass/fail/indeterminate, plus if 1 row, the same
and x or greater rows, pass/fail/indeterminate
if 1 row back, then if = x pass/fail/indeterminate
   if != pass/fail/indeterminate
   if between, not between, starts with, starts with after trim,
  ends with, ends with after trim, contains, not contains,
  not starts with, not ends with,';


--
-- Name: COLUMN tests.relid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.relid IS 'Link to a relation, which currently holds a task and script linked with a connection.  In future, macros as such will be at rel level.
This may help if we want to test something that is not a script, in the future.';


--
-- Name: COLUMN tests.connid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.connid IS 'Links to the connection object that this test is to be run against.  -3 may indicate multi-values?  For now, all tests are a single script against a single connection.';


--
-- Name: COLUMN tests.desiredoutcome; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.desiredoutcome IS 'What we want the outcome of the test to be: Either P)ass or F)ail.';


--
-- Name: COLUMN tests.actualoutcome; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.actualoutcome IS 'What was the actual output of the test?  Did it Pass or Fail or Indeterminate?
Indeterminate (I) means not enough information was gleaned in order to complete the test type and comparison type.  For instance, Is Alphabetic can''t be determined if value is Null.';


--
-- Name: COLUMN tests.outputvalue; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.outputvalue IS 'Value returned, somewhat mangled to fit  into a character column.';


--
-- Name: COLUMN tests.x; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.x IS 'first comparison value if applicable to the comparison test.  A Capitalization test won''t need any values, but a Greater Than test will.';


--
-- Name: COLUMN tests.y; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.y IS 'For between tests';


--
-- Name: COLUMN tests.invertcomparison; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.invertcomparison IS 'If true, make this a NOT test,  Do not use this in leu of setting desired outcome as Fail instead of Pass.
Pass should always be Green/Good.';


--
-- Name: COLUMN tests.comptypid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.comptypid IS 'Comparison Type: equal to, greater than, etc.';


--
-- Name: COLUMN tests.testtypid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.testtypid IS 'Test Type: first returned column, recordcount, time to execute, error code, timeout, etc.';


--
-- Name: COLUMN tests.taskid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.taskid IS 'It was starting to look like tests would be unmanageable, so I added taskid to give some organization to this.  So a set of tests supports a task, and the scripts attached to a task via a rel are available here, but not as individual scripts, since I list the "why" and if a task had the same script assigned twice, how would you know which was assigned to the test?';


--
-- Name: COLUMN tests.processorder; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.processorder IS 'Generic column that is the numeric order of the tests within a task #.  For display and for execution order if run as a batch.';


--
-- Name: COLUMN tests.assigntowho; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.assigntowho IS 'Who or what contact team will we assign this to?';


--
-- Name: COLUMN tests.firstassignedwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.firstassignedwhen IS 'When did we first assign it to the contact?  This includes sending an email to a monitored DG with cc''s to covering contacts, managers, even if no response.';


--
-- Name: COLUMN tests.firstacceptedwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.firstacceptedwhen IS 'Includes any email/voice that says "I''ll get to it" or working on it.';


--
-- Name: COLUMN tests.theirticket; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.theirticket IS 'JIRA ticket or whatever # they will recognize as a tracking number on the target''s side.';


--
-- Name: COLUMN tests.testmacros; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.testmacros IS 'Keypairs by semicolon list.  Parsed in and added to macro context so you can override macros.  Can''t really add new macros probably, since the scripts wouldn''t have been added to script list if they hadn''t resolved all the macros??';


--
-- Name: COLUMN tests.lastrunwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.lastrunwhen IS 'When was it last run?  Means SQL at least compiled, so not last time attempted to run.';


--
-- Name: COLUMN tests.stopbatchrunonfail; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.stopbatchrunonfail IS 'If set, then if this test does not pass in a batch run, then the batch run stops.';


--
-- Name: tests_testid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tests_testid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tests_testid_seq OWNER TO postgres;

--
-- Name: tests_testid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tests_testid_seq OWNED BY tests.testid;


--
-- Name: testtyps; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE testtyps (
    testtypid integer NOT NULL,
    testtypname character varying(100) NOT NULL,
    note text,
    isxreq boolean,
    isyreq boolean,
    isxreqnumeric boolean,
    isyreqnumeric boolean
);


ALTER TABLE public.testtyps OWNER TO postgres;

--
-- Name: TABLE testtyps; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE testtyps IS 'Types of tests, all singular, no combos, please.

Types are:
- returned row count
- error code
- col[0] value
- col[0] data type
- time to execute

DO NOT add types that your not prepared to code for. Each has a segment of code dedicated to processing it.';


--
-- Name: COLUMN testtyps.isxreqnumeric; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN testtyps.isxreqnumeric IS 'x, if present or required, must be numeric, as in, this test is a count or some such.';


--
-- Name: testtyps_testtypid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE testtyps_testtypid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.testtyps_testtypid_seq OWNER TO postgres;

--
-- Name: testtyps_testtypid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE testtyps_testtypid_seq OWNED BY testtyps.testtypid;


--
-- Name: transguides; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE transguides (
    transguideid integer NOT NULL,
    transguidename character varying(200),
    note text,
    transscriptid integer,
    transoutputscriptid integer
);


ALTER TABLE public.transguides OWNER TO postgres;

--
-- Name: TABLE transguides; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE transguides IS 'transformation guide or specification:
takes a list of macros, a 2d grid of inputs mapped to each of the macronames n times, a conversion script (id) that contains and an output script';


--
-- Name: COLUMN transguides.transguidename; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN transguides.transguidename IS 'Actually more meaningful than script names, it describes what the point of this transformation plan or guide is.';


--
-- Name: COLUMN transguides.transscriptid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN transguides.transscriptid IS 'The script with embedded macro names that will transform the attached macros in conjunction with their tag strings into an output transformation and save it.';


--
-- Name: COLUMN transguides.transoutputscriptid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN transguides.transoutputscriptid IS 'The output could be another script or a stream of data.  scripts support both being data or code.';


--
-- Name: transguide_transguideid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE transguide_transguideid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.transguide_transguideid_seq OWNER TO postgres;

--
-- Name: transguide_transguideid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE transguide_transguideid_seq OWNED BY transguides.transguideid;


--
-- Name: transguidemap; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW transguidemap AS
    SELECT tg.transguideid, tg.transguidename, t.tagid, t.tagname, t.looped, t.colno, m.outputvalue, m.loopno, s.scriptid, s.scriptplaintext, CASE WHEN ((t.looped IS TRUE) AND (m.loopno IS NULL)) THEN 'ERROR'::text WHEN ((t.looped IS FALSE) AND (m.loopno IS NOT NULL)) THEN 'ERROR'::text ELSE 'OK'::text END AS validity FROM (((transguides tg LEFT JOIN tags t ON ((tg.transguideid = t.transguideid))) LEFT JOIN macros m ON ((t.tagid = m.tagid))) LEFT JOIN scripts s ON ((tg.transscriptid = s.scriptid)));


ALTER TABLE public.transguidemap OWNER TO postgres;

--
-- Name: VIEW transguidemap; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON VIEW transguidemap IS 'Display all the properties of a transformation guide as a grid';


--
-- Name: userreports; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE userreports (
    urptid integer NOT NULL,
    urptname character varying(200) NOT NULL,
    note text
);


ALTER TABLE public.userreports OWNER TO postgres;

--
-- Name: TABLE userreports; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE userreports IS 'models/surveillances';


--
-- Name: userreports_urptid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE userreports_urptid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.userreports_urptid_seq OWNER TO postgres;

--
-- Name: userreports_urptid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE userreports_urptid_seq OWNED BY userreports.urptid;


--
-- Name: users; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE users (
    userid integer NOT NULL,
    note text,
    usersysid character varying(40) NOT NULL,
    sysdomain character varying(40),
    username character varying(100)
);


ALTER TABLE public.users OWNER TO postgres;

--
-- Name: TABLE users; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE users IS 'unique logins representing a person (contact).  In our company, users CANNOT share logins at this level.  No appids here.';


--
-- Name: users_userid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE users_userid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.users_userid_seq OWNER TO postgres;

--
-- Name: users_userid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE users_userid_seq OWNED BY users.userid;


--
-- Name: v_conn; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW v_conn AS
    SELECT j.connid, j.connname, j.loginid, j.instanceid, j.note, j.lastsucceeddatetime, j.lastfaildatetime, l.loginstr, l.loginpwd, l.islegit AS login_islegit, i.instancename, i.instanceaddress, i.insttypid, it.insttypname, i.envid, e.envstdname, e.envletter, j.dbname, j.isosauth, i.portno FROM ((((connections j LEFT JOIN logins l ON ((j.loginid = l.loginid))) LEFT JOIN instances i ON ((j.instanceid = i.instanceid))) LEFT JOIN insttyps it ON ((i.insttypid = it.insttypid))) LEFT JOIN environments e ON ((i.envid = e.envid)));


ALTER TABLE public.v_conn OWNER TO postgres;

--
-- Name: VIEW v_conn; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON VIEW v_conn IS 'Joins logins and instances together, and a left join to get a protocol.
Always select distinct against vj views, since history is kept in here, for simplicity.
Once a joining table is created, its easier to use it to store history, too, rather than to add a history table to link off of it.
';


--
-- Name: actionid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE actionlog ALTER COLUMN actionid SET DEFAULT nextval('actionlog_actionid_seq'::regclass);


--
-- Name: analysisid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE analysisbydate ALTER COLUMN analysisid SET DEFAULT nextval('analysisbydate_analysisid_seq'::regclass);


--
-- Name: analmetid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE analysismetric ALTER COLUMN analmetid SET DEFAULT nextval('analysismetric_analmetid_seq'::regclass);


--
-- Name: assignmentid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE assignments ALTER COLUMN assignmentid SET DEFAULT nextval('assignments_assignmentid_seq'::regclass);


--
-- Name: blockid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE blocks ALTER COLUMN blockid SET DEFAULT nextval('blocks_blockid_seq'::regclass);


--
-- Name: colid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE columns ALTER COLUMN colid SET DEFAULT nextval('columns_colid_seq'::regclass);


--
-- Name: comptypid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE comptyps ALTER COLUMN comptypid SET DEFAULT nextval('comptyp_compid_seq'::regclass);


--
-- Name: connid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE connections ALTER COLUMN connid SET DEFAULT nextval('connections_connid_seq'::regclass);


--
-- Name: identid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE contactidentifiers ALTER COLUMN identid SET DEFAULT nextval('contactidentifiers_identid_seq'::regclass);


--
-- Name: contactid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE contacts ALTER COLUMN contactid SET DEFAULT nextval('contacts_contactid_seq'::regclass);


--
-- Name: dbid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE databases ALTER COLUMN dbid SET DEFAULT nextval('dbs_dbid_seq'::regclass);


--
-- Name: elementid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE elements ALTER COLUMN elementid SET DEFAULT nextval('points_pointid_seq'::regclass);


--
-- Name: envid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE environments ALTER COLUMN envid SET DEFAULT nextval('environments_envid_seq'::regclass);


--
-- Name: factid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE facts ALTER COLUMN factid SET DEFAULT nextval('facts_factid_seq'::regclass);


--
-- Name: instanceid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE instances ALTER COLUMN instanceid SET DEFAULT nextval('instances_instanceid_seq'::regclass);


--
-- Name: insttypid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE insttyps ALTER COLUMN insttypid SET DEFAULT nextval('insttyps_insttypid_seq'::regclass);


--
-- Name: linkid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE links ALTER COLUMN linkid SET DEFAULT nextval('links_linkid_seq'::regclass);


--
-- Name: listertbid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE listertables ALTER COLUMN listertbid SET DEFAULT nextval('listertables_listertableid_seq'::regclass);


--
-- Name: loginid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE logins ALTER COLUMN loginid SET DEFAULT nextval('logins_loginid_seq'::regclass);


--
-- Name: macid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE macros ALTER COLUMN macid SET DEFAULT nextval('macros_macid_seq'::regclass);


--
-- Name: mapid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE mappings ALTER COLUMN mapid SET DEFAULT nextval('mappings_mapid_seq'::regclass);


--
-- Name: opid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE operations ALTER COLUMN opid SET DEFAULT nextval('operations_opid_seq'::regclass);


--
-- Name: osid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE oses ALTER COLUMN osid SET DEFAULT nextval('oses_osid_seq'::regclass);


--
-- Name: libid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE portable_executables ALTER COLUMN libid SET DEFAULT nextval('portable_executables_libid_seq'::regclass);


--
-- Name: procurid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE procurements ALTER COLUMN procurid SET DEFAULT nextval('procurements_procurid_seq'::regclass);


--
-- Name: projectid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE projects ALTER COLUMN projectid SET DEFAULT nextval('projects_projectid_seq'::regclass);


--
-- Name: questionid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE questions ALTER COLUMN questionid SET DEFAULT nextval('questions_qid_seq'::regclass);


--
-- Name: relid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE relations ALTER COLUMN relid SET DEFAULT nextval('relations_relid_seq'::regclass);


--
-- Name: roleid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE roles ALTER COLUMN roleid SET DEFAULT nextval('roles_roleid_seq'::regclass);


--
-- Name: scriptid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE scripts ALTER COLUMN scriptid SET DEFAULT nextval('scripts_scriptid_seq'::regclass);


--
-- Name: serverid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE servers ALTER COLUMN serverid SET DEFAULT nextval('servers_serverid_seq'::regclass);


--
-- Name: holderid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE stakeholders ALTER COLUMN holderid SET DEFAULT nextval('stakeholders_holderid_seq'::regclass);


--
-- Name: tbid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tables ALTER COLUMN tbid SET DEFAULT nextval('tables_tbid_seq'::regclass);


--
-- Name: tagid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tags ALTER COLUMN tagid SET DEFAULT nextval('tags_tagid_seq'::regclass);


--
-- Name: taskmacid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE taskmacros ALTER COLUMN taskmacid SET DEFAULT nextval('taskmacros_taskmacid_seq'::regclass);


--
-- Name: processorder; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE taskmacros ALTER COLUMN processorder SET DEFAULT currval('taskmacros_taskmacid_seq'::regclass);


--
-- Name: taskid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tasks ALTER COLUMN taskid SET DEFAULT nextval('tasks_taskid_seq'::regclass);


--
-- Name: testid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tests ALTER COLUMN testid SET DEFAULT nextval('tests_testid_seq'::regclass);


--
-- Name: processorder; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tests ALTER COLUMN processorder SET DEFAULT currval('tests_testid_seq'::regclass);


--
-- Name: testtypid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE testtyps ALTER COLUMN testtypid SET DEFAULT nextval('testtyps_testtypid_seq'::regclass);


--
-- Name: transguideid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE transguides ALTER COLUMN transguideid SET DEFAULT nextval('transguide_transguideid_seq'::regclass);


--
-- Name: urptid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE userreports ALTER COLUMN urptid SET DEFAULT nextval('userreports_urptid_seq'::regclass);


--
-- Name: userid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE users ALTER COLUMN userid SET DEFAULT nextval('users_userid_seq'::regclass);


--
-- Name: akconn; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY connections
    ADD CONSTRAINT akconn UNIQUE (connname);


--
-- Name: CONSTRAINT akconn ON connections; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON CONSTRAINT akconn ON connections IS 'Names must be unique since that is how the user browses in the ConnGrid.  Though I suppose the other attributes might give a clue.  Linkage and FKs are always on connid.';


--
-- Name: akfactname; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY facts
    ADD CONSTRAINT akfactname UNIQUE (factname);


--
-- Name: akinstance; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY instances
    ADD CONSTRAINT akinstance UNIQUE (instancename);


--
-- Name: aklink; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY links
    ADD CONSTRAINT aklink UNIQUE (linkname);


--
-- Name: aklistertable; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY listertables
    ADD CONSTRAINT aklistertable UNIQUE (listertbname);


--
-- Name: akpktesttyp; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY testtyps
    ADD CONSTRAINT akpktesttyp UNIQUE (testtypname);


--
-- Name: akprocur; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY procurements
    ADD CONSTRAINT akprocur UNIQUE (procurname);


--
-- Name: akquestion; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY questions
    ADD CONSTRAINT akquestion UNIQUE (questiontext);


--
-- Name: akrolename; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY roles
    ADD CONSTRAINT akrolename UNIQUE (rolename);


--
-- Name: aktransguide; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY transguides
    ADD CONSTRAINT aktransguide UNIQUE (transguidename);


--
-- Name: akurpt; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY userreports
    ADD CONSTRAINT akurpt UNIQUE (urptname);


--
-- Name: environments_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY environments
    ADD CONSTRAINT environments_pkey PRIMARY KEY (envid);


--
-- Name: pkactionlog; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY actionlog
    ADD CONSTRAINT pkactionlog PRIMARY KEY (actionid);


--
-- Name: pkanal; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY analysisbydate
    ADD CONSTRAINT pkanal PRIMARY KEY (analysisid);


--
-- Name: pkanalmet; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY analysismetric
    ADD CONSTRAINT pkanalmet PRIMARY KEY (analmetid);


--
-- Name: pkassignment; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY assignments
    ADD CONSTRAINT pkassignment PRIMARY KEY (assignmentid);


--
-- Name: pkblock; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY blocks
    ADD CONSTRAINT pkblock PRIMARY KEY (blockid);


--
-- Name: pkcol; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY columns
    ADD CONSTRAINT pkcol PRIMARY KEY (colid);


--
-- Name: pkcomptyp; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY comptyps
    ADD CONSTRAINT pkcomptyp PRIMARY KEY (comptypid);


--
-- Name: pkconn; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY connections
    ADD CONSTRAINT pkconn PRIMARY KEY (connid);


--
-- Name: CONSTRAINT pkconn ON connections; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON CONSTRAINT pkconn ON connections IS 'Name should be "pkconnections" but I worry about names getting too long.  What is pg max length?';


--
-- Name: pkcontact; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY contacts
    ADD CONSTRAINT pkcontact PRIMARY KEY (contactid);


--
-- Name: pkcontident; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY contactidentifiers
    ADD CONSTRAINT pkcontident PRIMARY KEY (identid);


--
-- Name: pkdb; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY databases
    ADD CONSTRAINT pkdb PRIMARY KEY (dbid);


--
-- Name: pkdual; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY dual
    ADD CONSTRAINT pkdual PRIMARY KEY (dummy);


--
-- Name: pkelem; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY elements
    ADD CONSTRAINT pkelem PRIMARY KEY (elementid);


--
-- Name: pkfact; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY facts
    ADD CONSTRAINT pkfact PRIMARY KEY (factid);


--
-- Name: pkholder; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY stakeholders
    ADD CONSTRAINT pkholder PRIMARY KEY (holderid);


--
-- Name: pkinstance; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY instances
    ADD CONSTRAINT pkinstance PRIMARY KEY (instanceid);


--
-- Name: pkinsttyp; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY insttyps
    ADD CONSTRAINT pkinsttyp PRIMARY KEY (insttypid);


--
-- Name: pklib; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY portable_executables
    ADD CONSTRAINT pklib PRIMARY KEY (libid);


--
-- Name: pklink; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY links
    ADD CONSTRAINT pklink PRIMARY KEY (linkid);


--
-- Name: pklistertable; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY listertables
    ADD CONSTRAINT pklistertable PRIMARY KEY (listertbid);


--
-- Name: pklogin; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY logins
    ADD CONSTRAINT pklogin PRIMARY KEY (loginid);


--
-- Name: pkmacro; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY macros
    ADD CONSTRAINT pkmacro PRIMARY KEY (macid);


--
-- Name: pkop; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY operations
    ADD CONSTRAINT pkop PRIMARY KEY (opid);


--
-- Name: pkos; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY oses
    ADD CONSTRAINT pkos PRIMARY KEY (osid);


--
-- Name: pkprocur; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY procurements
    ADD CONSTRAINT pkprocur PRIMARY KEY (procurid);


--
-- Name: pkproj; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY projects
    ADD CONSTRAINT pkproj PRIMARY KEY (projectid);


--
-- Name: pkquestion; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY questions
    ADD CONSTRAINT pkquestion PRIMARY KEY (questionid);


--
-- Name: pkrel; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT pkrel PRIMARY KEY (relid);


--
-- Name: pkrole; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY roles
    ADD CONSTRAINT pkrole PRIMARY KEY (roleid);


--
-- Name: pkscript; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY scripts
    ADD CONSTRAINT pkscript PRIMARY KEY (scriptid);


--
-- Name: pkserver; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY servers
    ADD CONSTRAINT pkserver PRIMARY KEY (serverid);


--
-- Name: pktag; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tags
    ADD CONSTRAINT pktag PRIMARY KEY (tagid);


--
-- Name: pktask; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tasks
    ADD CONSTRAINT pktask PRIMARY KEY (taskid);


--
-- Name: pktaskmac; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY taskmacros
    ADD CONSTRAINT pktaskmac PRIMARY KEY (taskmacid);


--
-- Name: pktb; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tables
    ADD CONSTRAINT pktb PRIMARY KEY (tbid);


--
-- Name: pktest; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tests
    ADD CONSTRAINT pktest PRIMARY KEY (testid);


--
-- Name: pktesttyp; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY testtyps
    ADD CONSTRAINT pktesttyp PRIMARY KEY (testtypid);


--
-- Name: pktransguide; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY transguides
    ADD CONSTRAINT pktransguide PRIMARY KEY (transguideid);


--
-- Name: pkurpt; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY userreports
    ADD CONSTRAINT pkurpt PRIMARY KEY (urptid);


--
-- Name: pkuser; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY users
    ADD CONSTRAINT pkuser PRIMARY KEY (userid);


--
-- Name: fki_fkanalmetscript; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkanalmetscript ON analysismetric USING btree (scriptid);


--
-- Name: fki_fkanalscript; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkanalscript ON analysisbydate USING btree (scriptid);


--
-- Name: fki_fkrelfromtb; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkrelfromtb ON relations USING btree (fromid);


--
-- Name: fki_fkrelobtab; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkrelobtab ON relations USING btree (fromtbid);


--
-- Name: fki_fkreltoobtab; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkreltoobtab ON relations USING btree (totbid);


--
-- Name: fki_fkreltotb; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkreltotb ON relations USING btree (toid);


--
-- Name: fki_fktestrel; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fktestrel ON tests USING btree (relid);


--
-- Name: scripts_script_idx; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX scripts_script_idx ON scripts USING btree (scriptplaintext);


--
-- Name: tasks_r_del; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE tasks_r_del AS ON DELETE TO tasks_r DO INSTEAD DELETE FROM relations WHERE (relations.relid = old.relid);


--
-- Name: tasks_r_upd; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE tasks_r_upd AS ON UPDATE TO tasks_r DO INSTEAD (UPDATE relations SET connid = new.relconnid, why = new.why, scripttarget = new.scripttarget, targetname = new.targetname, flushtarget = new.flushtarget, fastflushtarget = CASE new.fastflushtarget WHEN true THEN true WHEN false THEN false ELSE new.fastflushtarget END, rowlimit = new.rowlimit, processorder = new.processorder WHERE (relations.relid = old.relid); UPDATE scripts SET scriptplaintext = new.scriptplaintext, scriptrichtext = new.scriptrichtext, scriptname = new.scriptname, note = new.scriptnote WHERE (scripts.scriptid = old.scriptid); INSERT INTO actionlog (note) VALUES ('Updated tasks relations connection id'::text); );


--
-- Name: v_conn_del; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE v_conn_del AS ON DELETE TO v_conn DO INSTEAD DELETE FROM connections WHERE (connections.connid = old.connid);


--
-- Name: v_conn_ins; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE v_conn_ins AS ON INSERT TO v_conn DO INSTEAD (INSERT INTO connections (connid, connname, loginid, instanceid, lastsucceeddatetime, lastfaildatetime, note, dbname, isosauth) VALUES (DEFAULT, new.connname, getnewlogin(new.loginid, (new.loginstr)::text, (new.loginpwd)::text), new.instanceid, new.lastsucceeddatetime, new.lastfaildatetime, new.note, new.dbname, new.isosauth); UPDATE instances SET portno = new.portno, instancename = new.instancename, instanceaddress = new.instanceaddress, insttypid = new.insttypid, envid = new.envid WHERE (instances.instanceid = new.instanceid); INSERT INTO actionlog (note) VALUES ('Added connection'::text); );


--
-- Name: v_conn_upd; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE v_conn_upd AS ON UPDATE TO v_conn DO INSTEAD (UPDATE connections SET connname = new.connname, loginid = getnewlogin(new.loginid, (new.loginstr)::text, (new.loginpwd)::text), instanceid = new.instanceid, note = new.note, dbname = new.dbname, isosauth = new.isosauth WHERE (connections.connid = old.connid); UPDATE instances SET portno = new.portno, instancename = new.instancename, instanceaddress = new.instanceaddress, insttypid = new.insttypid, envid = new.envid WHERE (instances.instanceid = new.instanceid); INSERT INTO actionlog (note) VALUES ('Updated connection'::text); );


--
-- Name: akholdertask; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY stakeholders
    ADD CONSTRAINT akholdertask FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkanalenv; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysisbydate
    ADD CONSTRAINT fkanalenv FOREIGN KEY (envid) REFERENCES environments(envid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkanalmetanal; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysismetric
    ADD CONSTRAINT fkanalmetanal FOREIGN KEY (analysisid) REFERENCES analysisbydate(analysisid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkanalmetscript; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysismetric
    ADD CONSTRAINT fkanalmetscript FOREIGN KEY (scriptid) REFERENCES scripts(scriptid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkanalscript; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysisbydate
    ADD CONSTRAINT fkanalscript FOREIGN KEY (scriptid) REFERENCES scripts(scriptid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkanaltask; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysisbydate
    ADD CONSTRAINT fkanaltask FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkblockdepcontact; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY blocks
    ADD CONSTRAINT fkblockdepcontact FOREIGN KEY (deponwho) REFERENCES contacts(contactid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkcoltb; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY columns
    ADD CONSTRAINT fkcoltb FOREIGN KEY (tbid) REFERENCES tables(tbid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkcontidentcont; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY contactidentifiers
    ADD CONSTRAINT fkcontidentcont FOREIGN KEY (contactid) REFERENCES contacts(contactid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkholdercontact; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY stakeholders
    ADD CONSTRAINT fkholdercontact FOREIGN KEY (contactid) REFERENCES contacts(contactid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkholderrole; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY stakeholders
    ADD CONSTRAINT fkholderrole FOREIGN KEY (roleid) REFERENCES roles(roleid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkmacrotag; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY macros
    ADD CONSTRAINT fkmacrotag FOREIGN KEY (tagid) REFERENCES tags(tagid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkrelconn; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT fkrelconn FOREIGN KEY (connid) REFERENCES connections(connid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkrelobtab; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT fkrelobtab FOREIGN KEY (fromtbid) REFERENCES listertables(listertbid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkreltoobtab; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT fkreltoobtab FOREIGN KEY (totbid) REFERENCES listertables(listertbid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktasklink; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY links
    ADD CONSTRAINT fktasklink FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktaskmactask; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY taskmacros
    ADD CONSTRAINT fktaskmactask FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktaskpoint; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY elements
    ADD CONSTRAINT fktaskpoint FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktaskquest; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY questions
    ADD CONSTRAINT fktaskquest FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktestassgnto; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tests
    ADD CONSTRAINT fktestassgnto FOREIGN KEY (assigntowho) REFERENCES contacts(contactid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktestcomptyp; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tests
    ADD CONSTRAINT fktestcomptyp FOREIGN KEY (comptypid) REFERENCES comptyps(comptypid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktestconn; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tests
    ADD CONSTRAINT fktestconn FOREIGN KEY (connid) REFERENCES connections(connid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktestrel; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tests
    ADD CONSTRAINT fktestrel FOREIGN KEY (relid) REFERENCES relations(relid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktesttask; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tests
    ADD CONSTRAINT fktesttask FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktesttesttyp; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tests
    ADD CONSTRAINT fktesttesttyp FOREIGN KEY (testtypid) REFERENCES testtyps(testtypid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktransguidetag; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tags
    ADD CONSTRAINT fktransguidetag FOREIGN KEY (transguideid) REFERENCES transguides(transguideid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktransguidtransscript; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY transguides
    ADD CONSTRAINT fktransguidtransscript FOREIGN KEY (transscriptid) REFERENCES scripts(scriptid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- Name: comptyps; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE comptyps FROM PUBLIC;
REVOKE ALL ON TABLE comptyps FROM postgres;
GRANT ALL ON TABLE comptyps TO postgres;
GRANT ALL ON TABLE comptyps TO PUBLIC;


--
-- Name: connections; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE connections FROM PUBLIC;
REVOKE ALL ON TABLE connections FROM postgres;
GRANT ALL ON TABLE connections TO postgres;
GRANT ALL ON TABLE connections TO PUBLIC;


--
-- Name: databases; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE databases FROM PUBLIC;
REVOKE ALL ON TABLE databases FROM postgres;
GRANT ALL ON TABLE databases TO postgres;
GRANT ALL ON TABLE databases TO PUBLIC;


--
-- Name: environments; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE environments FROM PUBLIC;
REVOKE ALL ON TABLE environments FROM postgres;
GRANT ALL ON TABLE environments TO postgres;
GRANT ALL ON TABLE environments TO PUBLIC;


--
-- Name: instances; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE instances FROM PUBLIC;
REVOKE ALL ON TABLE instances FROM postgres;
GRANT ALL ON TABLE instances TO postgres;
GRANT ALL ON TABLE instances TO PUBLIC;


--
-- Name: insttyps; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE insttyps FROM PUBLIC;
REVOKE ALL ON TABLE insttyps FROM postgres;
GRANT ALL ON TABLE insttyps TO postgres;
GRANT ALL ON TABLE insttyps TO PUBLIC;


--
-- Name: logins; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE logins FROM PUBLIC;
REVOKE ALL ON TABLE logins FROM postgres;
GRANT ALL ON TABLE logins TO postgres;
GRANT ALL ON TABLE logins TO PUBLIC;


--
-- Name: oses; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE oses FROM PUBLIC;
REVOKE ALL ON TABLE oses FROM postgres;
GRANT ALL ON TABLE oses TO postgres;
GRANT ALL ON TABLE oses TO PUBLIC;


--
-- Name: scripts; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE scripts FROM PUBLIC;
REVOKE ALL ON TABLE scripts FROM postgres;
GRANT ALL ON TABLE scripts TO postgres;
GRANT ALL ON TABLE scripts TO PUBLIC;


--
-- Name: servers; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE servers FROM PUBLIC;
REVOKE ALL ON TABLE servers FROM postgres;
GRANT ALL ON TABLE servers TO postgres;
GRANT ALL ON TABLE servers TO PUBLIC;


--
-- Name: tables; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE tables FROM PUBLIC;
REVOKE ALL ON TABLE tables FROM postgres;
GRANT ALL ON TABLE tables TO postgres;
GRANT ALL ON TABLE tables TO PUBLIC;


--
-- Name: taskmacros; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE taskmacros FROM PUBLIC;
REVOKE ALL ON TABLE taskmacros FROM postgres;
GRANT ALL ON TABLE taskmacros TO postgres;
GRANT ALL ON TABLE taskmacros TO PUBLIC;


--
-- Name: tasks; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE tasks FROM PUBLIC;
REVOKE ALL ON TABLE tasks FROM postgres;
GRANT ALL ON TABLE tasks TO postgres;
GRANT ALL ON TABLE tasks TO PUBLIC;


--
-- Name: tests; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE tests FROM PUBLIC;
REVOKE ALL ON TABLE tests FROM postgres;
GRANT ALL ON TABLE tests TO postgres;
GRANT ALL ON TABLE tests TO PUBLIC;


--
-- Name: testtyps; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE testtyps FROM PUBLIC;
REVOKE ALL ON TABLE testtyps FROM postgres;
GRANT ALL ON TABLE testtyps TO postgres;
GRANT ALL ON TABLE testtyps TO PUBLIC;


--
-- Name: users; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE users FROM PUBLIC;
REVOKE ALL ON TABLE users FROM postgres;
GRANT ALL ON TABLE users TO postgres;
GRANT ALL ON TABLE users TO PUBLIC;


--
-- PostgreSQL database dump complete
--

