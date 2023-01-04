/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_SendMsg_reCpriLinkSat.c
 *  @brief  RE CPRI Link Start Noticeをcprに通知
 *  @date   2015/08/27 TDIPS)maruyama create(立ち上げ対象RE特定(m_rc_getStartUpRE.c)から分割）
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"
#include "m_rc_header.h"
#include "f_rrh_def.h"



/********************************************************************************************************************/
/**
 *  @brief	RE CPRI Link Start Noticeをcprに通知
 *  @note 	立ち上げRE分通知を行う
 *  @return	None
 *  @date	2015/08/07	TDIPS) maruyama create
 *  @date	2015/08/24	TDIPS) maruyama modify 送信MSGのバッファ確保、link_numを１からに変更
 *  @date	2015/08/26	TDIPS) maruyama modify 送信MSGのバッファの初期化コーディングミス修正、バッファ確保をループ内の立ち上げRE送信時のみ
 *  @date	2015/08/26	TDIPS) maruyama modify 関数の呼び元（m_rc_getStartUpRE()）で立ち上げ判定しているため判定処理しない
 *  @date   2015/08/27 TDIPS)maruyama create(立ち上げ対象RE特定(m_rc_getStartUpRE.c)から分割）
 *  @date   2015/11/17 TDI)satou ハソ-QA-047
 */
/********************************************************************************************************************/
VOID m_rc_SendMsg_reCpriLinkSat(UINT re_cnt, E_RRH_SFPLOG_TRIGGER trigger)
{
	UINT							a_rtn	= 0;		/* 関数戻り値		*/
	CMT_TSKIF_RE_CPRILNKSTART_NTC*	a_msg	= NULL;		/* 送信MSG			*/

	/* RE CPRI Link Start Noticeをcprに通知 	*/
	/* バッファ取得 */
	a_rtn = cm_BReq( CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_RE_CPRILNKSTART_NTC), (VOID **)&a_msg );
	/* 戻り値判定 */
	if( a_rtn != CMD_RES_OK ){
		/* バッファ取得失敗時はアボート処理 */
		cm_MAbort( CMD_ALMCD_BUFGET, "m_rc_SendMsg_reCpriLinkSat",
						   "cm_BReq NG",
						   a_rtn, sizeof(CMT_TSKIF_RE_CPRILNKSTART_NTC), CMD_NUM0 );
		return;
	}
	
	/* タスク間フレームのデータ作成 */
	a_msg->head.uiEventNo 		= CMD_TSKIF_RE_CPRI_START_NTC;		/* イベント番号			*/
	a_msg->head.uiDstPQueueID	= D_RRH_PROCQUE_L3;					/* 送信先プロセスQID	*/
	a_msg->head.uiSrcPQueueID	= D_RRH_PROCQUE_L3;					/* 送信元プロセスQID	*/
	a_msg->head.uiDstTQueueID	= CMD_TSKID_CPR;					/* 送信先スレッドQID	*/
	a_msg->head.uiSrcTQueueID	= CMD_TSKID_RCT;					/* 送信元スレッドQID	*/
	a_msg->head.uiLength 		= sizeof(CMT_TSKIF_RE_CPRILNKSTART_NTC);	/* 送信MSGサイズ	*/
	a_msg->link_num 			= re_cnt;							/* CPRI番号				*/
	a_msg->sfpLogTrigger        = (UINT)trigger;                    /* SFPログ履歴取得契機  */

	/* Send Log */
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, CMD_TSKID_RCT, CMD_TSKIF_RE_CPRI_START_NTC, (UCHAR*)a_msg);

	/* RE CPRI Link Start Noticeを送信 */
	/* cm_Enter(tskNo, tskSts, evtNo, dat_p);	*/
	/* tskNo		:送信先						*/
	/* tskSts		:タスクsts（起動事象）		*/
	/* evtNo		:イベント番号				*/
	/* dat_p		：送信MSG					*/
	a_rtn = cm_Enter( CMD_TSKID_CPR, CMD_QRB_NORMAL,
			CMD_TSKIF_RE_CPRI_START_NTC, a_msg );
	if( a_rtn != CMD_RES_OK )
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
	}
}


/* @} */
