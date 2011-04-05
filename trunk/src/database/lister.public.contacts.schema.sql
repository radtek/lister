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
-- Name: contacts; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE contacts (
    contactid integer NOT NULL,
    contactname character varying(200) NOT NULL,
    emailaddress character varying(200),
    note text,
    workphoneno character varying(30),
    homephoneno character varying(30),
    cellphoneno character varying(30),
    nonworkemailaddress character varying(100),
    bridgeline character varying(100)
);


ALTER TABLE public.contacts OWNER TO postgres;

--
-- Name: TABLE contacts; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE contacts IS 'Includes users with syslogins and externals, like BB support people, their email, etc.';


--
-- Name: COLUMN contacts.bridgeline; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN contacts.bridgeline IS 'Bridge line: Dial in + key assigned to that user.';


--
-- Name: contacts_contactid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE contacts_contactid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contacts_contactid_seq OWNER TO postgres;

--
-- Name: contacts_contactid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE contacts_contactid_seq OWNED BY contacts.contactid;


--
-- Name: contactid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE contacts ALTER COLUMN contactid SET DEFAULT nextval('contacts_contactid_seq'::regclass);


--
-- Name: pkcontact; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY contacts
    ADD CONSTRAINT pkcontact PRIMARY KEY (contactid);


--
-- PostgreSQL database dump complete
--

