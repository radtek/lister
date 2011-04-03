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
-- Name: searchsites; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE searchsites (
    searchsiteid integer NOT NULL,
    searchsiteaddress character varying(1024),
    note text
);


ALTER TABLE public.searchsites OWNER TO postgres;

--
-- Name: searchsites_searchsiteid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE searchsites_searchsiteid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.searchsites_searchsiteid_seq OWNER TO postgres;

--
-- Name: searchsites_searchsiteid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE searchsites_searchsiteid_seq OWNED BY searchsites.searchsiteid;


--
-- Name: searchsiteid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE searchsites ALTER COLUMN searchsiteid SET DEFAULT nextval('searchsites_searchsiteid_seq'::regclass);


--
-- Name: pksearchsite; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY searchsites
    ADD CONSTRAINT pksearchsite PRIMARY KEY (searchsiteid);


--
-- PostgreSQL database dump complete
--

