/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsndcmprsp_waiting_syspsndcmp_cnf_to.c
 * @brief  Event:ファイルデータ送信完了報告応答, 対RE状態:ファイルデータ送信完了報告応答(シスパラ)待ち
 * @date   2015/08/22 FPT)Yen Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */

/*!
 *  @brief  Event:ファイルデータ送信完了報告応答, 対RE状態:ファイルデータ送信完了報告応答(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_rec_fsndcmprsp_waiting_syspsndcmp_cnf_to(VOID* bufp, USHORT cpri_no)
{
	UINT							lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_fsndcmprsp_waiting_syspsndcmp_cnf_to] ENTER" );

	lwSysType = (( CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;

	/* 対RE状態 => ファイルデータ送信(シスパラ)待ち */
	gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_FILE_TRANS_WAIT;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_fsndcmprsp_waiting_syspsndcmp_cnf_to] RETURN" );

	return ;
}

/* @} */

