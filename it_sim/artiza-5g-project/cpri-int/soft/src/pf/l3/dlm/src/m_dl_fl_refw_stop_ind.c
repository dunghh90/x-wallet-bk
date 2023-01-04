/*!
 * @skip   $Id$
 * @file   m_dl_fl_refw_stop_ind.c
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
 *  REC主導:FHM自律DL処理中
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/25 FPT)VietCV Create
 *  @date   2015/09/10 FPT)DuongCD update change to call fhm main
 */
 
 
VOID m_dl_fl_refw_stop_ind(VOID* bufp, USHORT cpri_no)
{
	USHORT			lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_refw_stop_ind] ENTER" );
	/* 対RE状態遷移(FHM自律)へ入力する */
	m_dl_fhm_main(bufp, cpri_no);

	lwSysType = ((CMT_TSKIF_RENEWSTPNTC *)bufp)->systerm;
	/* 対REC情報初期化 */
	(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_IDLE;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_refw_stop_ind] RETURN" );
	
	return;
}
/** @} */
