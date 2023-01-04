/*!
 * @skip   $Id$
 * @file   m_dl_rl_finfrsp_waiting_syspinf_cnf_to.c
 * @brief  Event:システムパラメータ情報報告確認タイマタイムアウト通知, 対REC状態:REC主導ファイル情報報告応答(シスパラ)集約中
 * @date   2015/08/22 FPT)Yen Create
 * @date   2015/10/15 TDIPS)sasaki Update

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
 *  @brief Event:システムパラメータ情報報告確認タイマタイムアウト通知, 対REC状態:REC主導ファイル情報報告応答(シスパラ)集約中
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/15 FPT)Duong Update review comment
 *  @date   2015/10/15 TDIPS)sasaki Update MKレビュー指摘No.260対応ミスを修正
 *  @date   2015/10/15 TDIPS)sasaki Update IT2問処No.91水平展開
 */

VOID m_dl_rl_finfrsp_waiting_syspinf_cnf_to(VOID* bufp, USHORT cpri_no)
{
	USHORT		lwReNo;
	USHORT		lwSysType;
	USHORT		lwRspResult;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_finfrsp_waiting_syspinf_cnf_to] ENTER" );

	lwSysType = (( CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;

	if( CMD_SYS_3G == lwSysType )
	{
		/* 1:情報報告NG */
		lwRspResult = CMD_INFRPT_NG;
	}
	else
	{
		/* 51:処理NG（RE）*/
		lwRspResult = CMD_HDL_NG;
	}

	/* loop all RE, check response's result */
	for(lwReNo = D_RRH_CPRINO_RE_MIN; lwReNo <= D_RRH_CPRINO_RE_MAX; lwReNo++)
	{
		/* 未応答REを対象に対RE状態遷移(REC主導)へ入力する */
		if( DLD_RESTA_FILEINF_SYSPRM_WAIT == gt_dlw_refw_all_tbl[lwSysType][lwReNo - CMD_NUM1].re_sta && 
			gt_dlw_refw_all_tbl[lwSysType][lwReNo - CMD_NUM1].filedata_trans_rslt == DLD_RESLT_IDLE )
		{
			/* 対RE状態遷移(REC自律)へ入力する */
			m_dl_rec_main(bufp, lwReNo);
		}
		/* Checkファイル情報報告結果 */
		if( DLD_RESLT_NML == gt_dlw_refw_all_tbl[lwSysType][lwReNo - CMD_NUM1].fileinf_rep_rslt )
		{
			lwRspResult = CMD_NML;
		}
	}
	/********************/
	/* CPRI信号送信処理 */
	/********************/
	/* ファイル情報報告応答送信処理起動 */
	m_dl_cm_CRFileInfRspSnd( lwRspResult, lwSysType );
	/* REC向けファイル情報報告結果設定 */
	if(CMD_NML != lwRspResult)
	{
		for(lwReNo = D_RRH_CPRINO_RE_MIN; lwReNo <= D_RRH_CPRINO_RE_MAX; lwReNo++)
		{
			gt_dlw_refw_all_tbl[lwSysType][lwReNo-1].re_sta = DLD_RESTA_IDLE;
		}
		lwRspResult = DLD_RESLT_NG;
		/* Change 対REC状態 => アイドル */
		(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_IDLE;
	}
	else
	{
		lwRspResult = DLD_RESLT_NML;
		/* Change 対REC状態 => ファイルデータ送信(シスパラ)待ち */
		(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_SYSPARA_WAIT;
	}
	(*gt_dlw_rec_all_tbl[lwSysType]).fileinf_rep_rslt = lwRspResult;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_finfrsp_waiting_syspinf_cnf_to] RETURN" );

	return ;
}


/* @} */

