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
-- Name: links; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE links (
    linkid integer NOT NULL,
    linkname character varying(400),
    linkurl character varying(1200),
    note text,
    taskid integer
);


ALTER TABLE public.links OWNER TO postgres;

--
-- Name: TABLE links; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE links IS 'url links to web sites, local files and sharepoint folders attached to a task that support that task.';


--
-- Name: links_linkid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE links_linkid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.links_linkid_seq OWNER TO postgres;

--
-- Name: links_linkid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE links_linkid_seq OWNED BY links.linkid;


--
-- Name: linkid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE links ALTER COLUMN linkid SET DEFAULT nextval('links_linkid_seq'::regclass);


--
-- Name: aklink; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY links
    ADD CONSTRAINT aklink UNIQUE (linkname);


--
-- Name: pklink; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY links
    ADD CONSTRAINT pklink PRIMARY KEY (linkid);


--
-- Name: fktasklink; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY links
    ADD CONSTRAINT fktasklink FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

