/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsndcmprsp_waiting_syspsndcmp_rsp.c
 * @brief  Event:ファイルデータ送信完了報告応答, 対RE状態:ファイルデータ送信完了報告応答(シスパラ)待ち
 * @date   2015/08/22 FPT)Yen Create
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
 *  @brief  Event:ファイルデータ送信完了報告応答, 対RE状態:ファイルデータ送信完了報告応答(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/09 FPT)Duong update MKレビュー指摘No.206対応
 *  @date   2015/10/12 TDIPS)sasaki Update MKレビュー指摘No.284対応
 */

VOID m_dl_rec_fsndcmprsp_waiting_syspsndcmp_rsp(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILSNDFINRES*	ltFilSndFinRes_p = NULL;/* CPRI信号データ部	*/
	UINT							lwResult;
	UINT							lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_fsndcmprsp_waiting_syspsndcmp_rsp] ENTER" );

	/* CPRI信号データ部取得 */
	ltFilSndFinRes_p = (CMT_TSKIF_CPRIRCV_FILSNDFINRES *)bufp;
	lwSysType = ltFilSndFinRes_p->cpridat_filsndfinres.signal_kind & CMD_SYS_CHK_CPRID;
	/* 対象データを取得 */
	lwResult = ltFilSndFinRes_p->cpridat_filsndfinres.result;

	/* OKの場合 */
	if( CMD_NML == lwResult )
	{
		/* ファイルデータ送信完了報告応答結果(シスパラ)保存 */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].filedata_trans_rslt = DLD_RESLT_NML;
		/* 対RE状態 => アイドル */ 
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;
	}
	/* NGの場合 */
	else
	{
		/* ファイルデータ送信完了報告応答結果(シスパラ)保存 */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].filedata_trans_rslt = DLD_RESLT_NG;
		/* 対RE状態 => ファイルデータ送信(シスパラ)待ち */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_FILE_TRANS_WAIT;
	}

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_fsndcmprsp_waiting_syspsndcmp_rsp] RETURN" );

	return ;
}

/* @} */

