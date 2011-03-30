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
-- Name: sources; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE sources (
    sourceid integer NOT NULL,
    sourcename character varying(300) NOT NULL,
    note text,
    briefsummary text,
    history text
);


ALTER TABLE public.sources OWNER TO postgres;

--
-- Name: sources_sourceid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE sources_sourceid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.sources_sourceid_seq OWNER TO postgres;

--
-- Name: sources_sourceid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE sources_sourceid_seq OWNED BY sources.sourceid;


--
-- Name: sourceid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE sources ALTER COLUMN sourceid SET DEFAULT nextval('sources_sourceid_seq'::regclass);


--
-- Name: pksource; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY sources
    ADD CONSTRAINT pksource PRIMARY KEY (sourceid);


--
-- PostgreSQL database dump complete
--

