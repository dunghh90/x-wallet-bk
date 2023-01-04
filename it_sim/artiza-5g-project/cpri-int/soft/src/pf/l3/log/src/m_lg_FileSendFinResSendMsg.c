/*!
 * @skip   $Id$
 * @file   m_lg_FileSendFinResSendMsg.c
 * @brief  The function of sending respone message from l3/log to REC for ファイルデータ送信完了通知応答.
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
 *  @brief  The function of sending request message from l3/log to REC for ファイルデータ送信完了通知応答
 *  @note
 *  @param  sigsys        [in] the system signal type
 *  @param  link_num      [in] CPRI 番号
 *  @param  result        [in] Response Result
 *  @return RETURN CODE
 *  @date	2015/08/03 FPT)Tunghv create
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 */
/********************************************************************************************************************/

INT m_lg_FileSendFinResSendMsg( USHORT sigsys, USHORT link_num, USHORT result )
{
	CMT_TSKIF_CPRIRCV_FILSNDFINRES res_p;	/* タスク間フレームポインタ		*/

    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_StopTroublelogL3SendMsg] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s \n", __LINE__, __FUNCTION__);
#endif
	/* Init buffer */
	memset( &res_p, 0, sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINRES) );

	/********************/
	/* CPRI番号設定 */
	/********************/
	res_p.cprircv_inf.link_num = link_num;

	/*EventNo for sending to re/rec*/
	res_p.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;

	/* Set Signale Kind */
	res_p.head.uiSignalkind= CMD_CPRID_FILSNDFINRES + sigsys;

	/********************/
	/* CPRI信号種別設定 */
	/********************/
	res_p.cpridat_filsndfinres.signal_kind = CMD_CPRID_FILSNDFINRES+ sigsys;

	/* Set Result */
	res_p.cpridat_filsndfinres.result = result;

	res_p.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)
									+	sizeof(CMT_CPRIF_FILSNDFINRES);

	/* RE障害ログ取得中止要求をre/recに送信する*/
	return l3_com_sendMsg(CMD_TSKID_LOG, D_RRH_PROCQUE_RE, 0, (VOID *)&res_p, sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINRES));
}

/* @} */

