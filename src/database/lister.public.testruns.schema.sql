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
-- Name: testruns; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE testruns (
    testrunid integer NOT NULL,
    testid integer NOT NULL,
    taskdriverid integer
);


ALTER TABLE public.testruns OWNER TO postgres;

--
-- Name: TABLE testruns; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE testruns IS 'Run history of tests, including which task driver it was run for.  The task driver # in the test is what the NEXT run will use, not necessarily what the history shows. (Confusing!)';


--
-- Name: COLUMN testruns.testid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN testruns.testid IS 'The test this run was for.';


--
-- Name: COLUMN testruns.taskdriverid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN testruns.taskdriverid IS 'Null if just using default taskmacro values for scripts, but if set, the taskmacrodriverreplacement value is pivoted in if present for the driver selected.  This allows tests to run over different versions of the task macros, and really making tests more useful.';


--
-- Name: testruns_testrunid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE testruns_testrunid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.testruns_testrunid_seq OWNER TO postgres;

--
-- Name: testruns_testrunid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE testruns_testrunid_seq OWNED BY testruns.testrunid;


--
-- Name: testrunid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE testruns ALTER COLUMN testrunid SET DEFAULT nextval('testruns_testrunid_seq'::regclass);


--
-- Name: pktestrun; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY testruns
    ADD CONSTRAINT pktestrun PRIMARY KEY (testrunid);


--
-- Name: fktestrun_taskdriver; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY testruns
    ADD CONSTRAINT fktestrun_taskdriver FOREIGN KEY (taskdriverid) REFERENCES taskdrivers(driverid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktestrun_test; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY testruns
    ADD CONSTRAINT fktestrun_test FOREIGN KEY (testid) REFERENCES tests(testid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

