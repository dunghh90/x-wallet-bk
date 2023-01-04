/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_CpriConnectNtc.c
 *  @brief  REC CPRI Connection send function
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
* @brief  CPRIリンク接続通知
* @note   L3_RCTへCPRIリンク接続通知を送信する
* @param  linkno      [in] CPRIリンク番号
* @param  system_type [in] system type
* @return None
* @date   2015/07/22 TDIPS)ikeda create
*/
VOID m_cr_CpriConnectNtc(USHORT linkno, USHORT system_type)
{
	CMT_TSKIF_CPRICONN_NTC*	a_msg;		/* CPRIリンク接続通知	*/
	UINT		a_rtn				= CMD_RES_OK;

	/* バッファ取得 */
	a_rtn = cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_CPRICONN_NTC ), (VOID **)&a_msg );
	/* 戻り値判定 */
	if( a_rtn != CMD_RES_OK ){
		/* バッファ取得失敗時はアボート処理 */
		cm_MAbort( CMD_ALMCD_BUFGET, "m_cr_CpriConnectNtc",
						   "cm_BReq NG",
						   a_rtn, sizeof( CMT_TSKIF_CPRICONN_NTC ), CMD_NUM0 );
	}

	/* タスク間フレームのデータ作成 */
	a_msg->head.uiEventNo = CMD_TSKIF_CPRI_CONNECT_NTC;
	a_msg->head.uiDstPQueueID = D_RRH_PROCQUE_L3;
	a_msg->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
	a_msg->head.uiDstTQueueID = CMD_TSKID_RCT;
	a_msg->head.uiSrcTQueueID = CMD_TSKID_CPR;
	a_msg->head.uiLength = sizeof( CMT_TSKIF_CPRICONN_NTC );
	a_msg->link_num = linkno;
	a_msg->system_type = system_type;

	/* Send Log */
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, CMD_TSKID_RCT, CMD_TSKIF_CPRI_CONNECT_NTC, (UCHAR*)a_msg);

	/* RE制御タスクへCPRIリンク接続通知を送信 */
	a_rtn = cm_Enter( CMD_TSKID_RCT, CMD_QRB_NORMAL,
			CMD_TSKIF_CPRI_CONNECT_NTC, a_msg );
	if(a_rtn != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
	}
}

/* @} */

/* @} */
