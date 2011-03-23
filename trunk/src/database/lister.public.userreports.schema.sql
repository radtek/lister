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
-- Name: urptid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE userreports ALTER COLUMN urptid SET DEFAULT nextval('userreports_urptid_seq'::regclass);


--
-- Name: akurpt; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY userreports
    ADD CONSTRAINT akurpt UNIQUE (urptname);


--
-- Name: pkurpt; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY userreports
    ADD CONSTRAINT pkurpt PRIMARY KEY (urptid);


--
-- PostgreSQL database dump complete
--

