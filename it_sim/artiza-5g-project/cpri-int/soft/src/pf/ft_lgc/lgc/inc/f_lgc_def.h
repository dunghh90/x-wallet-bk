/*!
 * @skip		$ld:$
 * @file		f_lgc_def.h
 * @brief		ログスレッド デファイン定義.
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 * @date		2013/10/29 ALPHA)横山 docomo ZYNQ化 & 共通化対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010-2013
 */

/*!
 * @addtogroup RRH_PF_LGC
 * @{
 */
#ifndef F_LGC_DEF_H
#define F_LGC_DEF_H

/*!
 * @name ログスレッド状態
 * @note ログスレッドの状態定義
 * @{
 */
#define	D_LGC_THD_STS_INIT						0x0000		/*!< 初期状態							*/
#define	D_LGC_THD_STS_ACT						0x0001		/*!< 通常運用状態						*/
#define	D_LGC_STATENUM							0x0002		/*!< ログスレッド状態数					*/
/* @} */

/*!
 * @name ログスレッドイベント
 * @note ログスレッドのイベント定義
 * @{
 */
enum{
	E_LGC_EVENT_NUM0 = 0,
	E_SYS_MSGID_TROUBLELOG_OUT_REQ,
	E_SYS_MSGID_CYCLIC_LOG_SHELTER_TO,
	E_LGC_EVENT_MSI_REQ,
	E_LGC_EVENTNUM
};
/* @} */

/*!
 * @name タイマ値
 * @note タイマ値定義
 * @{
 */
#define	D_LGC_SHELTER_TIMER				3600000				/* ログ退避用タイマ(1時間*24回)	60*60*1000  MINTIME = 10msec		*/
/* @} */

/*!
 * @name APIの応答送信先管理
 * @note APIの応答送信先管理定義
 * @{
 */
enum{
	E_LGC_RSP_API_TLOGGET = 0,				/* 障害ログ取得要求	*/
	E_LGC_RSP_API_TLOGSTOP,					/* 障害ログ作成中止	*/
	E_LGC_RSP_API_TLOGDEL,					/* 障害ログ削除		*/
	E_LGC_RSP_MAX
};
/* @} */

/*!
 * @name CMD_RRHDBGコマンド数
 * @note CMD_RRHDBGコマンド数定義
 * @{
 */
#define	D_LGC_CMD_RRHDBG_CNT			7					/* CMD_RRHDBGコマンド数				*/
/* @} */

#define D_LGC_CMD_ON					1
#define D_LGC_CMD_OFF					0

/*!
 * @name TROUBLESHOOTINGLOG定義
 * @note TROUBLESHOOTINGLOG定義
 * @{
 */
#define D_LGC_TROUBLESHOOTINGLOG_NAME		"/O-RAN/log/FJ_troubleshootinglog.gz"
#define D_LGC_TROUBLESHOOTINGLOG_MAXSIZE	0x600000
#define D_LGC_LOGSTATE_STOP	0
#define D_LGC_LOGSTATE_RUNNING	1
/* @} */

#endif
/* @} */
