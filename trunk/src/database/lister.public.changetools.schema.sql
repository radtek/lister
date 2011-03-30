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
-- Name: changetools; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE changetools (
    changetoolid integer NOT NULL,
    changetoolname character varying(200)
);


ALTER TABLE public.changetools OWNER TO postgres;

--
-- Name: changetools_changetoolid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE changetools_changetoolid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.changetools_changetoolid_seq OWNER TO postgres;

--
-- Name: changetools_changetoolid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE changetools_changetoolid_seq OWNED BY changetools.changetoolid;


--
-- Name: changetoolid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE changetools ALTER COLUMN changetoolid SET DEFAULT nextval('changetools_changetoolid_seq'::regclass);


--
-- Name: pkchangetool; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY changetools
    ADD CONSTRAINT pkchangetool PRIMARY KEY (changetoolid);


--
-- PostgreSQL database dump complete
--

