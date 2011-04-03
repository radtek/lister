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

--
-- Name: tasks_r; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW tasks_r AS
    SELECT r.relid, t.taskid, t.taskname, t.srccode, t.note AS tasknote, t.priorityassigneddate, t.assignedbywho, t.assignedtowho, t.projectname, t.prioritywithinproject, s.scriptid, s.scriptname, s.scriptplaintext, s.note AS scriptnote, s.scriptrichtext, s.addtimestamp, r.why, r.connid AS relconnid, c.connname AS relconnname, r.scripttarget, r.targetname, r.flushtarget, r.fastflushtarget, r.rowlimit, r.processorder, t.taskdriverid FROM (((tasks t JOIN relations r ON (((t.taskid = r.fromid) AND (r.fromtbid = (SELECT listertables.listertbid FROM listertables WHERE ((listertables.listertbname)::text = 'tasks'::text)))))) LEFT JOIN scripts s ON (((r.toid = s.scriptid) AND (r.totbid = (SELECT listertables.listertbid FROM listertables WHERE ((listertables.listertbname)::text = 'scripts'::text)))))) LEFT JOIN connections c ON ((r.connid = c.connid)));


ALTER TABLE public.tasks_r OWNER TO postgres;

--
-- Name: tasks_r_del; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE tasks_r_del AS ON DELETE TO tasks_r DO INSTEAD DELETE FROM relations WHERE (relations.relid = old.relid);


--
-- Name: tasks_r_upd; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE tasks_r_upd AS ON UPDATE TO tasks_r DO INSTEAD (UPDATE relations SET connid = new.relconnid, why = new.why, scripttarget = new.scripttarget, targetname = new.targetname, flushtarget = new.flushtarget, fastflushtarget = CASE new.fastflushtarget WHEN true THEN true WHEN false THEN false ELSE new.fastflushtarget END, rowlimit = new.rowlimit, processorder = new.processorder WHERE (relations.relid = old.relid); UPDATE scripts SET scriptplaintext = new.scriptplaintext, scriptrichtext = new.scriptrichtext, scriptname = new.scriptname, note = new.scriptnote WHERE (scripts.scriptid = old.scriptid); INSERT INTO actionlog (note) VALUES ('Updated tasks relations connection id'::text); );


--
-- PostgreSQL database dump complete
--

