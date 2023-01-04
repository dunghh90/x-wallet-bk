/*!
 * @skip   $Id$
 * @file   m_dl_fhm_finfrsp_waiting_syspinf_cnf_to.c
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
 *  @brief  Event:システムパラメータ情報報告確認タイマタイムアウト通知, 対RE状態:ファイル情報報告応答(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.257対応
 *  @date   2021/03/03 M&C) Update src based on No.27 - Return if 3G
 */

VOID m_dl_fhm_finfrsp_waiting_syspinf_cnf_to(VOID* bufp, USHORT cpri_no)
{
	UINT		lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fhm_finfrsp_waiting_syspinf_cnf_to] ENTER" );

	lwSysType = (( CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;

	if (CMD_SYS_3G == lwSysType)
	{
		return;
	}

	/* Check ファイル情報報告(システムパラメータ)通知再送カウンタ */
	/* ファイル情報報告(システムパラメータ)通知再送カウンタ = ファイル情報報告(システムパラメータ)通知再送回数 */
	if( gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].fileinf_rep_tc >= DLD_FILEINFREP_SYSPARA_MAX )
	{
		/* Change 対RE状態 => アイドル */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;
		/* REシステムパラメータ更新応答(NG) */
		m_dl_rl_sysp_dl_rsp(cpri_no, CMD_NG, lwSysType);
	}
	else
	{
		/* ファイル情報報告(システムパラメータ)通知再送カウンタをインクリメント */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].fileinf_rep_tc++;
		/* ファイル情報報告通知(シスパラ)を対象配下REへ送信 */
		m_dl_fhm_sysp_fileinfo_req( cpri_no, lwSysType );
		/* タイマ起動 システムパラメータ情報報告確認タイマ */
		m_dl_fhm_sysp_fileinfo_timersta( cpri_no, lwSysType );
	}
	return ;
}

/* @} */

