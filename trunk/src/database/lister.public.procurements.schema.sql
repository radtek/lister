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
-- Name: procurid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE procurements ALTER COLUMN procurid SET DEFAULT nextval('procurements_procurid_seq'::regclass);


--
-- Name: akprocur; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY procurements
    ADD CONSTRAINT akprocur UNIQUE (procurname);


--
-- Name: pkprocur; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY procurements
    ADD CONSTRAINT pkprocur PRIMARY KEY (procurid);


--
-- PostgreSQL database dump complete
--

