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
-- Name: portable_executables; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE portable_executables (
    libid integer NOT NULL,
    libname character varying(200) NOT NULL,
    libpath character varying(1000),
    filesize integer,
    version character varying(100),
    note text,
    filetimestamp timestamp without time zone
);


ALTER TABLE public.portable_executables OWNER TO postgres;

--
-- Name: TABLE portable_executables; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE portable_executables IS 'dlls, sos so we can track what works.';


--
-- Name: portable_executables_libid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE portable_executables_libid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.portable_executables_libid_seq OWNER TO postgres;

--
-- Name: portable_executables_libid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE portable_executables_libid_seq OWNED BY portable_executables.libid;


--
-- Name: libid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE portable_executables ALTER COLUMN libid SET DEFAULT nextval('portable_executables_libid_seq'::regclass);


--
-- Name: pklib; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY portable_executables
    ADD CONSTRAINT pklib PRIMARY KEY (libid);


--
-- PostgreSQL database dump complete
--

