/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufwreprsp_waiting_ufwrep_req.c
 * @brief  the processing for "運用中ファームウェアファイル報告要求" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "運用中ファームウェアファイル報告要求" receiving at the 運用中ファームウェア報告応答集約中 REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */

VOID m_dl_rl_ufwreprsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no)
{
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufwreprsp_waiting_ufwrep_req] ENTER" );
	
	/* 実行中DL開始要因に1:RECを設定 */
	gt_dlw_dlm_tbl.dlstart_factor = DLD_FACTOR_REC;
	
	/* 対RE状態遷移(REC主導)へ入力する(L3状態が起動中か運用中のREを対象) */
	m_dl_rec_main(bufp, cpri_no);
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufwreprsp_waiting_ufwrep_req] RETURN" );
	
	return ;
}

/* @} */

