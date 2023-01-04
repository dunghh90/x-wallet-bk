/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsndcmprsp_waiting_sysp_stop_ind.c
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
 *  REC主導:ファイルデータ送信完了報告応答(シスパラ)集約中
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/25 FPT)VietCV Create
 *  @date   2015/08/31 FPT)Yen Update
 */
 
 
VOID m_dl_rl_fsndcmprsp_waiting_sysp_stop_ind(VOID* bufp, USHORT cpri_no)
{
	USHORT				lwTimerId;
	USHORT				lwSysType;
	UINT				llResult;
	INT				llErrcd;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsndcmprsp_waiting_sysp_stop_ind] ENTER" );

	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);

	lwSysType = ((CMT_TSKIF_RENEWSTPNTC *)bufp)->systerm;
	/* タイマ停止 システムパラメータ送信完了確認タイマ */
	/* All Re use 1 timer -> Fix Cpri = 1 */
	lwTimerId = CMD_TIMID_SYSPRM_SNDFIN + CMD_NUM1 +  D_RRH_CPRINO_RE_MAX * lwSysType - CMD_NUM1;

	llResult = cm_TStop(lwTimerId, &llErrcd);
	/* Stop timer NG */
	if( llResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, llResult, "システムパラメータ送信確認タイマ Stop NG" );
	}

	/* 対REC情報初期化 */
	(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_IDLE;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsndcmprsp_waiting_sysp_stop_ind] RETURN" );

	return;
}
/** @} */
