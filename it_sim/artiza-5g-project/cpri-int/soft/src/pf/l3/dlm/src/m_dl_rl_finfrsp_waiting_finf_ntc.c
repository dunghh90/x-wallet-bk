/*!
 * @skip   $Id$
 * @file   m_dl_rl_finfrsp_waiting_finf_ntc.c
 * @brief  the processing for "ファイル情報報告通知" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/29 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファイル情報報告通知" receiving at the ファイルデータ転送先決定待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/09/17 FPT)DuongCD update comment
 *  @date   2015/10/29 TDIPS)sasaki Update N/E切替問題対応
 */

VOID m_dl_rl_finfrsp_waiting_finf_ntc(VOID* bufp, USHORT cpri_no)
{
	INT		ldwerrcd;
	USHORT	lwLoop;

	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_finfrsp_waiting_finf_ntc] ENTER");

	/****************************************************************************/
	/* 各種テーブルをファイル情報報告通知受信前の状態に戻す						*/
	/****************************************************************************/
	/* ファームウェア送信確認タイマ停止 */
	(VOID)cm_TStop(CMD_TIMID_FIRMWARESND_S3G, &ldwerrcd);
	cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_dl_rl_finfrsp_waiting_finf_ntc] Timer Stop");			

	if (DLD_FILENAME_RE == gw_dl_FileType)
	{
		/* 全対RE状態#nをアイドルへ遷移 */
		for (lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++)
		{
			gt_dlw_refw_tbl[lwLoop].re_sta = DLD_RESTA_IDLE;
			gt_dlw_refw_tbl[lwLoop].trx_div_no = 0;
			gt_dlw_refw_tbl[lwLoop].trx_data_size = 0;
			gt_dlw_refw_tbl[lwLoop].fileinf_rep_rslt = DLD_RESLT_IDLE;
			gt_dlw_refw_tbl[lwLoop].filedata_trans_rslt = DLD_RESLT_IDLE;
			gt_dlw_refw_tbl[lwLoop].fileinf_rep_tc = 0;
			gt_dlw_refw_tbl[lwLoop].filedata_trans_tc = 0;
		}
	}

	/* 更新中リンク情報初期化0:未実行を設定 */
	gt_dlw_dlm_tbl.link_inf = CMD_NUM0;

	/* reset the handler */
	dlw_handle = CMD_NUM0;

	m_dl_idle_finf_ntc(bufp, cpri_no);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_finfrsp_waiting_finf_ntc] RETURN" );
	return ;
}

/* @} */

