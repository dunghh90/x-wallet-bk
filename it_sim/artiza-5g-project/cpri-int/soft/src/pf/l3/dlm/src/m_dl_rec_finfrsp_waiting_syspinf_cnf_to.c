/*!
 * @skip   $Id$
 * @file   m_dl_rec_finfrsp_waiting_syspinf_cnf_to.c
 * @brief  Event:システムパラメータ情報報告確認タイマタイムアウト通知, 対RE状態:ファイル情報報告応答(シスパラ)待ち
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
 *  @brief Event:システムパラメータ情報報告確認タイマタイムアウト通知, 対RE状態:ファイル情報報告応答(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_rec_finfrsp_waiting_syspinf_cnf_to(VOID* bufp, USHORT cpri_no)
{
	USHORT			lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rec_finfrsp_waiting_syspinf_cnf_to] ENTER" );

	lwSysType = (( CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;
	/* Change 対RE状態 => アイドル */
	gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rec_finfrsp_waiting_syspinf_cnf_to] RETURN" );
	return ;
}


/* @} */

