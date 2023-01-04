/*!
 * @skip 		$Id:$
 * @file 		f_pfm_def.h
 * @brief		pf_pfm thread define header
 * @date		2018/11/23 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_DHC

 *  @{
 */
#ifndef F_PFM_DEF_H
#define F_PFM_DEF_H

/****************************************************************************************************************/
/*  Thread status definition																					*/
/****************************************************************************************************************/
/*!
 * @name pf_pfm state
 * @note pf_pfm state definition
 * @{
 */
enum {
	E_PFM_STATE_INIT = 0,												/* 初期状態				*/
	E_PFM_STATE_ACT,													/* 運用状態				*/
	E_PFM_STATE_MAX														/* 状態数				*/
};
/* @} */

/****************************************************************************************************************/
/*  Thread internal event code																					*/
/****************************************************************************************************************/
/*!
 * @name pf_pfm event
 * @note pf_pfm event definition
 * @{
 */
enum {
	E_PFM_EVT_INIT = 0,													/* 初期化要求			*/
	E_PFM_EVT_MSI_RCV_IND,												/* MSI受信通知			*/
	E_PFM_EVT_1S_TIMEOUT_NTF,											/* タイムアウト通知		*/
	E_PFM_EVT_LOG_TIMEOUT_NTF,											/* タイムアウト通知		*/
	E_PFM_EVT_MAX														/* イベント数			*/
};
/* @} */

/****************************************************************************************************************/
/*  Thread internal event code																					*/
/****************************************************************************************************************/
/*!
 * @name pf_pfm event
 * @note pf_pfm event definition
 * @{
 */
enum {
	E_PFM_CORRECT_STATE_STOP = 0,										/* 停止中状態			*/
	E_PFM_CORRECT_STATE_ACTIVE_INIT,									/* 測定中状態(初期)		*/
	E_PFM_CORRECT_STATE_ACTIVE,											/* 測定中状態			*/
	E_PFM_CORRECT_STATE_WAIT_NOTIFICATION,								/* 通知待ち状態			*/
	E_PFM_CORRECT_STATE_MAX												/* 統計情報収集状態数	*/
};
/* @} */

/****************************************************************************************************************/
/*  Error Description																							*/
/****************************************************************************************************************/
/*!
 * @name Error Description
 * @note Error Description
 * @{
 */
#define	D_PFM_MSIERR_MSG_MEASURE_INT_ERR	"measure-int chg during monitor\0"
#define	D_PFM_MSIERR_MSG_MEASURE_INT_ZERO	"measurement-interval set zero \0"
/* @} */

/****************************************************************************************************************/
/*  measurement-object index値																					*/
/****************************************************************************************************************/
/*!
 * @name measurement-object index
 * @note measurement-object index
 * @{
 */
enum {
	E_PFM_OBJIDX_RX_ON_TIME = 0,
	E_PFM_OBJIDX_RX_EARLY,
	E_PFM_OBJIDX_RX_LATE,
	E_PFM_OBJIDX_RX_CORRUPT,
	E_PFM_OBJIDX_RX_DUPL,
	E_PFM_OBJIDX_RX_TOTAL,
	E_PFM_OBJIDX_MAX,
	E_PFM_OBJIDX_NOTHING = 0xFFFFFFFF
};

/****************************************************************************************************************/
/*  Performance Log Get Fact値																					*/
/****************************************************************************************************************/
/*!
 * @name Performance Log Get Fact
 * @note Performance Log Get Fact
 * @{
 */
enum {
	E_PFM_PFMLOG_FACT_MSI = 0,
	E_PFM_PFMLOG_FACT_TIMER,
	E_PFM_PFMLOG_FACT_MAX
};

/****************************************************************************************************************/
/*  measurement-object Activate値																				*/
/****************************************************************************************************************/
/*!
 * @name measurement-object Activate
 * @note measurement-object Activate
 * @{
 */
enum {
	E_PFM_OBJSTATE_DEACTIVE = 0,
	E_PFM_OBJSTATE_ACTIVE,
	E_PFM_OBJSTATE_CHG_DEACTIVE,
	E_PFM_OBJSTATE_CHG_ACTIVE
};


#define	D_PFM_LOG_TIME_VAL	60000			/* 60s */

#endif
/** @} */
