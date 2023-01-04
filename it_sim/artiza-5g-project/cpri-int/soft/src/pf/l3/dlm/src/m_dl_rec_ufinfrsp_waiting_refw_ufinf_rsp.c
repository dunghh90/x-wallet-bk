/*!
 * @skip   $Id$
 * @file   m_dl_rec_ufinfrsp_waiting_refw_ufinf_rsp.c
 * @brief  the processing for "運用中ファイル情報報告応答" receiving.
 * @date   2015/08/20 FPT)VietCV Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */

/*!
 *  @brief  the processing for "運用中ファイル情報報告応答" receiving
 *  対RE状態遷移: 運用中ファイル情報報告応答(FW)待ち state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/25 FPT)DuongCD Create
 */

VOID m_dl_rec_ufinfrsp_waiting_refw_ufinf_rsp(VOID* bufp, USHORT cpri_no)
{
	USHORT						lw_res_result;
	USHORT						lwTimerId;
	UINT						ldwResult;
	INT							ldwerrcd;
	
	
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_ufinfrsp_waiting_refw_ufinf_rsp] ENTER" );
	
	lw_res_result = ((CMT_TSKIF_CPRIRCV_UFILEINFRES *)bufp)->cpridat_ufileinfres.result;
	
	/* 対RE-FW情報テーブルに受信した応答結果を保存 */
	if(CMD_NML != lw_res_result)
	{
		lw_res_result = DLD_RESLT_NG;
	}
	else
	{
		lw_res_result = DLD_RESLT_NML;
	}
	gt_dlw_refw_tbl[cpri_no -1].fileinf_rep_rslt = lw_res_result;
	
	/* LTE system*/
	lwTimerId = CMD_TIMID_UFIRMFILE_INF + cpri_no -1;
	
	/* ファームウェアファイル情報報告確認(更新用)タイマ#n(※1)停止 */
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル情報報告確認(更新用)タイマ Stop NG" );
	}
	/* 応答NGで */
	if (gt_dlw_refw_tbl[cpri_no -1].fileinf_rep_rslt != DLD_RESLT_NML)
	{
		m_dl_UFileInfRes_NG(bufp, cpri_no);
	}
	else
	{
		/* 応答OK */
		m_dl_UFileInfRes_OK(cpri_no);
	}
	
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_ufinfrsp_waiting_refw_ufinf_rsp] RETURN" );
	return;
}
/** @} */
