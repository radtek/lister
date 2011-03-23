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
-- Name: tags; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tags (
    tagid integer NOT NULL,
    tagname character varying(200),
    transguideid integer,
    looped boolean,
    required boolean DEFAULT true,
    defaultvalue character varying(200),
    colno integer
);


ALTER TABLE public.tags OWNER TO postgres;

--
-- Name: TABLE tags; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE tags IS 'Just the macro names for a transformation guide';


--
-- Name: COLUMN tags.looped; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tags.looped IS 'Whether we travers through the macros and spit out a new output for each loopno present there, or whether we create an output from a macro only once.';


--
-- Name: COLUMN tags.colno; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN tags.colno IS 'To provide some structure so this can be turned into a grid for the user to enter across on.';


--
-- Name: tags_tagid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tags_tagid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tags_tagid_seq OWNER TO postgres;

--
-- Name: tags_tagid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tags_tagid_seq OWNED BY tags.tagid;


--
-- Name: tagid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tags ALTER COLUMN tagid SET DEFAULT nextval('tags_tagid_seq'::regclass);


--
-- Name: pktag; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tags
    ADD CONSTRAINT pktag PRIMARY KEY (tagid);


--
-- Name: fktransguidetag; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tags
    ADD CONSTRAINT fktransguidetag FOREIGN KEY (transguideid) REFERENCES transguides(transguideid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

