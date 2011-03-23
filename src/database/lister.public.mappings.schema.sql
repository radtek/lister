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
-- Name: mappings; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE mappings (
    mapid integer NOT NULL,
    targettbname character varying(100),
    targetcolname character varying(100),
    basortorder integer,
    baversionofrule character varying(1000),
    designfieldid integer,
    ispk boolean,
    nullable boolean,
    datatype character varying(30),
    datawidth integer,
    scale integer,
    defaultvalue character varying(100),
    releasenote text,
    prevreqrule character varying(1000),
    etlversionofrule character varying(1000),
    dbcomment character varying(1000),
    waitingforsource boolean,
    deponmapid integer,
    nullcount integer,
    distinctcount integer,
    iscolinprod boolean,
    iscolinuat boolean,
    iscolindev boolean
);


ALTER TABLE public.mappings OWNER TO postgres;

--
-- Name: TABLE mappings; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE mappings IS 'ETL BA mappings!!';


--
-- Name: COLUMN mappings.baversionofrule; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.baversionofrule IS 'BA''s version of transformation rule; deployed to ETL team.';


--
-- Name: COLUMN mappings.prevreqrule; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.prevreqrule IS 'What did Shaya originally scribble in here?';


--
-- Name: COLUMN mappings.etlversionofrule; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.etlversionofrule IS 'What does Prasad "say" he deployed?';


--
-- Name: COLUMN mappings.dbcomment; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.dbcomment IS 'Comment sucked out of Oracle.';


--
-- Name: COLUMN mappings.waitingforsource; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.waitingforsource IS 'waiting for STRAW to do something.  Should be an Assignment/Task attached, with a date, followup.';


--
-- Name: COLUMN mappings.deponmapid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.deponmapid IS 'Dependent on previous mapping being completed first, such as when the IPS account # is constructed, we then can use it to fetch a DIM KEY.';


--
-- Name: COLUMN mappings.iscolinprod; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN mappings.iscolinprod IS 'Based on csdr[x]_columns extract from "Fetch metadata from Oracle for mappings", set to y if column found in physical database.';


--
-- Name: mappings_mapid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE mappings_mapid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.mappings_mapid_seq OWNER TO postgres;

--
-- Name: mappings_mapid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE mappings_mapid_seq OWNED BY mappings.mapid;


--
-- Name: mapid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE mappings ALTER COLUMN mapid SET DEFAULT nextval('mappings_mapid_seq'::regclass);


--
-- PostgreSQL database dump complete
--

