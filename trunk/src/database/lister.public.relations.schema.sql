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
-- Name: relations; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE relations (
    relid integer NOT NULL,
    fromid integer NOT NULL,
    toid integer NOT NULL,
    why character varying(200),
    note text,
    fromtbid integer,
    totbid integer,
    connid integer,
    addtimestamp timestamp with time zone DEFAULT now(),
    scripttarget integer,
    targetname character varying(400),
    flushtarget boolean,
    fastflushtarget boolean,
    rowlimit integer DEFAULT (-1),
    processorder integer,
    outputtomacro character varying(300)
)
WITH (autovacuum_enabled=true);


ALTER TABLE public.relations OWNER TO postgres;

--
-- Name: TABLE relations; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE relations IS 'relation table describing relationship between anything with an id.

This is for from to relations.';


--
-- Name: COLUMN relations.why; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.why IS 'Critical documentation!  Code forces this to be populated at least to 10 characters, which may annoy some users, but without this there is no way to make sense of why a script was attached to a task.  What function does it provide?  Why did you attach it?  What was the output usually?  Is it really slow?';


--
-- Name: COLUMN relations.fromtbid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.fromtbid IS 'Just task ids for now.  A task can have tests and scripts attached.  Later will want to attach contacts and stuff.';


--
-- Name: COLUMN relations.totbid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.totbid IS 'For now, scripts or tests.  Currently scripts are being attached and tests aren''t as critical.  Tests are more for automation and will be cool when I can get to that level.';


--
-- Name: COLUMN relations.connid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.connid IS 'If a single connection supports this task relation (script usually), then we can store it here and connect automagically when user goes to execute the task script.';


--
-- Name: COLUMN relations.addtimestamp; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.addtimestamp IS 'Helpful for sorting (see tasks_r) so that attachments to a task are listed in order of creation, creating a process or flowchart of sorts.';


--
-- Name: COLUMN relations.scripttarget; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.scripttarget IS '0=undef, 1=screen, 2=table, in which case targettable must be defined.  No sense building this stuff into the script object since the actual script doesn''t care where its output and is more reusable without target detail embedded.';


--
-- Name: COLUMN relations.flushtarget; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.flushtarget IS 'A delete of all rows in target first.  In non-SQL sources, this would be a logged action if available.';


--
-- Name: COLUMN relations.fastflushtarget; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.fastflushtarget IS 'for sql targets, use truncate with no logging.  for non-sql, any non-logged delete of rows or clearing of target space.';


--
-- Name: COLUMN relations.rowlimit; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.rowlimit IS '-1 means no limit. 0 means run but don''t output.  Note that truncate may still take place!';


--
-- Name: COLUMN relations.processorder; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.processorder IS 'Put an arbitrary number here to control the order.  Nulls processed last.  Same numbers are unknown order of execution.';


--
-- Name: COLUMN relations.outputtomacro; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN relations.outputtomacro IS 'Empty mean no assignment of 1/1 row/col value to a macro, otherwise a string is created and stuffed on the ContextMacro task script inprocess macros.  It is only active during a batch run, then flushed.';


--
-- Name: relations_relid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE relations_relid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.relations_relid_seq OWNER TO postgres;

--
-- Name: relations_relid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE relations_relid_seq OWNED BY relations.relid;


--
-- Name: relid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE relations ALTER COLUMN relid SET DEFAULT nextval('relations_relid_seq'::regclass);


--
-- Name: pkrel; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT pkrel PRIMARY KEY (relid);


--
-- Name: fki_fkrelfromtb; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkrelfromtb ON relations USING btree (fromid);


--
-- Name: fki_fkrelobtab; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkrelobtab ON relations USING btree (fromtbid);


--
-- Name: fki_fkreltoobtab; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkreltoobtab ON relations USING btree (totbid);


--
-- Name: fki_fkreltotb; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_fkreltotb ON relations USING btree (toid);


--
-- Name: fkrelconn; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT fkrelconn FOREIGN KEY (connid) REFERENCES connections(connid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkrelobtab; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT fkrelobtab FOREIGN KEY (fromtbid) REFERENCES listertables(listertbid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkreltoobtab; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT fkreltoobtab FOREIGN KEY (totbid) REFERENCES listertables(listertbid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

