/*!
 *  @skip  $ld:$
 *  @file  l3_main_rom.c
 *  @brief ROM table(proc)
 *  @date  2013/11/11 FFCS)linb create for zynq\n
 *  @date  2015/08/10 TDIPS)maruyama T_SYS_THRDINFO l3_comr_ThrMngTbl[D_L3_THRDNO_MAX]からdiaを削除
 *  @date  2015/08/19 TDIPS)sasaki l3_mainr_cmusRecvThdCreMngにD_API_MSGID_FILE_FIRM_CANCEL_RSP, D_API_MSGID_FILE_FIRM_READ_RSP追加
 *  All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_L3_MAIN
 * @{
 */
 
#include "m_cm_header.h"			/* TRIF common head file     			*/
#include "l3_com_inc.h"
#include "f_com_aplCom.h"
#include "f_cmn_cmu_inc.h"

/****************************************************************************/
/* proc thread function matrix                                                 */
/****************************************************************************/
extern VOID l3_cpr_main(VOID*);
extern VOID l3_rct_main(VOID*);
extern VOID l3_dlm_main(VOID*);
extern VOID l3_dbg_main(VOID*);
extern VOID l3_snd_main(VOID*);
extern VOID l3_rcv_main(VOID*);
extern VOID l3_log_main(VOID*);

/**
 * @brief スレッド管理テーブル
 * @note  [ 用途     ] l3_main_Create()でスレッド生成処理に使用する
 */
const T_SYS_THRDINFO l3_comr_ThrMngTbl[D_L3_THRDNO_MAX] = 
{
	/*thread Q*/			/*name*/	/*Priority*/		/*stack size*/			/*Entry*/		/*schedule is FIFO*/
	{CMD_TSKID_INI,	"l3_main",	D_L3_PRIORITY_MAIN, D_L3_STACKSIZE_MAIN,	D_RRH_NULL,		D_RRH_ON},
	{CMD_TSKID_CPR,	"l3_cpr",	D_L3_PRIORITY_CPR,	D_L3_STACKSIZE_CPR,	    l3_cpr_main,	D_RRH_ON},
	{CMD_TSKID_RCT,	"l3_rct",	D_L3_PRIORITY_RCT,	D_L3_STACKSIZE_RCT,		l3_rct_main,	D_RRH_ON},
	{CMD_TSKID_DLM,	"l3_dlm",	D_L3_PRIORITY_DLM,	D_L3_STACKSIZE_DLM,		l3_dlm_main,	D_RRH_ON},
	{CMD_TSKID_LOG,	"l3_log",	D_L3_PRIORITY_LOG,	D_L3_STACKSIZE_LOG,		l3_log_main,	D_RRH_ON},
	{CMD_TSKID_SND,	"l3_send",	D_L3_PRIORITY_SEND,	D_L3_STACKSIZE_SEND,	l3_snd_main,	D_RRH_ON},
	{CMD_TSKID_RCV,	"l3_recv",	D_L3_PRIORITY_RECV,	D_L3_STACKSIZE_RECV,	l3_rcv_main,	D_RRH_ON},
	{CMD_TSKID_DBG,	"l3_debg",	D_L3_PRIORITY_DEBG,	D_L3_STACKSIZE_DEBG,	l3_dbg_main,	D_RRH_OFF}
};
/**
 * @brief CMU recv paramater thread create manage table
 * @note  [ 用途     ] 受信したプロセス間メッセージをどのスレッドに中継するかを決定するテーブル
 * @Bug_No  M-RRU-ZSYS-01686
 * @Bug_No  M-RRU-ZSYS-01720
 * @date  2015/09/10 TDI)satou 「CMD_TSKIF_RE_STARTUP_RSP」追加
 * @date  2015/09/14 TDIPS)uchida M-RRU-ZSYS-01686 CMD_TSKIF_CPRI_CONNECT_NTC CMD_TSKIF_CPRI_DISCONNECT_NTC CMD_TSKIF_RE_STARTUP_RSP CMD_TSKIF_RE_DISCONNECT_RSP を追加
 * @date  2015/09/24 TDIPS)Lay M-RRU-ZSYS-01720 FLD-MT接続強制解放要求受信時にRECに応答が返らない不具合を修正
 * @date  2015/09/27 TDI)satou REカード診断要求の送信先QIDをCMD_TSKID_RCTに修正
 * @date  2015/09/27 TDI)satou CMD_CPRID_FTPPORTSETREQを追加
 * @date  2015/10/02 TDI)satou CMD_TSKIF_REC_FWDNTC, CMD_CPRID_EXTDATSNDRESを追加
 * @date  2015/10/02 TDI)satou CMD_TSKIF_REC_FWDNTC, CMD_CPRID_EXTDATRCVREQを追加
 * @date  2015/10/05 TDI)satou D_API_MSGID_CPRI_SET_REUSE_INDを追加
 * @date  2016/01/29 TDI)satou D_API_MSGID_CPRI_DLY_ADJ_STEP10_RSPを追加
 * @date  2021/02/04 M&C)Huan.dh Add event CMD_CPRID_CARONOFFRES
 */
const T_CMU_CREATE_THREAD	l3_mainr_cmusRecvThdCreMng	= {
	{
        /* EventID */                           /* SignalKind */            /* 送信先QID */ /* dummy */
        { D_SYS_MSGID_ALLTSKININTC              ,0x00000000                 ,CMD_TSKID_INI  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_HCREQ            ,CMD_TSKID_CPR  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_RERSTREQ         ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_RETIMSETREQ      ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_LINKSTSREQ       ,CMD_TSKID_CPR  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FIRMFILEREQ      ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FILEINFREQ       ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FILEDATSND       ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FILSNDFINNTC     ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_UFIRMFILEREQ     ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_UFILEINFREQ      ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_UFILEDATSND      ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_UFILSNDFINNTC    ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_TRXSETREQ        ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_TRXRELREQ        ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_TOFFSETINFREQ    ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_CARONOFFREQ      ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_EQPSTRREQ        ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_SLOTINFREQ       ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_RESTSREQ         ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_CARDINFREQ       ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_REVERREQ         ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_RECARDCNTREQ     ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_CARDSTSREQ       ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_CARLSTSREQ       ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_REPORTCNTREQ     ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_REPORTSTSREQ     ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_RECRDDIAREQ      ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_REOBSLOGREQ      ,CMD_TSKID_LOG  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_REOBLOGSTPREQ    ,CMD_TSKID_LOG  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_EXTDATSNDREQ     ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_EXTDATRCVRES     ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FLDMTCNCRES      ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FLDFILRCVRES     ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_SVMTDATRCVRES    ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_SVMTCNTRES       ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FLMTCNCRELRES    ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_SVMTCNTRELRES    ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FLDCOMRCVRES     ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FTPPORTSETREQ    ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_CPRI_STATE_STATE_NTC      ,0                          ,CMD_TSKID_CPR  ,0x00000000 },
        { D_API_MSGID_L2_LINKSTATE_NTC          ,0                          ,CMD_TSKID_CPR  ,0x00000000 },
        { D_API_MSGID_SVP_STATE_CHG_NTC         ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_FILE_FIRM_ERASE_RSP       ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_FILE_FIRM_UPDATE_RSP      ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_FILE_FIRM_CANCEL_RSP      ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_MSGID_FILE_FIRM_READ_RSP        ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { D_API_LOG_MNR_TROUBLE_LOG_GET_RSP     ,0                          ,CMD_TSKID_LOG  ,0x00000000 },
        { D_API_MSGID_FILE_FIRM_VERSION_RSP     ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_FILE_FIRM_VERSION_RSP     ,0                          ,CMD_TSKID_INI  ,0x00000000 },
        { CMD_TSKIF_LCKBLKCNTREQ                ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_SVMTFORRELREQ    ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FLDCOMSNDREQ     ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FLMTCMPRELREQ    ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_SVMTDATSNDREQ    ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_FLDFILSNDREQ     ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_MTADDSETREQ      ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_TRXSETRES        ,CMD_TSKID_RCT  ,0x00000000 }, /* TRX設定応答 */
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_TRXRELRES        ,CMD_TSKID_RCT  ,0x00000000 }, /* TRX解放応答 */
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_CARDINFRES       ,CMD_TSKID_RCT  ,0x00000000 }, /* REカード情報報告応答 */
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_REVERRES         ,CMD_TSKID_RCT  ,0x00000000 }, /* REバージョン報告応答 */
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_REOBSLOGRES      ,CMD_TSKID_LOG  ,0x00000000 }, /* RE障害ログ取得応答 */
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FILEDATSND       ,CMD_TSKID_LOG  ,0x00000000 }, /* 障害ログファイルデータ送信要求 */
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FILSNDFINNTC     ,CMD_TSKID_LOG  ,0x00000000 }, /* 障害ログファイルデータ送信完了通知 */
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_REOBLOGSTPRES    ,CMD_TSKID_LOG  ,0x00000000 }, /* RE障害ログ取得中止応答 */
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_EXTDATSNDRES     ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_EXTDATRCVREQ     ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_SVMTFORRELRES    ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FLMTCNCRELREQ    ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FLDCOMRCVREQ     ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FLDMTCNCREQ      ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FLDCOMSNDRES     ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_SVMTCNTREQ       ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FLMTCMPRELRES    ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_SVMTDATSNDRES    ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_SVMTDATRCVREQ    ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FLDFILSNDRES     ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FLDFILRCVREQ     ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_MTADDSETRES      ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FIRMFILERES      ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FILEINFRES       ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FILSNDFINRES     ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_UFIRMFILERES     ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_UFILEINFRES      ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_UFILSNDFINRES    ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_SVMTCNTRELREQ    ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_CARONOFFRES      ,CMD_TSKID_RCT  ,0x00000000 }, /* 無変調キャリアON/OFF指定応答 */
        { CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_CARLSTSRES       ,CMD_TSKID_RCT  ,0x00000000 },
		{ CMD_TSKIF_REC_FWDNTC                  ,CMD_CPRID_FTPPORTSETRES    ,CMD_TSKID_RCT  ,0x00000000 }, /* CMD_CPRID_FTPPORTSETRES Hiep */
        { CMD_TSKIF_RECMPSTPNTC                 ,0                          ,CMD_TSKID_CPR  ,0x00000000 },
        { CMD_TSKIF_RESYSPARA_UPDATE_REQ        ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_REFIRM_DL_REQ               ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_USEREFILE_UPDATE_START_RSP  ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_SAVEFIRM_RSP                ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_REFIRM_DL_STOP_REQ          ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_REFIRM_DL_STOP_IND          ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_LOADFIRM_RSP                ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_CANCEL_LOADFIRM_RSP         ,0                          ,CMD_TSKID_DLM  ,0x00000000 },
        { CMD_TSKIF_RE_STARTUP_RSP              ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_L2_CPRIMSG_RCV_NTC        ,CMD_CPRID_HCRES            ,CMD_TSKID_CPR  ,0x00000000 },
        { CMD_TSKIF_CPRI_CONNECT_NTC            ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_CPRI_DISCONNECT_NTC         ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_RE_DISCONNECT_RSP           ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_RE_FORCESTOP_NTC            ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_REC_CPRI_PRE_RSP            ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_ALLTSKININTC                ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { D_API_MSGID_CPRI_SET_REUSE_IND        ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
		{ D_API_MSGID_CPRI_DLY_ADJ_STEP10_RSP   ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
        { CMD_TSKIF_TIMOUTNTC                   ,0                          ,CMD_TSKID_RCT  ,0x00000000 },
	},
	D_RRH_PROCID_L3,								/* プロセスID */
	D_RRH_PROCQUE_L3,								/* プロセスQID */
	D_RRH_THDID_L3_RECV,							/* Thread-ID */
	CMD_TSKID_RCV,							/* Thread-QID */
	CMD_TSKID_INI,							/* 起動元 Thrad-QID */
	E_RRH_CMU_RECV_ID,								/* スレッド情報(0:recv, 1:send) */
	"l3_recv"										/* スレッド名	 */
};

/* @} */
