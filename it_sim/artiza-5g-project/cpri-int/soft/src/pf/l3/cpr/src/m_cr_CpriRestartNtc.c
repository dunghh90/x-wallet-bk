/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_CpriRestartNtc.c
 *  @brief  CPRI再起動通知
 *  @note   RECとの間にEDWNを検出時、装置起動時と同じ起動をRCTに依頼する
 *  @date   2015/11/13 TDIPS)satou その他未実装-013 create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/*********************************************************************************/
/**
 * @addtogroup RRH_L3_CPR
 * @{
 */
/*********************************************************************************/
/* include                                                                       */
/*********************************************************************************/
#include "m_cm_header.h"
#include "m_cr_header.h"
/*********************************************************************************/
/* functions                                                                     */
/*********************************************************************************/
/**
* @brief  CPRI再起動通知
* @note   RECとの間にEDWNを検出時、装置起動時と同じ起動をRCTに依頼する
* @return None
* @date   2015/11/13 TDI)satou その他未実装-013 create
*/
VOID m_cr_CpriRestartNtc(VOID)
{
    CMT_TSKIF_HEAD *a_msg;
    UINT a_rtn;

	/* バッファ取得 */
    a_rtn = cm_BReq(CMD_BUFCA_TSKIF, sizeof *a_msg, (VOID**)&a_msg);
    if (a_rtn != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "m_cr_CpriRestartReq", "cm_BReq NG", a_rtn, sizeof *a_msg, CMD_NUM0);
	}

	/* タスク間フレームのデータ作成 */
	a_msg->uiEventNo     = CMD_TSKIF_CPRIRESTARTIND;
	a_msg->uiDstPQueueID = 0;
	a_msg->uiSrcPQueueID = 0;
	a_msg->uiDstTQueueID = CMD_TSKID_RCT;
	a_msg->uiSrcTQueueID = CMD_TSKID_CPR;
	a_msg->uiLength      = sizeof *a_msg;

	/* REプロセスへCPRI状態変化通知を送信 */
	a_rtn = cm_Enter(CMD_TSKID_RCT, CMD_QRB_NORMAL, CMD_TSKIF_CPRIRESTARTIND, a_msg);
	if(a_rtn != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG");
	}
}
/* @} */
