/*!
 * @skip  $ld:$
 * @file  re_main_rom.c
 * @brief ROM table
 * @date  2015/07/30 TDIPS) maruyama create
 * @date  2015/08/04 TDIPS) maruyama modify 	DBGスレッド追加、L2とのMSGテーブル追加
 * @date  2015/08/05 TDIPS) maruyama modify 	CMUスレッド起動用構造体(T_CMU_CREATE_THREAD)追加
 * date   2015/08/20 TDIPS) maruyama modify re_rec_mainを削除（代わりはf_trp_rec_Fcom_r_main()）、f_trp_rec.h追加
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 /*!
 * @addtogroup RRH_RE_MAIN
 * @{
 */
#include "m_cm_header.h"
#include "re_com_inc.h"
#include "f_com_aplCom.h"
#include "f_cmn_cmu_inc.h"
#include "f_trp_rec.h"
#include "rrhApi_Cpri.h"
#include "rrhApi_L2.h"

extern void f_trp_rec_Fcom_r_main( void * );
extern void re_rcv_main( void * );
extern void re_send_main( void * );
extern void re_dbg_main( void * );

/** REプロセス スレッド管理テーブル */
const T_SYS_THRDINFO re_mainr_ThrMngTbl[D_RE_THRDNO_MAX] = 
{
						/*name*/	/*Priority*/		/*stack size*/			/*need create*/	/*msgQID*/
	{CMD_TSKID_INI,		"re_main",	D_RE_PRIORITY_MAIN,	D_RE_STACKSIZE_MAIN,	D_RRH_NULL,	    D_RRH_ON},
	{CMD_TSKID_REC,		"re_rec",	D_RE_PRIORITY_REC,	D_RE_STACKSIZE_REC,		f_trp_rec_Fcom_r_main,	D_RRH_ON},
	{CMD_TSKID_RCV,		"re_rcv",	D_RE_PRIORITY_RCV,	D_RE_STACKSIZE_RCV,		re_rcv_main,	D_RRH_ON},
	{CMD_TSKID_SEND,	"re_send",	D_RE_PRIORITY_SEND,	D_RE_STACKSIZE_SEND,	re_send_main,	D_RRH_ON},
	{CMD_TSKID_DBG,		"re_dbg",	D_RE_PRIORITY_DBG,	D_RE_STACKSIZE_DBG,		re_dbg_main,	D_RRH_OFF}
};
 
/**
 * @brief   L2とのMSGテーブル
 * @Bug_No  M-RRU-ZSYS-01741
 * @date    2015/10/02 TDI)satou M-RRU-ZSYS-01741 CMD_CPRID_EXTDATSNDRESを追加
 * @date    2021/02/04 M&C)Huan.dh add CMD_CPRID_CARONOFFRES
 */
UINT	const	 mnt_comr_remsg_subTbl[MBD_REMSGNUM]=                   /* L2とのMSGテーブル	*/
{
    CMD_CPRID_TRXSETRES, 		
    CMD_CPRID_TRXRELRES, 		
    CMD_CPRID_TOFFSETINFRES, 	
    CMD_CPRID_EQPSTRRES, 		
    CMD_CPRID_SLOTINFRES, 		
    CMD_CPRID_RESTSRES, 		
    CMD_CPRID_CARDINFRES, 		
    CMD_CPRID_REVERRES, 		
    CMD_CPRID_RECARDCNTRES, 	
    CMD_CPRID_CARDSTSRES, 		
    CMD_CPRID_CARLSTSRES, 		
    CMD_CPRID_REPORTSTSRES, 	
    CMD_CPRID_REOBSLOGRES, 		
    CMD_CPRID_REOBLOGSTPRES,
    CMD_CPRID_MTADDSETRES, 		
    CMD_CPRID_FLDMTCNCREQ, 		
    CMD_CPRID_FLDCOMSNDRES, 	
    CMD_CPRID_FLDCOMRCVREQ, 	
    CMD_CPRID_FLDFILSNDRES, 	
    CMD_CPRID_FLDFILRCVREQ, 	
    CMD_CPRID_FLMTCNCRELREQ, 	
    CMD_CPRID_FLMTCMPRELRES, 	
    CMD_CPRID_SVMTCNTREQ, 		
    CMD_CPRID_SVMTDATSNDRES, 	
    CMD_CPRID_SVMTDATRCVREQ, 	
    CMD_CPRID_SVMTCNTRELREQ, 	
    CMD_CPRID_SVMTFORRELRES, 	
    CMD_CPRID_FIRMFILERES, 		
    CMD_CPRID_FILEINFRES, 		
    CMD_CPRID_FILSNDFINRES, 	
    CMD_CPRID_UFIRMFILERES, 	
    CMD_CPRID_UFILEINFRES, 		
    CMD_CPRID_UFILSNDFINRES, 	
    CMD_CPRID_EXTDATSNDRES,
    CMD_CPRID_EXTDATRCVREQ, 	
    CMD_CPRID_FILEDATSND, 
    CMD_CPRID_FILSNDFINNTC,
    CMD_CPRID_CARONOFFRES,
	CMD_CPRID_FTPPORTSETRES
};


/**
 * @brief CMU recv paramater thread create manage table
 * @Bug_No  M-RRU-ZSYS-01741
 * @Bug_No  M-RRU-ZSYS-01777
 * @date    2015/09/20 TDI)satou CMD_TSKIF_RE_FWDNTC RE時刻設定要求の中継をテーブルに追加
 * @date    2015/10/02 TDI)satou M-RRU-ZSYS-01741 D_API_MSGID_L2_CPRIMSG_RCV_NTC, CMD_CPRID_EXTDATSNDRESを追加
 * @date    2015/10/19 TDI)satou ハソ-QA-051 二次リセット
 * @date    2015/10/22 FPT)Quynh M-RRU-ZSYS-01777 Fix bug IT2 (BUG_082_IT2_20-06-04_20-06-03) (Delete redundant signal kind case: CMD_CPRID_CARDINFREQ )
 * @date    2021/01/18 M&C)Tri.hn Add case CARLSTS
 */
T_CMU_CREATE_THREAD	re_mainr_cmusRecvThdCreMng	=  {				/* CMUスレッド起動用構造体	*/
	{												/* メッセージ送信先情報管理テーブル			*/
		/* EventID */						/* SignalKind */			/* 送信先QID */		/* dummy */
		{ D_SYS_MSGID_ALLTSKININTC,			0x00000000,					CMD_TSKID_INI,		0x00000000 },
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_TRXSETRES, 		CMD_TSKID_REC,		0x00000000 },	/* TRX設定応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_TRXRELRES, 		CMD_TSKID_REC,		0x00000000 },	/* TRX解放応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_TOFFSETINFRES, 	CMD_TSKID_REC,		0x00000000 },	/* Toffset値報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_EQPSTRRES, 		CMD_TSKID_REC,		0x00000000 },	/* RE装置構成情報報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_SLOTINFRES, 		CMD_TSKID_REC,		0x00000000 },	/* REスロット情報報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_RESTSRES, 		CMD_TSKID_REC,		0x00000000 },	/* RE状態報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_CARDINFRES, 		CMD_TSKID_REC,		0x00000000 },	/* REカード情報報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_REVERRES, 		CMD_TSKID_REC,		0x00000000 },	/* REバージョン報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_RECARDCNTRES, 	CMD_TSKID_REC,		0x00000000 },	/* REカード制御応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_CARDSTSRES, 		CMD_TSKID_REC,		0x00000000 },	/* REカード状態報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_CARLSTSRES, 		CMD_TSKID_REC,		0x00000000 },	/* REキャリア状態報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_REPORTSTSRES, 	CMD_TSKID_REC,		0x00000000 },	/* RE PORT状態報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_REOBSLOGRES, 		CMD_TSKID_REC,		0x00000000 },	/* RE障害ログ取得応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_REOBLOGSTPRES, 	CMD_TSKID_REC,		0x00000000 },	/* RE障害ログ取得中止応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_MTADDSETRES, 		CMD_TSKID_REC,		0x00000000 },	/* MTアドレス設定応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FLDMTCNCREQ, 		CMD_TSKID_REC,		0x00000000 },	/* FLD-MT接続要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FLDCOMSNDRES, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MTデータコマンド転送送信応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FLDCOMRCVREQ, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MTデータコマンド転送受信要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FLDFILSNDRES, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MTデータファイル転送送信応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FLDFILRCVREQ, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MTデータファイル転送受信要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FLMTCNCRELREQ, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MT接続解放要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FLMTCMPRELRES, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MT接続強制解放応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_SVMTCNTREQ, 		CMD_TSKID_REC,		0x00000000 },	/* SV-MT接続要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_SVMTDATSNDRES, 	CMD_TSKID_REC,		0x00000000 },	/* SV-MTデータ送信応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_SVMTDATRCVREQ, 	CMD_TSKID_REC,		0x00000000 },	/* SV-MTデータ受信要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_SVMTCNTRELREQ, 	CMD_TSKID_REC,		0x00000000 },	/* SV-MT接続解放要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_SVMTFORRELRES, 	CMD_TSKID_REC,		0x00000000 },	/* SV-MT接続強制解放応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FIRMFILERES, 		CMD_TSKID_REC,		0x00000000 },	/* ファームウェアファイル報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FILEINFRES, 		CMD_TSKID_REC,		0x00000000 },	/* ファイル情報報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FILSNDFINRES, 	CMD_TSKID_REC,		0x00000000 },	/* ファイルデータ送信完了報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_UFIRMFILERES, 	CMD_TSKID_REC,		0x00000000 },	/* 運用中ファームウェアファイル報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_UFILEINFRES, 		CMD_TSKID_REC,		0x00000000 },	/* 運用中ファイル情報報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_UFILSNDFINRES, 	CMD_TSKID_REC,		0x00000000 },	/* 運用中ファイルデータ送信完了報告応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,   CMD_CPRID_EXTDATSNDRES,     CMD_TSKID_REC,      0x00000000 },   /* 外部装置データ送信応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_EXTDATRCVREQ, 	CMD_TSKID_REC,		0x00000000 },	/* 外部装置データ受信要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FILEDATSND, 		CMD_TSKID_REC,		0x00000000 },	/* 障害ログファイルデータ送信受信要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FILSNDFINNTC, 	CMD_TSKID_REC,		0x00000000 },	/* 障害ログファイルデータ送信完了通知要求 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_CARONOFFRES, 		CMD_TSKID_REC,		0x00000000 },	/* 無変調キャリアON/OFF指定応答 */
		{ D_API_MSGID_L2_CPRIMSG_RCV_NTC,	CMD_CPRID_FTPPORTSETRES,	CMD_TSKID_REC,		0x00000000 },	/*CMD_CPRID_FTPPORTSETRES Hiep */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_RETIMSETREQ, 		CMD_TSKID_REC,		0x00000000 },   /* RE時刻設定要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FLDFILSNDREQ, 	CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_TRXSETREQ, 		CMD_TSKID_REC,		0x00000000 },	/* TRX設定要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_TRXRELREQ, 		CMD_TSKID_REC,		0x00000000 },	/* TRX解放要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_CARDINFREQ, 		CMD_TSKID_REC,		0x00000000 },	/* REカード情報報告要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_REVERREQ, 		CMD_TSKID_REC,		0x00000000 },	/* REバージョン報告要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_RECARDCNTREQ, 	CMD_TSKID_REC,		0x00000000 },	/* REカード制御要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_MTADDSETREQ, 		CMD_TSKID_REC,		0x00000000 },	/* MTアドレス設定要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FLDMTCNCRES, 		CMD_TSKID_REC,		0x00000000 },	/* FLD-MT接続応答 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FLDCOMSNDREQ, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MTデータコマンド転送送信要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FLDCOMRCVRES, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MTデータコマンド転送受信応答 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FLDFILRCVRES, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MTデータファイル転送受信応答 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FLMTCNCRELRES, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MT接続解放応答 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FLMTCMPRELREQ, 	CMD_TSKID_REC,		0x00000000 },	/* FLD-MT接続強制解放要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_SVMTCNTRES, 		CMD_TSKID_REC,		0x00000000 },	/* SV-MT接続応答 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_SVMTDATSNDREQ, 	CMD_TSKID_REC,		0x00000000 },	/* SV-MTデータ送信要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_SVMTDATRCVRES, 	CMD_TSKID_REC,		0x00000000 },	/* SV-MTデータ受信応答 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_SVMTCNTRELRES, 	CMD_TSKID_REC,		0x00000000 },	/* SV-MT接続解放応答 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_SVMTFORRELREQ, 	CMD_TSKID_REC,		0x00000000 },	/* SV-MT接続強制解放要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_RERSTREQ, 		CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FIRMFILEREQ, 		CMD_TSKID_REC,		0x00000000 },	/* ファームウェアファイル報告要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FILEDATSND, 		CMD_TSKID_REC,		0x00000000 },	/* ファイルデータ送信(シスパラ用) */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FILSNDFINNTC, 	CMD_TSKID_REC,		0x00000000 },	/* ファイルデータ送信完了報告通知 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FILSNDFINRES, 	CMD_TSKID_REC,		0x00000000 },	/* ファイルデータ送信完了報告通知応答 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FILEINFREQ, 		CMD_TSKID_REC,		0x00000000 },	/* ファイル情報報告通知 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_UFIRMFILEREQ, 	CMD_TSKID_REC,		0x00000000 },	/* 運用中ファームウェアファイル報告要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_UFILEINFREQ, 		CMD_TSKID_REC,		0x00000000 },	/* 運用中ファイル情報報告通知 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_UFILSNDFINNTC, 	CMD_TSKID_REC,		0x00000000 },	/* 運用中ファイルデータ送信完了報告通知 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_EXTDATRCVRES, 	CMD_TSKID_REC,		0x00000000 },	/* 外部装置データ受信応答 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_EXTDATSNDREQ, 	CMD_TSKID_REC,		0x00000000 },	/* 外部装置データ送信要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_REOBSLOGREQ,	 	CMD_TSKID_REC,		0x00000000 },	/* RE障害ログ取得送信要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_REOBLOGSTPREQ, 	CMD_TSKID_REC,		0x00000000 },	/* RE障害ログ取得中止送信要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_MTADDSETTOREQ, 	CMD_TSKID_REC,		0x00000000 },	/*  */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_CARONOFFREQ, 		CMD_TSKID_REC,		0x00000000 },	/* 無変調キャリアON/OFF指定要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_CARLSTSREQ, 		CMD_TSKID_REC,		0x00000000 },	/* REキャリア状態報告要求 */
		{ CMD_TSKIF_RE_FWDNTC,				CMD_CPRID_FTPPORTSETREQ,	CMD_TSKID_REC,		0x00000000 },	/* CMD_CPRID_FTPPORTSETREQ Hiep */
		{ CMD_TSKIF_CPRSTACHGNTC,	        0x00000000, 				CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_RE_STARTUP_REQ,			0x00000000, 				CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_RE_DISCONNECT_REQ,		0x00000000, 				CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_RESYSPARA_UPDATE_RSP,	0x00000000, 				CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_REFIRM_DL_RSP,			0x00000000, 				CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_RE_FORCERESET_IND,		0x00000000, 				CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_RE_FORCESTOP_IND,		0x00000000, 				CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_REFILE_UPDATE_START_NTC,0x00000000, 				CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_FILEDATSND_TRANS_IND,	0x00000000, 				CMD_TSKID_REC,		0x00000000 },	/* ファイルデータ送信転送指示 */
		{ CMD_TSKIF_REFILE_UPDATE_COMP_NTC,	0x00000000, 				CMD_TSKID_REC,		0x00000000 },	/* 運用中REファイル更新完了通知 */
		{ CMD_TSKIF_2NDRST_REQ,				0x00000000, 				CMD_TSKID_REC,		0x00000000 },
		{ CMD_TSKIF_RE_SLFRST_PRVTOFF_REQ,	0x00000000, 				CMD_TSKID_REC,		0x00000000 },	/* RE自律リセット抑止解除要求 */
		{ 0x00000000,						0x00000000, 				0x00000000,			0x00000000 },
		{ 0x00000000,						0x00000000, 				0x00000000,			0x00000000 },
		{ 0x00000000,						0x00000000, 				0x00000000,			0x00000000 },
		{ 0x00000000,						0x00000000, 				0x00000000,			0x00000000 },
		{ 0x00000000,						0x00000000, 				0x00000000,			0x00000000 }
	},
	D_RRH_PROCID_RE,								/* プロセスID */
	D_RRH_PROCQUE_RE,								/* プロセスQID */
	D_RRH_THDID_RE_RCV,								/* Thread-ID */
	CMD_TSKID_RCV,									/* Thread-QID */
	CMD_TSKID_INI,									/* 送信元Thrad-QID */
	E_RRH_CMU_RECV_ID,								/* スレッド情報(0:recv, 1:send) */
	"re_rcv"										/* スレッド名 */
};
/* @} */
