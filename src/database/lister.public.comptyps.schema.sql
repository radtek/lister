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
-- Name: comptypid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE comptyps ALTER COLUMN comptypid SET DEFAULT nextval('comptyp_compid_seq'::regclass);


--
-- Name: pkcomptyp; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY comptyps
    ADD CONSTRAINT pkcomptyp PRIMARY KEY (comptypid);


--
-- Name: comptyps; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE comptyps FROM PUBLIC;
REVOKE ALL ON TABLE comptyps FROM postgres;
GRANT ALL ON TABLE comptyps TO postgres;
GRANT ALL ON TABLE comptyps TO PUBLIC;


--
-- PostgreSQL database dump complete
--

