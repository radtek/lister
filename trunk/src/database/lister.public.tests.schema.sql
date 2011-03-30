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

SET default_tablespace = '';

SET default_with_oids = false;

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
    stopbatchrunonfail boolean,
    taskdriverid integer
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

COMMENT ON COLUMN tests.outputvalue IS 'Value returned, somewhat mangled to fit  into a character column.
IMPLEMENTATION: Implemented.  R/O from test grid.  Assigned when test is run.';


--
-- Name: COLUMN tests.x; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.x IS 'first comparison value if applicable to the comparison test.  A Capitalization test won''t need any values, but a Greater Than test will.';


--
-- Name: COLUMN tests.y; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.y IS 'For between tests, like "if value between x and y".
IMPLEMENTATION: Not implemented.  Saveable from Test Grid, though.';


--
-- Name: COLUMN tests.invertcomparison; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.invertcomparison IS 'If true, make this a NOT test,  Do not use this in leu of setting desired outcome as Fail instead of Pass.
Pass should always be Green/Good.
IMPLEMENTATION: Settable from Test Grid, checked when test is run.';


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

COMMENT ON COLUMN tests.processorder IS 'Generic column that is the numeric order of the tests within a task #.  For display and for execution order if run as a batch.
IMPLEMENTATION: Complete.';


--
-- Name: COLUMN tests.assigntowho; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.assigntowho IS 'Who or what contact team will we assign this to?
IMPLEMENTATION: Added to test suite screen.  Can assign and sort by.';


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

COMMENT ON COLUMN tests.theirticket IS 'JIRA ticket or whatever # they will recognize as a tracking number on the target''s side.
IMPLEMENTATION: Not added to test suite screen yet.';


--
-- Name: COLUMN tests.testmacros; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.testmacros IS 'Keypairs by semicolon list.  Parsed in and added to macro context so you can override macros.  Can''t really add new macros probably, since the scripts wouldn''t have been added to script list if they hadn''t resolved all the macros??
IMPLEMENTATION: Not implemented yet.  Too much other stuff.';


--
-- Name: COLUMN tests.lastrunwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.lastrunwhen IS 'When was it last run?  Means SQL at least compiled, so not last time attempted to run.
IMPLEMENTATION: Assigned when run complete.';


--
-- Name: COLUMN tests.stopbatchrunonfail; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.stopbatchrunonfail IS 'If set, then if this test does not pass in a batch run, then the batch run stops.
IMPLEMENTATION: Settible from checkbox in test grid.  Complete.  Code checks when run in batch mode and stops if failure.';


--
-- Name: COLUMN tests.taskdriverid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tests.taskdriverid IS 'This is the driver (Path) that this test was run for.  R/O from the test grid.  Only set when the active driver for the test suite is selected at the task editor level and then the tests are run.';


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
-- Name: testid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tests ALTER COLUMN testid SET DEFAULT nextval('tests_testid_seq'::regclass);


--
-- Name: processorder; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tests ALTER COLUMN processorder SET DEFAULT currval('tests_testid_seq'::regclass);


--
-- Name: pktest; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tests
    ADD CONSTRAINT pktest PRIMARY KEY (testid);


--
-- Name: fki_fktestrel; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fktestrel ON tests USING btree (relid);


--
-- Name: fktest_taskdriver; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tests
    ADD CONSTRAINT fktest_taskdriver FOREIGN KEY (taskdriverid) REFERENCES taskdrivers(driverid) ON UPDATE RESTRICT ON DELETE RESTRICT;


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
-- Name: tests; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE tests FROM PUBLIC;
REVOKE ALL ON TABLE tests FROM postgres;
GRANT ALL ON TABLE tests TO postgres;
GRANT ALL ON TABLE tests TO PUBLIC;


--
-- PostgreSQL database dump complete
--

