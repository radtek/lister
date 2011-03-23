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
-- Name: servers; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE servers (
    serverid integer NOT NULL,
    serveraddress character varying(40) NOT NULL,
    note text,
    osid integer
);


ALTER TABLE public.servers OWNER TO postgres;

--
-- Name: servers_serverid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE servers_serverid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.servers_serverid_seq OWNER TO postgres;

--
-- Name: servers_serverid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE servers_serverid_seq OWNED BY servers.serverid;


--
-- Name: serverid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE servers ALTER COLUMN serverid SET DEFAULT nextval('servers_serverid_seq'::regclass);


--
-- Name: pkserver; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY servers
    ADD CONSTRAINT pkserver PRIMARY KEY (serverid);


--
-- Name: servers; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE servers FROM PUBLIC;
REVOKE ALL ON TABLE servers FROM postgres;
GRANT ALL ON TABLE servers TO postgres;
GRANT ALL ON TABLE servers TO PUBLIC;


--
-- PostgreSQL database dump complete
--

