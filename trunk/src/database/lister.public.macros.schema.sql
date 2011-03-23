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
-- Name: macros; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE macros (
    macid integer NOT NULL,
    outputvalue character varying(400) NOT NULL,
    tagid integer,
    loopno integer
);


ALTER TABLE public.macros OWNER TO postgres;

--
-- Name: TABLE macros; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE macros IS 'macros that spit out an output value when the tag string (at the end of the tag id) is found in a script, as defined in the transformation guide.  A loop number must be present if the tag is looped, and must not be present if the tag is not looped, but instead shared for the whole transguide run.';


--
-- Name: macros_macid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE macros_macid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.macros_macid_seq OWNER TO postgres;

--
-- Name: macros_macid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE macros_macid_seq OWNED BY macros.macid;


--
-- Name: macid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE macros ALTER COLUMN macid SET DEFAULT nextval('macros_macid_seq'::regclass);


--
-- Name: pkmacro; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY macros
    ADD CONSTRAINT pkmacro PRIMARY KEY (macid);


--
-- Name: fkmacrotag; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY macros
    ADD CONSTRAINT fkmacrotag FOREIGN KEY (tagid) REFERENCES tags(tagid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

