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
-- Name: listertables; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE listertables (
    listertbid integer NOT NULL,
    listertbname character varying(200) NOT NULL,
    physicaltbname character varying(200) NOT NULL,
    shortphysicalname character varying NOT NULL,
    "nameunique?" boolean NOT NULL,
    note text
);


ALTER TABLE public.listertables OWNER TO postgres;

--
-- Name: TABLE listertables; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE listertables IS 'all tables listed here.  These are here so we can automagically generate all the little tables.

So its <Name<s/es> for the table name
pk is always <pk>singular short name
ak is always <ak>singular short name
<shortname<id> for pk field, always serial, and not null
<shortname<name> for name, and flag for must be unique
<note> always nullable
all timestamps with tz?
';


--
-- Name: listertables_listertableid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE listertables_listertableid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.listertables_listertableid_seq OWNER TO postgres;

--
-- Name: listertables_listertableid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE listertables_listertableid_seq OWNED BY listertables.listertbid;


--
-- Name: listertbid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE listertables ALTER COLUMN listertbid SET DEFAULT nextval('listertables_listertableid_seq'::regclass);


--
-- Name: aklistertable; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY listertables
    ADD CONSTRAINT aklistertable UNIQUE (listertbname);


--
-- Name: pklistertable; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY listertables
    ADD CONSTRAINT pklistertable PRIMARY KEY (listertbid);


--
-- PostgreSQL database dump complete
--

