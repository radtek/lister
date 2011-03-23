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
-- Name: questions; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE questions (
    questionid integer NOT NULL,
    questiontext character varying(400) NOT NULL,
    answertext character varying(400),
    note text,
    taskid integer NOT NULL,
    status character varying(100)
);


ALTER TABLE public.questions OWNER TO postgres;

--
-- Name: TABLE questions; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE questions IS 'Questions that arise around the scoping of a task, and the answers found.';


--
-- Name: questions_qid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE questions_qid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.questions_qid_seq OWNER TO postgres;

--
-- Name: questions_qid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE questions_qid_seq OWNED BY questions.questionid;


--
-- Name: questionid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE questions ALTER COLUMN questionid SET DEFAULT nextval('questions_qid_seq'::regclass);


--
-- Name: akquestion; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY questions
    ADD CONSTRAINT akquestion UNIQUE (questiontext);


--
-- Name: pkquestion; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY questions
    ADD CONSTRAINT pkquestion PRIMARY KEY (questionid);


--
-- Name: fktaskquest; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY questions
    ADD CONSTRAINT fktaskquest FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

