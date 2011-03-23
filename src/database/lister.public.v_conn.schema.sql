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
-- Name: v_conn; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW v_conn AS
    SELECT j.connid, j.connname, j.loginid, j.instanceid, j.note, j.lastsucceeddatetime, j.lastfaildatetime, l.loginstr, l.loginpwd, l.islegit AS login_islegit, i.instancename, i.instanceaddress, i.insttypid, it.insttypname, i.envid, e.envstdname, e.envletter, j.dbname, j.isosauth, i.portno FROM ((((connections j LEFT JOIN logins l ON ((j.loginid = l.loginid))) LEFT JOIN instances i ON ((j.instanceid = i.instanceid))) LEFT JOIN insttyps it ON ((i.insttypid = it.insttypid))) LEFT JOIN environments e ON ((i.envid = e.envid)));


ALTER TABLE public.v_conn OWNER TO postgres;

--
-- Name: VIEW v_conn; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON VIEW v_conn IS 'Joins logins and instances together, and a left join to get a protocol.
Always select distinct against vj views, since history is kept in here, for simplicity.
Once a joining table is created, its easier to use it to store history, too, rather than to add a history table to link off of it.
';


--
-- Name: v_conn_del; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE v_conn_del AS ON DELETE TO v_conn DO INSTEAD DELETE FROM connections WHERE (connections.connid = old.connid);


--
-- Name: v_conn_ins; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE v_conn_ins AS ON INSERT TO v_conn DO INSTEAD (INSERT INTO connections (connid, connname, loginid, instanceid, lastsucceeddatetime, lastfaildatetime, note, dbname, isosauth) VALUES (DEFAULT, new.connname, getnewlogin(new.loginid, (new.loginstr)::text, (new.loginpwd)::text), new.instanceid, new.lastsucceeddatetime, new.lastfaildatetime, new.note, new.dbname, new.isosauth); UPDATE instances SET portno = new.portno, instancename = new.instancename, instanceaddress = new.instanceaddress, insttypid = new.insttypid, envid = new.envid WHERE (instances.instanceid = new.instanceid); INSERT INTO actionlog (note) VALUES ('Added connection'::text); );


--
-- Name: v_conn_upd; Type: RULE; Schema: public; Owner: postgres
--

CREATE RULE v_conn_upd AS ON UPDATE TO v_conn DO INSTEAD (UPDATE connections SET connname = new.connname, loginid = getnewlogin(new.loginid, (new.loginstr)::text, (new.loginpwd)::text), instanceid = new.instanceid, note = new.note, dbname = new.dbname, isosauth = new.isosauth WHERE (connections.connid = old.connid); UPDATE instances SET portno = new.portno, instancename = new.instancename, instanceaddress = new.instanceaddress, insttypid = new.insttypid, envid = new.envid WHERE (instances.instanceid = new.instanceid); INSERT INTO actionlog (note) VALUES ('Updated connection'::text); );


--
-- PostgreSQL database dump complete
--

