/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_ext.h
 * @brief  extern
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * @date   2009/01/17 FFCS)Shihzh modify for BI-TBD-557
 * @date   2011/12/11 FJT)K.Koshida  DMICRO-008
 * @date   2015/09/30 TDIPS)sasaki update for FHM
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2015
 */
/****************************************************************************/
#ifndef _F_TRP_REC_EXT_h
#define _F_TRP_REC_EXT_h

/*! @addtogroup TRIF_REC
 * @{ */

/*******/
/* RAM */
/*******/
extern USHORT			f_trp_rec_Fc02_evt;
extern USHORT			f_trp_rec_Fc02_3g_evt;
extern USHORT			f_trp_rec_Fc02_carr_evt;
extern USHORT			f_trp_rec_Fc02_carr_3g_evt;
/**********************/
/* RE状態管理テーブル */
/**********************/
extern T_REC_REMSC_TBL f_trp_rec_Wcom_remsc_tbl;
                                          /*!< RE状態管理(セクタ部)テーブル */
extern T_REC_REMCM_TBL f_trp_rec_Wcom_remcm_tbl;
                                            /*!< RE状態管理(共通部)テーブル */
extern T_REC_REMSC_TBL_3G f_trp_rec_Wcom_remsc_tbl_3g;
                                          /*!< RE状態管理(セクタ部)テーブル */
extern T_REC_REMCM_TBL f_trp_rec_Wcom_remcm_tbl_3g;
                                            /*!< RE状態管理(共通部)テーブル */
extern T_REC_REMSC_TBL f_trp_rec_Wcom_remsc_tbk;
                                      /*!< RE状態管理(セクタ部)テーブル(BK) */
extern T_REC_REMCM_TBL f_trp_rec_Wcom_remcm_tbk;
                                        /*!< RE状態管理(共通部)テーブル(BK) */
extern T_REC_REMSC_TBL_3G f_trp_rec_Wcom_remsc_tbk_3g;
                                      /*!< RE状態管理(セクタ部)テーブル(BK) */
extern T_REC_REMCM_TBL f_trp_rec_Wcom_remcm_tbk_3g;
                                        /*!< RE状態管理(共通部)テーブル(BK) */
extern T_REC_SEC_FBSTS_TBL f_trp_rec_Wcom_secfbsts_tbl;
extern T_REC_SEC_FBSTS_TBL f_trp_rec_Wcom_secfbsts_tbl_3g;

extern T_REC_OBJMNG_TBL		f_trp_rec_WobjMngTbl;
extern T_REC_OBJMNG_TBL		f_trp_rec_WobjMngTbl_3g;
/**************************/
/* クラス状態管理テーブル */
/**************************/
extern T_REC_C02_STSMNG_TBL f_trp_rec_Wc02_stsmng_tbl;
                                                   /*!< C02状態管理テーブル */
extern T_REC_C02_STSMNG_TBL f_trp_rec_Wc02_stsmng_tbl_3g;
                                                   /*!< C02状態管理テーブル */
extern T_REC_C03_STSMNG_TBL f_trp_rec_Wc03_stsmng_tbl;
                                                   /*!< C03状態管理テーブル */
//extern T_REC_CFF_STSMNG_TBL f_trp_rec_WcFF_stsmng_tbl;
                                                   /*!< CFF状態管理テーブル */
//extern T_REC_NESTSMNG_TBL   f_trp_rec_Wcom_nestsmng_tbl;
//                                                   /*!< NE Switch 状態管理テーブル */
extern T_REC_REAUTRST_TBL   f_trp_rec_Wcom_reautrst_tbl;
                                                   /*!< RE Auto reset table */
extern T_REC_C03_STSMNG_TBL_3G f_trp_rec_Wc03_stsmng_tbl_3g;
/****************************/
/* クラスパラメータテーブル */
/****************************/
extern T_REC_C02_PRM_TBL f_trp_rec_Wc02_prm_tbl;
                                                 /*!< C02パラメータテーブル */
extern T_REC_C02_PRM_TBL f_trp_rec_Wc02_prm_tbl_3g;
                                                 /*!< C02パラメータテーブル */
extern T_REC_C03_PRM_TBL f_trp_rec_Wc03_prm_tbl;
                                                 /*!< C03パラメータテーブル */
//extern T_REC_C04_PRM_TBL f_trp_rec_Wc04_prm_tbl;
//                                                 /*!< C04パラメータテーブル */
//extern T_REC_C05_PRM_TBL f_trp_rec_Wc05_prm_tbl;
//                                                 /*!< C05パラメータテーブル */
//extern T_REC_C06_PRM_TBL f_trp_rec_Wc06_prm_tbl;
//                                                 /*!< C06パラメータテーブル */
//extern T_REC_C07_PRM_TBL f_trp_rec_Wc07_prm_tbl;
//                                                 /*!< C07パラメータテーブル */
//extern T_REC_C08_PRM_TBL f_trp_rec_Wc08_prm_tbl;
//                                                 /*!< C08パラメータテーブル */
//extern T_REC_CFF_PRM_TBL f_trp_rec_WcFF_prm_tbl;
//                                                 /*!< CFFパラメータテーブル */
extern T_REC_C03_PRM_TBL f_trp_rec_Wc03_prm_tbl_3g;
                                                 /*!< C03パラメータテーブル */

/****************/
/* 変換テーブル */
/****************/
//extern T_REC_AMPSLT_CPR_TBL f_trp_rec_Wcom_ampslt_cpr_tbl;
//                             /*!< AMP番号スロット番号→CPRI番号変換テーブル */
//extern T_REC_CPR_AMP_TBL    f_trp_rec_Wcom_cpr_amp_tbl;
//                                         /*!< CPRI番号→AMP番号変換テーブル */
//extern T_REC_CPR_AMP_TBL    f_trp_rec_Wcom_cpr_amp_tbl_3g;
//                                         /*!< CPRI番号→AMP番号変換テーブル */
extern T_REC_TMK_VAL_TBL    f_trp_rec_Wcom_tmk_val_tbl;
                                      /*!< タイマ種別→タイマ値変換テーブル */
/**********/
/* その他 */
/**********/
extern T_REC_TIMMNG_TBL f_trp_rec_Wcom_timmng_tbl;
                                                    /*!< タイマ管理テーブル */
extern T_REC_RUNHIS_TBL f_trp_rec_Wcom_runhis_tbl;
                                                        /*!< RUNHISテーブル */
extern T_REC_REGHIS_TBL f_trp_rec_Wcom_reghis_tbl;
                                                        /*!< REGHISテーブル */
/*******/
/* ROM */
/*******/
/******************/
/* マトリクス関数 */
/******************/
extern VOID (*const f_trp_rec_Rc01_pre_m_fnc[EC_REC_C01_EVT_MAX][EC_REC_C01_STN_MAX])(VOID*);
                                             /*!< C01(前処理)マトリクス関数 */
extern VOID (*const f_trp_rec_Rc01_pst_m_fnc[EC_REC_C01_EVT_MAX][EC_REC_C01_STN_MAX])(VOID*);
                                             /*!< C01(後処理)マトリクス関数 */
extern VOID (*const f_trp_rec_Rc02_m_fnc[EC_REC_C02_EVT_MAX][EC_REC_C02_STN_MAX])(VOID*);
                                                     /*!< C02マトリクス関数 */
extern VOID (*const f_trp_rec_Rc02_m_fnc_3g[EC_REC_C02_EVT_MAX][EC_REC_C02_STN_MAX])(VOID*);
                                                     /*!< C02マトリクス関数 */
extern VOID (*const f_trp_rec_Rc03_m_fnc[EC_REC_C03_EVT_MAX][EC_REC_C03_STN_MAX])(VOID*);
                                                     /*!< C03マトリクス関数 */
extern VOID (*const f_trp_rec_Rc03_m_3G_fnc[EC_REC_C03_EVT_MAX][EC_REC_C03_STN_MAX])(VOID*);
                                                     /*!< C03マトリクス関数 */
extern VOID (*const f_trp_rec_Rc04_m_fnc[EC_REC_C04_EVT_MAX][EC_REC_C04_STN_MAX])(VOID*);
                                                     /*!< C04マトリクス関数 */
extern VOID (*const f_trp_rec_Rc05_m_fnc[EC_REC_C05_EVT_MAX][EC_REC_C05_STN_MAX])(VOID*);
                                                     /*!< C05マトリクス関数 */
extern VOID (*const f_trp_rec_Rc06_m_fnc[EC_REC_C06_EVT_MAX][EC_REC_C06_STN_MAX])(VOID*);
                                                     /*!< C06マトリクス関数 */
extern VOID (*const f_trp_rec_Rc07_m_fnc[EC_REC_C07_EVT_MAX][EC_REC_C07_STN_MAX])(VOID*);
                                                     /*!< C07マトリクス関数 */
extern VOID (*const f_trp_rec_Rc08_m_fnc[EC_REC_C08_EVT_MAX][EC_REC_C08_STN_MAX])(VOID*);
                                                     /*!< C08マトリクス関数 */
extern VOID (*const f_trp_rec_RcFF_m_fnc[EC_REC_CFF_EVT_MAX][EC_REC_CFF_STN_MAX])(VOID*);
                                                     /*!< CFFマトリクス関数 */

/**********/
/* その他 */
/**********/
extern T_REC_MSGRCV_TBL f_trp_rec_Wcom_msgrcv_tbl;
                                                       /*!< REC Receiving buffer */
extern T_REC_CRA_TBL f_trp_rec_Rcom_cra_tbl;
                                                       /*!< レジスタアドレステーブル */
extern T_REC_NRA_TBL f_trp_rec_Rcom_nra_tbl;
                                                       /*!< レジスタアドレステーブル */
extern T_REC_ERF_TBL f_trp_rec_Rcom_erf_tbl;
                                                       /*!< ERR要因テーブル */
extern T_REC_TMK_NTC_TBL f_trp_rec_Rcom_tmk_ntc_tbl;
                                                       /*!< タイマ種別→通知種別変換テーブル */
extern T_REC_RESLTFB_TBL    f_trp_rec_Rcom_sltfbinf_tbl;
                                                       /*!< RE Function block bit map table */
extern T_REC_REFBTRB_TBL    f_trp_rec_Rcom_fbtrbsts_tbl;
                                                       /*!< RE Function block trouble logical sum table */
extern T_REC_CRRBRCSTS_TBL  f_trp_rec_Rcom_crrbrcsts_tbl;
                                                       /*!< RE carrier&branch status logical sum table */
extern T_REC_LOGTBL_TBL f_trp_rec_Rcom_logtbl_tbl;
                                                       /*!< ログテーブルテーブル */
extern T_REC_IVLPRM_TBL f_trp_rec_Wcom_ivlprm_tbl;
                                                       /*!< Invalid log paramter Table */

extern	USHORT	CarrStateUnmatch[CMD_CPRINO_MAX];

extern	USHORT	RE_AlarmProtectTime_Debug;

extern	INT		RE_AlarmProtectTime;

extern USHORT	RE_multiFlag;

extern UCHAR*	f_trp_rec_fwdata_area;				/*!< shared memory of FW File Data  */

extern USHORT	f_trp_rec_Wcom_MtAdSetRetryCnt[CMD_MAX_SYSNUM];		/*!< MTアドレス設定要求(REC主導)送信回数 */
extern CMT_CPRIF_MTADDSETREQ	f_trp_rec_Wcom_MtAdSetInfo[CMD_MAX_SYSNUM];	/*!< MT address seting info */

extern T_REC_REQRSP_TBL f_trp_rec_Wcom_ReqRspLog_tbl[D_REC_LOGKIND_MAX][CMD_CPRINO_MAX];
extern T_REC_REQRSP_SAVE_TBL f_trp_rec_Wcom_ReqRspLogSave_tbl[D_REC_LOGKIND_MAX][CMD_CPRINO_MAX];

#if defined(OPT_CPRI_L2HARD)
extern UINT crr_reAutoCntReg[CMD_LINK_NUM_T][CMD_SLOTINF_MAX];
#endif

//extern	const	T_REC_FBTYPE2CMNSECINFO_TBL	f_trp_rec_Rcom_fbType2CmnSecInfo_tbl[29];
															/**< f_trp_rec_Rcom_fbType2CmnSecInfo_tbl  */
extern	const	USHORT			f_trp_rec_Rcom_reFbTrbStsCnv_tbl[32];
															/**< f_trp_rec_Rcom_reFbTrbStsCnv_tbl  */
extern	const	USHORT			f_trp_rec_Rcom_reSecFbTrbStsCnv_tbl[32];
															/**< f_trp_rec_Rcom_reSecFbTrbStsCnv_tbl  */
extern	const	USHORT			f_trp_rec_Rcom_fbTrbSts2Bit_tbl[5];
															/**< f_trp_rec_Rcom_fbTrbSts2Bit_tbl  */
extern	const	USHORT			f_trp_rec_Rcom_fbinfbit_tbl[28];
															/**< f_trp_rec_Rcom_fbinfbit_tbl  */
extern	const	USHORT			f_trp_rec_Rcom_brcinf_tbl[4];
															/**< f_trp_rec_Rcom_brcinf_tbl  */

/*	koshida try	*/
extern	USHORT	timeSetFlag[CMD_CPRINO_MAX];

/* 16B KCN add */
extern	USHORT	timeRecrdcscCount[CMD_CPRINO_MAX];
extern	USHORT	timeRecrdcscCount3g[CMD_CPRINO_MAX];
extern	struct	bpf_timeval	rst48hTim[CMD_CPRINO_MAX];

extern	USHORT	timeRecrdSndCount[CMD_CPRINO_MAX];
extern	USHORT	timeRecrdSndCount3g[CMD_CPRINO_MAX];
//extern	struct	bpf_timeval	rstBaseTim3g[CMD_CPRINO_MAX]; /* 3Gが自律リセット対応する場合は有効化する */

/* Carrier status */
extern	USHORT	f_trp_rec_Wcom_reCarrUseSts[CMD_CPRINO_MAX][CMD_CARNO_MAX];
extern	USHORT	f_trp_rec_Wcom_reCarrUseSts_3g[CMD_CPRINO_MAX][CMD_CARNO_MAX_3G];

#endif
/****************************************************************************/
/*
 */
/****************************************************************************/
