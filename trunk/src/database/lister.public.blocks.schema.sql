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
-- Name: blocks; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE blocks (
    blockid integer NOT NULL,
    blockdesc character varying(400),
    deponwho integer,
    waitstartedwhen date
);


ALTER TABLE public.blocks OWNER TO postgres;

--
-- Name: TABLE blocks; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE blocks IS 'Things blocking action or waiting for a response to a question, connectivity, install, upgrade, migration, ';


--
-- Name: blocks_blockid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE blocks_blockid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.blocks_blockid_seq OWNER TO postgres;

--
-- Name: blocks_blockid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE blocks_blockid_seq OWNED BY blocks.blockid;


--
-- Name: blockid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE blocks ALTER COLUMN blockid SET DEFAULT nextval('blocks_blockid_seq'::regclass);


--
-- Name: pkblock; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY blocks
    ADD CONSTRAINT pkblock PRIMARY KEY (blockid);


--
-- Name: fkblockdepcontact; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY blocks
    ADD CONSTRAINT fkblockdepcontact FOREIGN KEY (deponwho) REFERENCES contacts(contactid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

