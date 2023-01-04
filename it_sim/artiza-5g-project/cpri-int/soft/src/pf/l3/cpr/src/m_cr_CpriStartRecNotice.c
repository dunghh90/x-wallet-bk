/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_CpriStartRecNotice.c
 *  @brief  REC 起動開始通知 handler function
 *  @date   2015/07/22 TDIPS)ikeda create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"
#include "m_cr_header.h"
#include "rrhApi_L2.h"
/* @{ */
/**
* @brief 
* @note REC 起動開始通知イベント \n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2015/07/22 TDIPS)ikeda create
*
*/

VOID m_cr_CpriStartRecNotice( UINT * buff_adr )
{
	/* edit event 「CPRI信号送信通知」(データリンク設定要求) and send to LAPB task  */
	m_cr_DlinkSettingReq(D_RRH_CPRINO_REC);

	/* set task state to 4: running */
	crw_taskstate = CRD_TASKSTA_RUN;
}

/* @} */

/* @} */
