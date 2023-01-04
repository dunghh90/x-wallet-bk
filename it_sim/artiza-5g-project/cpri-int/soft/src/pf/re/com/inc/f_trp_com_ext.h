/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_com_ext.h
 * @brief  extern
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/05/25 FJT)Nagasima CR-00012-001
 *	@date	2008/07/29	FFCS)Tangj	S3G PRC eNB-008-002
 *  @date   2010/08/12 CMS)Takahashi modify for M-S3G-eNBSYS-01722
 *  @date   2010/09/21 CMS)Takahashi modify for CR***
 *  @date   2011/02/10 FJT)Tokunaga CeNB-F-062-033(DCM) 運用パラメータの反映元と反映タイミング
 *  @date   2011/11/10 FJT)K.Koshida D-MICRO-008 3002形対応(LAPB削除,H/S inf変更対応)
 *  @date   2016/10/07 KCN)hfuku 16B対応
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008 - 2016
 */
/****************************************************************************/
#ifndef _F_TRP_COM_EXT_h
#define _F_TRP_COM_EXT_h

#include "f_rrh_def.h"
#include "f_com_aplCom.h"

/*! @addtogroup TRIF_TCM
 * @{ */

//#include "f_com_isc_ram.h"

/****************************************************************************/
/* 共通テーブル外部参照宣言													*/
/****************************************************************************/
/*******/
/* RAM */
/*******/
///*!< プロセスディスクリプタ */
//extern	INT					f_trp_com_Wpd;
/*!< スレッド情報テーブル	*/
//extern	T_TCM_THRINF_TBL	f_trp_com_Wthrinf_tbl;
///*!< 通信スレッド情報テーブル	*/
//extern	T_TCM_CTHINF_TBL	f_trp_com_Wcthinf_tbl;
///*!< デバッグスレッド情報テーブル	*/
//extern	T_TCM_DTHINF_TBL	f_trp_com_Wdthinf_tbl;
///*!< CPRI NE切替情報管理テーブル */
//extern	CMT_CPRINECHGINFMNG_TBL	cmw_cprinechginfmng_tbl;
///* LAPB N/E切替情報管理テーブル */
//extern LBT_NESTKTBL			cmw_lapbnechginfmng_tbl[6];
///* REC N/E切替情報管理テーブル */
//extern UCHAR			cmw_recnechginfmng_tbl[1024*512];
///*******/
///* ROM */
///*******/
///*!< スレッド起動情報テーブル	*/
//extern	T_TCM_THRSUPINF_TBL f_trp_com_Rthrsupinf_tbl;
//
//extern UINT const f_trp_com_BandWidth_reg[CMD_CPRINO_MAX];
//extern UINT const f_trp_com_CarrOperate_reg[CMD_CPRINO_MAX];
//extern UINT const f_trp_com_S3G_CpriSteadyTime_reg[CMD_CPRINO_MAX];
//extern UINT const f_trp_com_3G_CpriSteadyTime_reg[CMD_CPRINO_MAX];
//#if !defined(OPT_CPRI_L2HARD)
//extern UINT const f_trp_com_DlReDly_reg[CMD_NUM3][CMD_CPRINO_MAX];
//#endif
//
///* CPRIリンクコンフィッグテーブル */
//extern CMT_CPRICONFTBL			cmw_cpriconftbl;

/* CPRI state共通管理テーブル */
//extern CMT_CPRILNKSTATBL		cmw_cprilnkstatbl[D_RRH_CPRINO_NUM];

//extern char f_trp_com_Rlm_ver[];
//extern char f_trp_com_Rlm_tim[];
//extern char f_trp_com_Rlm_hst[];
//
/****************************************************************************/
/* 共通部自身の変数定義  バッファ情報テーブル								*/
/****************************************************************************/
//extern CMT_BUFFINF_TBL f_trp_Rbuffinf_tbl[CMD_BUFFTYP_MAX];

//extern T_TCM_TIMEOUT_ENTRY f_trp_com_Rtimout_ent[D_TCM_THRINO_MAX];


///******************************************/
///* スレッド別ログコマンド関数テーブル */
///******************************************/
//extern T_TCM_LOGFNC_TBL f_trp_com_Rlogfnc_tbl;
//
//extern UCHAR	f_trp_com_Wsndarea[1024];
//
/* buffer busy flag */
//extern UCHAR	f_trp_com_Wbusyflg[CMD_BUFFTYP_MAX];
//
///*!< Alarm log table */
//extern T_CMM_USERLOG_MNGTBL	f_trp_com_WAlmLogTbl;
//
///*!< Alarm log table total size */
//extern UINT	f_trp_com_WAlmLogSize;

extern USHORT *	f_trp_com_Wsysprmp_COM;
extern USHORT *	f_trp_com_Wsysprmp_PF;
extern USHORT *	f_trp_com_Wsysprmp_CPRI[CMD_NUM6];
extern USHORT *	f_trp_com_Wsysprmp_FJT;

//extern USHORT   f_trp_com_Wapscmp_flg;  
//extern UCHAR const f_trp_com_local_ctrl[CMD_CPRINO_MAX/CMD_NUM2];
//							/* ローカルレジスタ間接アクセス制御 */
//extern UCHAR const f_trp_com_local_adr[CMD_CPRINO_MAX/CMD_NUM2];
//							/* ローカルレジスタ間接アクセスアドレス */
//extern UCHAR const f_trp_com_local_data[CMD_CPRINO_MAX/CMD_NUM2];
//							/* ローカルレジスタ間接アクセスデータ */
//extern UINT const f_trp_com_semphone_id[CMD_NUM2][CMD_CPRINO_MAX/CMD_NUM2];
//							/* Semophone ID for CPRIMUX local register access */
//
///*!< Auto Reset history Mng Table	*/
//extern T_COM_AUTORESETMNGTBL	*f_trp_com_AutoResetMngTbl;
extern T_COM_AUTORESETMNGTBL		f_trp_com_AutoResetMngTbl[D_RRH_CPRINO_RE_MAX][CMD_SLOTINF_MAX];
extern T_COM_AUTORESETMNGTBL		f_trp_com_AutoResetMngTbl_3g[D_RRH_CPRINO_RE_MAX];
//
//
//
//
///*!< L1 Log Table	*/
//extern UINT f_trp_com_Wl1rcvsndlog_index[D_TCM_L1LOGINDEX_MAX];
//extern T_TCM_L1RCVSNDLOG f_trp_com_Wl1rcvsndlog[D_TCM_L1LOGINDEX_MAX];
//extern UINT f_trp_com_Wl1sndcounter[CMD_LINK_NUM_T][CMD_NUM4];
//extern UINT f_trp_com_Wl1rcvcounter[CMD_LINK_NUM_T][CMD_NUM4];
//extern UINT const f_trp_com_l1reglog[CMD_LINK_NUM_T][CMD_NUM16];

extern UINT	f_trp_com_Wsndcmpcount[CMD_LINK_NUM_T];	/*!< HDLC sending complete counter */
extern UINT	f_trp_com_Wsndcount[CMD_LINK_NUM_T];	/*!< HDLC sending counter */

//extern UINT	f_trp_com_RecCardsts;
//
//#if defined(OPT_CPRI_L2HARD)
///*!< CPRIMUX init setting(syspara setting value) */
//extern UINT	f_trp_com_syspara_setval[D_TCM_SYSPARATBL_NUM];
//extern UINT const f_trp_com_syspara_setreg[D_TCM_SYSPARATBL_NUM];
//
///*	CPRI-MT mode manage table	*/
//extern T_TCM_MKMTMODE	f_trp_com_Wmkmtmode[CMD_LINK_NUM_T];
//
//extern CMT_DMADATINFO	const cmr_dmaRcvInfo[CMD_DMAKIND_RCV_NUM][CMD_LINK_NUM_T];
//extern CMT_DMADATINFO	const cmr_dmaSndInfo[CMD_LINK_NUM_T];
//
//#ifdef DEBUG_TRACE_T
//extern CHAR	dmaRcvDat[10260];
//extern UINT	dbgFlg[16];
//#endif
//
//#endif
//
//#ifdef D_OPT_CPRING_LOG
//extern T_TCM_IMPRVLOGMNGTBL	f_trp_tcm_WImprvLogMngTbl;			/**< Improve Log Management table  */
//#endif
//
/* AMP接続情報管理テーブル */
T_TCM_AMPCONNMNG	f_trp_com_WSyspAmpConnMngTbl[D_RRH_CPRINO_RE_MAX];		/**< System Parameter(AMP connect Information) management  */

/*!< RE未実装強化LOG Write Index   */
extern UINT	f_trp_com_WlogWriteIndex;									/**< Write Index */
extern T_TCM_TXSRXS_COUNT	f_trp_com_WlogTxsRxslog[CMD_LOG_WRITE_INDEX_MAX];	/**< TXS/RXS割り込み送受信表示ログ **/
extern T_TCM_REGLOG_TBL	f_trp_com_WlogReglog[CMD_LOG_WRITE_INDEX_MAX];		/**< レジスタログ **/
extern UINT	f_trp_com_WlogDspIndex;										/**< DSP Index */

extern T_TCM_TIMER_INFO		f_trp_com_timerInfo[64][D_RRH_CPRINO_RE_MAX][CMD_MAXSLOTNO][CMD_MAX_SYSNUM];
extern T_TCM_TIMEOUT_MNGTBL	f_trp_com_timerMngTbl;
#if 0
extern UINT	logflg;
extern UINT	logflg_ent;
#endif
//extern INT	f_trp_com_log_kind;
//extern INT	f_trp_com_log_kind2;
//extern INT	f_trp_com_log_kind3;
#endif
/*! @} */  /* group TRIF_TCM */
