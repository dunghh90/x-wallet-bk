/*!
 * @skip   $Id$
 * @file   m_dl_rl_finfrsp_waiting_refw_stop_ind.c
 * @brief  the processing for "REファイル更新中止指示" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/14 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "REファイル更新中止指示" receiving at the ファイルデータ転送先決定待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/21 FPT)DuongCD Create
 *  @date   2015/10/14 TDIPS)sasaki Update MKレビュー指摘No.292対応
 */

VOID m_dl_rl_finfrsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no)
{
	
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_finfrsp_waiting_refw_stop_ind] ENTER" );
	
	/* 対RE状態遷移(FHM自律)へ入力する */
	m_dl_rec_main(bufp, cpri_no);
	
	/* 対REC情報初期化 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;
	
	/* 更新中リンク情報初期化 */
	gt_dlw_dlm_tbl.link_inf = 0x0000;
	
	/* 実行中DL開始要因に0:未実行を設定 */
	gt_dlw_dlm_tbl.dlstart_factor	= DLD_FACTOR_NONE;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_finfrsp_waiting_refw_stop_ind] RETURN" );
	
	return ;
}

/* @} */

