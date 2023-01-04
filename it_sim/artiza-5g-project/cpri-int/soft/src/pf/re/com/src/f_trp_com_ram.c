/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_ram.c
 *  @brief  TRIF common RAM table define
 *  @date   2007/03/16  FFCS)linjunhua create
 *  @date   2007/05/25  FJT)Nagasima CR-00012-001
 *	@date	2008/07/29	FFCS)Tangj	S3G PRC eNB-008-002
 *  @date   2009/02/07 FFCS)Tangj S3G PRC 1st release system parameter getting
 *  @date   2010/08/12 CMS)Takahashi modify for M-S3G-eNBSYS-01722
 *  @date   2010/09/21 CMS)Takahashi modify for CR***
 *  @date   2011/11/10 FJT)K.Koshida D-MICRO-008 3002形対応(LAPB削除,H/S inf変更対応)
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2011
 */
 










#include "f_trp_com.h"
#include "f_trp_com_ext.h"
/*! @addtogroup TRIF_TCM
 * @{ */

/****************************************************************************/
/* 共通RAMテーブル宣言														*/
/****************************************************************************/
/*!< スレッド情報テーブル	*/
//T_TCM_THRINF_TBL	f_trp_com_Wthrinf_tbl;
//
///*!< 通信スレッド情報テーブル	*/
//T_TCM_CTHINF_TBL	f_trp_com_Wcthinf_tbl;
//
///*!< デバッグスレッド情報テーブル	*/
//T_TCM_DTHINF_TBL	f_trp_com_Wdthinf_tbl;
//
///*!< CPRIリンクコンフィッグテーブ	*/
//CMT_CPRICONFTBL			cmw_cpriconftbl;
//
/*!< CPRI state共通管理テーブ	*/
//CMT_CPRILNKSTATBL		cmw_cprilnkstatbl;

/*!< シスパラアドレス */
USHORT *			f_trp_com_Wsysprmp_COM;
USHORT *			f_trp_com_Wsysprmp_PF;
USHORT *			f_trp_com_Wsysprmp_CPRI[CMD_NUM6];
USHORT *			f_trp_com_Wsysprmp_FJT;
//
//
///*!< プロセスディスクリプ		*/
//INT					f_trp_com_Wpd;
//
///*!< CPRI NE切替情報管理テーブル */
//CMT_CPRINECHGINFMNG_TBL	cmw_cprinechginfmng_tbl;
//
///*!< LAPB N/E切替情報管理テーブル */
//LBT_NESTKTBL			cmw_lapbnechginfmng_tbl[6];
//
///*!< REC N/E切替情報管理テーブル */
//UCHAR			cmw_recnechginfmng_tbl[1024*512];
//
///*!< TCM : message send area */
//UCHAR	f_trp_com_Wsndarea[1024];
//
///*!< buffer busy flag */
//UCHAR	f_trp_com_Wbusyflg[CMD_BUFFTYP_MAX];
//
///*!< Alarm log table */
//T_CMM_USERLOG_MNGTBL	f_trp_com_WAlmLogTbl;
//
///*!< Alarm log table total size */
//UINT	f_trp_com_WAlmLogSize;
//
///*!< AP起動条件完了      */
//USHORT f_trp_com_Wapscmp_flg;  
//
///*!< Auto Reset history Mng Table	*/
//T_COM_AUTORESETMNGTBL		*f_trp_com_AutoResetMngTbl;
T_COM_AUTORESETMNGTBL		f_trp_com_AutoResetMngTbl[D_RRH_CPRINO_RE_MAX][CMD_SLOTINF_MAX];
T_COM_AUTORESETMNGTBL		f_trp_com_AutoResetMngTbl_3g[D_RRH_CPRINO_RE_MAX];
///*!< L1 Log Table	*/
//UINT f_trp_com_Wl1rcvsndlog_index[D_TCM_L1LOGINDEX_MAX];
//T_TCM_L1RCVSNDLOG f_trp_com_Wl1rcvsndlog[D_TCM_L1LOGINDEX_MAX];
//UINT f_trp_com_Wl1sndcounter[CMD_LINK_NUM_T][CMD_NUM4];
//UINT f_trp_com_Wl1rcvcounter[CMD_LINK_NUM_T][CMD_NUM4];
//
/*!   
 *  @brief HDLC sending complete counter
 */

UINT	f_trp_com_Wsndcmpcount[CMD_LINK_NUM_T];	/*!< HDLC sending complete counter */
UINT	f_trp_com_Wsndcount[CMD_LINK_NUM_T];	/*!< HDLC sending counter */

//UINT	f_trp_com_RecCardsts;
//
//#if defined(OPT_CPRI_L2HARD)
///*	CPRI-MT mode manage table	*/
//T_TCM_MKMTMODE	f_trp_com_Wmkmtmode[CMD_LINK_NUM_T];
//
///*!< CPRIMUX init setting(syspara setting value) */
//UINT	f_trp_com_syspara_setval[D_TCM_SYSPARATBL_NUM];
//#endif
//#ifdef DEBUG_TRACE_T
//CHAR	dmaRcvDat[10260];
//UINT	dbgFlg[16];
//#endif
//
//#ifdef D_OPT_CPRING_LOG
//T_TCM_IMPRVLOGMNGTBL	f_trp_tcm_WImprvLogMngTbl;			/**< Improve Log Management table  */
//#endif
//
/* Active Link Group management table */
T_TCM_AMPCONNMNG	f_trp_com_WSyspAmpConnMngTbl[D_RRH_CPRINO_RE_MAX];		/**< System Parameter(AMP connect Information) management  */

/*!< RE未実装強化LOG   */
UINT				f_trp_com_WlogWriteIndex;							/**< Write Index */
T_TCM_TXSRXS_COUNT	f_trp_com_WlogTxsRxslog[CMD_LOG_WRITE_INDEX_MAX];			/**< TXS/RXS割り込み送受信表示ログ **/
T_TCM_REGLOG_TBL	f_trp_com_WlogReglog[CMD_LOG_WRITE_INDEX_MAX];			/**< レジスタログ **/
UINT				f_trp_com_WlogDspIndex;								/**< DSP Index */

///*!< デバックコマンド用  */
//INT					f_trp_com_log_kind;
//INT					f_trp_com_log_kind2;
//INT					f_trp_com_log_kind3;


T_TCM_TIMER_INFO		f_trp_com_timerInfo[64][D_RRH_CPRINO_RE_MAX][CMD_MAXSLOTNO][CMD_MAX_SYSNUM];
T_TCM_TIMEOUT_MNGTBL	f_trp_com_timerMngTbl;
#if 0
UINT	logflg = 0;
UINT	logflg_ent = 0;
#endif

/* @} */  /* group TRIF_TCM */

