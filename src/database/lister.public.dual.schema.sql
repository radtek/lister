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
-- Name: dual; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE dual (
    dummy character(1) NOT NULL
);


ALTER TABLE public.dual OWNER TO postgres;

--
-- Name: TABLE dual; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE dual IS 'dummy one-row table for uses.';


--
-- Name: pkdual; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY dual
    ADD CONSTRAINT pkdual PRIMARY KEY (dummy);


--
-- PostgreSQL database dump complete
--

