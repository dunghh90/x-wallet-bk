/*!
 * @skip   $Id$
 * @file   m_dl_rec_idle_fsndcmp_ntc.c
 * @brief  Event: ファイルデータ送信完了報告通知 RE status アイドル
 * @date   2015/08/28 FPT)DuongCD Create
 * @date   2015/09/28 TDIPS)sasaki Update
 * @date   2015/10/12 TDIPS)sasaki Update

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
 *  @brief   Event: ファイルデータ送信完了報告通知 RE status アイドル
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @Bug_No M-RRU-ZSYS-01724
 *  @date   2015/08/28 FPT)DuongCD Create
 *  @date   2015/09/28 TDIPS)sasaki M-RRU-ZSYS-01724 Update MKレビューNo.192暫定対応(ファイルデータ送信完了報告通知・応答（ファーム） REに信号が出て行かない)
 *  @date   2015/10/12 TDIPS)sasaki Update MKレビューNo.192正式対応
 */
VOID m_dl_rec_idle_fsndcmp_ntc(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_idle_fsndcmp_ntc] ENTER" );

	if(gt_dlw_dlm_tbl.link_inf & (0x0001 << (cpri_no - 1)))
	{
		/* forward msg to re */
		((CMT_TSKIF_CPRIRCV_FILSNDFINNTC*)bufp)->cprircv_inf.link_num = cpri_no;
		/* (0x4001)RE Forwarding Notice(ファイルデータ送信完了報告通知) */
		l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, bufp, sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINNTC));
	}

	gt_dlw_dlm_tbl.link_inf &= ~(0x0001 << (cpri_no - 1));
	gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_NONE;
    gt_dlw_refw_tbl[cpri_no-1].use_timer = DLD_RERETIM_NONE;
	gt_dlw_refw_tbl[cpri_no-1].fhmrefile_inf = 0xFFFF;
	gt_dlw_refw_tbl[cpri_no-1].trx_div_no = 0;
	gt_dlw_refw_tbl[cpri_no-1].trx_data_size = 0;
	gt_dlw_refw_tbl[cpri_no-1].fw_rep_rslt = DLD_RESLT_IDLE;
	gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_rslt = DLD_RESLT_IDLE;
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_IDLE;
	gt_dlw_refw_tbl[cpri_no-1].fw_rep_tc = 0;
	gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_tc = 0;
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_tc = 0;

	/* アイドル */
	gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_IDLE;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_idle_fsndcmp_ntc] RETURN" );
	return;
	
}
/** @} */
