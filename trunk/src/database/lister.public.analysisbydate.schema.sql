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
-- Name: analysisbydate; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE analysisbydate (
    analysisid integer NOT NULL,
    analysisname character varying(200),
    totalcount bigint,
    bydate date,
    envid integer,
    taskid integer,
    noprobcount bigint,
    unresolvedcount bigint,
    resolvablecount bigint,
    unresolvablecount bigint,
    note text,
    scriptid integer
);


ALTER TABLE public.analysisbydate OWNER TO postgres;

--
-- Name: analysisbydate_analysisid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE analysisbydate_analysisid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.analysisbydate_analysisid_seq OWNER TO postgres;

--
-- Name: analysisbydate_analysisid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE analysisbydate_analysisid_seq OWNED BY analysisbydate.analysisid;


--
-- Name: analysisid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE analysisbydate ALTER COLUMN analysisid SET DEFAULT nextval('analysisbydate_analysisid_seq'::regclass);


--
-- Name: pkanal; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY analysisbydate
    ADD CONSTRAINT pkanal PRIMARY KEY (analysisid);


--
-- Name: fki_fkanalscript; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkanalscript ON analysisbydate USING btree (scriptid);


--
-- Name: fkanalenv; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysisbydate
    ADD CONSTRAINT fkanalenv FOREIGN KEY (envid) REFERENCES environments(envid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkanalscript; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysisbydate
    ADD CONSTRAINT fkanalscript FOREIGN KEY (scriptid) REFERENCES scripts(scriptid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkanaltask; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysisbydate
    ADD CONSTRAINT fkanaltask FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

