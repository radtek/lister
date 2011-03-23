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
-- Name: tasks; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tasks (
    taskid integer NOT NULL,
    srccode character varying(100),
    note text,
    priorityassigneddate timestamp with time zone,
    projectname character varying(100),
    prioritywithinproject integer,
    taskname character varying(200),
    addtimestamp timestamp with time zone DEFAULT now(),
    taskdesc text,
    solutiondesc text,
    nexttargetdate date,
    targetdateexpectation character varying(200),
    depassignedwhen date,
    depexpectsampwhen date,
    depassigndesc character varying(400),
    depfeedback character varying(1000),
    enduser character varying(100),
    prodlivedate date,
    uatdate date,
    status character varying(200),
    cancelled bit(1),
    deponwho5 integer,
    assignedtowho integer,
    deponwho integer,
    deponwho2 integer,
    deponwho3 integer,
    deponwho4 integer,
    assignedbywho integer,
    cause text,
    isbug boolean,
    hidden boolean,
    processorder integer
);


ALTER TABLE public.tasks OWNER TO postgres;

--
-- Name: TABLE tasks; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE tasks IS 'List of QCs and tasks, linkage to QC system.  Some are not QCs.  Assigned from user or elsewhere, date assigned.';


--
-- Name: COLUMN tasks.srccode; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.srccode IS 'Currently used to hold the Ticket # in the Quality Center (Qual Assurance).  Allows me to keep linked to the web Mercury tool without having to go in there.';


--
-- Name: COLUMN tasks.priorityassigneddate; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.priorityassigneddate IS 'Assigned by user/manager as high priority on this date.  This changes (sarcastic harumph here).';


--
-- Name: COLUMN tasks.projectname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.projectname IS 'Use to group a set of tasks for prioritization. Unlike QCs, which have priorities across all time and space, making them meaningless.';


--
-- Name: COLUMN tasks.prioritywithinproject; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.prioritywithinproject IS 'Within same "projectname", 0 is top priority.';


--
-- Name: COLUMN tasks.targetdateexpectation; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.targetdateexpectation IS 'What is expected by this date?';


--
-- Name: COLUMN tasks.status; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.status IS 'Not a code.  A description of the current status, and when last updated the status.';


--
-- Name: COLUMN tasks.cause; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.cause IS 'Enter causes found for problem describe in the task, if it is a problem or bug.';


--
-- Name: COLUMN tasks.isbug; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.isbug IS 'Useful to discriminate from tasks that are just to do work, run scripts, generate output, maintenance.';


--
-- Name: COLUMN tasks.hidden; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tasks.hidden IS 'Hidden from main task list on main screen.';


--
-- Name: tasks_taskid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tasks_taskid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tasks_taskid_seq OWNER TO postgres;

--
-- Name: tasks_taskid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tasks_taskid_seq OWNED BY tasks.taskid;


--
-- Name: taskid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tasks ALTER COLUMN taskid SET DEFAULT nextval('tasks_taskid_seq'::regclass);


--
-- Name: pktask; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tasks
    ADD CONSTRAINT pktask PRIMARY KEY (taskid);


--
-- Name: tasks; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE tasks FROM PUBLIC;
REVOKE ALL ON TABLE tasks FROM postgres;
GRANT ALL ON TABLE tasks TO postgres;
GRANT ALL ON TABLE tasks TO PUBLIC;


--
-- PostgreSQL database dump complete
--

