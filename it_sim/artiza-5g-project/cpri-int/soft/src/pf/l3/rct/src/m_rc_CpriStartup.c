/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_CpriStartup.c
 *  @brief  CpriStartup通知
 *  @date   2015/08/07 TDIPS)maruyama create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */

#include "f_rrh_inc.h"
#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_rc_header.h"			/* RE監視タスクヘッダファイル			*/
#include "rrhApi_Svp.h"
#include "f_com_aplCom.h"
#include "m_cr_header.h"  			/* CPRI管理 task header file */

/*********************************************************************************/
/**
 *  @brief  CpriStartup通知受信
 *  @return	none
 *  @date   2015/08/07 TDIPS)maruyama create
 *  @date   2015/08/24 TDIPS)maruyama modify 送信MSGのバッファ確保、CMD_TSKIF_CPR_STA_NTC MSG送信削除
 */
/*********************************************************************************/
VOID m_rc_CpriStartup( VOID )
{
	CMT_TSKIF_CPRILNKPRE_REQ	*a_msg = NULL;
	UINT		a_rtn				= CMD_RES_OK;

	if(( rcw_first_act_flg != CMD_ON) || ( rcw_first_prc_act_flg != CMD_ON ))	/* ACTかつ全タスク初期化完了の場合 */
	{
		cm_Assert( CMD_ASL_USELOW, (((rcw_first_act_flg << 16 ) & 0xFFFF0000 ) | ( rcw_first_prc_act_flg & 0x0000FFFF )), "Now standby status... " );
		return;
	}

	/* バッファ取得 */
	a_rtn = cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_CPRILNKPRE_REQ), (VOID **)&a_msg );
	/* 戻り値判定 */
	if( a_rtn != CMD_RES_OK ){
		/* バッファ取得失敗時はアボート処理 */
		cm_MAbort( CMD_ALMCD_BUFGET, "m_rc_CpriStartup",
						   "cm_BReq NG",
						   a_rtn, sizeof( CMT_TSKIF_CPRILNKPRE_REQ ), CMD_NUM0 );
		return;
	}
	
	/* タスク間フレームのデータ作成 */
	a_msg->head.uiEventNo 		= CMD_TSKIF_REC_CPRI_PRE_REQ;
	a_msg->head.uiDstPQueueID	= D_RRH_PROCQUE_L3;
	a_msg->head.uiSrcPQueueID	= D_RRH_PROCQUE_L3;
	a_msg->head.uiDstTQueueID	= CMD_TSKID_CPR;
	a_msg->head.uiSrcTQueueID	= CMD_TSKID_RCT;
	a_msg->head.uiLength 		= sizeof( CMT_TSKIF_CPRILNKPRE_REQ );

	/* Send Log */
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, CMD_TSKID_RCT, CMD_TSKIF_REC_CPRI_PRE_REQ, (UCHAR*)a_msg);

	/* CPRIリンク開始要求を送信 */
	a_rtn = cm_Enter( CMD_TSKID_CPR, CMD_QRB_NORMAL,
			CMD_TSKIF_REC_CPRI_PRE_REQ, a_msg );
	if(a_rtn != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
	}
}
/* @} */
