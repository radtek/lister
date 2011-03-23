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
-- Name: stakeholders; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE stakeholders (
    holderid integer NOT NULL,
    contactid integer NOT NULL,
    taskid integer NOT NULL,
    why character varying(400),
    note text,
    roleid integer
);


ALTER TABLE public.stakeholders OWNER TO postgres;

--
-- Name: TABLE stakeholders; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE stakeholders IS 'People or groups involved';


--
-- Name: stakeholders_holderid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE stakeholders_holderid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.stakeholders_holderid_seq OWNER TO postgres;

--
-- Name: stakeholders_holderid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE stakeholders_holderid_seq OWNED BY stakeholders.holderid;


--
-- Name: holderid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE stakeholders ALTER COLUMN holderid SET DEFAULT nextval('stakeholders_holderid_seq'::regclass);


--
-- Name: pkholder; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY stakeholders
    ADD CONSTRAINT pkholder PRIMARY KEY (holderid);


--
-- Name: akholdertask; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY stakeholders
    ADD CONSTRAINT akholdertask FOREIGN KEY (taskid) REFERENCES tasks(taskid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkholdercontact; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY stakeholders
    ADD CONSTRAINT fkholdercontact FOREIGN KEY (contactid) REFERENCES contacts(contactid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- Name: fkholderrole; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY stakeholders
    ADD CONSTRAINT fkholderrole FOREIGN KEY (roleid) REFERENCES roles(roleid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

