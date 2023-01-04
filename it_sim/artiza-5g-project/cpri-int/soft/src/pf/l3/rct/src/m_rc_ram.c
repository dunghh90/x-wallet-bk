/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_rc_ram.c
 *  @brief   declare file for RCT TRA table
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2011/05/13 FJT)koshida modify for CeNB-F-073-008(F)(REにおける故障発生時の機能部状態報告)
 *  @date   2015/08/07 TDIPS)maruyama 装置立ち明けの変数
 *  @date   2015/08/24 TDIPS)maruyama 装置立ち明けの変数の型変更、不要変数削除、変通追加
 *  @date   2015/09/10 TDIPS)maruyama get_re_EqpSlotInfo_＊＊をローカル変数に変更
 *  @date   2015/09/30 TDIPS)maruyama re_StartUp_CPRI_Conn_flg追加
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 *  @date   2015/11/07 TDIPS)sasaki IT2問処No.160対処
 *  @date   2016/04/25 FJT)koshida 3001-15B-TS123対応(3G/LTE個別にTILT-INF信号の変化情報を作成する)
 *  @date   2017/04/07 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2017
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/* MDIF共通ヘッダファイル				*/
#include "m_rc_header.h"			/* RE制御タスクヘッダファイル			*/


/********************************************************************************************************************/
/**
 *  @brief     declare file for RCT TRA table
 *  @note     declare file for RCT TRA table
 *  @param  None
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)linllj create
 */
/********************************************************************************************************************/

/****************************************************************************/
/* ログカウンタテーブル														*/
/****************************************************************************/
USHORT					rcw_logcnt;

/****************************************************************************/
/* ログ取得テーブル															*/
/****************************************************************************/
RCT_LOG_T				rcw_log_tbl[RCD_LOG_MAX];

/****************************************************************************/
/* タスク状態管理テーブル													*/
/****************************************************************************/
USHORT					rcw_tskstn;


/****************************************************************************/
/* CPRI message flag for 3G/S3G  0:3G 1:S3G										*/
/****************************************************************************/
USHORT					rcw_cpri_msg_flg;

/****************************************************************************/
/* REカード状態作成Work領域													*/
/****************************************************************************/
RCT_RECARDSTAWORK_T		rcw_reCardStaWork_tbl[RCD_FB_NUM];

USHORT					rcw_remotecnt_flg;

/****************************************************************************/
/* TRA card first active flag */
/****************************************************************************/
USHORT					rcw_first_act_flg;
USHORT					rcw_first_prc_act_flg;

/** @name device kind tbl for RCT */
/* @{ */

/****************************************************************************/
/* device kind tbl for RCT */
/****************************************************************************/
USHORT					rcw_device_kind;
/* @} */

/** @name carrier status management table for RCT */
/* @{ */

/****************************************************************************/
/* carrier status management table for RCT */
/****************************************************************************/
RCT_CARSTS_MNG_T					rcw_carsts_tbl;
/* @} */

/**
* @brief SV-MT/eNB-MT接続強制解放送信先
* @note  [ 値の説明 ] Sent/Not-Sent
* @note  [ 用途     ] Save status of sending SV-MT/eNB-MT接続強制解放要求 to 配下RE
* @note  [ 注意事項 ]
* @note  [ 更新契機 ] 以下の契機
*                     - 立ち上げ : Not Sent
*                     - SV-MT/eNB-MT接続強制解放要求受信 : Sent
*                     - SV-MT/eNB-MT接続強制解放確認タイマ タイムアウト : Not-Sent
*                     - 要求を送信したすべての配下REから正常応答を受信 : Not-Sent
*/
USHORT	gw_rcw_SvEnb_Mt_ForRelConn_ReqDstSndSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX] = {{ D_L3_OFF }};

/**
*
* [Name]  テーブル更新 REバージョン報告送信先
*
* [値の説明] Sent/Not-Sent
*
* [用途] Save status of sending REバージョン報告要求 to 配下RE
*
* [注意事項]
*
* [値の設定タイミング]
*
*                    立ち上げ Not Sent
*
*                   REバージョン報告要求受信 Sent
*
*					REバージョン報告確認タイマ タイムアウト Not-Sent
*
*					要求を送信したすべての配下REから正常応答を受信 Not-Sent
*/
USHORT	gw_rcw_ReVerRep_ReqDstSndSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX] = {{ D_L3_OFF }};


/**
*
* [Name]  テーブル更新SV-MT/eNB-MT接続強制解放応答元
*
* [値の説明] Received/Not-Received
*
* [用途] Save status of receiving SV-MT/FLD-MT接続強制解放応答受信 from 配下RE
*
* [注意事項]
*
* [値の設定タイミング]
*
*                   立ち上げ Not-Received
*
*                   配下REからのSV-MT/FLD-MT接続強制解放応答受信 Received
*
*					要求を送信したすべての配下REから正常応答を受信 Not-received
*/
USHORT	gw_rcw_SvEnb_Mt_ForRelConn_ResSrcRcvSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX] = {{D_L3_OFF }};

/**
*
* [Name]  テーブル更新 REバージョン報告応答元
*
* [値の説明] Received/Not-Received
*
* [用途] Save status of receiving REバージョン報告応答受信 from 配下RE
*
* [注意事項]
*
* [値の設定タイミング]
*
*                   立ち上げ Not-Received
*
*                   配下REからのREバージョン報告応答受信 Received
*
*					要求を送信したすべての配下REから正常応答を受信 Not-received
*/
USHORT	gw_rcw_ReVerRep_ResSrcRcvSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX] = {{D_L3_OFF }};

/**
 * [Name]     FLD-MTデータコマンド転送受信応答待ちフラグ
 *
 * [値の説明] FLD-MTデータコマンド転送受信応答待ちの状態を保持する
 *
 * [用途]     FLD-MTデータコマンド転送受信応答を受信するタイミングで待ち応答フラグをチェックして、未待ちであれば、実施しない。
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 *
 *                   立ち上げ  未待ち
 *
 *                   FLD-MTデータコマンド転送受信要求受信  待ち
 *
 *                   FLD-MTデータコマンド転送受信応答を送信時  未待ち
 *
 *                   FLD-MTデータコマンド転送受信確認タイマタイムアウト検出時  未待ち
 */
USHORT	gw_rcw_FldMt_CmdRes_WaitFlg[CMD_MAX_SYSNUM] = { D_L3_OFF };

/**
 * @brief FLD-MTデータファイル転送送信応答待ちフラグ
 * @note  [ 値の説明 ] FLD-MTデータファイル転送送信応答待ちの状態を保持する
 * @note  [ 用途     ] FLD-MTデータファイル転送送信応答を受信するタイミングでチェックして、未待ちであれば実施しない
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] 以下の契機
 *                     - 立ち上げ : 未待ち
 *                     - FLD-MTデータファイル転送送信要求受信 : 待ち
 *                     - FLD-MTデータファイル転送送信応答を送信時 : 未待ち
 *                     - FLD-MTデータファイル転送送信確認タイマタイムアウト検出時 : 未待ち
 */
USHORT	gw_rcw_FldMt_FilRes_WaitFlg[CMD_MAX_SYSNUM] = { D_L3_OFF };

/**
 * @brief FLD-MTデータコマンド転送受信要求保持データ
 * @note  [ 値の説明 ] FLD-MTデータコマンド転送受信要求RE番号, 全受信データ数, 送信データ順序番号を保持する
 * @note  [ 用途     ] FLD-MTデータコマンド転送受信応答を送信時、保持データを使って該当REに送信する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
RCT_RCV_DATA_CMD_T	gw_rcw_FldMt_CmdRcvReqData[CMD_MAX_SYSNUM];


/**
* @brief FLD-MT接続応答待ちフラグ
* @note  [ 値の説明 ] Wait/Not-wait
* @note  [ 用途     ] Save status of MT tool
* @note  [ 注意事項 ]
* @note  [ 更新契機 ] 以下の契機
*                     - FLD-MT接続解放確認タイマ停止 : Not-wait
*                     - FLD-MT接続解放確認タイマ起動 : Wait

*/
USHORT	gw_rcw_FldMt_ConnRel_WaitFlg[CMD_MAX_SYSNUM] = { D_L3_OFF };

/**
* @brief FLD-MTソースRE
* @note  [ 値の説明 ] FLD-MT接続要求を送信する配下RE
* @note  [ 用途     ] Save status of MT tool
* @note  [ 注意事項 ]
* @note  [ 更新契機 ] 以下の契機
*                     - 立ち上げ : 0
*                     - FLD-MT接続解放要求受信 : RE number
*/

/**
 * @brief FLD-MTデータファイル転送送信応答待ちフラグ
 * @note  [ 値の説明 ] FLD-MTデータファイル転送送信応答待ちの状態を保持する
 * @note  [ 用途     ] FLD-MTデータファイル転送送信応答を受信するタイミングで待ち応答フラグをチェックして、
 *                     未待ちであれば実施しない
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] 以下の契機
 *                     - 立ち上げ : 未待ち
 *                     - FLD-MTデータファイル転送送信要求受信 : 待ち
 *                     - FLD-MTデータファイル転送送信応答を送信時 : 未待ち
 *                     - FLD-MTデータファイル転送送信確認タイマタイムアウト検出時 : 未待ち
 */
USHORT	gw_rcw_FldMt_ConnRes_WaitFlg[CMD_MAX_SYSNUM] = { D_L3_OFF };



USHORT	gw_rcw_FldMt_RelReqRe[CMD_MAX_SYSNUM] = {CMD_NUM0};

/**
 * @brief SV-MT/eNB-MTデータファイル転送送信応答待ちフラグ
 * @note  [ 値の説明 ] SV-MT/eNB-MTデータファイル転送送信応答待ちの状態を保持する
 * @note  [ 用途     ] SV-MT/eNB-MTデータファイル転送送信応答を受信するタイミングで待ち応答フラグをチェックして、
 *                     未待ちであれば、実施しない。
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] 以下の契機
 *                     - 立ち上げ : 未待ち
 *                     - SV-MT/eNB-MTデータファイル転送送信要求受信 : 待ち
 *                     - SV-MT/eNB-MTデータファイル転送送信応答を送信時 : 未待ち
 *                     - SV-MT/eNB-MTデータファイル転送送信確認タイマタイムアウト検出時 : 未待ち
 */
USHORT					gw_rcw_SvMt_ConnRes_WaitFlg[CMD_MAX_SYSNUM] = { D_L3_OFF };

/**
* @brief FLD-MT接続強制解放送信先テーブル
* @note  [ 値の説明 ] Send/Not-Sent
* @note  [ 用途     ] Save status of sending FLD-MT接続強制解放要求 to 配下RE
* @note  [ 注意事項 ]
* @note  [ 更新契機 ] 以下の契機
*                     - 立ち上げ : Not-Send
*                     - FLD-MT接続強制解放要求受信 : Sent
*                     - FLD-MT接続強制解放確認タイマ タイムアウト : Not-Sent
*                     - 要求を送信したすべての配下REから正常応答を受信 : Not-Sent
*/
USHORT					gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX] = {{D_L3_OFF}};

/**
* @brief FLD-MT接続強制解放応答元テーブル
* @note  [ 値の説明 ] Received/Not-received
* @note  [ 用途     ] Save status of receiving FLD-MT接続強制解放応答受信 from 配下RE
* @note  [ 注意事項 ]
* @note  [ 更新契機 ] 以下の契機
*                     - 立ち上げ : Not-Received
*                     - 配下REからのFLD-MT接続強制解放応答受信 : Received
*                     - 要求を送信したすべての配下REから正常応答を受信 : Not-received
*/
USHORT				gw_rcw_Fld_Mt_ForRelConn_ResSrcRcvSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX] = {{D_L3_OFF}};


/**
* @brief SV-MT接続強制解放要求再送カウンタ
* @note  [ 値の説明 ] Number
* @note  [ 用途     ] count number send SV-MT接続強制解放要求
* @note  [ 注意事項 ]
* @note  [ 更新契機 ] 以下の契機
*                     - 要求送信 : init(0)
*                     - 応答受信 : init(0)
*                     - TimeOut : count up(+1)
*/
UCHAR				g_SvMt_retry_counter[CMD_MAX_SYSNUM] = { D_RETRY_COUNTER_INIT };

/**
* @brief FLD-MT接続強制解放要求再送カウンタ
* @note  [ 値の説明 ] Number
* @note  [ 用途     ] count number send FLD-MT接続強制解放要求
* @note  [ 注意事項 ]
* @note  [ 更新契機 ] 以下の契機
*                     - 要求送信 : init(0)
*                     - 応答受信 : init(0)
*                     - TimeOut : count up(+1)
*/
UCHAR				g_FldMt_retry_counter[CMD_MAX_SYSNUM] = { D_RETRY_COUNTER_INIT };




/**
 * @brief FLD-MT file receive response wait flag
 */
USHORT	gw_rcw_FLDMt_RcvRsp_WaitFlag[CMD_MAX_SYSNUM] = { D_L3_NOT_RES_WAIT };

/**
 * @brief TRX設定一時保存用テーブル(3G)
 */
CMT_CPRIF_TRXSETREQ					gt_rcw_trxset_save_3g_tmp[CMD_MAX_CAR_3G];

/**
 * @brief TRX設定一時保存用テーブル(LTE)
 */
CMT_CPRIF_TRXSETREQ_S3G				gt_rcw_trxset_save_lte_tmp[CMD_MAX_CAR_LTE];
USHORT								gt_rcw_trxset_datSize[CMD_MAX_CAR_LTE];

/**
 * @brief TRX設定送信先(3G)
 */
USHORT                  gw_rcw_trxset_req_flg_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G] = {{ D_L3_OFF }};

/**
 * @brief TRX設定送信先(LTE)
 */
USHORT					gw_rcw_trxset_req_flg_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE] = {{ D_L3_OFF }};

/**
 * @brief TRX設定応答元テーブル(3G)
 */
USHORT					gw_rcw_trxset_res_flg_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G] = {{ D_L3_OFF }};

/**
 * @brief TRX設定応答元テーブル(LTE)
 */
USHORT					gw_rcw_trxset_res_flg_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE] = {{ D_L3_OFF }};

/**
 * @brief TRX設定応答結果テーブル(3G)
 */
USHORT					gw_rcw_trxset_res_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G] = {{ D_L3_OFF }};

/**
 * @brief TRX設定応答結果テーブル(LTE)
 */
USHORT					gw_rcw_trxset_res_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE] = {{ D_L3_OFF }};

/**
 * @brief 応答待ちフラグ(TRX解放応答(3G))
 */
USHORT					gw_rcw_WaitResFlag_trxrel_3g[CMD_MAX_CAR_3G] = { D_L3_OFF };

/**
 * @brief 応答待ちフラグ(TRX解放応答(LTE))
 */
USHORT					gw_rcw_WaitResFlag_trxrel_lte[CMD_MAX_CAR_LTE] = { D_L3_OFF };

/**
 * @brief TRX解放送信先テーブル(3G)
 */
USHORT					gw_rcw_trxrel_req_flg_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G] = {{ D_L3_OFF }};

/**
 * @brief TRX解放送信先テーブル(LTE)
 */
USHORT					gw_rcw_trxrel_req_flg_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE] = {{ D_L3_OFF }};

/**
 * @brief TRX解放応答元テーブル(3G)
 */
USHORT					gw_rcw_trxrel_res_flg_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G] = {{ D_L3_OFF }};

/**
 * @brief TRX解放応答元テーブル(LTE)
 */
USHORT					gw_rcw_trxrel_res_flg_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE] = {{ D_L3_OFF }};

/**
 * @brief TRX解放応答結果テーブル(3G)
 */
USHORT					gw_rcw_trxrel_res_3g_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_3G] = {{ D_L3_OFF }};

/**
 * @brief TRX解放応答結果テーブル(LTE)
 */
USHORT					gw_rcw_trxrel_res_lte_tbl[D_RRH_CPRINO_RE_MAX][CMD_MAX_CAR_LTE] = {{ D_L3_OFF }};


/**
 * @brief REカード状態報告応答ALM/ERR情報格納用テーブル(3G)
 */
USHORT 					gw_3g_err_code[CMD_MAX_ERR_NUM];						/* ERRコード格納用ワークエリア	*/
USHORT 					gw_3g_alm_code[CMD_MAX_ALM_NUM];						/* 発生ALM情報格納用ワークエリア*/

/**
 * @brief REカード状態報告応答ALM/ERR情報格納用テーブル(LTE)
 */
USHORT 					gw_lte_err_code[CMD_MAX_ERR_NUM_HIG_BITRAT];			/* ERRコード格納用ワークエリア	*/
USHORT 					gw_lte_alm_code[CMD_MAX_ALM_NUM_HIG_BITRAT];			/* 発生ALM情報格納用ワークエリア*/

/****************************************************************************/
/* テーブル初期化REバージョン報告応答結果 */
/****************************************************************************/
UINT					gw_rcw_ReVerRep_res_tbl[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX] = {{ D_L3_OFF }};
/****************************************************************************/
/* テーブル初期化REバージョンREC報告内容 */
/****************************************************************************/
REVER_INF               gw_rcw_ReVerRecRepCnt_tbl[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX];
/**
 * [Name]    外部装置データ送信中状態
 *
 * [値の説明]	[0]	3Gの外部装置データ送信中状態
				[1]	LTEの外部装置データ送信中状態
				0	RCD_EXTSNDREQ_IDLE						未処理
				1	RCD_EXTSNDREQ_AISG						AISG信号
				2	RCD_EXTSNDREQ_NONEAISG					非AISG信号
 *
 * [用途]    外部装置データ送信要求／応答処理の競合を判断し競合時はバッファリングする。
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 *
 *                   立ち上げ  未処理
 *
 *                   RECからの外部装置データ送信要求受信(FHMの自ポート以外)  AISG送信中 / 非AISG送信中
 *
 *                   RECに外部装置データ送信応答を送信時  未処理
 *
 *                   配下REからの外部装置データ送信応答待ちタイムアウト検出時  未処理
 */
USHORT	gw_rcw_ExtSndReqSta[CMD_MAX_SYSNUM] = { RCD_EXTSNDREQ_IDLE };

/**
 * [Name]    信号送信対象RE
 *
 * [値の説明]	[0][0～15]		3Gの信号送信対象RE[RE#1～16]
				[1][0～15]		LTEの信号送信対象RE[RE#1～16]
				0	RCD_SIGSNDRE_IDLE						未処理
				1	RCD_SIGSNDRE_RUNNING					送信処理中
				2	RCD_SIGSNDRE_OK							OK応答受信
				3	RCD_SIGSNDRE_NG							NG応答受信


 *
 * [用途]    REから外部装置データ送信応答を受信時に期待するREからの受信であるかを判断する。
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 *
 *                  立ち上げ  未処理
 *
 *                  外部装置データ送信要求で信号送信時  対象RE：RE番号
 *
 					対象REからの外部装置データ送信応答を受信時  対象RE：応答結果
 *
 *                  全対象REから外部装置データ送信応答を受信時  全て：未処理
 					対象REからの外部装置データ送信応答待ちタイムアウト時  全て：未処理
 */
USHORT	gw_rcw_SigSndReNo[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX] = {{ RCD_SIGSNDRE_IDLE }};

/**
*
* [Name]  Data transfer Send Information・
*
* [値の説明]
*
* [用途] Save Data transer information of  FLD-MTデータコマンド転送送信要求
*
* [注意事頁E
*
* [値の設定タイミング]
*
*                    立ち上げ
*
*                     FLD-MTデータコマンド転送送信要求
*
*/
RCT_DATA_CMD_T	gt_rcw_DataTransInfo[CMD_MAX_SYSNUM];

/**
*
* [Name]  FLD-MTデータコマンド転送送信応答待ちフラグ
*
* [値の説明] Wait/Not-wait
*
* [用途] Save FLD-MTデータコマンド転送送信応答待ちフラグ
*
* [注意事項]
*
* [値の設定タイミング]
*
*                   立ち上げ Not wait
*
*                   FLD-MTデータコマンド転送送信要求 wait
*
*					FLD-MTデータコマンド転送送信応答 Not-wait
*
*					FLD-MTデータコマンド転送送信確認タイマTO Not-wait
*/
 USHORT	gw_rcw_FldMt_DataTranferRes_WaitFlg[CMD_MAX_SYSNUM] = { D_L3_OFF };

/**
*
* [Name]  Save Re which sent SV-MT/eNB-MT接続解放要求
*
* [値の説明] Re No
*
* [用途] Save Re which sent SV-MT/eNB-MT接続解放要求
*
* [注意事項]
*
* [値の設定タイミング]
*
*						SV-MT/eNB-MT接続解放要求 Re1->Re16
						立ち上げ 0
*/
USHORT					gw_rcw_SvMt_ConnRelReqRe[CMD_MAX_SYSNUM] = { 0 };

/**
*
* [Name]  テーブル更新送信先
*
* [値の説明] wait/Not wait
*
* [用途] Save status of wait SV-MT/eNB-MT接続解放応答
*
* [注意事項]
*
* [値の設定タイミング]
*
*                   立ち上げ Not wait
*
*                   SV-MT/eNB-MT接続解放要求受信 wait
*
*					SV-MT/eNB-MT接続解放確認タイマタイムアウト not wait
*
*					SV-MT/eNB-MT接続解放応答受信 not wait
*/
USHORT					gw_rcw_SvMt_CntRel_WaitFlg[CMD_MAX_SYSNUM] = { D_L3_OFF };


/** @name 全RE装置構成情報保持テーブル (3G)  */
/* @{ */
/****************************************************************************/
	/* 全RE装置構成情報保持テーブル (3G)*/
/****************************************************************************/
CMT_CPRIF_EQPSTRRES				gt_recw_ReEqpInfArr_3G[D_RRH_CPRINO_RE_MAX];
/* @} */

/** @name 全RE装置構成情報保持テーブル (LTE) */
/* @{ */
/****************************************************************************/
/*  (LTE) 全RE装置構成情報保持テーブル*/
/****************************************************************************/
CMT_CPRIF_EQPSTRRES_S3G			gt_recw_ReEqpInfArr_LTE[D_RRH_CPRINO_RE_MAX];
/* @{ */

/** @name RE装置構成情報更新テーブル (3G)  */
/* @{ */
/****************************************************************************/
	/* RE装置構成情報更新テーブル (3G)*/
/****************************************************************************/
CPRIEQPSTR_INF				gt_recw_ReEqpStrInfTbl_3G;
/* @} */


/** @name RE装置構成情報更新テーブル (LTE)  */
/* @{ */
/****************************************************************************/
	/* RE装置構成情報更新テーブル (LTE) */
/****************************************************************************/
CPRIEQPSTR_INF_S3G			gt_recw_ReEqpStrInfTbl_LTE;

/* @} */


/**
 * [Name]    外部装置データ送信要求バッファ
 *
 * [値の説明]	[0]	3Gの外部装置データ送信要求バッファ
				[1]	LTEの外部装置データ送信要求バッファ

 *
 * [用途]	外部装置データ送信要求処理の競合に後発の外部装置データ送信要求信号をバッファリングする。
			先発の外部装置データ送信要求処理が完了時に、バッファリングされている外部装置データ送信要求処理を行う。

 *
 * [注意事項]
 *
 * [値の設定タイミング]
 *
			・外部装置データ送信要求処理の競合時に後発信号をバッファリングする。
			・バッファリングされている外部装置データ送信要求処理が完了時にクリアする。
 */
RCT_EXT_DATA_SND_T		gt_rcw_ExtDataSndBuf[CMD_MAX_SYSNUM];

/* @} */

/**
*
* [Name]  SV-MT/eNB-MT接続状態
*
* [値の説明] Connect/Not-Connect
*
* [用途] Save status of MT tool
*
* [注意事項]
*
* [値の設定タイミング]
*
*                    立ち上げ Not-Connect
*
*                    SV-MT/eNB-MT接続強制解放要求受信 Not Connect
					 FLD-MT接続要求受信 Connect
*/
USHORT	 gw_rcw_SvEnbMt_ConnSts[CMD_MAX_SYSNUM] = {D_L3_MT_NOT_CONNECT};
/**
*
* [Name]  SV-MT接続先
*
* [値の説明] SV-MTに接続した配下RE
*
* [用途] Save status of SV-MT tool
*
* [注意事項]at the initialization, the initial value is different from any RE number value
*
* [値の設定タイミング]
*
*                  SV-MT/eNB-MTデータ受信要求  RE number
*
*
*/
	USHORT	gw_rcw_SvEnbMt_ConnDst[CMD_MAX_SYSNUM] = { 0 };

/**
*
* [Name]  FLD-MT接続先
*
* [値の説明] FLD-MTに接続した配下RE
*
* [用途] Save status of MT tool
*
* [注意事項]at the initialization, the initial value is different from any RE number value
*
* [値の設定タイミング]
*
*                   FLD-MT接続解放応答を受信する  RE number
*
*
*/
USHORT	gw_rcw_FldMt_ConnDst[CMD_MAX_SYSNUM] = { 0 };

/**
* [Name]   FLD-MT接続状態
*
* [値の説明] Connect/Not-Connect
*
* [用途] Save status of MT tool
*
* [注意事項]
*
* [値の設定タイミング]
*
*                    立ち上げ Not-Connect
*
*                    FLD-MT接続強制解放要求受信 Not Connect
*					 FLD-MT接続要求受信 Connect
*/
USHORT	gw_rcw_FldMt_ConnSts[CMD_MAX_SYSNUM] = {D_L3_MT_NOT_CONNECT};

/* @} */

/**
*
* [Name]   テーブル参照 SV-MT/eNB-MT接続先
*
* [値の説明] Sent/Not-Sent
*
* [用途] テーブルの「SV-MT/eNB-MT接続先」で送信すべき配下REを判定する
*
* [注意事項]
*
* [値の設定タイミング]
*
*                    立ち上げ Not Sent
*
*                   SV-MT/eNB-MT接続強制解放要求受信 Sent
*
*					SV-MT/eNB-MT接続強制解放確認タイマ タイムアウト Not-Sent
*
*					要求を送信したすべての配下REから正常応答を受信 Not-Sent
*/
USHORT	gw_rcw_SvEnb_Mt_RelConn_ReqDstSndSts[CMD_MAX_SYSNUM][D_RRH_CPRINO_RE_MAX] = {{ D_L3_OFF }};

/**
*
* [Name]  Save Re which sent SV-MT/eNB-MTデータ受信要求
*
* [値の説明] Re No
*
* [用途] Save Re which sent SV-MT/eNB-MTデータ受信要求
*
* [注意事項]
*
* [値の設定タイミング]
*
*						SV-MT/eNB-MTデータ受信要求 Re1->Re16
						立ち上げ 0
*/
USHORT					gw_rcw_SvMt_DatRcvReqRe[CMD_MAX_SYSNUM] = { 0 };



/**
*
* [Name]  Re which sent FLD-MTデータファイル転送受信要求
*
* [値の説明] Re No
*
* [用途] Save Re which sent FLD-MTデータファイル転送受信要求
*
* [注意事項]
*
* [値の設定タイミング]
*
*						FLD-MTデータファイル転送受信要求 Re1->Re16
						立ち上げ 0
*/
USHORT					gw_rcw_FldMt_FilTransReqRe[CMD_MAX_SYSNUM] = { 0 };


/**

* [Name]  テーブル更新送信先
*
* [値の説明] Wait/Not wait
*
* [用途] Save status of wait SV-MT/eNB-MTデータ受信応答
*
* [注意事項]
*
* [値の設定タイミング]
*
*                   立ち上げ Not wait
*
*                   SV-MT/eNB-MTデータ受信要求受信 wait
*
*					SV-MT/eNB-MTデータ受信確認タイマタイムアウト not wait
*
*					SV-MT/eNB-MTデータ受信応答受信 not wait
*/

USHORT					gw_recw_WaitFlag_MT_DatRcv[CMD_MAX_SYSNUM] = { D_L3_OFF };


/**

* [Name]  SV-MT/eNB-MT接続応答待ちフラグ
*
* [値の説明] Wait/Not wait
*
* [用途] Save status of wait SV-MT/eNB-MT接続応答
*
* [注意事項]
*
* [値の設定タイミング]
*
*                   立ち上げ Not wait
*
*                   SV-MT/eNB-MT接続要求受信 wait
*
*					 SV-MT/eNB-MT接続確認タイマタイムアウト not wait
*
*					SV-MT/eNB-MT接続応答受信 not wait
*/
USHORT gw_rcw_SvMt_Conn_WaitFlg[CMD_MAX_SYSNUM] =  { D_L3_OFF } ;


/**

* [Name]  SV-MT/eNBデータ送信応答待ちフラグ
*
* [値の説明] Wait/Not wait
*
* [用途] Save status of wait SV-MT/eNBデータ送信応答待ちフラグ
*
* [注意事項]
*
* [値の設定タイミング]
*
*                   立ち上げ Not wait
*
*                   SV-MT/eNBデータ送信要求受信 wait
*
*					 SV-MT/eNBデータ送信確認タイマタイムアウト not wait
*
*					SV-MT/eNBデータ送信応答受信 not wait
*/
USHORT gw_rcw_SvMt_DataSnd_WaitFlg[CMD_MAX_SYSNUM] =  { D_L3_ON } ;

/**

* [Name]  FLD-MTデータファイル転送受信応答待ちフラグ
*
* [値の説明] Wait/Not wait
*
* [用途] Save status of  FLD-MTデータファイル転送受信応答待ちフラグ
*
* [注意事項]
*
* [値の設定タイミング]
*
*                   立ち上げ Not wait
*
*                   FLD-MTデータファイル転送受信要求受信 wait
*
*					FLD-MTデータファイル転送受信確認タイマタイムアウト not wait
*
*					FLD-MTデータファイル転送受信応答受信 not wait
*/
USHORT	gw_rcw_FldMt_FilTrans_WaitFlg[CMD_MAX_SYSNUM] = { D_L3_OFF };

/**
 * [Name]    外部装置データ受信中状態
 *
 * [値の説明]	[0]		未処理
				[1]		RE主導外部装置データ受信要求処理中
				[11]	FHM主導の状態報告信号処理中
				[12]	FHM主導の局リセット信号処理中
				[13]	FHM主導の配下REリセット報告信号処理中
				[14]	FHM主導の局ON/OFF報告信号処理中
				[15]	FHM主導の配下REON/OFF報告信号処理中
				[16]	FHM主導の配下ヘルスチェック応答信号処理中
				[17]	FHM主導の状態変化信号処理中

 *
 * [用途]	外部装置データ受信要求／応答処理の競合を判断し競合時はバッファリングする
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 *
 *			RECに外部装置データ受信要求送信時に該当処理状態を設定
 *			RECから外部装置データ受信応答受信時に未処理を設定
 *			RECから外部装置データ受信応答待ちタイムアウト検出時に未処理を設定
 */
USHORT		gw_rcw_ExtRcvReqSta[CMD_MAX_SYSNUM];

/**
 * @brief 外部装置データ受信応答送信先CPRIリンク番号
 * @note  [値の説明]
 * @note  [用途    ] 外部装置データ受信応答を送信する宛先のCPRIリンク番号
 * @note  [更新契機] RECに外部装置データ受信要求を送信時
 */
USHORT gw_rcw_ExtRcvRspDestLinkNum[CMD_MAX_SYSNUM];

/**
 * [Name]    外部装置データ受信要求バッファ
 *
 * [値の説明]	[0]	3Gの外部装置データ受信要求バッファ
 				[1]	LTEの外部装置データ受信要求バッファ

 *
 * [用途]	外部装置データ受信要求処理の競合に後発の外部装置データ受信要求信号をバッファリングする。
			先発の外部装置データ受信要求処理が完了時に、バッファリングされている外部装置データ受信要求処理を行う。
 *
 * [注意事項]
 *
 * [値の設定タイミング]
 *
			外部装置データ受信要求処理の競合時に後発信号をバッファリングする。
			バッファリングされている外部装置データ誦ディン要求処理が完了時にクリアする。
 */
RCT_EXT_DATA_RCV_T		gt_rcw_ExtDataRcvBuf[CMD_MAX_SYSNUM];

/**
 * @brief   該当配下REリセット状態[RE数]
 * @note    [ 値の説明 ]	
 *          - 0 リセットなし
 *          - 1 配下REリセット中
 * @note    [ 用途     ] 配下REリセット後、リセットされた配下REの起動完了を待ってからの「配下REリセット報告信号」送信とすべく、起動完了待ちを認識する情報。
 * @note    [ 注意事項 ]
 * @note    [ 更新契機 ]
 *			- TILT-INF信号の「配下REリセット要求」受信時にON
 *			- TILT-INF信号の「配下REリセット要求」による、配下REの起動完了時にOFF
 *			- TILT-INF信号の「配下REリセット要求」による、配下REの起動完了待ちタイムアウト時にOFF
 * @date    2015/10/05 TDIPS)sasaki M-RRU-ZSYS-01747 リセットされて再起動した配下REの最新RE状態を取得していない不具合を修正
 */
USHORT 					gw_rcw_reRstSta[CMD_MAX_SYSNUM][DEV_NUM_MAX] = {{CMD_OFF}};

/**
 * [Name]    TILTヘルスチェック状態
 *
 * [値の説明]	0	ヘルスチェック要求未受信
 *				1	初回ヘルスチェック要求受信済
 * [用途]	TILT-INF信号によるヘルスチェック要求受信時において、初回受信と以降受信により異なる動作を判断する。
 *
 */
USHORT					gw_rcw_tiltHcSta[CMD_MAX_SYSNUM] = {RCD_TILTHC_NONE, RCD_TILTHC_NONE};

/* REC主導に対するMTアドレス設定状態*/
USHORT	gw_rcw_Mt_AdressSetReqSts[CMD_MAX_SYSNUM] = { D_L3_OFF };

/*MT address seting info*/
CMT_CPRIF_MTADDSETREQ gwt_Mt_AdressSetInfo[CMD_MAX_SYSNUM];

/* RE version target type */
USHORT		inf_kind_val;
/* RE version slot number */
USHORT		inf_slot_no;

/**
 * @brief REカード情報報告管理テーブル
 * @note  [ 値の説明 ] RCT_CARD_INFO_DATA_T参照
 * @note  [ 用途     ] RECから(3G)REカード情報報告要求を受信時に、配下REのREカード情報報告を
 * @note               収集、合算してRECにREカード情報報告応答を送信するための管理テーブル
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] 以下の契機
 *                     - RECから(3G)REカード情報報告要求を受信時
 *                     - 配下REから(3G)REカード情報報告応答受信時
 *                     - 応答待ちタイムアウト
 */
RCT_CARD_INFO_DATA_T    rcw_CardInfoData_tbl;


/****************************************************************************/
/* CPRI 接続通知バッファ */
/****************************************************************************/
USHORT 				rcw_cpri[D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1];			/* CPRI番号REとLT共用 */
USHORT				rcw_system_type[D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1];	/* lteか3gか */
UINT				rcw_thread_qid[D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1];	/* thread qid */

USHORT				rcw_buffcnt = CMD_NUM0; 	/* CPRI接続通知バッファカウンタ */

USHORT	re_startup[D_RRH_CPRINO_NUM];	/* 立ち上げ対象RE			*/
UINT	re_startupREQ[D_RRH_CPRINO_NUM][CMD_MAX_SYSNUM];	/*立ち上げ要求RE		*/
UINT	re_startupRSP[D_RRH_CPRINO_NUM][CMD_MAX_SYSNUM];	/*立ち上げ応答RE		*/
UINT	re_StartUp_CPRI_Conn_flg;	/* 立ち上げ時CPRI接続通知判定（RE/REC)フラグ	*/
UINT	fhm_StartUp_flg = D_RRH_OFF;

/**
 * @brief REC立ち上げ判定フラグ
 * @note  [ 値の説明 ] 0=起動中, 1=起動完了(MTアドレス設定(SV-MT)受信)
 * @note  [ 用途     ] RECの起動完了を判定するのに使用する
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ] 以下の契機
 *                     - RECからMTアドレス設定(SV-MT)受信時(1=起動完了)
 *                     - RECとのCPRIリンク切断時(0=起動中)
 * @TBD_No RECとのCPRIリンク切断時の処理を追加する必要あり
 */
//TODO RECとのCPRIリンク切断時の処理を追加する必要あり
UINT rec_Startup_First_flg[CMD_MAX_SYSNUM];

/* 遅延量算出・設定済フラグ */
USHORT	delay_adjust_step3 = D_RRH_OFF;


/**
 * @brief   指定周波数帯カウンター(3G)
 * @note    [ 値の説明 ]
 *          - 0～  周波数帯毎に受信した回数
 * @note    [ 用途     ]
 *          - REカード情報報告応答のカード名の決定に用いる
 *          - 複数種類の周波数帯が指定された時は、一番多く受信した周波数帯のカード名とする
 * @note    [ 注意事項 ]
 * @note    [ 更新契機 ]
 *          - 立ち上げ時のSTEP3実行直前
 * @date    2016/02/26 TDIPS)enoki Create
 */
USHORT 		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_MAXVAL+1] = {D_RRH_OFF};


/**
 * @brief   共有メモリ(RE PORT状態報告)LOG用
 * @note    [ 用途     ]
 *          - RECに対してRE PORT状態報告応答(LTE)実施時の統計情報更新用に使用する
 * @note    [ 注意事項 ]
 *          - 統計情報以外の更新では使用しないこと!!!
 * @note    [ 更新契機 ]
 *          - RECに対してRE PORT状態報告応答(LTE)実施時
 * @date    2016/05/12 FJT)koshida Create
 */
T_RRH_REPORTSTA_S3G				*gw_rcw_shmRePortSta_logp = NULL;	/* 共有メモリ(RE PORT状態報告)LOG用 */

UINT	baseTim360;                                 /* 時刻 */

/*	REカード状態の機能部故障状態反映有無フラグ	*/
/*	装置起動およびCPRI使用中から最初のState7orStep10が完了するまではCPRI異常を故障状態に反映しない	*/
UINT	re_funcFailValid[D_RRH_CPRINO_NUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/* @} */
