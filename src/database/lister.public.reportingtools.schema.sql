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
-- Name: reportingtools; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE reportingtools (
    reportingtoolid integer NOT NULL,
    reportingtoolname character varying(200)
);


ALTER TABLE public.reportingtools OWNER TO postgres;

--
-- Name: TABLE reportingtools; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE reportingtools IS 'Found a cool list of reporting vendors, and added it.';


--
-- Name: reportingtools_reportingtoolid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE reportingtools_reportingtoolid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.reportingtools_reportingtoolid_seq OWNER TO postgres;

--
-- Name: reportingtools_reportingtoolid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE reportingtools_reportingtoolid_seq OWNED BY reportingtools.reportingtoolid;


--
-- Name: reportingtoolid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE reportingtools ALTER COLUMN reportingtoolid SET DEFAULT nextval('reportingtools_reportingtoolid_seq'::regclass);


--
-- Name: pkreptool; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY reportingtools
    ADD CONSTRAINT pkreptool PRIMARY KEY (reportingtoolid);


--
-- PostgreSQL database dump complete
--

