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
-- Name: analysismetric; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE analysismetric (
    analmetid integer NOT NULL,
    analysisid integer NOT NULL,
    metricvalue bigint,
    metricdesc character varying(300),
    note text,
    scriptid integer
);


ALTER TABLE public.analysismetric OWNER TO postgres;

--
-- Name: analysismetric_analmetid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE analysismetric_analmetid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.analysismetric_analmetid_seq OWNER TO postgres;

--
-- Name: analysismetric_analmetid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE analysismetric_analmetid_seq OWNED BY analysismetric.analmetid;


--
-- Name: analmetid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE analysismetric ALTER COLUMN analmetid SET DEFAULT nextval('analysismetric_analmetid_seq'::regclass);


--
-- Name: pkanalmet; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY analysismetric
    ADD CONSTRAINT pkanalmet PRIMARY KEY (analmetid);


--
-- Name: fki_fkanalmetscript; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkanalmetscript ON analysismetric USING btree (scriptid);


--
-- Name: fkanalmetanal; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysismetric
    ADD CONSTRAINT fkanalmetanal FOREIGN KEY (analysisid) REFERENCES analysisbydate(analysisid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkanalmetscript; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY analysismetric
    ADD CONSTRAINT fkanalmetscript FOREIGN KEY (scriptid) REFERENCES scripts(scriptid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

