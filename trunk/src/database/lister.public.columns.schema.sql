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
-- Name: columns; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE columns (
    colid integer NOT NULL,
    colname character varying(200),
    srccolcode character varying(200),
    srcdatatypcode character varying(100),
    srcdatatypwdth integer,
    srcdataffstartpos integer,
    srcdatafffinpos integer,
    tbid integer,
    physord integer,
    note text,
    srcdatatypdec integer,
    expwrapinquotes boolean,
    bestfitwdth integer
);


ALTER TABLE public.columns OWNER TO postgres;

--
-- Name: TABLE columns; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE columns IS 'or field mappings for flat files (FF)';


--
-- Name: COLUMN columns.bestfitwdth; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN columns.bestfitwdth IS 'Most common say.... 4 sigma width';


--
-- Name: columns_colid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE columns_colid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.columns_colid_seq OWNER TO postgres;

--
-- Name: columns_colid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE columns_colid_seq OWNED BY columns.colid;


--
-- Name: colid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE columns ALTER COLUMN colid SET DEFAULT nextval('columns_colid_seq'::regclass);


--
-- Name: pkcol; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY columns
    ADD CONSTRAINT pkcol PRIMARY KEY (colid);


--
-- Name: fkcoltb; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY columns
    ADD CONSTRAINT fkcoltb FOREIGN KEY (tbid) REFERENCES tables(tbid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

