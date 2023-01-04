/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_finf_ntc.c
 * @brief  the processing for "ファイル情報報告通知" receiving.
 * @date   2015/10/29 TDIPS)sasaki Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファイル情報報告通知" receiving at the ファイルデータ送信(FW)待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-02007
 *  @date   2015/10/29 TDIPS)sasaki Create N/E切替問題対応
 *  @date   2015/11/25 TDIPS)sasaki M-RRU-ZSYS-02007 Update IT2問処No.194対処 使用中タイマ情報を無視してファームウェアファイル報告確認(更新用)タイマ停止処理を実行
 */
VOID m_dl_rl_fsnd_waiting_finf_ntc(VOID* bufp, USHORT cpri_no)
{
	UINT	ldwResult;
	INT		ldwerrcd;
	USHORT	lwLoop;

	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_fsnd_waiting_finf_ntc] ENTER");

	/****************************************************************************/
	/* 各種テーブルをファイル情報報告通知受信前の状態に戻す						*/
	/****************************************************************************/
	/* ファームウェア送信確認タイマ停止 */
	(VOID)cm_TStop(CMD_TIMID_FIRMWARESND_S3G, &ldwerrcd);
	cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_dl_rl_fsnd_waiting_finf_ntc] Timer Stop");

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

//			if(gt_dlw_refw_tbl[lwLoop].use_timer != DLD_RERETIM_NONE)
			{
				ldwResult = cm_TStop((CMD_TIMID_FIRMFILE_INF + lwLoop), &ldwerrcd);
				if(ldwResult != CMD_RES_OK)
				{
					cm_Assert(CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル情報報告確認タイマ停止 NG");
				}

				gt_dlw_refw_tbl[lwLoop].use_timer = DLD_RERETIM_NONE;
			}
		}
	}

	/* 更新中リンク情報初期化0:未実行を設定 */
	gt_dlw_dlm_tbl.link_inf = CMD_NUM0;

	/* reset the handler */
	dlw_handle = CMD_NUM0;

	m_dl_idle_finf_ntc(bufp, cpri_no);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_fsnd_waiting_finf_ntc] RETURN" );
	return ;
}

/* @} */

