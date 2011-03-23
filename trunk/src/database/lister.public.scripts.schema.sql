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
-- Name: scripts; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE scripts (
    scriptid integer NOT NULL,
    scriptplaintext text NOT NULL,
    scriptname character varying(200),
    scriptrichtext text,
    note text,
    addtimestamp timestamp with time zone DEFAULT now()
);


ALTER TABLE public.scripts OWNER TO postgres;

--
-- Name: TABLE scripts; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE scripts IS 'scripts to execute against connections.';


--
-- Name: COLUMN scripts.scriptplaintext; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN scripts.scriptplaintext IS 'Plain Text version for executing.  No QTF codes embedded, or font information (RichText).';


--
-- Name: COLUMN scripts.scriptname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN scripts.scriptname IS 'I haven''t been populating this, and it may be dropped eventually, but we may want to run scripts by name in the future.';


--
-- Name: COLUMN scripts.scriptrichtext; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN scripts.scriptrichtext IS 'U++ supports a script language called QTF which is a series of embedded codes that bold or underline or create html tables, etc.  Saving the script in this format in parallel allows colorization and such to be saved.';


--
-- Name: scripts_scriptid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE scripts_scriptid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.scripts_scriptid_seq OWNER TO postgres;

--
-- Name: scripts_scriptid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE scripts_scriptid_seq OWNED BY scripts.scriptid;


--
-- Name: scriptid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE scripts ALTER COLUMN scriptid SET DEFAULT nextval('scripts_scriptid_seq'::regclass);


--
-- Name: pkscript; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY scripts
    ADD CONSTRAINT pkscript PRIMARY KEY (scriptid);


--
-- Name: scripts_script_idx; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX scripts_script_idx ON scripts USING btree (scriptplaintext);


--
-- Name: scripts; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE scripts FROM PUBLIC;
REVOKE ALL ON TABLE scripts FROM postgres;
GRANT ALL ON TABLE scripts TO postgres;
GRANT ALL ON TABLE scripts TO PUBLIC;


--
-- PostgreSQL database dump complete
--

