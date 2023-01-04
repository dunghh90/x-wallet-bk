/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   m_cr_L3StaNtc.c
 *  @brief  CPRI link state notice function
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "f_rrh_def.h"
#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  /* CPRI管理 task header file */

/* @{ */
/**
* @brief CPRI link state notice function
* @note edit CPRI link status notice and send to MKM task.\n
* @param linkno [in] CPRIリンク番号
* @param system [in] 通信種別
* @param onOff [in] 状態ON/OFF
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
*
*/

VOID m_cr_cm_L3LnkStaNtc(USHORT linkno, USHORT system,USHORT onOff)
 {
 	CMT_TSKIF_L3STATENTC*	a_l3stae_p;	/* notification buffer pointer */
	INT						buf_ret;

	/* 戻り値判定 */
	if( cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_L3STATENTC ), (VOID**)&a_l3stae_p ) != CMD_RES_OK )
	{
			/* バッファ取得失敗時はアボート処理 */
		cm_MAbort( CMD_ALMCD_BUFGET, "m_cr_cm_L3LnkStaNtc", 
							   "cm_BReq NG", 
							   CMD_NG, sizeof( CMT_TSKIF_L3STATENTC ), CMD_NUM0 );
	}
	/* set L3 state to MNT process*/
	a_l3stae_p->head.uiEventNo = CMD_TSKIF_L3STANTC;
	a_l3stae_p->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
	a_l3stae_p->head.uiSrcTQueueID = CMD_TSKID_CPR;
	a_l3stae_p->head.uiDstPQueueID = D_RRH_PROCQUE_MT;
	a_l3stae_p->head.uiDstTQueueID = 0;
	a_l3stae_p->head.uiLength = sizeof(CMT_TSKIF_L3STATENTC);
	a_l3stae_p->link_num = linkno;
	a_l3stae_p->system_type	= system;
	a_l3stae_p->onOff = onOff;	            /**< onOff*/

	/* Send Log */
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, D_RRH_PROCQUE_MT, CMD_TSKIF_L3STANTC, (UCHAR*)a_l3stae_p);

	/* send L3リンク state通知 to MNT */
	buf_ret = cm_Enter(D_RRH_PROCQUE_MT,CMD_QRB_NORMAL,CMD_TSKIF_L3STANTC,a_l3stae_p);
	if(buf_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
	}

	return;
 }
/* @} */

/* @} */ 
