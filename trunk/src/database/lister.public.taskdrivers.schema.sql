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
-- Name: taskdrivers; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE taskdrivers (
    driverid integer NOT NULL,
    drivername character varying(400) NOT NULL,
    note text,
    processorder integer NOT NULL,
    taskid integer NOT NULL
);


ALTER TABLE public.taskdrivers OWNER TO postgres;

--
-- Name: TABLE taskdrivers; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE taskdrivers IS 'A name and a taskid.  So for a task, there might be:
BBBAUK, BBBATK.  Each task macro must either be associated with the default (null) driver for a task, or one of these, at least macros assigned to that task.

It would be nice to eventually share these drivers for other tasks, perhaps a project level??  Anything with STRAW would have the same set and a lot of the same macros, too.

We''ll get there, but not today.';


--
-- Name: COLUMN taskdrivers.drivername; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskdrivers.drivername IS 'Must be set.  Otherwise we won''t have a header for grid column.  Must be unique per task id.';


--
-- Name: COLUMN taskdrivers.processorder; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskdrivers.processorder IS 'order of column listing from left to right in macro grid.  Should probably process that way too if you could run ALL macros x drivers in a batch (cool)';


--
-- Name: COLUMN taskdrivers.taskid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskdrivers.taskid IS 'task that owns these drivers.  Only accessible from task editor.  Must be populated.  A driver without a task makes no sense.';


--
-- Name: macrodrivers_driverid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE macrodrivers_driverid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.macrodrivers_driverid_seq OWNER TO postgres;

--
-- Name: macrodrivers_driverid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE macrodrivers_driverid_seq OWNED BY taskdrivers.driverid;


--
-- Name: driverid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE taskdrivers ALTER COLUMN driverid SET DEFAULT nextval('macrodrivers_driverid_seq'::regclass);


--
-- Name: aktaskdriver_taskdrvnm; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY taskdrivers
    ADD CONSTRAINT aktaskdriver_taskdrvnm UNIQUE (taskid, drivername);


--
-- Name: aktaskdriver_taskprocord; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY taskdrivers
    ADD CONSTRAINT aktaskdriver_taskprocord UNIQUE (taskid, processorder);


--
-- Name: pkmacrodriver; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY taskdrivers
    ADD CONSTRAINT pkmacrodriver PRIMARY KEY (driverid);


--
-- Name: fktaskdrivertask; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY taskdrivers
    ADD CONSTRAINT fktaskdrivertask FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

