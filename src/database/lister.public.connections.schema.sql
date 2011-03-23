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
-- Name: connections; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE connections (
    connid integer NOT NULL,
    loginid integer NOT NULL,
    instanceid integer NOT NULL,
    note text,
    lastsucceeddatetime timestamp with time zone,
    lastfaildatetime timestamp with time zone,
    connname character varying(100),
    dbname character varying(200),
    isosauth boolean
);


ALTER TABLE public.connections OWNER TO postgres;

--
-- Name: TABLE connections; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE connections IS 'Join logins to instances.  I can login (using TOAD) to D426 wth several logins, each one goes here.
Some work some don''t anymore.  They each have a last fail and last succeed date.
';


--
-- Name: COLUMN connections.connid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.connid IS 'Note that GetLastInsertedID function in code will try to build pk serial sequence name from table name and this field name, so be careful when changing.';


--
-- Name: COLUMN connections.loginid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.loginid IS 'login string and password pair are stored in separate table.';


--
-- Name: COLUMN connections.instanceid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.instanceid IS 'What instance this connection is to. Multiple instances can sit on the same server.  In DB, this may be the actual database name, as in BACKOFF400, though I believe there is only one per server.';


--
-- Name: COLUMN connections.connname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.connname IS 'Unique name of the connection.  I always lead with the connection environment chain or system name so its easier to read, for example, CSDR-Dev.  Or CSDR-Power, to show a powerful connection with maybe alter and drop table capabilities, not to be used lightly.';


--
-- Name: COLUMN connections.dbname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN connections.dbname IS 'Connections to PostgreSQL provide the database name.  SQL server supports a default database to connect, which will greatly affect SQL that expects this and does not name the database in a 3-part query format.  Tests would break.
Should possibly be linking to the dbs table, but U++ dropgrid object may not support dynamic filtering when used in GridCtrl for an embedded Object Ctrl.  Another words, it would show all dbs across all instances!  Difficult to use.';


--
-- Name: connections_connid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE connections_connid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.connections_connid_seq OWNER TO postgres;

--
-- Name: connections_connid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE connections_connid_seq OWNED BY connections.connid;


--
-- Name: connid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE connections ALTER COLUMN connid SET DEFAULT nextval('connections_connid_seq'::regclass);


--
-- Name: akconn; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY connections
    ADD CONSTRAINT akconn UNIQUE (connname);


--
-- Name: CONSTRAINT akconn ON connections; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON CONSTRAINT akconn ON connections IS 'Names must be unique since that is how the user browses in the ConnGrid.  Though I suppose the other attributes might give a clue.  Linkage and FKs are always on connid.';


--
-- Name: pkconn; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY connections
    ADD CONSTRAINT pkconn PRIMARY KEY (connid);


--
-- Name: CONSTRAINT pkconn ON connections; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON CONSTRAINT pkconn ON connections IS 'Name should be "pkconnections" but I worry about names getting too long.  What is pg max length?';


--
-- Name: connections; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE connections FROM PUBLIC;
REVOKE ALL ON TABLE connections FROM postgres;
GRANT ALL ON TABLE connections TO postgres;
GRANT ALL ON TABLE connections TO PUBLIC;


--
-- PostgreSQL database dump complete
--

