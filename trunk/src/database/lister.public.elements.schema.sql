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
-- Name: elements; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE elements (
    elementid integer NOT NULL,
    elementname character varying(100),
    elementdesc text,
    status character varying(100),
    taskid integer,
    note text
);


ALTER TABLE public.elements OWNER TO postgres;

--
-- Name: TABLE elements; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE elements IS 'Point of a task requiring specific examination,
like "Target CUSIP incorrect", "Position 10 pt off"

These can each be either "Resolved", or "Pending Investigation"
or "Unable to correct at this time"';


--
-- Name: COLUMN elements.status; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN elements.status IS 'Resolved, Irrelevant, Rework, Cannot Resolve, Pending Time, Deprioritized';


--
-- Name: points_pointid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE points_pointid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.points_pointid_seq OWNER TO postgres;

--
-- Name: points_pointid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE points_pointid_seq OWNED BY elements.elementid;


--
-- Name: elementid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE elements ALTER COLUMN elementid SET DEFAULT nextval('points_pointid_seq'::regclass);


--
-- Name: pkelem; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY elements
    ADD CONSTRAINT pkelem PRIMARY KEY (elementid);


--
-- Name: fktaskpoint; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY elements
    ADD CONSTRAINT fktaskpoint FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

