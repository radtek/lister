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
-- Name: assignments; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE assignments (
    assignmentid integer NOT NULL,
    assignedtoid integer NOT NULL,
    requestedbyid integer,
    assignedbyid integer,
    note text,
    depfromtaskid integer,
    assignedwhen timestamp with time zone DEFAULT now(),
    completedwhen timestamp with time zone,
    uatreleasewhen date,
    qascheduledwhen date,
    assigneeproxyid integer,
    beneficiaryid integer,
    unittestcompletewhen date,
    sampleprovidedwhen date,
    floormanagerid integer,
    beneficiaryrepmanagerid integer,
    ticket character varying(100),
    lastupdfromassigneewhen date,
    assignmentname character varying(200)
);


ALTER TABLE public.assignments OWNER TO postgres;

--
-- Name: TABLE assignments; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE assignments IS 'Like a task, but this is something assigned to a person/group, may have a proxy (like Shaya), with a stream of upstream people to contact, a manager to harass
An assignment date, the request date from Nitin for me to reach out to contact
A log of contacts made: phone/email/phone meeting
Leveraged people, like Bob or Nitin, or the source manager.
Resistance level: They don''t want to do it.
Each response: When, who, who cc''d, level of commitment to fulfillment, request for more information
I fulfilled request for more information
Incorrect assumptions made
I detect an incorrect assumption I made
Request change definition from my side
Partial completion/commitment
Refusal to do part x of request
Redirect to another party
"Not my job" response
No response note with date
Re-request for "Is this your job", acceptance
Implicit acceptance identified
Known that this is recipients duty, no need for explict commitment
Tracking system: Nexus, etc
Assigned party disconnected from point in flow (vacation, new role, left company)
Reassigned worker from x to y (Hi, I''ll be your new contact)
Date commitment for fulfillment, commitment to sample provision
Environment requirements (Must go Dev->QA->UAT->Preprod->Prod
Expectations for involvment asserted by source
Test results of part of returned work: fail, indeterminate, too vague, incorrect solution, source solved incorrect problem
';


--
-- Name: COLUMN assignments.assignedtoid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.assignedtoid IS 'Contact assigned to by some method, hopefully email.';


--
-- Name: COLUMN assignments.requestedbyid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.requestedbyid IS 'Contact (aka Nitin or Rob) who requested that I contact such and such (STRAW) to do something.';


--
-- Name: COLUMN assignments.assignedbyid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.assignedbyid IS 'Probably assigned by me, but this could track heresay';


--
-- Name: COLUMN assignments.depfromtaskid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.depfromtaskid IS 'Task id that this assignment supports.  Assignments are only created from within a task, to simplify tracking.  Plus, an assignment doesn''t support more than one task, to keep tracking simple.';


--
-- Name: COLUMN assignments.assignedwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.assignedwhen IS 'When did I actually send the email that I felt assigned it, clearly.';


--
-- Name: COLUMN assignments.completedwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.completedwhen IS 'If set, then its completed.  Not when tested, though.  I do a unit test, who cares about QA testing.';


--
-- Name: COLUMN assignments.uatreleasewhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.uatreleasewhen IS 'When is it scheduled for release to uat?  This changes at the project level, so I probably shouldn''t put it here.';


--
-- Name: COLUMN assignments.qascheduledwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.qascheduledwhen IS 'When is QA scheduled to look at it?  Are they?';


--
-- Name: COLUMN assignments.assigneeproxyid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.assigneeproxyid IS 'The contact (Shaya) who supposedly interacts and proxies between STRAW and CSDR.';


--
-- Name: COLUMN assignments.beneficiaryid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.beneficiaryid IS 'Gerry Curry is the ultimate user for which an assignment is made and completed';


--
-- Name: COLUMN assignments.unittestcompletewhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.unittestcompletewhen IS 'My unit testing on a sample provided from the source or data in dev/uat';


--
-- Name: COLUMN assignments.sampleprovidedwhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.sampleprovidedwhen IS 'When did we get a sample from source?';


--
-- Name: COLUMN assignments.floormanagerid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.floormanagerid IS 'Rob, a project manager, but really does the day to day.  Nitin is just the high-up interface for the whole team with the Director. Always CCd.';


--
-- Name: COLUMN assignments.beneficiaryrepmanagerid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.beneficiaryrepmanagerid IS 'This would be Mike Sukovitch, who manages the developers and BAs for Gerry Curry.';


--
-- Name: COLUMN assignments.ticket; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.ticket IS 'A code I generate uniquely to track.  Attach to correspondence.';


--
-- Name: COLUMN assignments.lastupdfromassigneewhen; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN assignments.lastupdfromassigneewhen IS 'Last date that the assignee (STRAW) responded with a meaningful status.  Don''t date bogus statuses like "Oh yeah, forgot, I''ll get to it today" means "No change".';


--
-- Name: assignments_assignmentid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE assignments_assignmentid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.assignments_assignmentid_seq OWNER TO postgres;

--
-- Name: assignments_assignmentid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE assignments_assignmentid_seq OWNED BY assignments.assignmentid;


--
-- Name: assignmentid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE assignments ALTER COLUMN assignmentid SET DEFAULT nextval('assignments_assignmentid_seq'::regclass);


--
-- Name: pkassignment; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY assignments
    ADD CONSTRAINT pkassignment PRIMARY KEY (assignmentid);


--
-- PostgreSQL database dump complete
--

