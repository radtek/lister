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
-- Name: operations; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE operations (
    opid integer NOT NULL,
    opname character varying(400) NOT NULL,
    note text
);


ALTER TABLE public.operations OWNER TO postgres;

--
-- Name: TABLE operations; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE operations IS 'BA actions/ops that help with classifying work or tasks, or singular steps in a task.';


--
-- Name: operations_opid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE operations_opid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.operations_opid_seq OWNER TO postgres;

--
-- Name: operations_opid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE operations_opid_seq OWNED BY operations.opid;


--
-- Name: opid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE operations ALTER COLUMN opid SET DEFAULT nextval('operations_opid_seq'::regclass);


--
-- Name: pkop; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY operations
    ADD CONSTRAINT pkop PRIMARY KEY (opid);


--
-- PostgreSQL database dump complete
--

