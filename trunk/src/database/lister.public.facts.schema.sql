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
-- Name: factid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE facts ALTER COLUMN factid SET DEFAULT nextval('facts_factid_seq'::regclass);


--
-- Name: akfactname; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY facts
    ADD CONSTRAINT akfactname UNIQUE (factname);


--
-- Name: pkfact; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY facts
    ADD CONSTRAINT pkfact PRIMARY KEY (factid);


--
-- PostgreSQL database dump complete
--

