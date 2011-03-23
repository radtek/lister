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
-- Name: statuses; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE statuses (
);


ALTER TABLE public.statuses OWNER TO postgres;

--
-- Name: TABLE statuses; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE statuses IS 'What is the status of a task, or a running script (job), or a script?  Is it runnable?  Running?
Is a task cancelled or active
urgent? behind? stuck waiting for dependency?';


--
-- PostgreSQL database dump complete
--

