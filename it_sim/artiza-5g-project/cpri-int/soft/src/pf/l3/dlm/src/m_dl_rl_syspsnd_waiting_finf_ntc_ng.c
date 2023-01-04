/*!
 * @skip   $Id$
 * @file   m_dl_rl_syspsnd_waiting_finf_ntc_ng.c
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
 *  @brief  the processing for "ファイル情報報告通知" receiving at the ファイルデータ送信(シスパラ)待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/10/29 TDIPS)sasaki Create N/E切替問題対応
 */

VOID m_dl_rl_syspsnd_waiting_finf_ntc_ng(VOID* bufp, USHORT cpri_no)
{
	UINT	ldwsigchk;
	INT		ldwerrcd;
	USHORT	lw_data = ((CMT_TSKIF_CPRIRCV_FILEINFREQ*)bufp)->cpridat_fileinfreq.data;
	USHORT	lw_systype;
	USHORT	lwLoop;
	USHORT	lw_rsp_result[CMD_MAX_SYSNUM] = {CMD_INFRPT_NG, CMD_HDL_NG};


	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_syspsnd_waiting_finf_ntc_ng] ENTER");

	ldwsigchk = m_cm_chksig(((CMT_TSKIF_CPRIRCV_FILEINFREQ*)bufp)->cpridat_fileinfreq.signal_kind, &lw_systype);
	if(ldwsigchk != CMD_RES_OK)
	{
		m_dl_cm_CRFileInfRspSnd(CMD_MAKER_RENG, lw_systype);
		cm_Assert(CMD_ASL_USELOW, ldwsigchk, "validate signal NG");

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
	/* システムパラメータ送信確認タイマ停止 */
	(VOID)cm_TStop((CMD_TIMID_SYSPRMSND_3G + lw_systype) , &ldwerrcd);
	cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_dl_rl_syspsnd_waiting_finf_ntc_ng] Timer Stop");			

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

	m_dl_SysPro(bufp, lw_systype);
	m_dl_rl_finfrsp_waiting_sysp_proc(lw_systype);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_syspsnd_waiting_finf_ntc_ng] RETURN" );
	return ;
}

/* @} */

