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
-- Name: oses; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE oses (
    osid integer NOT NULL,
    osname character varying(100),
    note text
);


ALTER TABLE public.oses OWNER TO postgres;

--
-- Name: TABLE oses; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE oses IS 'Operating Systems that servers sit on';


--
-- Name: oses_osid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE oses_osid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.oses_osid_seq OWNER TO postgres;

--
-- Name: oses_osid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE oses_osid_seq OWNED BY oses.osid;


--
-- Name: osid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE oses ALTER COLUMN osid SET DEFAULT nextval('oses_osid_seq'::regclass);


--
-- Name: pkos; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY oses
    ADD CONSTRAINT pkos PRIMARY KEY (osid);


--
-- Name: oses; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE oses FROM PUBLIC;
REVOKE ALL ON TABLE oses FROM postgres;
GRANT ALL ON TABLE oses TO postgres;
GRANT ALL ON TABLE oses TO PUBLIC;


--
-- PostgreSQL database dump complete
--

