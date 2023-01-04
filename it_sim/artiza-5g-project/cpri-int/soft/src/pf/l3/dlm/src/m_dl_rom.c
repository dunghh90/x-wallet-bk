/*!
 * @skip   $Id$
 * @file   M_dl_rom.c
 * @brief  DLM read only table definition.
 * @date   2008/07/15 FFCS)Wuh Create for eNB-008-001.
 * @date   2009/01/28  FFCS)wuh M-S3G-eNBPF-01235
 * @date   2009/04/30  FFCS)Wuh modify for CR-00038-005
 *                      move the process of FLASH erasing to file data sending
 * @date   2009/12/27 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 * @date   2015/09/29 TDIPS)sasaki Update N/E切替問題対応(ファイル情報報告/運用中ファイル情報報告)
 * @date   2015/11/19 TDIPS)sasaki Update N/E切替問題対応(ファームウェアファイル報告/運用中ファームウェアファイル報告)
 * @date   2015/11/20 TDIPS)sasaki Update N/E切替問題対応(ROMテーブル変更漏れを修正)
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
*/

#include "m_cm_header.h"
#include "m_dl_header.h"

/*
 * @addtogroup RRH_L3_DLM
 * @{ 
*/
/**	global evet code and task internal event code conversion table */
DLT_EVENT_CTBL    const dlr_event_ctbl[EC_DLM_MAIN_EVT_MAX] =
{
    /* Event                                 , AddInfo (Signal kind or Timer Id )         , AddInfo 2 (Timer Id Max)  **/
    /* スレッド起動 */
    { CMD_TSKIF_INIREQ                       ,CMD_NUM0                                    ,CMD_NUM0},
    /* REシステムパラメータ更新要求 */
    { CMD_TSKIF_RESYSPARA_UPDATE_REQ         ,CMD_NUM0                                    ,CMD_NUM0},
    /* REファイル更新要求 */
    { CMD_TSKIF_REFIRM_DL_REQ                ,CMD_NUM0                                    ,CMD_NUM0},
    /* REファイル更新中止要求 */
    { CMD_TSKIF_REFIRM_DL_STOP_REQ           ,CMD_NUM0                                    ,CMD_NUM0},
    /* REファイル更新中止指示 */
    { CMD_TSKIF_REFIRM_DL_STOP_IND           ,CMD_NUM0                                    ,CMD_NUM0},
    /* 運用中REファイル更新開始応答 */
    { CMD_TSKIF_USEREFILE_UPDATE_START_RSP   ,CMD_NUM0                                    ,CMD_NUM0},
    /* REファイルデータ読込応答 */
    { D_API_MSGID_FILE_FIRM_READ_RSP         ,CMD_NUM0                                    ,CMD_NUM0},
    /* REファイルデータ読込中止応答 */
    { D_API_MSGID_FILE_FIRM_CANCEL_RSP       ,CMD_NUM0                                    ,CMD_NUM0},
    /* FWファイル更新応答 */
    { D_API_MSGID_FILE_FIRM_UPDATE_RSP       ,CMD_NUM0                                    ,CMD_NUM0},
    /* CPRI信号受信通知                      , ファイル情報報告通知 */
    { CMD_TSKIF_CPRIRCVNTC                   ,CMD_CPRID_FILEINFREQ                        ,CMD_NUM0},
    /* CPRI信号受信通知                      , ファイル情報報告応答 */
    { CMD_TSKIF_REC_FWDNTC                   ,CMD_CPRID_FILEINFRES                        ,CMD_NUM0},
    /* CPRI信号受信通知                      , ファイルデータ送信 */
    { CMD_TSKIF_CPRIRCVNTC                   ,CMD_CPRID_FILEDATSND                        ,CMD_NUM0},
    /* CPRI信号受信通知                      , ファイルデータ送信完了報告通知 */
    { CMD_TSKIF_CPRIRCVNTC                   ,CMD_CPRID_FILSNDFINNTC                      ,CMD_NUM0},
    /* CPRI信号受信通知                      , ファイルデータ送信完了報告応答 */
    { CMD_TSKIF_REC_FWDNTC                   ,CMD_CPRID_FILSNDFINRES                      ,CMD_NUM0},
    /* CPRI信号受信通知                      , ファームウェアファイル報告要求 */
    { CMD_TSKIF_CPRIRCVNTC                   ,CMD_CPRID_FIRMFILEREQ                       ,CMD_NUM0},
    /* CPRI信号受信通知                      , ファームウェアファイル報告応答 */
    { CMD_TSKIF_REC_FWDNTC                   ,CMD_CPRID_FIRMFILERES                       ,CMD_NUM0},
    /* CPRI信号受信通知                      , 運用中ファイル情報報告通知 */
    { CMD_TSKIF_CPRIRCVNTC                   ,CMD_CPRID_UFILEINFREQ                       ,CMD_NUM0},
    /* CPRI信号受信通知                      , 運用中ファイル情報報告応答 */
    { CMD_TSKIF_REC_FWDNTC                   ,CMD_CPRID_UFILEINFRES                       ,CMD_NUM0},
    /* CPRI信号受信通知                      , 運用中ファイルデータ送信 */
    { CMD_TSKIF_CPRIRCVNTC                   ,CMD_CPRID_UFILEDATSND                       ,CMD_NUM0},
    /* CPRI信号受信通知                      , 運用中ファイルデータ送信完了報告通知 */
    { CMD_TSKIF_CPRIRCVNTC                   ,CMD_CPRID_UFILSNDFINNTC                     ,CMD_NUM0},
    /* CPRI信号受信通知                      , 運用中ファイルデータ送信完了報告応答 */
    { CMD_TSKIF_REC_FWDNTC                   ,CMD_CPRID_UFILSNDFINRES                     ,CMD_NUM0},
    /* CPRI信号受信通知                      , 運用中ファームウェアファイル報告要求 */
    { CMD_TSKIF_CPRIRCVNTC                   ,CMD_CPRID_UFIRMFILEREQ                      ,CMD_NUM0},
    /* CPRI信号受信通知                      , 運用中ファームウェアファイル報告応答 */
    { CMD_TSKIF_REC_FWDNTC                   ,CMD_CPRID_UFIRMFILERES                      ,CMD_NUM0},
    /* タイムアウト発生通知                  , ファイルデータ送信間隔タイマ */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_FILEDATSND                        ,CMD_TIMID_FILEDATSND_MAX},
    /* タイムアウト発生通知                  , システムパラメータ情報報告確認タイマ */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_SYSPRM_INF                        ,CMD_TIMID_SYSPRM_INF_MAX},
    /* タイムアウト発生通知                  , システムパラメータ送信完了確認タイマ */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_SYSPRM_SNDFIN                     ,CMD_TIMID_SYSPRM_SNDFIN_MAX},
    /* タイムアウト発生通知                  , システムパラメータ送信確認タイマ */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_SYSPRMSND_3G                      ,CMD_TIMID_SYSPRMSND_S3G},
    /* タイムアウト発生通知                  , ファームウェアファイル報告確認タイマ */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_FIRMFILE_REP_3G                   ,CMD_TIMID_FIRMFILE_REP_MAX},
    /* タイムアウト発生通知                  , ファームウェアファイル情報報告確認タイマ */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_FIRMFILE_INF                      ,CMD_TIMID_FIRMFILE_INF_MAX},
    /* タイムアウト発生通知                  , ファームウェア送信完了確認タイマ */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_FIRMFILE_SNDFIN                   ,CMD_TIMID_FIRMFILE_SNDFIN_MAX},
    /* タイムアウト発生通知                  , ファームウェア送信確認タイマ */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_FIRMWARESND_S3G                   ,CMD_TIMID_FIRMWARESND_S3G},
    /* タイムアウト発生通知                  , ファームウェアファイル報告確認タイマ(更新用) */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_UFIRMFILE_REP                     ,CMD_TIMID_UFIRMFILE_REP_MAX},
    /* タイムアウト発生通知                  , ファームウェアファイル情報報告確認タイマ(更新用) */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_UFIRMFILE_INF                     ,CMD_TIMID_UFIRMFILE_INF_MAX},
    /* タイムアウト発生通知                  , ファームウェア送信完了確認タイマ(更新用) */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_UFIRMFILE_SNDFIN                  ,CMD_TIMID_UFIRMFILE_SNDFIN_MAX},
    /* タイムアウト発生通知                  , ファームウェア送信確認タイマ(更新用) */
    { CMD_TSKIF_TIMOUTNTC                    ,CMD_TIMID_USEFIRMWARESND                    ,CMD_TIMID_USEFIRMWARESND},
    /* firmware erase response */
    { D_API_MSGID_FILE_FIRM_ERASE_RSP        ,CMD_NUM0                                    ,CMD_NUM0},
    /* End                                   , End */
    { DLD_ECTBL_END                          ,CMD_NUM0                                    ,CMD_NUM0}
};
/** 
 * @brief   main matrix table
 * @Bug_No  M-RRU-ZSYS-01703
 * @Bug_No  M-RRU-ZSYS-01749
 * @date    2015/10/04 TDIPS)enoki M-RRU-ZSYS-01703 RE2台接続時に2台目REに暫定シスパラ値配信が動作しない不具合修正
 * @date    2015/10/04 TDIPS)enoki M-RRU-ZSYS-01749 RE2台接続時に2台目REにファームウェア報告要求が動作しない不具合修正(M-RRU-ZSYS-01703の水平展開)
 */
VOID    (* const dlr_mtrxtbl[EC_DLM_MAIN_EVT_MAX][EC_DLM_MAIN_STN_MAX]) (VOID* bufp, USHORT cpri_no) =
{
	/* 0: スレッド起動 */
	{ m_dl_init,											/* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 1: REシステムパラメータ更新要求 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_sysp_dl_req,                                  /* 1: アイドル */
	m_dl_idle_sysp_dl_req,	//20151004 enoki m_dl_err,      /* 2: FHM自律DL処理中 */
	m_dl_rl_sysp_dl_ng,                                     /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_sysp_dl_ng,                                     /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_sysp_dl_ng,                                     /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_sysp_dl_ng,                                     /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_sysp_dl_ng,                                     /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_sysp_dl_ng,                                     /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_sysp_dl_ng,                                     /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_sysp_dl_ng,                                     /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_sysp_dl_ng,                                     /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_sysp_dl_ng,                                     /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_sysp_dl_ng },                                   /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 2: REファイル更新要求 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_refw_dl_req,                                  /* 1: アイドル */
	m_dl_idle_refw_dl_req,	//20151004 enoki m_dl_err,      /* 2: FHM自律DL処理中 */
	m_dl_rl_refw_dl_ng,                                     /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_refw_dl_ng,                                     /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_refw_dl_ng,                                     /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_refw_dl_ng,                                     /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_refw_dl_ng,                                     /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_refw_dl_ng,                                     /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_refw_dl_ng,                                     /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_refw_dl_ng,                                     /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_refw_dl_ng,                                     /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_refw_dl_ng,                                     /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_refw_dl_ng },                                   /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 3: REファイル更新中止要求 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_refw_stop_req,                                /* 1: アイドル */
	m_dl_fl_refw_stop_req,                                  /* 2: FHM自律DL処理中 */
	m_dl_rl_finfrsp_waiting_sysp_stop_req,                  /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_fsnd_waiting_sysp_stop_req,                     /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_fsndcmprsp_waiting_sysp_stop_req,               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_fwreprsp_waiting_refw_stop_req,                 /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_finfrsp_waiting_refw_stop_req,                  /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_fw_stop_req,                       /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_fsndcmprsp_waiting_refw_stop_req,               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_ufwreprsp_waiting_refw_stop_req,                /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_ufinfrsp_waiting_refw_stop_req,                 /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_fw_stop_req,                      /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_ufsndcmprsp_waiting_refw_stop_req },            /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 4: REファイル更新中止指示 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_refw_stop_ind,                                /* 1: アイドル */
	m_dl_fl_refw_stop_ind,                                  /* 2: FHM自律DL処理中 */
	m_dl_rl_finfrsp_waiting_sysp_stop_ind,                  /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_fsnd_waiting_sysp_stop_ind,                     /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_fsndcmprsp_waiting_sysp_stop_ind,               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_fwreprsp_waiting_refw_stop_ind,                 /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_finfrsp_waiting_refw_stop_ind,                  /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_fw_stop_ind,                       /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_fsndcmprsp_waiting_refw_stop_ind,               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_ufwreprsp_waiting_refw_stop_ind,                /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_ufinfrsp_waiting_refw_stop_ind,                 /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_fw_stop_ind,                      /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_ufsndcmprsp_waiting_refw_stop_ind },            /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 5: 運用中REファイル更新開始応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_refw_start_rsp,                    /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_refw_start_rsp,                   /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 6: REファイルデータ読込応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_load_refw_rsp,                                  /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 7: REファイルデータ読込中止応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_load_refw_stop_rsp,                             /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 8: FWファイル更新応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 1: アイドル */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 2: FHM自律DL処理中 */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 6: ファームウェア報告応答集約中 */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 7: ファイルデータ転送先決定待ち */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_save_fw_rsp,                                    /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_fwsavecmp_waiting_fwsave_rsp,                      /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_usave_fw_rsp },                                 /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 9: ファイル情報報告通知 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_finf_ntc,                                     /* 1: アイドル */
	m_dl_fl_finf_ntc,                                       /* 2: FHM自律DL処理中 */
	m_dl_rl_syspinfrsp_waiting_finf_ntc_ng,                 /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_syspsnd_waiting_finf_ntc_ng,                    /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_syspsndcmprsp_waiting_finf_ntc_ng,              /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_fwreprsp_waiting_finf_ntc_ng,                   /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_finfrsp_waiting_finf_ntc,                       /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_finf_ntc,                          /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_fsndcmprsp_waiting_finf_ntc,                    /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_ufwreprsp_waiting_finf_ntc_ng,                  /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_ufinfrsp_waiting_finf_ntc_ng,                   /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_finf_ntc_ng,                      /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_ufsndcmprsp_waiting_finf_ntc_ng },              /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 10: ファイル情報報告応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_finf_rsp,                                       /* 2: FHM自律DL処理中 */
	m_dl_rl_finfrsp_waiting_sysp_finf_rsp,                  /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_finf_rsp,                          /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 11: ファイルデータ送信 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_fsnd_waiting_syspsnd,                           /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_finfrsp_waiting_fwsnd,                          /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_fwsnd,                             /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_fsndcmprsp_waiting_fwsnd,                       /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 12: ファイルデータ送信完了報告通知 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_fsndcmp_ntc,                                  /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_fsnd_waiting_syspsndcmp_ntc,                    /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_fwsndcmp_ntc,                      /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 13: ファイルデータ送信完了報告応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_fsndcmp_rsp,                                    /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_fsndcmprsp_waiting_syspsndcmp_rsp,              /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_fsndcmprsp_waiting_fwsndcmp_rsp,                /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 14: ファームウェアファイル報告要求 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_fwrep_req,                                    /* 1: アイドル */
	m_dl_fl_fwrep_req,                                      /* 2: FHM自律DL処理中 */
	m_dl_rl_syspinfrsp_waiting_fwrep_req,                   /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_syspsnd_waiting_fwrep_req,                      /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_syspsndcmprsp_waiting_fwrep_req,                /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_fwreprsp_waiting_fwrep_req,                     /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_finfrsp_waiting_fwrep_req,                      /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_fwrep_req,                         /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_fsndcmprsp_waiting_fwrep_req,                   /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_ufwreprsp_waiting_fwrep_req,                    /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_ufinfrsp_waiting_fwrep_req,                     /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_fwrep_req,                        /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_ufsndcmprsp_waiting_fwrep_req },                /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 15: ファームウェアファイル報告応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_fwrep_rsp,                                      /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_fwrep_rsp,                                      /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 16: 運用中ファイル情報報告通知 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_ufinf_ntc,                                    /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_rl_syspinfrsp_waiting_ufinf_ntc_ng,                /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_syspsnd_waiting_ufinf_ntc_ng,                   /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_syspsndcmprsp_waiting_ufinf_ntc_ng,             /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_fwreprsp_waiting_ufinf_ntc_ng,                  /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_finfrsp_waiting_ufinf_ntc_ng,                   /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_ufinf_ntc_ng,                      /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_fsndcmprsp_waiting_ufinf_ntc_ng,                /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_ufwreprsp_waiting_ufinf_ntc_ng,                 /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_ufinfrsp_waiting_ufinf_ntc,                     /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_ufinf_ntc,                        /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_ufsndcmprsp_waiting_ufinf_ntc },                /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 17: 運用中ファイル情報報告応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_ufinf_rsp,                        /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 18: 運用中ファイルデータ送信 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_ufinfrsp_waiting_ufwsnd,                        /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_ufwsnd,                           /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_ufsndcmprsp_waiting_ufwsnd },                   /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 19: 運用中ファイルデータ送信完了報告通知 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_ufsndcmp_ntc,                                 /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_ufwsndcmp_ntc,                    /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 20: 運用中ファイルデータ送信完了報告応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_ufsndcmprsp_waiting_ufwsndcmp_rsp },            /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 21: 運用中ファームウェアファイル報告要求 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_idle_ufwrep_req,                                   /* 1: アイドル */
	m_dl_fl_ufwrep_req,                                     /* 2: FHM自律DL処理中 */
	m_dl_rl_syspinfrsp_waiting_ufwrep_req,                  /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_syspsnd_waiting_ufwrep_req,                     /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_syspsndcmprsp_waiting_ufwrep_req,               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_fwreprsp_waiting_ufwrep_req,                    /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_finfrsp_waiting_ufwrep_req,                     /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_ufwrep_req,                        /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_fsndcmprsp_waiting_ufwrep_req,                  /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_ufwreprsp_waiting_ufwrep_req,                   /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_ufinfrsp_waiting_ufwrep_req,                    /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_ufwrep_req,                       /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_ufsndcmprsp_waiting_ufwrep_req },               /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 22: 運用中ファームウェアファイル報告応答 */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_ufwrep_rsp,                                     /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 23: ファイルデータ送信間隔タイマ */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_fsnd_itv_to,                                    /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 24: システムパラメータ情報報告確認タイマ */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_syspinf_cnf_to,                                 /* 2: FHM自律DL処理中 */
	m_dl_rl_finfrsp_waiting_syspinf_cnf_to,                 /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 25: システムパラメータ送信完了確認タイマ */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_syspsndcmp_cnf_to,                              /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_rl_fsndcmprsp_waiting_syspsndcmp_cnf_to,           /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 26: システムパラメータ送信確認タイマ */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_rl_fsnd_waiting_syspsnd_cnf_to,                    /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 27: ファームウェアファイル報告確認タイマ */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_fwrep_cnf_to,                                   /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_rl_fwrep_cnf_to,                                   /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 28: ファームウェアファイル情報報告確認タイマ */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_fwinf_cnf_to,                                   /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_fwinf_cnf_to,                      /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 29: ファームウェア送信完了確認タイマ */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_fl_fwsndcmp_cnf_to,                                /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_rl_fsndcmprsp_waiting_fwsndcmp_cnf_to,             /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 30: ファームウェア送信確認タイマ */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_rl_finfrsp_waiting_fwsnd_cnf_to,                   /* 7: ファイルデータ転送先決定待ち */
	m_dl_rl_fsnd_waiting_fwsnd_cnf_to,                      /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 31: ファームウェアファイル報告確認タイマ(更新用) */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_rl_ufwrep_cnf_to,                                  /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 32: ファームウェアファイル情報報告確認タイマ(更新用) */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_ufwinf_cnf_to,                    /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err },                                             /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 33: ファームウェア送信完了確認タイマ(更新用) */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_rl_ufsndcmprsp_waiting_ufwsndcmp_cnf_to },         /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	/* 34: ファームウェア送信確認タイマ(更新用) */
	{ m_dl_err,                                             /* 0: 初期状態 */
	m_dl_err,                                               /* 1: アイドル */
	m_dl_err,                                               /* 2: FHM自律DL処理中 */
	m_dl_err,                                               /* 3: ファイル情報報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 4: ファイルデータ送信(シスパラ)待ち */
	m_dl_err,                                               /* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	m_dl_err,                                               /* 6: ファームウェア報告応答集約中 */
	m_dl_err,                                               /* 7: ファイルデータ転送先決定待ち */
	m_dl_err,                                               /* 8: ファイルデータ送信(FW)待ち */
	m_dl_err,                                               /* 9: ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	m_dl_err,                                               /* 10: 運用中ファームウェア報告応答集約中 */
	m_dl_rl_ufinfrsp_waiting_ufwsnd_cnf_to,                 /* 11: 運用中ファイルデータ転送先決定待ち */
	m_dl_rl_ufsnd_waiting_ufwsnd_cnf_to,                    /* 12: 運用中ファイルデータ送信(FW)待ち */
	m_dl_err }                                              /* 13: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
};

/** main matrix table */
 VOID    (* const dlr_fhm_mtrxtbl[EC_DLM_FHMDL_EVT_MAX][EC_DLM_FHMDL_STN_MAX]) (VOID* bufp, USHORT cpri_no) = 
{
 	/* REシステムパラメータ更新要求 */
	{ 
	m_dl_fhm_idle_sysp_dl_req ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err 
	},
	/* REファイル更新要求 */
	{ m_dl_fhm_idle_refw_dl_req ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err
	},
	/* REファイル更新中止要求 */
	{ m_dl_fhm_nop ,
	m_dl_fhm_finfrsp_waiting_sysp_stop_req ,
	m_dl_fhm_fsndcmprsp_waiting_sysp_stop_req ,
	m_dl_fhm_fwreprsp_waiting_refw_stop_req ,
	m_dl_fhm_finfrsp_waiting_refw_stop_req ,
	m_dl_fhm_fwload_waiting_refw_stop_req ,
	m_dl_fhm_fsnd_waiting_fw_stop_req ,
	m_dl_fhm_fsndcmprsp_waiting_refw_stop_req
	},
	/* REファイル更新中止指示 */
	{ m_dl_fhm_nop ,
	m_dl_fhm_finfrsp_waiting_sysp_stop_ind ,
	m_dl_fhm_fsndcmprsp_waiting_sysp_stop_ind ,
	m_dl_fhm_fwreprsp_waiting_refw_stop_ind ,
	m_dl_fhm_finfrsp_waiting_refw_stop_ind ,
	m_dl_fhm_fwload_waiting_refw_stop_ind ,
	m_dl_fhm_fsnd_waiting_fw_stop_ind ,
	m_dl_fhm_fsndcmprsp_waiting_refw_stop_ind
	},
	/* REファイルデータ読込応答 */
	{ m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_fwload_waiting_fwload_rsp ,
	m_dl_fhm_err ,
	m_dl_fhm_err
	},
	/* REファイルデータ読込中止応答 */
	{ m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_fwload_waiting_fwloadstop_rsp ,
	m_dl_fhm_err ,
	m_dl_fhm_err
	},
	/* ファイル情報報告通知(対象データ:シスパラ) */
	{ m_dl_fhm_nop ,
	m_dl_fhm_syspinfrsp_waiting_syspinf_ntc ,
	m_dl_fhm_syspsndcmprsp_waiting_syspinf_ntc ,
	m_dl_fhm_fwreprsp_waiting_syspinf_ntc ,
	m_dl_fhm_finfrsp_waiting_syspinf_ntc ,
	m_dl_fhm_fwload_waiting_syspinf_ntc ,
	m_dl_fhm_fsnd_waiting_syspinf_ntc ,
	m_dl_fhm_fsndcmprsp_waiting_syspinf_ntc
	},
	/* ファイル情報報告応答 */
	{ m_dl_fhm_err ,
	m_dl_fhm_finfrsp_waiting_finf_rsp ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_finfrsp_waiting_fwinf_rsp ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err
	},
	/* ファイルデータ送信完了報告応答 */
	{ m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_rsp ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_fsndcmprsp_waiting_fsndcmp_rsp
	},
	/* ファームウェアファイル報告要求 */
	{ m_dl_fhm_nop ,
	m_dl_fhm_syspinfrsp_waiting_fwrep_req ,
	m_dl_fhm_syspsndcmprsp_waiting_fwrep_req ,
	m_dl_fhm_fwreprsp_waiting_fwrep_req ,
	m_dl_fhm_finfrsp_waiting_fwrep_req ,
	m_dl_fhm_fwload_waiting_fwrep_req ,
	m_dl_fhm_fsnd_waiting_fwrep_req ,
	m_dl_fhm_fsndcmprsp_waiting_fwrep_req
	},
	/* ファームウェアファイル報告応答 */
	{ m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_fwreprsp_waiting_fwrep_rsp ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err
	},
	/* 運用中ファームウェアファイル報告要求 */
	{ m_dl_fhm_nop ,
	m_dl_fhm_syspinfrsp_waiting_ufwrep_req ,
	m_dl_fhm_syspsndcmprsp_waiting_ufwrep_req ,
	m_dl_fhm_fwreprsp_waiting_ufwrep_req ,
	m_dl_fhm_finfrsp_waiting_ufwrep_req ,
	m_dl_fhm_fwload_waiting_ufwrep_req ,
	m_dl_fhm_fsnd_waiting_ufwrep_req ,
	m_dl_fhm_fsndcmprsp_waiting_ufwrep_req
	},
	/* ファイルデータ送信間隔タイマ */
	{ m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_fsnd_waiting_fsnd_itv_to ,
	m_dl_fhm_err
	},
	/* システムパラメータ情報報告確認タイマ */
	{ m_dl_fhm_err ,
	m_dl_fhm_finfrsp_waiting_syspinf_cnf_to ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err
	},
	/* システムパラメータ送信完了確認タイマ */
	{ m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_cnf_to ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err
	},
	/* ファームウェアファイル報告確認タイマ */
	{ m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err
	},
	/* ファームウェアファイル情報報告確認タイマ */
	{ m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_finfrsp_waiting_fwinf_cnf_to ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err
	},
	/* ファームウェア送信完了確認タイマ */
	{ m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_err ,
	m_dl_fhm_fsndcmprsp_waiting_fwsndcmp_cnf_to
	}
};

/** matrix table (REC主導) */
VOID    (* const dlr_rec_mtrxtbl[EC_DLM_RECDL_EVT_MAX][EC_DLM_RECDL_STN_MAX]) (VOID* bufp, USHORT cpri_no)=
{
	/* 0: REファイル更新中止要求 */
	{ m_dl_rec_err ,
	m_dl_rec_finfrsp_waiting_sysp_stop_req ,
	m_dl_rec_fsnd_waiting_sysp_stop_req ,
	m_dl_rec_fsndcmprsp_waiting_sysp_stop_req ,
	m_dl_rec_fwreprsp_waiting_refw_stop_req ,
	m_dl_rec_finfrsp_waiting_refw_stop_req ,
	m_dl_rec_fsnd_waiting_fw_stop_req ,
	m_dl_rec_fsndcmprsp_waiting_refw_stop_req ,
	m_dl_rec_ufwreprsp_waiting_refw_stop_req ,
	m_dl_rec_ufinfrsp_waiting_refw_stop_req ,
	m_dl_rec_ufsnd_waiting_fw_stop_req ,
	m_dl_rec_ufsndcmprsp_waiting_refw_stop_req
	 },
	/* 1: REファイル更新中止指示 */
	{ m_dl_rec_err ,
	m_dl_rec_finfrsp_waiting_sysp_stop_ind ,
	m_dl_rec_fsnd_waiting_sysp_stop_ind ,
	m_dl_rec_fsndcmprsp_waiting_sysp_stop_ind ,
	m_dl_rec_fwreprsp_waiting_refw_stop_ind ,
	m_dl_rec_finfrsp_waiting_refw_stop_ind ,
	m_dl_rec_fsnd_waiting_fw_stop_ind ,
	m_dl_rec_fsndcmprsp_waiting_refw_stop_ind ,
	m_dl_rec_ufwreprsp_waiting_refw_stop_ind ,
	m_dl_rec_ufinfrsp_waiting_refw_stop_ind ,
	m_dl_rec_ufsnd_waiting_fw_stop_ind ,
	m_dl_rec_ufsndcmprsp_waiting_refw_stop_ind
	 },
	/* 2: 運用中REファイル更新開始応答 */
	{ m_dl_rec_idle_fupdate_rsp ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 3: ファイル情報報告通知（対象データ：シスパラ） */
	{ m_dl_rec_idle_finf_ntc ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 4: ファイル情報報告応答 */
	{ m_dl_rec_err ,
	m_dl_rec_finfrsp_waiting_sysp_finf_rsp ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_finfrsp_waiting_refw_finf_rsp ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 5: ファイルデータ送信 */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_fsnd_waiting_syspsnd ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_finfrsp_waiting_fwsnd ,
	m_dl_rec_fsnd_waiting_fwsnd ,
	m_dl_rec_fsndcmprsp_waiting_fwsnd ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 6: ファイルデータ送信完了報告通知 */
	{ m_dl_rec_idle_fsndcmp_ntc ,
	m_dl_rec_err ,
	m_dl_rec_fsnd_waiting_syspsndcmp_ntc ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_fsnd_waiting_fwsndcmp_ntc ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 7: ファイルデータ送信完了報告応答 */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_fsndcmprsp_waiting_syspsndcmp_rsp ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_fsndcmprsp_waiting_fwsndcmp_rsp ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 8: ファームウェアファイル報告要求 */
	{ m_dl_rec_idle_fwrep_req ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_fwreprsp_waiting_fwrep_req ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 9: ファームウェアファイル報告応答 */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_fwreprsp_waiting_fwrep_rsp ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 11: 運用中ファイル情報報告応答 */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_ufinfrsp_waiting_refw_ufinf_rsp ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 12: 運用中ファイルデータ送信 */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_ufinfrsp_waiting_ufwsnd ,
	m_dl_rec_ufsnd_waiting_ufwsnd ,
	m_dl_rec_ufsndcmprsp_waiting_ufwsnd
	 },
	/* 13: 運用中ファイルデータ送信完了報告通知 */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_ufsnd_waiting_ufwsndcmp_ntc ,
	m_dl_rec_err
	 },
	/* 14: 運用中ファイルデータ送信完了報告応答 */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_rsp
	 },
	/* 15: 運用中ファームウェアファイル報告要求 */
	{ m_dl_rec_idle_ufwrep_req ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_ufwreprsp_waiting_ufwrep_req ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 16: 運用中ファームウェアファイル報告応答 */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_ufwreprsp_waiting_ufwrep_rsp ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 17: システムパラメータ情報報告確認タイマ */
	{ m_dl_rec_err ,
	m_dl_rec_finfrsp_waiting_syspinf_cnf_to ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 18: システムパラメータ送信完了確認タイマ */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_fsndcmprsp_waiting_syspsndcmp_cnf_to ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 20: ファームウェアファイル報告確認タイマ */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_fwreprsp_waiting_fwrep_cnf_to ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 21: ファームウェアファイル情報報告確認タイマ */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_finfrsp_waiting_fwinf_cnf_to ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 22: ファームウェア送信完了確認タイマ */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_fsndcmprsp_waiting_fwsndcmp_cnf_to ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 24: ファームウェアファイル報告確認タイマ(更新用) */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_ufwreprsp_waiting_ufwrep_cnf_to ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 25: ファームウェアファイル情報報告確認タイマ(更新用) */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_ufinfrsp_waiting_ufwinf_cnf_to ,
	m_dl_rec_err ,
	m_dl_rec_err
	 },
	/* 26: ファームウェア送信完了確認タイマ(更新用) */
	{ m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_err ,
	m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_cnf_to
	}
};
/*!
 * @name 3G and S3G systemparameter size table.
 */
/* @{ */
const	USHORT    dlr_syssize_settbl[] =
{
    DLD_SYSPAR3G_SIZE,        /* the 3G systemparameter size */
	DLD_SYSPARS3G_SIZE        /* the S3G systemparameter size */
};
/* @} */

/*!
 * @name 3G and S3G maker name for 3001 type table.
 */
const	USHORT	dlr_makername_settbl[] =
{
	DLD_MAKER1,				/* the maker name 'F' */
	DLD_MAKER2,				/* the maker name 'R' */
	/** DCM Zynq: add "K" **/
	DLD_MAKER3,				/* the maker name 'K' */
	/*TDD: add "V"*/
	DLD_MAKER4				/* the maker name 'V' */
	
};

/*!
 * @name system parameter-D valid check table.
 */

const DLT_SYSPRMCHK dlr_sysprmchk[DLD_SYSPRAD_NUM] = 
{	
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_1 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_2 },
	{ DLD_MIN_0001,   DLD_MAX_00FF,   DLD_TYP_3 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_4 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_5 },
	{ DLD_MIN_0000,   DLD_MAX_0010,   DLD_TYP_6 },
	{ DLD_MIN_0000,   DLD_MAX_0010,   DLD_TYP_7 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_8 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_9 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_10 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_11 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_12 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_13 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_14 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_15 },
	{ DLD_MIN_0001,   DLD_MAX_0002,   DLD_TYP_16 }
};

/*!
 * @name system parameter-E,F valid check table.
 */
const DLT_SYSPRMCHK dlr_sysprmefchk[DLD_SYSPRAEF_NUM] = 
{
	{ DLD_MIN_0000,   DLD_MAX_0001,	  DLD_TYP_17 },
	{ DLD_MIN_0000,   DLD_MAX_C924,   DLD_TYP_18 },
	{ DLD_MIN_0000,   DLD_MAX_FFFF,   DLD_TYP_19 },
	{ DLD_MIN_0000,   DLD_MAX_0002,   DLD_TYP_20 },
	{ DLD_MIN_0000,   DLD_MAX_C210,   DLD_TYP_21 },
	{ DLD_MIN_0000,   DLD_MAX_C210,   DLD_TYP_22 },
	{ DLD_MIN_0000,   DLD_DUMMY_FFFF, DLD_TYP_23 },
	{ DLD_MIN_0000,   DLD_DUMMY_FFFF, DLD_TYP_24 },
	{ DLD_MIN_9612,   DLD_MAX_9888,   DLD_TYP_25 },
	{ DLD_MIN_9612,   DLD_MAX_9888,   DLD_TYP_26 },
	{ DLD_MIN_9612,   DLD_MAX_9888,   DLD_TYP_27 },
	{ DLD_MIN_9612,   DLD_MAX_9888,   DLD_TYP_28 },
	{ DLD_MIN_0000,   DLD_MAX_0001,   DLD_TYP_29 },
	{ DLD_MIN_0001,   DLD_MAX_FFFF,   DLD_TYP_30 },
	{ DLD_MIN_0001,   DLD_DUMMY_FFFF, DLD_TYP_31 },
	{ DLD_MIN_0000,   DLD_DUMMY_FFFF, DLD_TYP_32 }
};

/* @} */



