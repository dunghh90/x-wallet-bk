/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsndcmprsp_waiting_sysp_stop_ind.c
 * @brief  Event:REファイル更新中止指示, 対RE状態:ファイルデータ送信完了報告応答(シスパラ)待ち
 * @date   2015/08/31 FPT)Yen Create

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
 *  @brief Event:REファイル更新中止指示, 対RE状態:ファイルデータ送信(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/31 FPT)Yen Create
 */

VOID m_dl_rec_fsndcmprsp_waiting_sysp_stop_ind(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rec_fsndcmprsp_waiting_sysp_stop_ind] ENTER" );

	/* Change 対RE状態 => アイドル */
	gt_dlw_refw_all_tbl[CMD_SYS_3G][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;
	gt_dlw_refw_all_tbl[CMD_SYS_S3G][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rec_fsndcmprsp_waiting_sysp_stop_ind] RETURN" );
	return ;
}


/* @} */

