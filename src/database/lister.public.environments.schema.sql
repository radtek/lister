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
-- Name: environments; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE environments (
    envid integer NOT NULL,
    envstdname character varying(100) NOT NULL,
    note text,
    envlngname character varying(100),
    envshrtname character varying(40),
    envletter character(1)
);


ALTER TABLE public.environments OWNER TO postgres;

--
-- Name: TABLE environments; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE environments IS 'environments that an instance or a database or server supports.

-3 = supports several environments, you need to look at the hosted objects.  Could be a bit map?  But then linking....
';


--
-- Name: COLUMN environments.envstdname; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN environments.envstdname IS 'Standard name that people say on a phone, like "Is it in dev?", not is it in Development.';


--
-- Name: environments_envid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE environments_envid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.environments_envid_seq OWNER TO postgres;

--
-- Name: environments_envid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE environments_envid_seq OWNED BY environments.envid;


--
-- Name: envid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE environments ALTER COLUMN envid SET DEFAULT nextval('environments_envid_seq'::regclass);


--
-- Name: environments_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY environments
    ADD CONSTRAINT environments_pkey PRIMARY KEY (envid);


--
-- Name: environments; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE environments FROM PUBLIC;
REVOKE ALL ON TABLE environments FROM postgres;
GRANT ALL ON TABLE environments TO postgres;
GRANT ALL ON TABLE environments TO PUBLIC;


--
-- PostgreSQL database dump complete
--

