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
-- Name: projects; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE projects (
    projectname character varying(400),
    projectbriefname character varying(100),
    note text,
    projectid integer NOT NULL,
    masterprojectname character varying(400)
);


ALTER TABLE public.projects OWNER TO postgres;

--
-- Name: TABLE projects; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE projects IS 'Projects, not necessarily a big deal, just a way to group Tasks together for prioritization.';


--
-- Name: projects_projectid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE projects_projectid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.projects_projectid_seq OWNER TO postgres;

--
-- Name: projects_projectid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE projects_projectid_seq OWNED BY projects.projectid;


--
-- Name: projectid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE projects ALTER COLUMN projectid SET DEFAULT nextval('projects_projectid_seq'::regclass);


--
-- Name: pkproj; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY projects
    ADD CONSTRAINT pkproj PRIMARY KEY (projectid);


--
-- PostgreSQL database dump complete
--

