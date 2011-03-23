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
-- Name: contactidentifiers; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE contactidentifiers (
    identid integer NOT NULL,
    identifier character varying(200) NOT NULL,
    contactid integer,
    identtypid integer
);


ALTER TABLE public.contactidentifiers OWNER TO postgres;

--
-- Name: TABLE contactidentifiers; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE contactidentifiers IS 'strings that identify a contact: For instance Bill MacSweeny, or William MacSwiney??? No one gets it right, so we put both here. I don''t even care which is right, just need a single key.

Also, sales rep identifiers can link here, logins, etc.  An FTS rapid search resolves to a set of contact ids, which we distinct on, and get hopefully one.';


--
-- Name: contactidentifiers_identid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE contactidentifiers_identid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contactidentifiers_identid_seq OWNER TO postgres;

--
-- Name: contactidentifiers_identid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE contactidentifiers_identid_seq OWNED BY contactidentifiers.identid;


--
-- Name: identid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE contactidentifiers ALTER COLUMN identid SET DEFAULT nextval('contactidentifiers_identid_seq'::regclass);


--
-- Name: pkcontident; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY contactidentifiers
    ADD CONSTRAINT pkcontident PRIMARY KEY (identid);


--
-- Name: fkcontidentcont; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY contactidentifiers
    ADD CONSTRAINT fkcontidentcont FOREIGN KEY (contactid) REFERENCES contacts(contactid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

