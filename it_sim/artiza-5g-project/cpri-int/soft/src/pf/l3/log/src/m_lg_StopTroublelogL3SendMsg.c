/*!
 * @skip   $Id$
 * @file   m_lg_StopTroublelogL3SendMsg.c
 * @brief  The function of sending respone message from l3/log to REC for RE障害ログ取得中止要求.
 * @date   2015/07/31 FPT)Tunghv Create.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup TRA_LOG
 * @{
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/********************************************************************************************************************/
/**
 *  @brief  The function of sending request message from l3/log to REC for RE障害ログ取得中止要求処理
 *  @note
 *  @param  sigsys        [in] the system signal type
 *  @param  link_num      [in] CPRI 番号
 *  @return RETURN CODE
 *  @date	2015/08/03 FPT)Tunghv create
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 */
/********************************************************************************************************************/

INT m_lg_StopTroublelogL3SendMsg( USHORT sigsys, USHORT link_num )
{
	CMT_TSKIF_CPRISND_REOBLOGSTPREQ req_p;	/* タスク間フレームポインタ		*/

	/* Init buffer */
	memset( &req_p, 0, sizeof(CMT_TSKIF_CPRISND_REOBLOGSTPREQ) );

	/********************/
	/* CPRI番号設定 */
	/********************/
	req_p.cprisnd_inf.link_num = link_num;

	/*EventNo for sending to re/rec*/
	req_p.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;

	/* Set Signale Kind */
	req_p.head.uiSignalkind= CMD_CPRID_REOBLOGSTPREQ + sigsys;

	/********************/
	/* CPRI信号種別設定 */
	/********************/
	req_p.cpridat_reoblogstpreq.signal_kind = CMD_CPRID_REOBLOGSTPREQ + sigsys;

	req_p.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)
									+	sizeof(CMT_CPRIF_REOBLOGSTPREQ);

	/* RE障害ログ取得中止要求をre/recに送信する*/
	return l3_com_sendMsg(CMD_TSKID_LOG, D_RRH_PROCQUE_RE, 0, (VOID *)&req_p, sizeof(CMT_TSKIF_CPRISND_REOBLOGSTPREQ));
}

/* @} */

