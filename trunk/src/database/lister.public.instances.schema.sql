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
-- Name: instances; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE instances (
    instanceid integer NOT NULL,
    instancename character varying(100) NOT NULL,
    instanceaddress character varying(100) NOT NULL,
    note text,
    insttypid integer,
    envid integer,
    portno integer
);


ALTER TABLE public.instances OWNER TO postgres;

--
-- Name: TABLE instances; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE instances IS 'On oracle, D426, U426, the SID.
On SQL Server XXXX/DB, port';


--
-- Name: COLUMN instances.instancename; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.instancename IS 'For oracle, can be the SID or global name.';


--
-- Name: COLUMN instances.instanceaddress; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.instanceaddress IS 'computer address either an IP or a DNS resolvable name.  This is what would be TNSPING''d for Oracle.';


--
-- Name: COLUMN instances.insttypid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.insttypid IS 'What some call the database type or vendor, the brand.  SQL Server, Oracle, etc.  NOT the version, as in Oracle7, SQL Server 2000.  This is used in the Connection creation, and generally the same client connection code can be used across versions.  If not, another attribute in the instance row must dictate that specifically for this instance.';


--
-- Name: COLUMN instances.envid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.envid IS 'What business environment does this instance support?  There is -2 for when this is multiple environments, or -1 when not known.  The concept of a "devprod" may be created, since I develop in and use for real productive work connections from the same database (shock!)';


--
-- Name: COLUMN instances.portno; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN instances.portno IS 'For cases when the port is not the default and is necessary for the connection, or for documentation.  If not necessary for connecting, there should be an attribute to say so, because these often are changed by production support, and if forcibly passed when not necessary, you create an unnecessary breakage point in the future.';


--
-- Name: instances_instanceid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE instances_instanceid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.instances_instanceid_seq OWNER TO postgres;

--
-- Name: instances_instanceid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE instances_instanceid_seq OWNED BY instances.instanceid;


--
-- Name: instanceid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE instances ALTER COLUMN instanceid SET DEFAULT nextval('instances_instanceid_seq'::regclass);


--
-- Name: akinstance; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY instances
    ADD CONSTRAINT akinstance UNIQUE (instancename);


--
-- Name: pkinstance; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY instances
    ADD CONSTRAINT pkinstance PRIMARY KEY (instanceid);


--
-- Name: instances; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE instances FROM PUBLIC;
REVOKE ALL ON TABLE instances FROM postgres;
GRANT ALL ON TABLE instances TO postgres;
GRANT ALL ON TABLE instances TO PUBLIC;


--
-- PostgreSQL database dump complete
--

