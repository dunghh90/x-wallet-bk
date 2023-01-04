/*!
 * @skip   $Id$
 * @file   m_dl_rec_ufwreprsp_waiting_ufwrep_cnf_to.c
 * @brief  the processing for "ファームウェアファイル報告確認(更新用)" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"												/* MDIF common head file			*/
#include "m_dl_header.h"												/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファームウェアファイル報告確認(更新用)" receiving at the 運用中ファームウェア報告応答集約中 REC state
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */

VOID m_dl_rec_ufwreprsp_waiting_ufwrep_cnf_to(VOID* bufp, USHORT cpri_no)
{
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufwreprsp_waiting_ufwrep_cnf_to] ENTER" );
	
	/* 当該対RE-FW情報テーブルのファームウェアファイル報告結果にタイムアウトを設定 */
	gt_dlw_refw_tbl[cpri_no -1].fw_rep_rslt = DLD_RESLT_TO;
	
	/* 当該対RE-FW情報テーブルの使用中タイマを初期化 */
	gt_dlw_refw_tbl[cpri_no -1].use_timer = DLD_RERETIM_NONE;
	
	/* 更新開始要因#nを初期化 */
	gt_dlw_refw_tbl[cpri_no -1].start_factor = DLD_UPDATE_FACTOR_NONE;
	
	/* 更新中リンク情報のT.O.発生CPRI#ビットOFF */
	gt_dlw_dlm_tbl.link_inf &= ~(CMD_ON << (cpri_no -1));
	
	/* アイドル */
	gt_dlw_refw_tbl[cpri_no -1].re_sta = DLD_RESTA_IDLE;
	
	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,	"[m_dl_rec_ufwreprsp_waiting_ufwrep_cnf_to] RETURN" );
	return ;

}

/* @} */

