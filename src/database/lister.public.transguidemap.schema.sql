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

--
-- Name: transguidemap; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW transguidemap AS
    SELECT tg.transguideid, tg.transguidename, t.tagid, t.tagname, t.looped, t.colno, m.outputvalue, m.loopno, s.scriptid, s.scriptplaintext, CASE WHEN ((t.looped IS TRUE) AND (m.loopno IS NULL)) THEN 'ERROR'::text WHEN ((t.looped IS FALSE) AND (m.loopno IS NOT NULL)) THEN 'ERROR'::text ELSE 'OK'::text END AS validity FROM (((transguides tg LEFT JOIN tags t ON ((tg.transguideid = t.transguideid))) LEFT JOIN macros m ON ((t.tagid = m.tagid))) LEFT JOIN scripts s ON ((tg.transscriptid = s.scriptid)));


ALTER TABLE public.transguidemap OWNER TO postgres;

--
-- Name: VIEW transguidemap; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON VIEW transguidemap IS 'Display all the properties of a transformation guide as a grid';


--
-- PostgreSQL database dump complete
--

