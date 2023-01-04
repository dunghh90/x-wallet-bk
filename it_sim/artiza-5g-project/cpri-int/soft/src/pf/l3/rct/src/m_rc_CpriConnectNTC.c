/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_CpriConnectNTC.c
 *  @brief  CpriConnect通知
 *  @date   2015/08/07 TDIPS)maruyama create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_rc_header.h"
#include "f_com_aplCom.h"

/*********************************************************************************/
/**
 *  @brief  CpriConnect通知受信
 *  @param  buff_adr [in] 受信メッセージへのポインタ(CMT_TSKIF_CPRICONN_NTC)
 *  @Bug_No M-RRU-ZSYS-01778
 *  @date   2015/08/07 TDIPS)maruyama create
 *  @date   2015/08/26 TDIPS)maruyama modify RE recへの送信MSGをポインタに変更し、バッファ確保
 *  @date   2015/10/15 TDI)satou M-RRU-ZSYS-01778 問処番号No.83対応
 */
/*********************************************************************************/
VOID m_rc_CpriConnectNTC(VOID* buff_adr)
{
	UINT	cprLinkNo				= 0;			/* CPRI Link番号		*/
	UINT	system_type				= 0;			/* system type			*/
	UINT	ret						= 0;			/* 関数戻り値			*/
	CMT_TSKIF_RESTR_REQ				a_msg;			/* RE recへの送信MSG	*/
	CMT_TSKIF_CPRICONN_NTC*	param_p	= NULL;			/* Cpri接続通知(受信MSG)*/

	param_p = (CMT_TSKIF_CPRICONN_NTC*) buff_adr;
	/* Link番号取得				*/
	cprLinkNo = param_p->link_num;
	/* system_type種別取得		*/
	system_type = param_p->system_type;

	/* 初期化 */
	memset(&a_msg,         0, sizeof a_msg        );
	/* REのrecに(0x4005)RE Startup Request送信	*/
    a_msg.head.uiEventNo        = CMD_TSKIF_RE_STARTUP_REQ;         /* Event  ID         */
    a_msg.head.uiSignalkind     = 0;                                /* Signal Kind       */
    a_msg.head.uiDstPQueueID    = D_RRH_PROCQUE_RE;                 /* Destination P QID */
    a_msg.head.uiDstTQueueID    = 0;                                /* Destination T QID */
    a_msg.head.uiSrcPQueueID    = D_RRH_PROCQUE_L3;                 /* Source P QID      */
    a_msg.head.uiDstTQueueID    = 0;                                /* Destination T QID */
    a_msg.head.uiLength         = sizeof(CMT_TSKIF_RESTR_REQ);      /* Length            */
    a_msg.link_num              = cprLinkNo;                        /* Cpri Link番号     */
    a_msg.system_type           = system_type;                      /* system_type       */

	/* apl_com_msgQSend(UINT msgQid,UINT msgid,void *msgp,UINT msgsz )	*/
	/* msgQid	:送信先QID												*/
	/* msgid	:イベント番号											*/
	/* msgp		:送信MSG												*/
	/* msgsz	:MSGサイズ												*/
	ret = apl_com_msgQSend(
			D_RRH_PROCQUE_RE,					/* Thread queue ID			*/
			CMD_TSKIF_RE_STARTUP_REQ,			/* Event ID					*/
			(VOID *)&a_msg,						/* The message to transmit	*/
			sizeof(CMT_TSKIF_RESTR_REQ)	 		/* The size of message		*/
		);
	/* checking parameter */
	if( ret != D_RRH_OK ) {
		cm_Assert(CMD_ASL_DBGLOW, ret,
		    (CHAR*)"[m_rc_CpriConnectNTC] Send RE Startup Request  NG" );
	}
	return;
}

/* @} */
