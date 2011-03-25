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
-- Name: taskmacrodriverreplacement; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE taskmacrodriverreplacement (
    tskmacdrvrepid integer NOT NULL,
    taskmacroid integer NOT NULL,
    replacewith character varying(400),
    note text,
    taskdriverid integer NOT NULL
);


ALTER TABLE public.taskmacrodriverreplacement OWNER TO postgres;

--
-- Name: TABLE taskmacrodriverreplacement; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE taskmacrodriverreplacement IS 'Replace task macro''s search value with this, for a specific task driver.  So a task with 3 macros and 2 drivers would have one set of macro definitions in taskmacros, and 2x3 rows in here to provide values.  If no row is found here, the value from taskmacros is used.  This saves work where a macro like TESTDATE is just repeated out.  Should be a check box in taskmacro saying its mandatory to have a value here.  Hmm.';


--
-- Name: COLUMN taskmacrodriverreplacement.taskmacroid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacrodriverreplacement.taskmacroid IS 'The task macro that this "replacewith" is related to.  If it is not the same taskid as the others, then it won''t show up on the same task editor screen.  Bizarre.';


--
-- Name: COLUMN taskmacrodriverreplacement.replacewith; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacrodriverreplacement.replacewith IS 'Value for this driver.  Either replaces the default replacewith in taskmacros or extends by looking for "{default}xxx" or somesuch.
One issue is: Does NULL mean, replace the searchfor value with empty, or use the default replacewith from taskmacro?
I guess if someone bothered to insert a row, they wanted to use zap out the default replacewith value for this driver.';


--
-- Name: COLUMN taskmacrodriverreplacement.taskdriverid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN taskmacrodriverreplacement.taskdriverid IS 'Which driver this supports.';


--
-- Name: taskmacrodriverreplacement_tskmacdrvrepid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE taskmacrodriverreplacement_tskmacdrvrepid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.taskmacrodriverreplacement_tskmacdrvrepid_seq OWNER TO postgres;

--
-- Name: taskmacrodriverreplacement_tskmacdrvrepid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE taskmacrodriverreplacement_tskmacdrvrepid_seq OWNED BY taskmacrodriverreplacement.tskmacdrvrepid;


--
-- Name: tskmacdrvrepid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE taskmacrodriverreplacement ALTER COLUMN tskmacdrvrepid SET DEFAULT nextval('taskmacrodriverreplacement_tskmacdrvrepid_seq'::regclass);


--
-- Name: aktskmacdrvrep_tskmacidtskdrvid; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY taskmacrodriverreplacement
    ADD CONSTRAINT aktskmacdrvrep_tskmacidtskdrvid UNIQUE (taskmacroid, taskdriverid);


--
-- Name: pktskmacdrvrep; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY taskmacrodriverreplacement
    ADD CONSTRAINT pktskmacdrvrep PRIMARY KEY (tskmacdrvrepid);


--
-- Name: fktskmacdrvrep_tskdrv; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY taskmacrodriverreplacement
    ADD CONSTRAINT fktskmacdrvrep_tskdrv FOREIGN KEY (taskdriverid) REFERENCES taskdrivers(driverid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fktskmacdrvrep_tskmac; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY taskmacrodriverreplacement
    ADD CONSTRAINT fktskmacdrvrep_tskmac FOREIGN KEY (taskmacroid) REFERENCES taskmacros(taskmacid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

