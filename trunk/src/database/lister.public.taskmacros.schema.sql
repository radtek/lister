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
-- Name: taskmacros; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE taskmacros (
    taskmacid integer NOT NULL,
    taskid integer NOT NULL,
    searchfor character varying(200) NOT NULL,
    replacewith character varying(200) NOT NULL,
    processorder integer NOT NULL,
    note text,
    CONSTRAINT cktaskmacdiff CHECK (((searchfor)::text <> (replacewith)::text))
)
WITH (autovacuum_enabled=true);


ALTER TABLE public.taskmacros OWNER TO postgres;

--
-- Name: TABLE taskmacros; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE taskmacros IS 'Macro strings (very simple) attached to tasks, input string found in script, then converted to outputstring.  outputstring could be another macro, though.';


--
-- Name: COLUMN taskmacros.taskid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacros.taskid IS 'When this task is selected, these macros are loaded into an array that scripts scan.  The macros are a heirarchy within the task''s set of macros and do not currently reference other tasks'' macros.';


--
-- Name: COLUMN taskmacros.searchfor; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacros.searchfor IS 'String to search for, always bracketed thusly: [[STRING]].  As the ExpandMacros function progresses through the taskmacros assigned to the current task (in processorder), it searches the previous macros for this string and transforms it recursively, as well as searching some hardcoded patterns like TPLUS{n}.';


--
-- Name: COLUMN taskmacros.replacewith; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacros.replacewith IS 'String that replaces the found input string in the script being searched. This can be another bunch of macros.  I can''t remember if it matters where they are in the order. I think they are mostly referring to previously listed task macros or common macs like TPLUS1, etc.
';


--
-- Name: COLUMN taskmacros.processorder; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacros.processorder IS 'Order that a script reads through the macros assigned to a task.  This way the output of one can feed the input of another.  Generated automatically when a row is inserted from a grid.  Updated when drag/drop reorders a grid.  That op is buggy, though.';


--
-- Name: taskmacros_taskmacid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE taskmacros_taskmacid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.taskmacros_taskmacid_seq OWNER TO postgres;

--
-- Name: taskmacros_taskmacid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE taskmacros_taskmacid_seq OWNED BY taskmacros.taskmacid;


--
-- Name: taskmacid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE taskmacros ALTER COLUMN taskmacid SET DEFAULT nextval('taskmacros_taskmacid_seq'::regclass);


--
-- Name: processorder; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE taskmacros ALTER COLUMN processorder SET DEFAULT currval('taskmacros_taskmacid_seq'::regclass);


--
-- Name: aktaskmac_taskidprocord; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY taskmacros
    ADD CONSTRAINT aktaskmac_taskidprocord UNIQUE (taskid, processorder);


--
-- Name: pktaskmac; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY taskmacros
    ADD CONSTRAINT pktaskmac PRIMARY KEY (taskmacid);


--
-- Name: fktaskmactask; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY taskmacros
    ADD CONSTRAINT fktaskmactask FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: taskmacros; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE taskmacros FROM PUBLIC;
REVOKE ALL ON TABLE taskmacros FROM postgres;
GRANT ALL ON TABLE taskmacros TO postgres;
GRANT ALL ON TABLE taskmacros TO PUBLIC;


--
-- PostgreSQL database dump complete
--

