/*!
 * @skip   $Id$
 * @file   m_dl_rl_finfrsp_waiting_sysp_stop_ind.c
 * @brief  the processing for "REファイル更新中止指示(L3/DLM←L3/RCT)" receiving.
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
 *  @brief  the processing for "REファイル更新中止指示(L3/DLM←L3/RCT)" receiving
 *  REC主導:ファイル情報報告応答(シスパラ)集約中
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/25 FPT)VietCV Create
 *  @date   2015/08/31 FPT)Yen Update
 */
 
 
VOID m_dl_rl_finfrsp_waiting_sysp_stop_ind(VOID* bufp, USHORT cpri_no)
{
	USHORT				lwTimerId;
	USHORT				lwSysType;
	INT					llResult;
	int				llErrcd;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_finfrsp_waiting_sysp_stop_ind] ENTER" );

	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);

	lwSysType = ((CMT_TSKIF_RENEWSTPNTC *)bufp)->systerm;

	/* タイマ停止 システムパラメータ情報報告確認タイマ */
	/* All Re start 1 timer */
	lwTimerId = CMD_TIMID_SYSPRM_INF + lwSysType;

	llResult = cm_TStop( lwTimerId, &llErrcd );
	/*Stop timer NG*/
	if(llResult != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "システムパラメータ情報報告確認タイマ Stop NG" );
	}

	/* 対REC情報初期化 */
	(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_IDLE;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_finfrsp_waiting_sysp_stop_ind] RETURN" );

	return;
}
/** @} */
