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
-- Name: accounts; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE accounts (
    accountid integer NOT NULL,
    accountname character varying(100) NOT NULL,
    note text,
    serverid integer
);


ALTER TABLE public.accounts OWNER TO postgres;

--
-- Name: TABLE accounts; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE accounts IS 'Almost same as logins, except they include groups, roles, owners, and schemas, and I may not have a password for them.
Also, attached to servers.';


--
-- Name: accounts_accountid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE accounts_accountid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.accounts_accountid_seq OWNER TO postgres;

--
-- Name: accounts_accountid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE accounts_accountid_seq OWNED BY accounts.accountid;


--
-- Name: accountid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE accounts ALTER COLUMN accountid SET DEFAULT nextval('accounts_accountid_seq'::regclass);


--
-- Name: pkaccount; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY accounts
    ADD CONSTRAINT pkaccount PRIMARY KEY (accountid);


--
-- Name: fkaccountserver; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY accounts
    ADD CONSTRAINT fkaccountserver FOREIGN KEY (serverid) REFERENCES servers(serverid) ON UPDATE RESTRICT ON DELETE RESTRICT;


--
-- PostgreSQL database dump complete
--

