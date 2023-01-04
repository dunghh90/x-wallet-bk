/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufsnd_waiting_finf_ntc_ng.c
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
 *  @brief  the processing for "ファイル情報報告通知" receiving at the 運用中ファイルデータ送信(FW)待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-02007
 *  @date   2015/10/29 TDIPS)sasaki Create N/E切替問題対応
 *  @date   2015/11/25 TDIPS)sasaki M-RRU-ZSYS-02007 Update IT2問処No.194対処 使用中タイマ情報を無視してファームウェアファイル報告確認(更新用)タイマ停止処理を実行
 */
VOID m_dl_rl_ufsnd_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no)
{
	UINT	ldwResult;
	UINT	ldwsigchk;
	INT		ldwerrcd;
	USHORT	lw_data = ((CMT_TSKIF_CPRIRCV_FILEINFREQ*)bufp)->cpridat_fileinfreq.data;
	USHORT	lw_systype;
	USHORT	lwLoop;
	USHORT	lw_rsp_result[CMD_MAX_SYSNUM] = {CMD_INFRPT_NG, CMD_HDL_NG};


	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_ufsnd_waiting_finf_ntc_ng] ENTER");

	ldwsigchk = m_cm_chksig(((CMT_TSKIF_CPRIRCV_FILEINFREQ*)bufp)->cpridat_fileinfreq.signal_kind, &lw_systype);
	if(ldwsigchk != CMD_RES_OK)
	{
		m_dl_cm_CRFileInfRspSnd(CMD_MAKER_RENG, lw_systype);
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "validate signal NG(ret=%d,sys=%d)", ldwsigchk, lw_systype);

		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	switch(lw_data)
	{
		case CMD_SYSPAR:	/* 対象データがシスパラ */
			break;

		case CMD_FRM:		/* 対象データがファームウェア */
			m_dl_cm_CRFileInfRspSnd(lw_rsp_result[lw_systype], lw_systype);
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "STATE NG(data=%d)", lw_data);

			#ifdef FHM_DLM_FOR_DEBUG_IT1
			printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
			#endif
			return;

		default:			/* 対象データがシスパラ、ファームウェア以外 */
			m_dl_cm_CRFileInfRspSnd(lw_rsp_result[lw_systype], lw_systype);
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "PARAMETER NG(data=%d)", lw_data);

			#ifdef FHM_DLM_FOR_DEBUG_IT1
			printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
			#endif
			return ;
	}

	/****************************************************************************/
	/* 各種テーブルをファイル情報報告通知受信前の状態に戻す						*/
	/****************************************************************************/
//	if(DLD_RECRETIM_SND_UPDATE == gt_dlw_rec_tbl.use_timer)
	{
		/* ファームウェア送信確認(更新用)タイマ停止 */
		(VOID)cm_TStop(CMD_TIMID_USEFIRMWARESND, &ldwerrcd);
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_dl_rl_ufsnd_waiting_finf_ntc_ng] Timer Stop");			

		/* タイマ管理テーブルのタイマ起動情報をタイマ未起動に設定 */
		gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_NONE;
	}

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

			if(gt_dlw_refw_tbl[lwLoop].use_timer != DLD_RERETIM_NONE)
			{
				ldwResult = cm_TStop((CMD_TIMID_UFIRMFILE_INF + lwLoop), &ldwerrcd);
				if(ldwResult != CMD_RES_OK)
				{
					cm_Assert(CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル情報報告確認(更新用)タイマ停止 NG");
				}

				gt_dlw_refw_tbl[lwLoop].use_timer = DLD_RERETIM_NONE;
			}
		}
	}

	/* 更新中リンク情報初期化0:未実行を設定 */
	gt_dlw_dlm_tbl.link_inf = CMD_NUM0;

	/* reset the handler */
	dlw_handle = CMD_NUM0;

	m_dl_SysPro(bufp, lw_systype);
	m_dl_rl_finfrsp_waiting_sysp_proc(lw_systype);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_ufsnd_waiting_finf_ntc_ng] RETURN" );
	return ;
}

/* @} */

