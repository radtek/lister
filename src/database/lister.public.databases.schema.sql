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
-- Name: databases; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE databases (
    dbid integer NOT NULL,
    dbname character varying(100) NOT NULL,
    note text,
    dbaddress character varying(200)
);


ALTER TABLE public.databases OWNER TO postgres;

--
-- Name: TABLE databases; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE databases IS 'Mostly for sql server where perms are per database.  In Oracle, there is no schema level perm, but you can logically access a schema.';


--
-- Name: dbs_dbid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE dbs_dbid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.dbs_dbid_seq OWNER TO postgres;

--
-- Name: dbs_dbid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE dbs_dbid_seq OWNED BY databases.dbid;


--
-- Name: dbid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE databases ALTER COLUMN dbid SET DEFAULT nextval('dbs_dbid_seq'::regclass);


--
-- Name: pkdb; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY databases
    ADD CONSTRAINT pkdb PRIMARY KEY (dbid);


--
-- Name: databases; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE databases FROM PUBLIC;
REVOKE ALL ON TABLE databases FROM postgres;
GRANT ALL ON TABLE databases TO postgres;
GRANT ALL ON TABLE databases TO PUBLIC;


--
-- PostgreSQL database dump complete
--

