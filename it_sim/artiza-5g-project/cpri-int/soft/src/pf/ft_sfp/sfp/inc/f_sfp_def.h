/*!
 * @skip		$ld:$
 * @file		f_sfp_def.h
 * @brief		SFPスレッド デファイン定義.
 * @date		2018/08/20 Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_SFP
 * @{
 */
#ifndef F_SFP_DEF_H
#define F_SFP_DEF_H


/* @} */

/*!
 * @name SFPスレッド状態
 * @note SFPスレッドの状態定義
 * @{
 */
#define	D_SFP_THD_STS_INIT						0x0000				/*!< 初期状態							*/
#define	D_SFP_THD_STS_ACT						0x0001				/*!< 通常運用状態						*/
#define	D_SFP_THD_STS_DBG						0x0002				/*!< デバッグ状態						*/
#define	D_SFP_STATENUM							0x0003				/*!< 診断スレッド状態数					*/
/* @} */

#define D_SFP_POLL_COUNT						11					/*!< Polling周期:11回(1.1Sec)			*/
#define D_SFP_TIMEVAL_POLL						100					/*!< Polling周期:100msec				*/
#define D_SFP_TIMEVAL_SFPGUARD					300					/*!< SFPガード時間:300msec				*/
#define D_SFP_TIMEVAL_FAILSAFE					1000				/*!< Fail Safe周期:1000msec				*/
#define D_SFP_COUNT_SFPMOUNT					2					/*!< SFP mount回数						*/
#define D_SFP_MAXNUM							D_RRH_SFP_MAXNUM	/*!< SFP接続数							*/

#define D_SFP_MONSTATEL_UNMOUNT					0					/*!< SFP Monitor状態：Unmount			*/
#define D_SFP_MON_STATE_MOUNT					1					/*!< SFP Monitor状態：Mount				*/
#define D_SFP_MON_STATE_MOUNT_CHG				2					/*!< SFP Monitor状態：Unmount→Mount	*/

#define D_SFP_MONITOR_TIMING					20					/*!< SFP Monitorタイミング (20s)		*/
#define	D_SFP_FAILSAFE3_MAXCNT					60					/*!< SFP Fail Safe 条件③監視回数 (30s)	*/
#define	D_SFP_FAULTNTF_STOP_MAXCNT				300					/*!< SFP Fail通知抑止回数 (300s=5m)		*/
#define	D_SFP_FAILSAFE_RECOVER_MAXCNT			60					
/* @} */

#endif
/* @} */
