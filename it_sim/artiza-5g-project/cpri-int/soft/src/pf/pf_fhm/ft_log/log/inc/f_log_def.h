/*!
 * @skip		$ld:$
 * @file		f_log_def.h
 * @brief		ログスレッド デファイン定義.
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 * @date		2013/10/29 ALPHA)横山 docomo ZYNQ化 & 共通化対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010-2013
 */

/*!
 * @addtogroup RRH_PF_LOG
 * @{
 */
#ifndef F_LOG_DEF_H
#define F_LOG_DEF_H

/*!
 * @name ログスレッド状態
 * @note ログスレッドの状態定義
 * @{
 */
#define	D_LOG_THD_STS_INIT						0x0000		/*!< 初期状態							*/
#define	D_LOG_THD_STS_ACT						0x0001		/*!< 通常運用状態						*/
#define	D_LOG_THD_STS_WAITLOG					0x0002		/*!< 運用ログ出力完了待ち状態			*/
#define	D_LOG_STATENUM							0x0003		/*!< ログスレッド状態数					*/
/* @} */

/*!
 * @name ログスレッドイベント
 * @note ログスレッドのイベント定義
 * @{
 */
enum{
	E_LOG_EVENT_NUM0 = 0,
	E_API_LOG_MNR_TROUBLE_LOG_GET_REQ,
	E_API_LOG_MNT_STOP_TROUBLE_LOG_GET,
	E_API_LOG_MNR_TROUBLE_LOG_DEL,
	E_SYS_MSGID_RUNLOG_OUT_NTC,
	E_SYS_MSGID_TROUBLELOG_OUT_REQ,
	E_SYS_MSGID_CYCLIC_LOG_SHELTER_TO,
	E_LOG_EVENTNUM
};
/* @} */

/*!
 * @name タイマ値
 * @note タイマ値定義
 * @{
 */
#define	D_LOG_SHELTER_TIMER				7200000				/* ログ退避用タイマ(2時間*12回)			*/
/* @} */

/*!
 * @name APIの応答送信先管理
 * @note APIの応答送信先管理定義
 * @{
 */
enum{
	E_LOG_RSP_API_TLOGGET = 0,				/* 障害ログ取得要求	*/
	E_LOG_RSP_API_TLOGSTOP,					/* 障害ログ作成中止	*/
	E_LOG_RSP_API_TLOGDEL,					/* 障害ログ削除		*/
	E_LOG_RSP_MAX
};
/* @} */

/*!
 * @name CMD_RRHDBGコマンド数
 * @note CMD_RRHDBGコマンド数定義
 * @{
 */
#define	D_LOG_CMD_RRHDBG_CNT			7					/* CMD_RRHDBGコマンド数				*/
/* @} */


#endif
/* @} */
