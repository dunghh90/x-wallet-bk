/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_ram.c
 * @brief  RAMテーブル
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * @date   2009/09/03 M-S3G-eNBPF-02449
 * @date   2015/09/30 TDIPS)sasaki update for FHM
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
USHORT			f_trp_rec_Fc02_evt = CMD_OFF;
USHORT			f_trp_rec_Fc02_3g_evt = CMD_OFF;
USHORT			f_trp_rec_Fc02_carr_evt = CMD_OFF;
USHORT			f_trp_rec_Fc02_carr_3g_evt = CMD_OFF;
/**********************/
/* RE状態管理テーブル */
/**********************/
T_REC_REMSC_TBL f_trp_rec_Wcom_remsc_tbl;
                                          /*!< RE状態管理(セクタ部)テーブル */
T_REC_REMCM_TBL f_trp_rec_Wcom_remcm_tbl;
                                          /*!< RE状態管理(共通部)テーブル */
T_REC_REMSC_TBL_3G f_trp_rec_Wcom_remsc_tbl_3g;
                                          /*!< RE状態管理(セクタ部)テーブル */
T_REC_REMCM_TBL f_trp_rec_Wcom_remcm_tbl_3g;
                                          /*!< RE状態管理(共通部)テーブル */
T_REC_REMSC_TBL f_trp_rec_Wcom_remsc_tbk;
                                          /*!< RE状態管理(セクタ部)テーブル(BK) */
T_REC_REMCM_TBL f_trp_rec_Wcom_remcm_tbk;
                                          /*!< RE状態管理(共通部)テーブル(BK) */
T_REC_REMSC_TBL_3G f_trp_rec_Wcom_remsc_tbk_3g;
                                          /*!< RE状態管理(セクタ部)テーブル(BK) */
T_REC_REMCM_TBL f_trp_rec_Wcom_remcm_tbk_3g;
                                          /*!< RE状態管理(共通部)テーブル(BK) */

T_REC_SEC_FBSTS_TBL f_trp_rec_Wcom_secfbsts_tbl;
T_REC_SEC_FBSTS_TBL f_trp_rec_Wcom_secfbsts_tbl_3g;

T_REC_OBJMNG_TBL		f_trp_rec_WobjMngTbl;
T_REC_OBJMNG_TBL		f_trp_rec_WobjMngTbl_3g;

/**************************/
/* クラス状態管理テーブル */
/**************************/
T_REC_C02_STSMNG_TBL f_trp_rec_Wc02_stsmng_tbl;
                                                   /*!< C02状態管理テーブル */
T_REC_C02_STSMNG_TBL f_trp_rec_Wc02_stsmng_tbl_3g;
                                                   /*!< C02_3G状態管理テーブル */
T_REC_C03_STSMNG_TBL f_trp_rec_Wc03_stsmng_tbl;
                                                   /*!< C03状態管理テーブル */
T_REC_C03_STSMNG_TBL_3G f_trp_rec_Wc03_stsmng_tbl_3g;
                                                   /*!< C03状態管理テーブル */
T_REC_NESTSMNG_TBL  f_trp_rec_Wcom_nestsmng_tbl;
                                                   /*!< NE Switch 状態管理テーブル */

/****************************/
/* クラスパラメータテーブル */
/****************************/
T_REC_C02_PRM_TBL f_trp_rec_Wc02_prm_tbl;
                                                 /*!< C02パラメータテーブル */
T_REC_C02_PRM_TBL f_trp_rec_Wc02_prm_tbl_3g;
                                                 /*!< C02_3gパラメータテーブル */
T_REC_C03_PRM_TBL f_trp_rec_Wc03_prm_tbl;
                                                 /*!< C03パラメータテーブル */
T_REC_C03_PRM_TBL f_trp_rec_Wc03_prm_tbl_3g;
                                                 /*!< C03パラメータテーブル */

/****************/
/* 変換テーブル */
/****************/
//T_REC_AMPSLT_CPR_TBL f_trp_rec_Wcom_ampslt_cpr_tbl;
//                                        /*!< AMP/OFTRXスロット番号→CPRI番号変換テーブル */
//T_REC_CPR_AMP_TBL    f_trp_rec_Wcom_cpr_amp_tbl;
//                                        /*!< CPRI番号→AMP/OFTRX変換テーブル */
//T_REC_CPR_AMP_TBL    f_trp_rec_Wcom_cpr_amp_tbl_3g;
//                                        /*!< CPRI番号→AMP/OFTRX変換テーブル */
T_REC_TMK_VAL_TBL    f_trp_rec_Wcom_tmk_val_tbl;
                                        /*!< タイマ種別→タイマ値変換テーブル */
/**********/
/* その他 */
/**********/
T_REC_MSGRCV_TBL f_trp_rec_Wcom_msgrcv_tbl;            /*!< Receiving buffer */

T_REC_TIMMNG_TBL f_trp_rec_Wcom_timmng_tbl;            /*!< タイマ管理テーブル */

T_REC_RUNHIS_TBL f_trp_rec_Wcom_runhis_tbl;            /*!< RUNHISテーブル */

T_REC_REGHIS_TBL f_trp_rec_Wcom_reghis_tbl;            /*!< RUNHISテーブル */

T_REC_IVLPRM_TBL f_trp_rec_Wcom_ivlprm_tbl;            /*!< Invalid log paramter Table */


T_REC_REQRSP_TBL f_trp_rec_Wcom_ReqRspLog_tbl[D_REC_LOGKIND_MAX][CMD_CPRINO_MAX];
T_REC_REQRSP_SAVE_TBL f_trp_rec_Wcom_ReqRspLogSave_tbl[D_REC_LOGKIND_MAX][CMD_CPRINO_MAX];

USHORT	CarrStateUnmatch[CMD_CPRINO_MAX];

USHORT	RE_AlarmProtectTime_Debug;

INT		RE_AlarmProtectTime;

USHORT	RE_multiFlag;

UCHAR*	f_trp_rec_fwdata_area;			/*!< shared memory of FW File Data  */

CMT_CPRIF_MTADDSETREQ	f_trp_rec_Wcom_MtAdSetInfo[CMD_MAX_SYSNUM];	/*!< MT address seting info */

USHORT	f_trp_rec_Wcom_MtAdSetRetryCnt[CMD_MAX_SYSNUM] = {0};		/*!< MTアドレス設定要求(REC主導)送信回数 */

/*	koshida try	*/
USHORT	timeSetFlag[CMD_CPRINO_MAX];

/* 16B KCN add */
USHORT	timeRecrdcscCount[CMD_CPRINO_MAX];
USHORT	timeRecrdcscCount3g[CMD_CPRINO_MAX];
struct	bpf_timeval	rst48hTim[CMD_CPRINO_MAX];

USHORT	timeRecrdSndCount[CMD_CPRINO_MAX];
USHORT	timeRecrdSndCount3g[CMD_CPRINO_MAX];
//struct	bpf_timeval	rstBaseTim3g[CMD_CPRINO_MAX]; /* 3Gが自律リセット対応する場合は有効化する */

/**
 * @brief REカード状態報告応答ALM/ERR情報格納用テーブル(3G)
 */
USHORT 					gw_3g_err_code_re[CMD_MAX_ERR_NUM];						/* ERRコード格納用ワークエリア	*/
USHORT 					gw_3g_alm_code_re[CMD_MAX_ALM_NUM];						/* 発生ALM情報格納用ワークエリア*/

/**
 * @brief REカード状態報告応答ALM/ERR情報格納用テーブル(LTE)
 */
//USHORT 					gw_lte_err_code_re[CMD_MAX_ERR_NUM_HIG_BITRAT];			/* ERRコード格納用ワークエリア	*/
//USHORT 					gw_lte_alm_code_re[CMD_MAX_ALM_NUM_HIG_BITRAT];			/* 発生ALM情報格納用ワークエリア*/

/* Carrier status */
USHORT	f_trp_rec_Wcom_reCarrUseSts[CMD_CPRINO_MAX][CMD_CARNO_MAX];
USHORT	f_trp_rec_Wcom_reCarrUseSts_3g[CMD_CPRINO_MAX][CMD_CARNO_MAX_3G];
/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
