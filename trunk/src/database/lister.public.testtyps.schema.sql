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
-- Name: testtypid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE testtyps ALTER COLUMN testtypid SET DEFAULT nextval('testtyps_testtypid_seq'::regclass);


--
-- Name: akpktesttyp; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY testtyps
    ADD CONSTRAINT akpktesttyp UNIQUE (testtypname);


--
-- Name: pktesttyp; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY testtyps
    ADD CONSTRAINT pktesttyp PRIMARY KEY (testtypid);


--
-- Name: testtyps; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE testtyps FROM PUBLIC;
REVOKE ALL ON TABLE testtyps FROM postgres;
GRANT ALL ON TABLE testtyps TO postgres;
GRANT ALL ON TABLE testtyps TO PUBLIC;


--
-- PostgreSQL database dump complete
--

