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
-- Name: roles; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE roles (
    roleid integer NOT NULL,
    rolename character varying(200) NOT NULL,
    note text
);


ALTER TABLE public.roles OWNER TO postgres;

--
-- Name: TABLE roles; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE roles IS 'What role does stakeholder play in this task?';


--
-- Name: roles_roleid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE roles_roleid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.roles_roleid_seq OWNER TO postgres;

--
-- Name: roles_roleid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE roles_roleid_seq OWNED BY roles.roleid;


--
-- Name: roleid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE roles ALTER COLUMN roleid SET DEFAULT nextval('roles_roleid_seq'::regclass);


--
-- Name: akrolename; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY roles
    ADD CONSTRAINT akrolename UNIQUE (rolename);


--
-- Name: pkrole; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY roles
    ADD CONSTRAINT pkrole PRIMARY KEY (roleid);


--
-- PostgreSQL database dump complete
--

