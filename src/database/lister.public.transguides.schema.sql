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
-- Name: transguides; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE transguides (
    transguideid integer NOT NULL,
    transguidename character varying(200),
    note text,
    transscriptid integer,
    transoutputscriptid integer
);


ALTER TABLE public.transguides OWNER TO postgres;

--
-- Name: TABLE transguides; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE transguides IS 'transformation guide or specification:
takes a list of macros, a 2d grid of inputs mapped to each of the macronames n times, a conversion script (id) that contains and an output script';


--
-- Name: COLUMN transguides.transguidename; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN transguides.transguidename IS 'Actually more meaningful than script names, it describes what the point of this transformation plan or guide is.';


--
-- Name: COLUMN transguides.transscriptid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN transguides.transscriptid IS 'The script with embedded macro names that will transform the attached macros in conjunction with their tag strings into an output transformation and save it.';


--
-- Name: COLUMN transguides.transoutputscriptid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN transguides.transoutputscriptid IS 'The output could be another script or a stream of data.  scripts support both being data or code.';


--
-- Name: transguide_transguideid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE transguide_transguideid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.transguide_transguideid_seq OWNER TO postgres;

--
-- Name: transguide_transguideid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE transguide_transguideid_seq OWNED BY transguides.transguideid;


--
-- Name: transguideid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE transguides ALTER COLUMN transguideid SET DEFAULT nextval('transguide_transguideid_seq'::regclass);


--
-- Name: aktransguide; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY transguides
    ADD CONSTRAINT aktransguide UNIQUE (transguidename);


--
-- Name: pktransguide; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY transguides
    ADD CONSTRAINT pktransguide PRIMARY KEY (transguideid);


--
-- Name: fktransguidtransscript; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY transguides
    ADD CONSTRAINT fktransguidtransscript FOREIGN KEY (transscriptid) REFERENCES scripts(scriptid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

