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
-- Name: actionlog; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE actionlog (
    actionid integer NOT NULL,
    note text NOT NULL,
    whenadded timestamp with time zone
);


ALTER TABLE public.actionlog OWNER TO postgres;

--
-- Name: actionlog_actionid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE actionlog_actionid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.actionlog_actionid_seq OWNER TO postgres;

--
-- Name: actionlog_actionid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE actionlog_actionid_seq OWNED BY actionlog.actionid;


--
-- Name: actionid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE actionlog ALTER COLUMN actionid SET DEFAULT nextval('actionlog_actionid_seq'::regclass);


--
-- Name: pkactionlog; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY actionlog
    ADD CONSTRAINT pkactionlog PRIMARY KEY (actionid);


--
-- PostgreSQL database dump complete
--

