/*!
 * @skip   $Id$
 * @file   m_dl_fl_ufwrep_req.c
 * @brief  the processing for "運用中ファームウェアファイル報告要求" receiving.
 * @date   2015/08/22 FPT)Khiemnv1 Create
 * @date   2015/10/14 TDIPS)sasaki Update
 * @date   2015/11/18 TDIPS)sasaki Update

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
 *  @brief  the processing for "運用中ファームウェアファイル報告要求" receiving at the FHM自律DL処理中 REC state.
 *  @note   7.6.1
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/14 TDIPS)sasaki Update MKレビュー指摘No.294対応
 *  @date   2015/11/18 TDIPS)sasaki Update REC CNT-N/E切替問題対応
 */

VOID m_dl_fl_ufwrep_req(VOID* bufp, USHORT cpri_no)
{
	UINT	ldwSrcPQIdSave = ((CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;
	
	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fl_ufwrep_req] ENTER");

	/* LTE用対RE状態遷移(FHM自律)へ入力する */
	((CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID = CMD_SYS_S3G;
	m_dl_fhm_main(bufp, cpri_no);

	/* 3G用対RE状態遷移(FHM自律)へ入力する */
	((CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID = CMD_SYS_3G;
	m_dl_fhm_main(bufp, cpri_no);

	/*更新中リンク情報初期化 */
	gt_dlw_dlm_tbl.link_inf = 0x0000;

	/* 実行中DL開始要因に0:未実行を設定*/
	gt_dlw_dlm_tbl.dlstart_factor = DLD_UPDATE_FACTOR_NONE;

	/* 対RE状態遷移(REC主導)へ入力する */
	((CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID = ldwSrcPQIdSave;
	m_dl_rec_main(bufp, cpri_no);

	/* 実行中DL開始要因に1:RECを設定 */
	gt_dlw_dlm_tbl.dlstart_factor = DLD_FACTOR_REC;

	/* 対REC状態を運用中ファームウェア報告応答集約中へ遷移 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_USE_FW_REP_CON;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fl_ufwrep_req] RETURN");
}
/* @} */