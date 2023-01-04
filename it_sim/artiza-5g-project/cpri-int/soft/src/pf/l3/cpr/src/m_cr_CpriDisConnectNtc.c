/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_CpriDisConnectNtc.c
 *  @brief  REC CPRI Disconnection send function
 *  @date   2015/08/03 TDIPS)ikeda create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"
#include "m_cr_header.h"
#include "rrhApi_L2.h"

/**
* @brief  CPRIリンク切断通知
* @note   L3_RCTにCPRIリンク切断通知を送信する.
* @param  linkno [in] CPRIリンク番号
* @param  system_type [in] 通信種別
* @param  state [in] 状態
* @return None
* @date   2015/07/22 TDIPS)ikeda create
* @date   2016/02/05 TDI)satou l3/logにも通知するように. 障害ログ取得中にCPRIリンク断->復旧すると、RECからログが再度すぐに取得できない問題の対処
*/
VOID m_cr_CpriDisConnectNtc(USHORT linkno, USHORT system_type, USHORT state)
{
	CMT_TSKIF_CPRIDISCONN_NTC*	a_msg;		/* CPRIリンク切断通知	*/
	UINT		a_rtn				= CMD_RES_OK;
	UINT a_dstTQueueID[2] = { CMD_TSKID_RCT, CMD_TSKID_LOG };
	INT a_queue_idx;

	for (a_queue_idx = 0; a_queue_idx < 2; a_queue_idx++)
	{
		/* バッファ取得 */
		a_rtn = cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_CPRIDISCONN_NTC ), (VOID **)&a_msg );
		/* 戻り値判定 */
		if( a_rtn != CMD_RES_OK ){
			/* バッファ取得失敗時はアボート処理 */
			cm_MAbort( CMD_ALMCD_BUFGET, "m_cr_CpriDisConnectNtc",
							   "cm_BReq NG",
							   a_rtn, sizeof( CMT_TSKIF_CPRIDISCONN_NTC ), CMD_NUM0 );
		}

		/* タスク間フレームのデータ作成 */
		a_msg->head.uiEventNo = CMD_TSKIF_CPRI_DISCONNECT_NTC;
		a_msg->head.uiDstPQueueID = D_RRH_PROCQUE_L3;
		a_msg->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
		a_msg->head.uiDstTQueueID = a_dstTQueueID[a_queue_idx];
		a_msg->head.uiSrcTQueueID = CMD_TSKID_CPR;
		a_msg->head.uiLength = sizeof( CMT_TSKIF_CPRIDISCONN_NTC );
		a_msg->link_num = linkno;
		a_msg->system_type = system_type;
		a_msg->layer_kind = state;

		/* Send Log */
		m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, a_dstTQueueID[a_queue_idx], CMD_TSKIF_CPRI_DISCONNECT_NTC, (UCHAR*)a_msg);

		/* RE制御タスクへCPRIリンク切断通知を送信 */
		a_rtn = cm_Enter( a_dstTQueueID[a_queue_idx], CMD_QRB_NORMAL,
				CMD_TSKIF_CPRI_DISCONNECT_NTC, a_msg );
		if(a_rtn != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
		}
	}
}

/* @} */
