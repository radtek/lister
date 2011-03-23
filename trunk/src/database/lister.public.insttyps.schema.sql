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
-- Name: insttyps; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE insttyps (
    insttypid integer NOT NULL,
    insttypname character varying(40) NOT NULL,
    note text
);


ALTER TABLE public.insttyps OWNER TO postgres;

--
-- Name: TABLE insttyps; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE insttyps IS 'instances of server databases, what type they are?
SQL Server?
Not version.';


--
-- Name: insttyps_insttypid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE insttyps_insttypid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.insttyps_insttypid_seq OWNER TO postgres;

--
-- Name: insttyps_insttypid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE insttyps_insttypid_seq OWNED BY insttyps.insttypid;


--
-- Name: insttypid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE insttyps ALTER COLUMN insttypid SET DEFAULT nextval('insttyps_insttypid_seq'::regclass);


--
-- Name: pkinsttyp; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY insttyps
    ADD CONSTRAINT pkinsttyp PRIMARY KEY (insttypid);


--
-- Name: insttyps; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE insttyps FROM PUBLIC;
REVOKE ALL ON TABLE insttyps FROM postgres;
GRANT ALL ON TABLE insttyps TO postgres;
GRANT ALL ON TABLE insttyps TO PUBLIC;


--
-- PostgreSQL database dump complete
--

