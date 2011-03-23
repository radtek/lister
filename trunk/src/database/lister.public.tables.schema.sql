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
-- Name: tbid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tables ALTER COLUMN tbid SET DEFAULT nextval('tables_tbid_seq'::regclass);


--
-- Name: pktb; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tables
    ADD CONSTRAINT pktb PRIMARY KEY (tbid);


--
-- Name: tables; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE tables FROM PUBLIC;
REVOKE ALL ON TABLE tables FROM postgres;
GRANT ALL ON TABLE tables TO postgres;
GRANT ALL ON TABLE tables TO PUBLIC;


--
-- PostgreSQL database dump complete
--

