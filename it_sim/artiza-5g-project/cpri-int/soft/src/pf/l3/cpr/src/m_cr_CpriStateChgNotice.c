/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_CpriStateChgNotice.c
 *  @brief  RE CPRI Status change send function
 *  @date   2015/09/29 TDIPS)ikeda create
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
* @brief  CPRI状態変化通知
* @note   RE_RECへCPRI状態変化通知を送信する
* @param  linkno [in] CPRIリンク番号
* @param  cprista [in] cpri状態
* @return None
* @date   2015/09/29 TDIPS)ikeda create
*/
VOID m_cr_CpriStateChgNotice(USHORT linkno, USHORT cprista)
{
	CMT_TSKIF_CPRISTATECHG_NTC*	a_msg;		/* CPRI状態変化通知	*/
	UINT		a_rtn				= CMD_RES_OK;

	/* バッファ取得 */
	a_rtn = cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_CPRISTATECHG_NTC ), (VOID **)&a_msg );
	/* 戻り値判定 */
	if( a_rtn != CMD_RES_OK ){
		/* バッファ取得失敗時はアボート処理 */
		cm_MAbort( CMD_ALMCD_BUFGET, "m_cr_CpriStateChgNotice",
						   "cm_BReq NG",
						   a_rtn, sizeof( CMT_TSKIF_CPRISTATECHG_NTC ), CMD_NUM0 );
	}

	/* タスク間フレームのデータ作成 */
	a_msg->head.uiEventNo = CMD_TSKIF_CPRSTACHGNTC;
	a_msg->head.uiDstPQueueID = D_RRH_PROCQUE_RE;
	a_msg->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
	a_msg->head.uiDstTQueueID = 0;
	a_msg->head.uiSrcTQueueID = CMD_TSKID_CPR;
	a_msg->head.uiLength = sizeof( CMT_TSKIF_CPRISTATECHG_NTC );
	a_msg->link_num = linkno;
	a_msg->cprista = cprista;

	/* REプロセスへCPRI状態変化通知を送信 */
	a_rtn = cm_Enter( D_RRH_PROCQUE_RE, CMD_QRB_NORMAL,
			CMD_TSKIF_CPRSTACHGNTC, a_msg );
	if(a_rtn != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
	}
}

/* @} */

/* @} */
