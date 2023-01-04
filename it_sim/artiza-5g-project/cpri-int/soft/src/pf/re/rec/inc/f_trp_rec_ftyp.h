/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_ftyp.h
 * @brief  prototype
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * @date   2008/07/21 FFCS)Shihzh modify for S3G PRC
 * @date   2009/01/17 FFCS)Shihzh modify for BI-TBD-557
 * @date   2009/01/26 FFCS)Shihzh M-S3G-eNBPF-01220
 * @date   2009/03/04 FFCS)Shihzh modify for API 2nd release
 * @date   2009/06/23 FFCS)Shihzh M-S3G-eNBPF-02129
 * @date   2010/10/25 FJT)Koshida modify for eNB-004 of TS-656・ｽ・ｽ5・ｽﾎ会ｿｽ.
 * @date   2011/02/17 FJT)Koshida modify for M-S3G-eNBSYS-02253(TS-111)・ｽﾎ会ｿｽ.
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 * @date   2021/02/04 M&C)Huan.dh add CARONOFF handle
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2016
 */
/****************************************************************************/
#ifndef _F_TRP_REC_FTYP_h
#define _F_TRP_REC_FTYP_h

//#include "f_trp_com_Dlist.h"
#include "f_trp_rec_typ.h"

/*! @addtogroup TRIF_REC
 * @{ */

//VOID                    f_trp_rec_Fc01_pre_m_aps_cmp(VOID *);
//VOID                    f_trp_rec_Fc01_pre_m_err(VOID *);
//VOID                    f_trp_rec_Fc01_pre_m_ini_init_exe(VOID *);
//VOID                    f_trp_rec_Fc01_pre_m_nes_end(VOID *);
//VOID                    f_trp_rec_Fc01_pre_m_nes_sta(VOID *);
//VOID                    f_trp_rec_Fc01_pre_m_nop(VOID *);
//VOID                    f_trp_rec_Fc01_pre_m_sigstp_end(VOID *);
//VOID                    f_trp_rec_Fc01_pre_m_suc_end(VOID *);
//VOID                    f_trp_rec_Fc01_pre_m_suc_sta(VOID *);
//VOID                    f_trp_rec_Fc01_pre_r(VOID *);
//VOID                    f_trp_rec_Fc01_pst_m_err(VOID *);
//VOID                    f_trp_rec_Fc01_pst_m_ini_init_exe(VOID *);
//VOID                    f_trp_rec_Fc01_pst_m_nop(VOID *);
//VOID                    f_trp_rec_Fc01_pst_r(VOID *);
//ET_REC_C01_EVT          f_trp_rec_Fc01_r_evt_cnv(VOID *);
//ET_REC_C01_EVT          f_trp_rec_Fc01_t_evt_get();
//VOID                    f_trp_rec_Fc01_t_evt_set(ET_REC_C01_EVT);
//VOID                    f_trp_rec_Fc01_t_it_nechgrsp_snd();
//VOID                    f_trp_rec_Fc01_t_it_thrinirsp_snd(USHORT);
//ET_REC_C01_STN          f_trp_rec_Fc01_t_stn_get();
//VOID                    f_trp_rec_Fc01_t_stn_set(ET_REC_C01_STN);
//VOID                    f_trp_rec_Fc01_t_tbl_from_sys_gen();


VOID                    f_trp_rec_Fc02_m_2nd_bak(VOID *);
VOID                    f_trp_rec_Fc02_m_2nd_bak_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_2nd_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_2nd_exe_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_2nd_rdy(VOID *);
VOID                    f_trp_rec_Fc02_m_2nd_rdy_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_2nd_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_2nd_sta_3g(VOID *);
//VOID                    f_trp_rec_Fc02_m_2nd_stp(VOID *);
//VOID                    f_trp_rec_Fc02_m_act_cmp(VOID *);
//VOID                    f_trp_rec_Fc02_m_act_exe(VOID *);
//VOID                    f_trp_rec_Fc02_m_act_mtset_sta(VOID *);
//VOID                    f_trp_rec_Fc02_m_act_rdy(VOID *);
//VOID                    f_trp_rec_Fc02_m_act_react_exe(VOID *);
//VOID                    f_trp_rec_Fc02_m_act_reres_sta(VOID *);
//VOID                    f_trp_rec_Fc02_m_act_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_dsc_resini_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_dsc_resini_exe_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_dsc_resrst_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_dsc_resrst_exe_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_dsc_useidl_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_dsc_useidl_exe_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_dsc_useini_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_dsc_useini_exe_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_err(VOID *);
VOID                    f_trp_rec_Fc02_m_res_restsuse_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_idl_restsuse_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_idl_restsuse_exe_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_ini_init_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_ini_init_exe_3g(VOID *);
//VOID                    f_trp_rec_Fc02_m_int_dlyoft_exe(VOID *);
//VOID                    f_trp_rec_Fc02_m_neg_actstsini_exe(VOID *);
//VOID                    f_trp_rec_Fc02_m_neg_cmp(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_actset_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_act_cmp(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_dsc_E_exe(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_dsc_F_exe(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_mtsetf_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_mtsets_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_resltinf_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_neg_cmp(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_F_rdy(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_L3_rdy(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_nes_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_nop(VOID *);
//VOID                    f_trp_rec_Fc02_m_rcv_exe(VOID *);
//VOID                    f_trp_rec_Fc02_m_res_actset_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_res_actset_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_cmp(VOID *);
VOID                    f_trp_rec_Fc02_m_res_cmp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_mtsetf_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_mtsetf_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_mtsets_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_mtsets_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_rdy(VOID *);
VOID                    f_trp_rec_Fc02_m_res_rdy_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrdscm_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrdscm_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrdscm_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrdssc_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrdssc_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrdssc_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrrsts_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrrsts_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrrsts_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reeqpinf_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reeqpinf_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reeqpinf_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_res_refil_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_refil_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_refil_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_refil_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_refrcrst_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_refrcrst_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reprm_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reprm_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reprm_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reprm_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reprtsts_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reprtsts_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reprtsts_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_res_resltinf_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_resltinf_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_resltinf_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_res_rests_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_rests_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_rests_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_res_retimset_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_res_retimset_exe_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_tofval_end(VOID *);
VOID                    f_trp_rec_Fc02_m_res_tofval_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_res_tofval_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_reautrst_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_reautrst_exe_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_frcstp_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_frcstp_exe_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_refrcrst_cmp(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_refrcrst_cmp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_refrcrst_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_refrcrst_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_refrcrst_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_refrcrst_stp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_rerst_cmp(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_rerst_cmp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_rerst_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_rerst_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_rerst_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_rst_rerst_stp_3g(VOID *);
//VOID                    f_trp_rec_Fc02_m_rty_dlyoft_exe(VOID *);
//VOID                    f_trp_rec_Fc02_m_suc_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_suc_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_use_mtsetf_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_use_mtsetf_end(VOID *);
VOID                    f_trp_rec_Fc02_m_use_mtsets_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_use_mtsets_end(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrdscm_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_use_recrdscm_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrdscm_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrdssc_end(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrdssc_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrdssc_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrrsts_end(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrrsts_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrrsts_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_use_reeqpinf_end(VOID *);
VOID                    f_trp_rec_Fc02_m_use_reeqpinf_sta(VOID *);
//VOID                    f_trp_rec_Fc02_m_use_reeqpinf_stp(VOID *);
//VOID                    f_trp_rec_Fc02_m_use_reprtsts_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_use_reprtsts_sta(VOID *);
//VOID                    f_trp_rec_Fc02_m_use_reprtsts_stp(VOID *);
//VOID                    f_trp_rec_Fc02_m_use_resltinf_end(VOID *);
//VOID                    f_trp_rec_Fc02_m_use_resltinf_stp(VOID *);
VOID                    f_trp_rec_Fc02_m_use_rests_end(VOID *);
VOID                    f_trp_rec_Fc02_m_use_rests_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_use_rests_stp(VOID *);
//VOID                    f_trp_rec_Fc02_m_use_retimset_exe(VOID *);
VOID                    f_trp_rec_Fc02_m_use_tofval_end(VOID *);
VOID                    f_trp_rec_Fc02_m_use_tofval_sta(VOID *);
VOID                    f_trp_rec_Fc02_m_use_tofval_stp(VOID *);
VOID                    f_trp_rec_Fc02_r(VOID *);
ET_REC_C02_EVT          f_trp_rec_Fc02_r_evt_cnv(VOID *, USHORT *);
ET_REC_C02_EVT          f_trp_rec_Fc02_r_evt_cnv_3g(VOID *, USHORT *);
VOID                    f_trp_rec_Fc02_t_actsts_get(VOID *);
//USHORT                  f_trp_rec_Fc02_t_allact_get(USHORT actlnk_gr);
VOID                    f_trp_rec_Fc02_t_ampslt_cpr_tbl_set();
VOID                    f_trp_rec_Fc02_t_cnftim_ccl();
VOID                    f_trp_rec_Fc02_t_cnftim_ccl_3g();
UINT                    f_trp_rec_Fc02_t_cprpar_chk(VOID *);
USHORT                  f_trp_rec_Fc02_t_crdscmidx_get(USHORT, USHORT);
USHORT                  f_trp_rec_Fc02_t_crdsscidx_get(USHORT, USHORT);
UINT                    f_trp_rec_Fc02_t_crg_red(ET_REC_CRK, UINT *);
UINT                    f_trp_rec_Fc02_t_crg_wrt(ET_REC_CRK, UINT);
UINT                    f_trp_rec_Fc02_t_dlyoft_set();
VOID                    f_trp_rec_Fc02_t_err_clr(ET_REC_ERK);
VOID                    f_trp_rec_Fc02_t_err_set(ET_REC_ERK);
ET_REC_C02_EVT          f_trp_rec_Fc02_t_evt_get();
VOID                    f_trp_rec_Fc02_t_evt_set(ET_REC_C02_EVT);
VOID                    f_trp_rec_Fc02_t_hw_refrcrst_snd();
VOID                    f_trp_rec_Fc02_t_hw_refrcrst_snd_3g();
VOID                    f_trp_rec_Fc02_t_inttim_ccl();
VOID                    f_trp_rec_Fc02_t_inttim_ccl_3g();
VOID                    f_trp_rec_Fc02_t_inttim_ent();
VOID                    f_trp_rec_Fc02_t_inttim_ent_3g();
VOID                    f_trp_rec_Fc02_t_ip_fldactntc_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_ip_lnkestntc_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_ip_reactntc_snd();
VOID                    f_trp_rec_Fc02_t_ip_recrdscmntc_snd(USHORT, USHORT);
VOID                    f_trp_rec_Fc02_t_ip_recrdsscntc_snd(USHORT, USHORT);
VOID                    f_trp_rec_Fc02_t_ip_recrrstsntc_snd();
VOID                    f_trp_rec_Fc02_t_ip_reeqpinfntc_snd();
VOID                    f_trp_rec_Fc02_t_ip_remnginintc_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_ip_reprtstsntc_snd();
VOID                    f_trp_rec_Fc02_t_ip_rescmpntc_snd(USHORT, USHORT);
VOID                    f_trp_rec_Fc02_t_ip_resltinfntc_snd();
VOID                    f_trp_rec_Fc02_t_ip_tofvalntc_snd();
VOID                    f_trp_rec_Fc02_t_it_2ndprc_snd();
VOID                    f_trp_rec_Fc02_t_it_2ndrsp_snd();
VOID                    f_trp_rec_Fc02_t_it_2ndrsp_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_actchgind_snd(USHORT);
//VOID                    f_trp_rec_Fc02_t_it_actsetprc_snd();
VOID                    f_trp_rec_Fc02_t_it_actsetreq_snd();
VOID                    f_trp_rec_Fc02_t_it_frcstpntc_snd(USHORT, T_RRH_RESTOP_FACTOR);
VOID                    f_trp_rec_Fc02_t_it_frcstpntc_snd_3g(USHORT, T_RRH_RESTOP_FACTOR);
VOID                    f_trp_rec_Fc02_t_it_frcstpprc_snd(T_RRH_RESTOP_FACTOR);
VOID                    f_trp_rec_Fc02_t_it_frcstpprc_snd_3g(T_RRH_RESTOP_FACTOR);
VOID                    f_trp_rec_Fc02_t_it_mtsetfprc_snd();
VOID                    f_trp_rec_Fc02_t_it_mtsetfreq_snd();
VOID                    f_trp_rec_Fc02_t_it_mtsetsprc_snd();
VOID                    f_trp_rec_Fc02_t_it_mtsetsreq_snd();
VOID                    f_trp_rec_Fc02_t_it_recrdscmprc_snd();
VOID                    f_trp_rec_Fc02_t_it_recrdsscprc_snd();
VOID                    f_trp_rec_Fc02_t_it_recrrstsprc_snd();
VOID                    f_trp_rec_Fc02_t_it_redlstpind_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_it_redlstpind_snd_3g(USHORT);
VOID                    f_trp_rec_Fc02_t_it_reeqpinfprc_snd();
VOID                    f_trp_rec_Fc02_t_it_refilprc_snd();
VOID                    f_trp_rec_Fc02_t_it_refilprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_refilreq_snd();
VOID                    f_trp_rec_Fc02_t_it_refilreq_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_refrcrstprc_snd();
VOID                    f_trp_rec_Fc02_t_it_refrcrstsetprc_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_it_reprmprc_snd();
VOID                    f_trp_rec_Fc02_t_it_reprmprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_reprmreq_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_it_reprtstsprc_snd();
VOID                    f_trp_rec_Fc02_t_it_rerstprc_snd();
VOID                    f_trp_rec_Fc02_t_it_rerstprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_rescmpntc_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_it_rescmpprc_snd();
VOID                    f_trp_rec_Fc02_t_it_rescmpprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_resltinfprc_snd();
VOID                    f_trp_rec_Fc02_t_it_restsprc_snd();
VOID                    f_trp_rec_Fc02_t_it_retimsetprc_snd();
VOID                    f_trp_rec_Fc02_t_it_retimsetprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_tofvalprc_snd();
VOID                    f_trp_rec_Fc02_t_it_txraprc_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_it_txsaprc_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_l3_recrdscmreq_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_l3_recrdsscreq_snd(USHORT);
VOID                    f_trp_rec_Fc02_t_l3_recrrstsreq_snd();
VOID                    f_trp_rec_Fc02_t_l3_reeqpinfreq_snd();
VOID                    f_trp_rec_Fc02_t_l3_reprtstsreq_snd();
VOID                    f_trp_rec_Fc02_t_l3_rerstreq_snd();
/* 2021/03/03 M&C) Update src based on No.27 - Delete start */
// VOID                    f_trp_rec_Fc02_t_l3_rerstreq_snd_3g();
/* 2021/03/03 M&C) Update src based on No.27 - Delete end */
VOID                    f_trp_rec_Fc02_t_l3_resltinfreq_snd();
VOID                    f_trp_rec_Fc02_t_l3_restsreq_snd();
VOID                    f_trp_rec_Fc02_t_l3_retimsetreq_snd();
VOID                    f_trp_rec_Fc02_t_l3_retimsetreq_snd_3g();
VOID                    f_trp_rec_Fc02_t_l3_tofvalreq_snd();
VOID                    f_trp_rec_Fc02_t_ngrecrdscm_set(USHORT);
VOID                    f_trp_rec_Fc02_t_ngrecrdssc_set(USHORT);
// [16B] add start
VOID                    f_trp_rec_Fc02_t_ngrecrdssc_set(USHORT);
VOID                    f_trp_rec_Fc02_t_ngrecrdssc_set_3g(USHORT);
// [16B] add end
ET_REC_C02_STN          f_trp_rec_Fc02_t_otrstn_get(USHORT);
VOID                    f_trp_rec_Fc02_t_otrstn_set(USHORT, ET_REC_C02_STN);
USHORT                  f_trp_rec_Fc02_t_ownact_get();
VOID                    f_trp_rec_Fc02_t_ownact_set(USHORT);
ET_REC_C02_STN          f_trp_rec_Fc02_t_ownstn_get();
VOID                    f_trp_rec_Fc02_t_ownstn_set(ET_REC_C02_STN);
UINT                    f_trp_rec_Fc02_t_recrdscm_a_ini(USHORT);
VOID                    f_trp_rec_Fc02_t_recrdscm_i_get(USHORT, USHORT, VOID *);
UINT                    f_trp_rec_Fc02_t_recrdscm_i_ini(USHORT, USHORT);
UINT                    f_trp_rec_Fc02_t_recrdscm_i_set(USHORT, VOID *);
UINT                    f_trp_rec_Fc02_t_recrdssc_a_ini(USHORT);
UINT                    f_trp_rec_Fc02_t_recrdssc_a_ini_3g(USHORT);
VOID                    f_trp_rec_Fc02_t_recrdssc_i_get(USHORT, USHORT, VOID *);
UINT                    f_trp_rec_Fc02_t_recrdssc_i_ini(USHORT, USHORT);
UINT                    f_trp_rec_Fc02_t_recrdssc_i_set(USHORT, VOID *);
// [16B] add start
VOID                    f_trp_rec_Fc02_t_recrdssc_i_merge();
/*  Convert function kind to index */
USHORT                  f_trp_rec_Fc02_t_recrdssc_i_ConvFuncKindToId( USHORT  );
/*  compare 配下REの機閨E部単位情報, get 機閨E部単位情報  */
VOID                    f_trp_rec_Fc02_t_recrdssc_i_GetFuncEvery( 	T_RRH_CARDST_S3G_FUNC_EVERY* ,
																	T_RRH_CARDST_S3G_FUNC_EVERY,
																	USHORT* );
/* 昇順ソート関数(unsigned short用) */
VOID                    f_trp_rec_Fc02_t_recrdssc_i_ushortsort_asc( USHORT ,
																	USHORT,
																	USHORT* );
// [16B] add end
UINT                    f_trp_rec_Fc02_t_recrdsts_chk(VOID *);
VOID                    f_trp_rec_Fc02_t_recrralm_add(USHORT, USHORT);
VOID                    f_trp_rec_Fc02_t_recrralm_del(USHORT, USHORT);
UINT                    f_trp_rec_Fc02_t_recrrsts_a_ini(USHORT);
UINT                    f_trp_rec_Fc02_t_recrrsts_chk(VOID *);
VOID                    f_trp_rec_Fc02_t_recrrsts_set(VOID *);
UINT                    f_trp_rec_Fc02_t_reeqpinf_chk(VOID *);
//VOID                    f_trp_rec_Fc02_t_reeqpinf_get(VOID *);
UINT                    f_trp_rec_Fc02_t_reeqpinf_ini(USHORT);
UINT                    f_trp_rec_Fc02_t_reeqpinf_ini_3g(USHORT);
UINT                    f_trp_rec_Fc02_t_reeqpinf_set(VOID *);
VOID                    f_trp_rec_Fc02_t_refbstsntc_snd(USHORT);
UINT                    f_trp_rec_Fc02_t_refbsts_ini();
UINT                    f_trp_rec_Fc02_t_refbsts_gen();
UINT                    f_trp_rec_Fc02_t_remcm_tbl_ini(USHORT);
VOID                    f_trp_rec_Fc02_t_remng_get(VOID *);
UINT                    f_trp_rec_Fc02_t_remsc_tbl_ini(USHORT);
UINT                    f_trp_rec_Fc02_t_remsc_tbl_ini_3g(USHORT);
USHORT                    f_trp_rec_Fc02_t_reprtsts_get(VOID *);
//UINT                    f_trp_rec_Fc02_t_reprtsts_ini(USHORT);
UINT                    f_trp_rec_Fc02_t_reprtsts_set(VOID *);
//UINT                    f_trp_rec_Fc02_t_resltinfcm_ini(USHORT);
UINT                    f_trp_rec_Fc02_t_resltinfsc_ini(USHORT);
UINT                    f_trp_rec_Fc02_t_resltinf_chk(VOID *);
UINT                    f_trp_rec_Fc02_t_resltinf_chk_3g(VOID *);
//VOID                    f_trp_rec_Fc02_t_resltinf_get(VOID *);
UINT                    f_trp_rec_Fc02_t_resltinf_set(VOID *);
//VOID                    f_trp_rec_Fc02_t_resltlcm_get(T_REC_SLTLST *);
//VOID                    f_trp_rec_Fc02_t_resltlsc_get(T_REC_SLTLST *);
UINT                    f_trp_rec_Fc02_t_rests_chk(VOID *);
// UINT                    f_trp_rec_Fc02_t_rests_chk_3g(VOID *); /* 2021/01/19 M&C)DuongMX Item No.21 - RE Floating Condition Monitoring (LTE) (delete) */
UINT                    f_trp_rec_Fc02_t_rests_ini(USHORT);
UINT                    f_trp_rec_Fc02_t_rests_set(VOID *);
VOID                    f_trp_rec_Fc02_t_tc_all_ini();
VOID                    f_trp_rec_Fc02_t_tc_all_ini_3g();
USHORT                  f_trp_rec_Fc02_t_tc_dlyoft_get();
VOID                    f_trp_rec_Fc02_t_tc_dlyoft_set(USHORT);
VOID                    f_trp_rec_Fc02_t_tc_ini();
VOID                    f_trp_rec_Fc02_t_tc_ini_3g();
VOID                    f_trp_rec_Fc02_t_tc_recrdstsreq_dec(USHORT);
USHORT                  f_trp_rec_Fc02_t_tc_recrdstsreq_get(USHORT);
VOID                    f_trp_rec_Fc02_t_tc_recrdstsreq_ini(USHORT);
VOID                    f_trp_rec_Fc02_t_tc_recrdstsreq_ini_3g(USHORT);
VOID                    f_trp_rec_Fc02_t_use_recrdstsreq_dec(USHORT);
USHORT                  f_trp_rec_Fc02_t_use_recrdstsreq_get(USHORT);
VOID                    f_trp_rec_Fc02_t_use_recrdstsreq_ini(USHORT);
VOID                    f_trp_rec_Fc02_t_use_recrdstsreq_ini_3g(USHORT);
VOID                    f_trp_rec_Fc02_t_tc_recrrstsreq_dec();
USHORT                  f_trp_rec_Fc02_t_tc_recrrstsreq_get();
VOID                    f_trp_rec_Fc02_t_tc_recrrstsreq_ini();
VOID                    f_trp_rec_Fc02_t_tc_recrrstsreq_ini_3g();
VOID                    f_trp_rec_Fc02_t_use_recrrstsreq_dec();
USHORT                  f_trp_rec_Fc02_t_use_recrrstsreq_get();
VOID                    f_trp_rec_Fc02_t_use_recrrstsreq_ini();
VOID                    f_trp_rec_Fc02_t_use_recrrstsreq_ini_3g();
VOID                    f_trp_rec_Fc02_t_tc_reeqpinfreq_dec();
USHORT                  f_trp_rec_Fc02_t_tc_reeqpinfreq_get();
VOID                    f_trp_rec_Fc02_t_tc_reeqpinfreq_ini();
VOID                    f_trp_rec_Fc02_t_tc_reeqpinfreq_ini_3g();
VOID                    f_trp_rec_Fc02_t_use_reeqpinfreq_ini();
VOID                    f_trp_rec_Fc02_t_use_reeqpinfreq_ini_3g();
VOID                    f_trp_rec_Fc02_t_tc_refrcrstsnd_dec();
VOID                    f_trp_rec_Fc02_t_tc_refrcrstsnd_dec_3g();
USHORT                  f_trp_rec_Fc02_t_tc_refrcrstsnd_get();
VOID                    f_trp_rec_Fc02_t_tc_refrcrstsnd_ini();
VOID                    f_trp_rec_Fc02_t_tc_refrcrstsnd_ini_3g();
VOID                    f_trp_rec_Fc02_t_tc_refrcrst_dec();
VOID                    f_trp_rec_Fc02_t_tc_refrcrst_dec_3g();
USHORT                  f_trp_rec_Fc02_t_tc_refrcrst_get();
USHORT                  f_trp_rec_Fc02_t_tc_refrcrst_get_3g();
VOID                    f_trp_rec_Fc02_t_tc_refrcrst_ini();
VOID                    f_trp_rec_Fc02_t_tc_refrcrst_ini_3g();
VOID                    f_trp_rec_Fc02_t_tc_reprtstsreq_dec();
USHORT                  f_trp_rec_Fc02_t_tc_reprtstsreq_get();
VOID                    f_trp_rec_Fc02_t_tc_reprtstsreq_ini();
VOID                    f_trp_rec_Fc02_t_tc_reprtstsreq_ini_3g();
VOID                    f_trp_rec_Fc02_t_use_reprtstsreq_ini();
VOID                    f_trp_rec_Fc02_t_use_reprtstsreq_ini_3g();
VOID                    f_trp_rec_Fc02_t_tc_rerstreq_dec();
/* 2021/03/03 M&C) Update src based on No.27 - Delete start */
// VOID                    f_trp_rec_Fc02_t_tc_rerstreq_dec_3g();
/* 2021/03/03 M&C) Update src based on No.27 - Delete end */
USHORT                  f_trp_rec_Fc02_t_tc_rerstreq_get();
/* 2021/03/03 M&C) Update src based on No.27 - Delete start */
// USHORT                  f_trp_rec_Fc02_t_tc_rerstreq_get_3g();
/* 2021/03/03 M&C) Update src based on No.27 - Delete end */
VOID                    f_trp_rec_Fc02_t_tc_rerstreq_ini();
VOID                    f_trp_rec_Fc02_t_tc_rerstreq_ini_3g();
VOID                    f_trp_rec_Fc02_t_tc_resltinfreq_dec();
USHORT                  f_trp_rec_Fc02_t_tc_resltinfreq_get();
VOID                    f_trp_rec_Fc02_t_tc_resltinfreq_ini();
VOID                    f_trp_rec_Fc02_t_tc_resltinfreq_ini_3g();
VOID                    f_trp_rec_Fc02_t_tc_restsreq_dec();
USHORT                  f_trp_rec_Fc02_t_tc_restsreq_get();
VOID                    f_trp_rec_Fc02_t_tc_restsreq_ini();
VOID                    f_trp_rec_Fc02_t_tc_restsreq_ini_3g();
VOID                    f_trp_rec_Fc02_t_use_restsreq_ini();
VOID                    f_trp_rec_Fc02_t_tc_tofvalreq_dec();
USHORT                  f_trp_rec_Fc02_t_tc_tofvalreq_get();
VOID                    f_trp_rec_Fc02_t_tc_tofvalreq_ini();
UINT                    f_trp_rec_Fc02_t_timer_ccl(ET_REC_TMK, USHORT);
UINT                    f_trp_rec_Fc02_t_timer_ent(ET_REC_TMK, USHORT);
VOID                    f_trp_rec_Fc02_t_tofval_get(VOID *);
UINT                    f_trp_rec_Fc02_t_tofval_ini(USHORT);
UINT                    f_trp_rec_Fc02_t_tofval_ini_3g(USHORT);
UINT                    f_trp_rec_Fc02_t_tofval_set(VOID *);
USHORT                  f_trp_rec_Fc02_t_ugrecrdscm_get();
USHORT                  f_trp_rec_Fc02_t_ugrecrdssc_get();
// [16B] add start
USHORT                  f_trp_rec_Fc02_t_ugrecrdssc_get_3g();
// [16B] add end
USHORT                  f_trp_rec_Fc02_t_AutoResetTime_chk(USHORT);
USHORT                  f_trp_rec_Fc02_t_AutoResetTime_chk_3g();
//VOID                    f_trp_rec_Fc02_m_use_dlink_ind(VOID *);
VOID                    f_trp_rec_Fc02_t_autorst_chk( CARDSTS_INF_S3G* );
VOID                    f_trp_rec_Fc02_t_autorst_chk_3g( CARDSTS_INF* );
//VOID                    f_trp_rec_Fc02_t_autorst_ccl(T_REC_CARDOBJ*);
VOID                    f_trp_rec_Fc02_multi_tbl_set();
//VOID                    f_trp_rec_Fc02_m_redl_permit_rcv( VOID* bufp );
//VOID                    f_trp_rec_Fc02_m_mtdata_permit_rcv( VOID* bufp );
VOID                    f_trp_rec_Fc02_t_redl_permitrsp_snd( VOID );
VOID                    f_trp_rec_Fc02_t_mtdata_permitrsp_snd( VOID );

//VOID                    f_trp_rec_Fcom_t_actswrslt_snd();

// ・ｽ・ｽ・ｽ・ｽ・ｽX・ｽ・ｽ・ｽb・ｽg・ｽﾎ会ｿｽ ・ｽV・ｽK・ｽﾖ撰ｿｽ
//UINT                    f_trp_rec_Fc02_t_recrd_i_multi_set(USHORT slt_no, VOID* l3_bufp);
//UINT                    f_trp_rec_Fc02_t_recrd_i_multi_ini(USHORT ink, T_REC_CARDOBJ* cardObj);
//UINT                    f_trp_rec_Fc02_t_recrd_a_multi_ini(USHORT ink);
//VOID                    f_trp_rec_Fc02_t_recrdntcmsg_multi_set(T_DATA_RECRDSTS_INF* msgBuf, T_REC_CARDOBJ* cardObj);
VOID                    f_trp_rec_Fc02_t_ip_recrdntc_multi_snd( USHORT type );
//VOID                    f_trp_rec_Fcom_t_fbsts_dsp(FILE *);
USHORT                  f_trp_rec_Fc02_t_single_or_multi_get(VOID);
VOID					f_trp_rec_Fc02_m_use_sendtypeswitch();


VOID                    f_trp_rec_Fc03_m_err(VOID *);
//VOID                    f_trp_rec_Fc03_m_ini_init_exe(VOID *);
//VOID                    f_trp_rec_Fc03_m_nes_end(VOID *);
//VOID                    f_trp_rec_Fc03_m_nes_sta(VOID *);
VOID                    f_trp_rec_Fc03_m_nop(VOID *);
//VOID                    f_trp_rec_Fc03_m_rcv_exe(VOID *);
//VOID                    f_trp_rec_Fc03_m_suc_end(VOID *);
//VOID                    f_trp_rec_Fc03_m_suc_sta(VOID *);
VOID                    f_trp_rec_Fc03_m_txr_a_end(VOID *);
VOID                    f_trp_rec_Fc03_m_txr_a_sta(VOID *);
VOID                    f_trp_rec_Fc03_m_txr_a_stp(VOID *);
VOID                    f_trp_rec_Fc03_m_txr_m_end(VOID *);
VOID                    f_trp_rec_Fc03_m_txr_m_rac(VOID *);
VOID                    f_trp_rec_Fc03_m_txr_m_sta(VOID *);
VOID                    f_trp_rec_Fc03_m_txr_m_stp(VOID *);
VOID                    f_trp_rec_Fc03_m_txr_n_end(VOID *);
VOID                    f_trp_rec_Fc03_m_txr_n_stp(VOID *);
VOID                    f_trp_rec_Fc03_m_txs_a_end(VOID *);
VOID                    f_trp_rec_Fc03_m_txs_a_sta(VOID *);
VOID                    f_trp_rec_Fc03_m_txs_a_stp(VOID *);
VOID                    f_trp_rec_Fc03_m_txs_m_end(VOID *);
VOID                    f_trp_rec_Fc03_m_txs_m_rac(VOID *);
VOID                    f_trp_rec_Fc03_m_txs_m_sta(VOID *);
VOID                    f_trp_rec_Fc03_m_txs_m_stp(VOID *);
VOID                    f_trp_rec_Fc03_m_txs_n_end(VOID *);
VOID                    f_trp_rec_Fc03_m_txs_n_stp(VOID *);
VOID                    f_trp_rec_Fc03_r(VOID *);
VOID                    f_trp_rec_Fc03_r_3g(VOID *);
ET_REC_C03_EVT          f_trp_rec_Fc03_r_evt_cnv(VOID *, USHORT *, USHORT *);
ET_REC_C03_EVT          f_trp_rec_Fc03_r_evt_cnv_3g(VOID *, USHORT *, USHORT *);
//USHORT                  f_trp_rec_Fc03_t_bnd_get();
//VOID                    f_trp_rec_Fc03_t_bnd_set(USHORT );
//VOID                    f_trp_rec_Fc03_t_bndbrc_set();
USHORT                  f_trp_rec_Fc03_t_brc_get();
VOID                    f_trp_rec_Fc03_t_brc_set(USHORT );
//VOID                    f_trp_rec_Fc03_t_cmnid_get(T_DATA_COMMONID *);
//VOID                    f_trp_rec_Fc03_t_cmnid_set(T_DATA_COMMONID);
UINT                    f_trp_rec_Fc03_t_crg_red(ET_REC_CRK, UINT *);
UINT                    f_trp_rec_Fc03_t_crg_wrt(ET_REC_CRK, UINT);
VOID                    f_trp_rec_Fc03_t_crrof_set();
VOID                    f_trp_rec_Fc03_t_crron_set();
VOID                    f_trp_rec_Fc03_t_crrof_set_3g();
VOID                    f_trp_rec_Fc03_t_crron_set_3g();
ET_REC_C03_EVT          f_trp_rec_Fc03_t_evt_get();
VOID                    f_trp_rec_Fc03_t_evt_set(ET_REC_C03_EVT);
VOID                    f_trp_rec_Fc03_t_evt_set_3g(ET_REC_C03_EVT);
USHORT                  f_trp_rec_Fc03_t_frq_get();
VOID                    f_trp_rec_Fc03_t_frq_set(USHORT);
VOID                    f_trp_rec_Fc03_t_ip_recrrstsntc_snd();
VOID                    f_trp_rec_Fc03_t_ip_txrrsp_snd(USHORT);
VOID                    f_trp_rec_Fc03_t_ip_txsrsp_snd(USHORT);
//VOID                    f_trp_rec_Fc03_t_it_nmcaprc_snd(USHORT);
//VOID                    f_trp_rec_Fc03_t_it_txraprc_snd();
//VOID                    f_trp_rec_Fc03_t_it_txsaprc_snd();
VOID                    f_trp_rec_Fc03_t_l3_txrreq_snd();
VOID                    f_trp_rec_Fc03_t_l3_txsreq_snd();
VOID                    f_trp_rec_Fc03_t_l3_txrreq_snd_3g();
VOID                    f_trp_rec_Fc03_t_l3_txsreq_snd_3g();
//ET_REC_C03_STN          f_trp_rec_Fc03_t_otrstn_get(USHORT, USHORT);
//VOID                    f_trp_rec_Fc03_t_otrstn_set(USHORT, USHORT, ET_REC_C03_STN);
ET_REC_C03_STN          f_trp_rec_Fc03_t_ownstn_get();
ET_REC_C03_STN          f_trp_rec_Fc03_t_ownstn_get_3g();
VOID                    f_trp_rec_Fc03_t_ownstn_set(ET_REC_C03_STN);
VOID                    f_trp_rec_Fc03_t_ownstn_set_3g(ET_REC_C03_STN);
//VOID                    f_trp_rec_Fc03_t_recrralm_add();
//VOID                    f_trp_rec_Fc03_t_recrralm_del();
//UINT                    f_trp_rec_Fc03_t_repid_get();
//VOID                    f_trp_rec_Fc03_t_repid_set(UINT);
USHORT                  f_trp_rec_Fc03_t_sct_get();
VOID                    f_trp_rec_Fc03_t_sct_set(USHORT);
USHORT                  f_trp_rec_Fc03_t_sndant_get();
VOID                    f_trp_rec_Fc03_t_sndant_set(USHORT);
VOID                    f_trp_rec_Fc03_t_tc_ini();
VOID                    f_trp_rec_Fc03_t_tc_ini_3g();
VOID                    f_trp_rec_Fc03_t_tc_txr_dec();
USHORT                  f_trp_rec_Fc03_t_tc_txr_get();
VOID                    f_trp_rec_Fc03_t_tc_txr_dec_3g();
USHORT                  f_trp_rec_Fc03_t_tc_txr_get_3g();
VOID                    f_trp_rec_Fc03_t_tc_txr_ini();
VOID                    f_trp_rec_Fc03_t_tc_txr_ini_3g();
VOID                    f_trp_rec_Fc03_t_tc_txs_dec();
USHORT                  f_trp_rec_Fc03_t_tc_txs_get();
VOID                    f_trp_rec_Fc03_t_tc_txs_dec_3g();
USHORT                  f_trp_rec_Fc03_t_tc_txs_get_3g();
VOID                    f_trp_rec_Fc03_t_tc_txs_ini();
VOID                    f_trp_rec_Fc03_t_tc_txs_ini_3g();
UINT                    f_trp_rec_Fc03_t_timer_ccl(ET_REC_TMK);
UINT                    f_trp_rec_Fc03_t_timer_ent(ET_REC_TMK);
UINT                    f_trp_rec_Fc03_t_timer_ccl_3g(ET_REC_TMK);
UINT                    f_trp_rec_Fc03_t_timer_ent_3g(ET_REC_TMK);
UINT*                   f_trp_rec_Rc03_m_use_flg_get(VOID*, USHORT);

VOID                    f_trp_rec_Rc03_m_idle_sts(VOID*);
VOID                    f_trp_rec_Rc03_m_idle_sts_3g(VOID*);

VOID                    f_trp_rec_Rc03_m_idle_txs(VOID*);
VOID                    f_trp_rec_Rc03_m_idle_txs_3g(VOID*);
VOID                    f_trp_rec_Rc03_m_rst(VOID*);
VOID                    f_trp_rec_Rc03_m_rst_3g(VOID*);
VOID                    f_trp_rec_Rc03_m_txr_to(VOID*);
VOID                    f_trp_rec_Rc03_m_txr_to_3g(VOID*);
VOID                    f_trp_rec_Rc03_m_txr_txr(VOID*);
VOID                    f_trp_rec_Rc03_m_txr_txr_3g(VOID*);
VOID                    f_trp_rec_Rc03_m_txs_to(VOID*);
VOID                    f_trp_rec_Rc03_m_txs_to_3g(VOID*);
VOID                    f_trp_rec_Rc03_m_txs_txs(VOID*);
VOID                    f_trp_rec_Rc03_m_txs_txs_3g(VOID*);
VOID                    f_trp_rec_Fc03_m_txr_idl_fw(VOID*);
VOID                    f_trp_rec_Fc03_m_txr_idl_fw_3g(VOID*);
VOID                    f_trp_rec_Fc03_m_txs_idl_rcv(VOID*);
VOID                    f_trp_rec_Fc03_m_txr_idl_rcv(VOID*);
VOID                    f_trp_rec_Fc03_m_txs_idl_rcv_3g(VOID*);
VOID                    f_trp_rec_Fc03_m_txr_idl_rcv_3g(VOID*);
VOID                    f_trp_rec_Fc03_m_txs_idl_fw(VOID*);
VOID                    f_trp_rec_Fc03_m_txs_idl_fw_3g(VOID*);
VOID                    f_trp_rec_Rc03_m_txs_fw(VOID*);
VOID                    f_trp_rec_Rc03_m_txs_fw_3g(VOID*);
VOID                    f_trp_rec_Fc03_m_caronoff_idl_rcv(VOID*);
VOID                    f_trp_rec_Fc03_m_caronoff_idl_fw(VOID*);
VOID 					f_trp_rec_Fc03_m_carstsrep_idl_fw(VOID*);
VOID 					f_trp_rec_Fc03_m_carstsrep_idl_rcv(VOID*);


VOID                    f_trp_rec_Fcom_r(VOID *);
VOID                    f_trp_rec_Fcom_r_main();
VOID                    f_trp_rec_Fcom_t_ampslt_cpr_tbl_set(USHORT, USHORT, USHORT);
VOID                    f_trp_rec_Fcom_t_bcdtim_get(T_REC_BCDTIM *);
USHORT                  f_trp_rec_Fcom_t_brc_get(USHORT, USHORT);
ET_REC_C02_STN          f_trp_rec_Fcom_t_c02stn_get(USHORT);
VOID                    f_trp_rec_Fcom_t_c02stn_set(USHORT, ET_REC_C02_STN);
ET_REC_C03_STN          f_trp_rec_Fcom_t_c03stn_get(USHORT, USHORT);
ET_REC_C03_STN          f_trp_rec_Fcom_t_c03stn_get_3g(USHORT, USHORT);
VOID                    f_trp_rec_Fcom_t_c03stn_set(USHORT, USHORT, ET_REC_C03_STN);
VOID                    f_trp_rec_Fcom_t_c03stn_set_3g(USHORT, USHORT, ET_REC_C03_STN);
VOID                    f_trp_rec_Fcom_t_cpr_hton_cnv(VOID *, UINT);
VOID                    f_trp_rec_Fcom_t_cpr_ntoh_cnv(VOID *, UINT);
//UINT                    f_trp_rec_Fcom_t_crg_red(ET_REC_CRK, USHORT, UINT *);
UINT                    f_trp_rec_Fcom_t_crg_wrt(ET_REC_CRK, USHORT, UINT); 
VOID                    f_trp_rec_Fcom_t_elm_key_cnv(T_REC_TIMELM *, bpf_ru_hrtm_key_inf_t *);
UINT                    f_trp_rec_Fcom_t_hw_refrcrst_snd(USHORT);
VOID                    f_trp_rec_Fcom_t_ip_recrrstsntc_snd(USHORT);
//INT                     f_trp_rec_Fcom_t_ip_snd(VOID *,UINT, USHORT, UINT);
VOID                    f_trp_rec_Fcom_t_it_2ndrsp_snd(USHORT);
//VOID                    f_trp_rec_Fcom_t_it_actchgind_snd(USHORT, USHORT);
//VOID                    f_trp_rec_Fcom_t_it_actsetreq_snd(USHORT);
VOID                    f_trp_rec_Fcom_t_it_frcstpntc_snd(USHORT, USHORT, T_RRH_RESTOP_FACTOR);
VOID                    f_trp_rec_Fcom_t_it_frcstpntc_snd_3g(USHORT, USHORT, T_RRH_RESTOP_FACTOR);
VOID                    f_trp_rec_Fcom_t_it_redlstpind_snd(USHORT, USHORT, USHORT);
VOID                    f_trp_rec_Fcom_t_it_refilreq_snd(USHORT,USHORT,USHORT);
VOID                    f_trp_rec_Fcom_t_it_reprmreq_snd(USHORT);
//VOID                    f_trp_rec_Fcom_t_it_rescmpntc_snd(USHORT, USHORT);
//VOID                    f_trp_rec_Fcom_t_it_sigcmpntc_snd();
INT                     f_trp_rec_Fcom_t_it_snd(VOID *,UINT, USHORT, UINT);
//VOID                    f_trp_rec_Fcom_t_it_thrinirsp_snd(USHORT);
//VOID                    f_trp_rec_Fcom_t_it_thrsupcmp_snd(USHORT);
VOID                    f_trp_rec_Fcom_t_key_elm_cnv(UINT, T_REC_TIMELM *);
//UINT                    f_trp_rec_Fcom_t_l3_nmcreq_snd(USHORT, USHORT, USHORT, USHORT);
UINT                    f_trp_rec_Fcom_t_l3_recrdctlreq_snd(USHORT, USHORT, USHORT);
//UINT                    f_trp_rec_Fcom_t_l3_recrddiareq_snd(USHORT, USHORT);
UINT                    f_trp_rec_Fcom_t_l3_recrdstsreq_snd(USHORT, USHORT);
UINT                    f_trp_rec_Fcom_t_l3_recrrstsreq_snd(USHORT);
UINT                    f_trp_rec_Fcom_t_l3_reeqpinfreq_snd(USHORT);
//UINT                    f_trp_rec_Fcom_t_l3_reloggetreq_snd(USHORT, USHORT, CHAR[]);
//UINT                    f_trp_rec_Fcom_t_l3_relogstpreq_snd(USHORT);
//UINT                    f_trp_rec_Fcom_t_l3_reprtctlreq_snd(USHORT, USHORT, USHORT);
UINT                    f_trp_rec_Fcom_t_l3_reprtstsreq_snd(USHORT);
UINT                    f_trp_rec_Fcom_t_l3_rerstreq_snd(USHORT, USHORT, USHORT);
/* 2021/03/03 M&C) Update src based on No.27 - Delete start */
// UINT                    f_trp_rec_Fcom_t_l3_rerstreq_snd_3g(USHORT, USHORT, USHORT);
/* 2021/03/03 M&C) Update src based on No.27 - Delete end */
UINT                    f_trp_rec_Fcom_t_l3_resltinfreq_snd(USHORT);
UINT                    f_trp_rec_Fcom_t_l3_restsreq_snd(USHORT);
UINT                    f_trp_rec_Fcom_t_l3_retimsetreq_snd(USHORT);
UINT                    f_trp_rec_Fcom_t_l3_retimsetreq_snd_3g(USHORT);
//UINT                    f_trp_rec_Fcom_t_l3_reverreq_snd(USHORT, USHORT, USHORT, USHORT);
UINT                    f_trp_rec_Fcom_t_l3_tofvalreq_snd(USHORT);
UINT                    f_trp_rec_Fcom_t_l3_txrreq_snd(USHORT, USHORT);
UINT                    f_trp_rec_Fcom_t_l3_txrreq_snd_3g(USHORT, USHORT);
UINT                    f_trp_rec_Fcom_t_l3_txsreq_snd(USHORT, USHORT);
UINT                    f_trp_rec_Fcom_t_l3_txsreq_snd_3g(USHORT, USHORT);
UINT                    f_trp_rec_Fcom_t_nrg_red(ET_REC_NRK, UINT *);
//UINT                    f_trp_rec_Fcom_t_nrg_wrt(ET_REC_NRK, UINT);
//VOID                    f_trp_rec_Fcom_t_recrralm_add(USHORT, USHORT, USHORT);
//VOID                    f_trp_rec_Fcom_t_recrralm_del(USHORT, USHORT, USHORT);
//VOID                    f_trp_rec_Fcom_t_recrrsts_get(USHORT, VOID *);
VOID                    f_trp_rec_Fcom_t_reghis_add(UINT *, UINT);
VOID                    f_trp_rec_Fcom_t_reghis_dsp(FILE *);
//VOID                    f_trp_rec_Fcom_t_remng_get(VOID *, USHORT);
UINT                    f_trp_rec_Fcom_t_runhis_add(UCHAR, VOID *);
VOID                    f_trp_rec_Fcom_t_runhis_del(USHORT);
VOID                    f_trp_rec_Fcom_t_runhis_dsp(FILE *);
VOID                    f_trp_rec_Fcom_t_sta_dsp(INT *, FILE *);
VOID                    f_trp_rec_Fcom_t_runhis_dmp(FILE* stream,UINT dmp_idx);
VOID                    f_trp_rec_Fcom_t_stn_dsp(FILE *);
UINT                    f_trp_rec_Fcom_t_timmng_tbl_add(bpf_ru_hrtm_key_inf_t *, UINT);
UINT                    f_trp_rec_Fcom_t_timmng_tbl_del(bpf_ru_hrtm_key_inf_t *);
UINT                    f_trp_rec_Fcom_t_timmng_tbl_sch(bpf_ru_hrtm_key_inf_t *);
VOID                    f_trp_rec_Fcom_t_tim_dsp(FILE *);
bpf_ru_hrtm_notify_type_t  f_trp_rec_Fcom_t_tmk_ntc_cnv(ET_REC_TMK);
UINT                    f_trp_rec_Fcom_t_tmk_val_cnv(ET_REC_TMK);
VOID                    f_trp_rec_Fcom_t_tmk_val_tbl_gen();
VOID                    f_trp_rec_Fcom_t_ushortsort_asc(USHORT, USHORT, USHORT *);

VOID                    f_trp_rec_Fcom_t_l3_LogStore( USHORT, USHORT, USHORT );
VOID                    f_trp_rec_Fcom_t_l3_LogSave( USHORT, USHORT, USHORT );
VOID                    f_trp_rec_Fcom_t_l3_LogClr( USHORT );
VOID                    f_trp_rec_Fcom_t_tbldump_dsp(FILE *);
VOID                    f_trp_rec_Fcom_t_tbldump_dsp2(FILE *);

/* FHM add start : function for 3G systerm */
VOID                    f_trp_rec_Fc02_m_res_tofval_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reeqpinf_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_resltinf_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_rests_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrrsts_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_tofval_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reeqpinf_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_resltinf_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_rests_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_use_rests_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrrsts_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrrsts_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_tofval_stp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_reeqpinf_stp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_resltinf_stp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_rests_stp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_use_rests_stp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrrsts_stp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrrsts_stp_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_use_rests_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrrsts_sta_3g(VOID *);
UINT                    f_trp_rec_Fc02_t_recrrsts_a_ini_3g(USHORT);
UINT                    f_trp_rec_Fc02_t_resltinfsc_ini_3g(USHORT);
VOID                    f_trp_rec_Fc02_m_res_reprtsts_sta_3g( VOID *);
VOID                    f_trp_rec_Fc02_m_res_reprtsts_end_3g( VOID *);
VOID                    f_trp_rec_Fc02_m_res_reprtsts_stp_3g( VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrdssc_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_use_recrdssc_end_3g(VOID* );
VOID                    f_trp_rec_Fc02_t_use_recrdstsreq_ini_3g(USHORT);
UINT                    f_trp_rec_Fc02_t_recrdssc_i_ini_3g( USHORT, USHORT); 
VOID                    f_trp_rec_Fc02_m_use_recrdssc_stp_3g( VOID*);
VOID                    f_trp_rec_Fc02_m_res_recrdssc_stp_3g(VOID *);
VOID                    f_trp_rec_Fc02_t_refbstsntc_snd_3g(VOID *);
VOID                    f_trp_rec_Fc02_t_use_recrdstsreq_dec_3g(USHORT);
USHORT                  f_trp_rec_Fc02_t_use_recrdstsreq_get_3g(USHORT);
VOID                    f_trp_rec_Fc02_m_use_recrdssc_sta_3g(VOID *);

VOID                    f_trp_rec_Fc02_t_it_reprtstsprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_recrdsscprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_refrcrstprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_reeqpinfprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_recrrstsprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_mtsetfprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_mtsetfprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_resltinfprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_resltinfprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_restsprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_tofvalprc_snd_3g();
//VOID                    f_trp_rec_Fc02_t_it_actsetprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_tofvalprc_snd_3g();

VOID                    f_trp_rec_Fc02_t_use_recrrstsreq_dec_3g();
USHORT                  f_trp_rec_Fc02_t_use_recrrstsreq_get_3g();


VOID                    f_trp_rec_Fc02_t_l3_reprtstsreq_snd_3g();
VOID                    f_trp_rec_Fc02_t_l3_resltinfreq_snd_3g();
VOID                    f_trp_rec_Fc02_t_l3_restsreq_snd_3g();
VOID                    f_trp_rec_Fc02_t_l3_tofvalreq_snd_3g();
VOID                    f_trp_rec_Fc02_t_l3_recrrstsreq_snd_3g();
// VOID                    f_trp_rec_Fc02_t_l3_reeqpinfreq_snd_3g(); /* 2021/01/19 M&C)DuongMX Item No.20 - RE Device Configuration Information Monitoring (delete) */


USHORT                  f_trp_rec_Fc02_t_tc_restsreq_get_3g();
USHORT                  f_trp_rec_Fc02_t_tc_refrcrstsnd_get_3g();
USHORT                  f_trp_rec_Fc02_t_tc_reprtstsreq_get_3g();
VOID                    f_trp_rec_Fc02_t_tc_reeqpinfreq_dec_3g();
USHORT                  f_trp_rec_Fc02_t_tc_recrrstsreq_get_3g();
VOID                    f_trp_rec_Fc02_t_tc_recrrstsreq_dec_3g();
USHORT                  f_trp_rec_Fc02_t_tc_reeqpinfreq_get_3g();
VOID                    f_trp_rec_Fc02_t_tc_reprtstsreq_dec_3g();
USHORT                  f_trp_rec_Fc02_t_tc_resltinfreq_get_3g();
VOID                    f_trp_rec_Fc02_t_tc_resltinfreq_dec_3g();
VOID                    f_trp_rec_Fc02_t_tc_restsreq_dec_3g();
USHORT                  f_trp_rec_Fc02_t_tc_tofvalreq_get_3g();
VOID                    f_trp_rec_Fc02_t_tc_tofvalreq_dec_3g();


VOID                    f_trp_rec_Fc02_t_use_restsreq_ini_3g();
 	
UINT                    f_trp_rec_Fcom_t_l3_tofvalreq_snd_3g(USHORT);
// UINT                    f_trp_rec_Fcom_t_l3_restsreq_snd_3g(USHORT); /* 2021/01/19 M&C)DuongMX Item No.21 - RE Floating Condition Monitoring (LTE) (delete) */
UINT                    f_trp_rec_Fcom_t_l3_resltinfreq_snd_3g(USHORT);
UINT                    f_trp_rec_Fcom_t_l3_reprtstsreq_snd_3g(USHORT);
// UINT                    f_trp_rec_Fcom_t_l3_reeqpinfreq_snd_3g(USHORT); /* 2021/01/19 M&C)DuongMX Item No.20 - RE Device Configuration Information Monitoring (delete) */
UINT                    f_trp_rec_Fcom_t_l3_recrrstsreq_snd_3g(USHORT);
UINT                    f_trp_rec_Fc02_t_recrrsts_chk_3g(VOID *);
// UINT                    f_trp_rec_Fc02_t_reeqpinf_chk_3g(VOID *); /* 2021/01/19 M&C)DuongMX Item No.20 - RE Device Configuration Information Monitoring (delete) */
UINT                    f_trp_rec_Fc02_t_rests_ini_3g(USHORT ink);
// UINT                    f_trp_rec_Fc02_t_rests_set_3g( VOID * );
UINT                    f_trp_rec_Fc02_t_rests_ini_3g( USHORT ink);
UINT                    f_trp_rec_Fc02_t_tofval_set_3g( VOID * );
UINT                    f_trp_rec_Fc02_t_rests_set_3g( VOID * );
UINT                    f_trp_rec_Fc02_t_resltinf_set_3g( VOID * );
UINT                    f_trp_rec_Fc02_t_reeqpinf_set_3g( VOID * );
VOID                    f_trp_rec_Fc02_t_recrrsts_set_3g( VOID * );
UINT                    f_trp_rec_Fc02_t_cprpar_chk_3g( VOID * );
UINT                    f_trp_rec_Fc02_t_timer_ccl_3g(ET_REC_TMK, USHORT);
UINT                    f_trp_rec_Fc02_t_timer_ent_3g(ET_REC_TMK, USHORT);
UINT                    f_trp_rec_Fc02_t_resltinfsc_ini_3g(USHORT);
INT						f_trp_rec_Fcom_t_l2_msg_fw(VOID*,USHORT);
INT						f_trp_rec_Fcom_t_l3_msg_fw(VOID*,USHORT);
USHORT                  f_trp_rec_Fc02_t_ownact_get_3g();
VOID                    f_trp_rec_Fc02_t_ownact_set_3g(USHORT);
ET_REC_C02_STN          f_trp_rec_Fc02_t_ownstn_get_3g();
VOID                    f_trp_rec_Fc02_t_ownstn_set_3g(ET_REC_C02_STN);
ET_REC_C02_STN          f_trp_rec_Fcom_t_c02stn_get_3g(USHORT);
VOID                    f_trp_rec_Fcom_t_c02stn_set_3g(USHORT, ET_REC_C02_STN);
ET_REC_C02_EVT          f_trp_rec_Fc02_r_evt_cnv_3g(VOID *, USHORT *);
VOID                    f_trp_rec_Fc02_t_evt_set_3g(ET_REC_C02_EVT);
VOID                    f_trp_rec_Fc02_r_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_recrdssc_sta_3g(VOID *);
UINT                    f_trp_rec_Fc02_t_recrdssc_i_set_3g(USHORT, VOID *);
// [16B] add start
VOID                    f_trp_rec_Fc02_t_recrdssc_i_merge_3g();
// [16B] add end
USHORT                  f_trp_rec_Fc02_t_tc_recrdstsreq_get_3g(USHORT);
VOID                    f_trp_rec_Fc02_t_l3_recrdsscreq_snd_3g(USHORT);
VOID                    f_trp_rec_Fc02_t_tc_recrdstsreq_dec_3g(USHORT);
UINT                    f_trp_rec_Fc02_t_recrdsts_chk_3g(VOID *);
UINT                    f_trp_rec_Fcom_t_l3_recrdstsreq_snd_3g(USHORT, USHORT);

INT                     f_trp_rec_Fcom_t_msg_snd(VOID *,UINT, UINT, UINT, UINT);

VOID                    f_trp_rec_Fc02_m_res_usrefil_end(VOID *);
VOID                    f_trp_rec_Fc02_m_use_refilup_start_req(VOID *);
VOID                    f_trp_rec_Fc02_m_use_refilup_start_res(USHORT, USHORT);

VOID					f_trp_rec_Fc02_m_use_reprtsts_sta(VOID*);
VOID					f_trp_rec_Fc02_m_use_reprtsts_end(VOID*);
VOID					f_trp_rec_Fc02_m_use_reprtsts_stp(VOID*);

VOID                    f_trp_rec_FcXX_m_use_ReCardReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_ReTimSet_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_reverreq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_reverres_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_UFirmFileReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_UFirmFileRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_UFilSndFinNtc_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_UFilSndFinRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_UFileInfReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_UFileInfRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FilSndFinNtc_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FilSndFinRes_fw(VOID *);
USHORT                  f_trp_rec_FcXX_m_frcstp_refilnewstp(VOID*);
VOID                    f_trp_rec_FcXX_m_use_FileDatSnd_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_UFileDatSnd_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FileDatSndInd_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FileInfReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FileInfRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FirmFileReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FirmFileRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_SvMtForceRelReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_ExtDatSndReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_refldcntrsp_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_refldfrrelcntreq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_resvmtcntrsp_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_resvmtdatsndreq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_SvMtDatRcvRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_SvMtCntRelRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_ExtRcvtSndRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FldMtRelRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_SvMtForceRelRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_ExtDatSndRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_refldcntreq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_refldfrrelcntrsp_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_resvmtcntreq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_resvmtdatsndrsp_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_SvMtDatRcvReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_SvMtCntRelReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_ExtDatRcvReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FldMtRelReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FldFilSndReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FldFilSndRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FldCmdRcvReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_FldCmdRcvRes_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_ReDisconnectRes(VOID *);
VOID                    f_trp_rec_ReDisConn_AutoRst_lte(USHORT);
VOID                    f_trp_rec_ReDisConn_AutoRst_3g(USHORT);
VOID                    f_trp_rec_FcXX_m_ReSelfReset_PreventOff_Req(VOID *);


VOID                    f_trp_rec_FcXX_m_use_CardInfoReq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_CardInfoRsp_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_ReForceRst_Ind(VOID *);
VOID                    f_trp_rec_FcXX_m_use_ReForceStp_Ind(VOID *);
VOID                    f_trp_rec_FcXX_m_use_fldcomsndreq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_fldfilrcvreq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_rerstreq_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_fldcomsndres_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_use_fldfilrcvres_fw(VOID *);
VOID                    f_trp_rec_FcXX_m_mtsetf_req(VOID *);
VOID                    f_trp_rec_FcXX_m_mtsetf_res(VOID *);
VOID 			f_trp_rec_FcXX_m_use_TroublelogGetReq_fw(VOID*);
VOID 			f_trp_rec_FcXX_m_use_TroublelogGetRes_fw(VOID*);
VOID 			f_trp_rec_FcXX_m_use_TroubleLogFilSndFinNtc_fw(VOID*);
VOID 			f_trp_rec_FcXX_m_use_TroubleLogFilSndFinRes_fw(VOID*);
VOID 			f_trp_rec_FcXX_m_use_TroubleLogFilSndNtc_fw(VOID*);
VOID 			f_trp_rec_FcXX_m_use_TroublelogGetStopReq_fw(VOID*);
VOID 			f_trp_rec_FcXX_m_use_TroublelogGetStopRes_fw(VOID*);
/* 2021/02/18 M&C)Tri.hn - Update based on No67 - Sequence 3-30 - Add start */
VOID 					f_trp_rec_FcXX_m_use_recarddiareq_fw(VOID *);
VOID 					f_trp_rec_FcXX_m_use_recarddiares_fw(VOID *);
/* 2021/02/18 M&C)Tri.hn - Update based on No67 - Sequence 3-30 - Add start */

/* 2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-48) - add start */
VOID					f_trp_rec_FcXX_m_use_ftpportsetreq_fw(VOID*);
VOID					f_trp_rec_FcXX_m_use_ftpportsetres_fw(VOID*);
/* 2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-48) - add end*/
#ifdef FHM_DEBUG_FOR_IT1
VOID 					f_trp_rec_FcDB_r(VOID* bufp);
#endif
VOID 					f_trp_rec_FcXX_r(VOID* bufp);
USHORT 					f_trp_rec_Fc02_t_mtgetsts( USHORT awReNo );
//VOID 					f_trp_rec_Fc02_m_mtsetf_rcv( VOID* bufp );
VOID 					f_trp_rec_Fc02_m_mtsetf_res_snd( USHORT awResutl);
VOID 					f_trp_rec_Fc02_t_mtsetsts( USHORT awReNo, USHORT awStn);
VOID 					f_trp_rec_Fc02_t_tc_mtsetffactor_set(USHORT, USHORT);
USHORT 					f_trp_rec_Fc02_t_tc_mtsetffactor_get(USHORT);
VOID 					f_trp_rec_Fc02_m_res_mtsetres_rec();
VOID					f_trp_rec_Fc02_m_mtsetf_req_snd(USHORT);
VOID 					f_trp_rec_Fc02_m_res_mtsetfres_fhm();
VOID 					f_trp_rec_Fc02_m_res_mtsetsres_fhm();
USHORT 					f_trp_rec_Fc02_t_tc_mtsetfreq_get();
VOID 					f_trp_rec_Fc02_t_tc_mtsetfreq_set( USHORT awRetryCnt);
USHORT 					f_trp_rec_Fc02_t_tc_mtsetsreq_get();
VOID 					f_trp_rec_Fc02_t_tc_mtsetsreq_set( USHORT awRetryCnt);
VOID 					f_trp_rec_Fc02_m_mtsetf_fhm_to( VOID* bufp );
VOID 					f_trp_rec_Fc02_m_mtsets_fhm_to( VOID* bufp );
VOID 					f_trp_rec_FcXX_m_mtsetf_rec_to( VOID* bufp );
VOID 					f_trp_rec_Fc02_t_it_mtsetsprc_snd();

USHORT 					f_trp_rec_Fc02_t_mtgetsts_3g( USHORT awReNo );
//VOID 					f_trp_rec_Fc02_m_mtsetf_rcv_3g( VOID* bufp );
VOID 					f_trp_rec_Fc02_m_mtsetf_res_snd_3g( USHORT awResutl);
VOID 					f_trp_rec_Fc02_t_mtsetsts_3g( USHORT awReNo, USHORT awStn);
VOID 					f_trp_rec_Fc02_t_tc_mtsetffactor_set_3g(USHORT, USHORT);
USHORT 					f_trp_rec_Fc02_t_tc_mtsetffactor_get_3g(USHORT);
VOID 					f_trp_rec_Fc02_m_res_mtsetres_rec_3g();
VOID					f_trp_rec_Fc02_m_mtsetf_req_snd_3g(USHORT);
VOID 					f_trp_rec_Fc02_m_res_mtsetfres_fhm_3g();
VOID 					f_trp_rec_Fc02_m_res_mtsetsres_fhm_3g();
USHORT 					f_trp_rec_Fc02_t_tc_mtsetfreq_get_3g();
VOID 					f_trp_rec_Fc02_t_tc_mtsetfreq_set_3g( USHORT awRetryCnt);
USHORT 					f_trp_rec_Fc02_t_tc_mtsetsreq_get_3g();
VOID 					f_trp_rec_Fc02_t_tc_mtsetsreq_set_3g( USHORT awRetryCnt);
VOID 					f_trp_rec_Fc02_m_mtsetf_fhm_to_3g( VOID* bufp );
VOID 					f_trp_rec_Fc02_m_mtsets_fhm_to_3g( VOID* bufp );
VOID 					f_trp_rec_Fc02_m_mtsetf_rec_to_3g( VOID* bufp );
VOID 					f_trp_rec_Fc02_t_it_mtsetsprc_snd_3g();
VOID                    f_trp_rec_Fc02_t_it_mtsetfreq_snd_3g();

VOID                    f_trp_rec_Fc02_m_res_mtsetf_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_mtsetf_sta_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_mtsets_end_3g(VOID *);
VOID                    f_trp_rec_Fc02_m_res_mtsets_sta_3g(VOID *);

VOID 					f_trp_rec_Fc02_t_it_mtsetsreq_snd_3g( );

USHORT                  f_trp_rec_Fcom_t_cpr_ampoft_cnv_3g(USHORT);

VOID					f_trp_rec_Fc02_t_tc_tofvalreq_ini();
VOID					f_trp_rec_Fc02_t_tc_tofvalreq_ini_3g();
UINT					f_trp_rec_Fcom_t_timmng_tbl_get(T_REC_TIMELM);
UINT					f_trp_rec_Fcom_t_timmng_tbl03_get(T_REC_TIMELM);
int BPF_RU_MACC_BCMP(const void *data1, const void *data2, unsigned int n, int *result);
int BPF_RU_MACC_BCOPY(void *dst, const void *src, unsigned int n);

/*	koshida try	*/
UINT					f_trp_rec_FcXX_timer_ent(ET_REC_TMK, USHORT, USHORT);

#endif
/** @} */
