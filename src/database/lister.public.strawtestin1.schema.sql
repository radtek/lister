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
-- Name: strawtestin1; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE strawtestin1 (
    trex_rf_id character varying(220),
    tdc_id character varying(100),
    straw_id character varying(220),
    tdor_sys_tk_no character varying(100),
    td_sta_cd character varying(40),
    tdor_sta_cd character varying(40),
    tdor_sys_td_sta_cd character varying(100),
    tdor_sys_rc_sbty_lkup_cd character varying(100),
    tdor_sys_rc_sbty_lkup_dim_ky character varying(22),
    lkup_src_cd character varying(220),
    lkup_nm character varying(1020),
    td_qy character varying(22),
    td_lcl_px character varying(22),
    td_cpt_br_id character varying(40),
    td_ico_td_in character varying(4),
    td_ytm character varying(22),
    td_cpn_rt character varying(22),
    td_vl character varying(22),
    cob_dt timestamp with time zone,
    td_dt timestamp with time zone,
    trex_ts timestamp with time zone,
    td_stl_dt timestamp with time zone,
    td_mat_dt timestamp with time zone,
    td_prpy_ac_in character varying(4),
    td_ac_in character varying(4),
    td_bysl_cd character varying(40),
    td_prm_bk_nm character varying(1020),
    td_cpt_bk_nm character varying(40),
    td_cli_coper_id character varying(22),
    td_cli_pty_dim_ky character varying(22),
    td_cpt_ml_lgey_id character varying(100),
    td_cpt_id character varying(220),
    td_cpt_ac_no character varying(220),
    td_cpt_ac_dim_ky character varying(22),
    td_gci_id character varying(100),
    td_cusip_insm_id character varying(40),
    td_isin_insm_id character varying(100),
    td_insm_dim_ky character varying(22),
    td_insm_hit_atr_vl character varying(100),
    td_sub_insm_hit_atr_vl character varying(1020),
    td_bb_tck_id character varying(220),
    trex_dsk_tx character varying(400),
    td_sc_ty_cd character varying(1020),
    td_srcs_tdr_id character varying(220),
    td_nt_tx character varying(1020),
    tdor_src_nm character varying(220),
    td_fd_src_nm character varying(220)
);


ALTER TABLE public.strawtestin1 OWNER TO postgres;

--
-- PostgreSQL database dump complete
--

