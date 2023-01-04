/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_CpriLinkPrepareRes.c
 *  @brief  REC CPRI Link Preparation response function
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
* @brief  REC CPRIリンク開始応答
* @return None
* @date   2015/07/22 TDIPS)ikeda create
*/

VOID m_cr_CpriLinkPrepareRes()
{
	CMT_TSKIF_CPRILNKPRE_RES*	a_msg;		/* REC CPRIリンク開始応答	*/
    UINT a_rtn = CMD_RES_OK;

	/* バッファ取得 */
	a_rtn = cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_CPRILNKPRE_RES ), (VOID **)&a_msg );
	/* 戻り値判定 */
	if( a_rtn != CMD_RES_OK ){
		/* バッファ取得失敗時はアボート処理 */
		cm_MAbort( CMD_ALMCD_BUFGET, "m_cr_CpriLinkPrepareRes",
						   "cm_BReq NG",
						   a_rtn, sizeof( CMT_TSKIF_CPRILNKPRE_RES ), CMD_NUM0 );
	}

	/* タスク間フレームのデータ作成 */
	a_msg->head.uiEventNo = CMD_TSKIF_REC_CPRI_PRE_RSP;
	a_msg->head.uiDstPQueueID = D_RRH_PROCQUE_L3;
	a_msg->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
	a_msg->head.uiDstTQueueID = CMD_TSKID_RCT;
	a_msg->head.uiSrcTQueueID = CMD_TSKID_CPR;
	a_msg->head.uiLength = sizeof( CMT_TSKIF_CPRILNKPRE_RES );

	/* Send Log */
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, CMD_TSKID_RCT, CMD_TSKIF_REC_CPRI_PRE_RSP, (UCHAR*)a_msg);

	/* RE制御タスクへREC CPRIリンク開始応答を送信 */
	a_rtn = cm_Enter( CMD_TSKID_RCT, CMD_QRB_NORMAL,
			CMD_TSKIF_REC_CPRI_PRE_RSP, a_msg );
	if(a_rtn != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
	}
}
/* @} */
