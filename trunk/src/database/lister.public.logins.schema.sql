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
-- Name: logins; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE logins (
    loginid integer NOT NULL,
    loginstr character varying(40),
    loginpwd character varying(100),
    islegit boolean,
    passwordrulelist character varying(200),
    prevpassword character varying(200),
    lastchangedwhen timestamp with time zone,
    isldap boolean,
    isosauth boolean,
    ispassencrypted boolean
);


ALTER TABLE public.logins OWNER TO postgres;

--
-- Name: TABLE logins; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE logins IS 'just a login paired with a password.';


--
-- Name: COLUMN logins.loginstr; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN logins.loginstr IS 'actual string entered into interface
Can be null (once!) to support the isosauth connection, and so it will have something to link to.';


--
-- Name: COLUMN logins.islegit; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN logins.islegit IS 'gained officially and available for safe, open use.';


--
-- Name: COLUMN logins.isosauth; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN logins.isosauth IS 'Is OS Authorization';


--
-- Name: COLUMN logins.ispassencrypted; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN logins.ispassencrypted IS 'Encrypted strings can be passed for Oracle and Sybase.';


--
-- Name: logins_loginid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE logins_loginid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.logins_loginid_seq OWNER TO postgres;

--
-- Name: logins_loginid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE logins_loginid_seq OWNED BY logins.loginid;


--
-- Name: loginid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE logins ALTER COLUMN loginid SET DEFAULT nextval('logins_loginid_seq'::regclass);


--
-- Name: pklogin; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY logins
    ADD CONSTRAINT pklogin PRIMARY KEY (loginid);


--
-- Name: logins; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE logins FROM PUBLIC;
REVOKE ALL ON TABLE logins FROM postgres;
GRANT ALL ON TABLE logins TO postgres;
GRANT ALL ON TABLE logins TO PUBLIC;


--
-- PostgreSQL database dump complete
--

