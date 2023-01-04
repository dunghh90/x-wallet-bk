/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufsnd_waiting_fw_stop_req.c
 * @brief  the processing for "REファイル更新中止要求" receiving.
 * @date   2015/08/18 FPT)DuongCD Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "REファイル更新中止要求" receiving at the 運用中ファイルデータ送信(FW)待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */

VOID m_dl_rl_ufsnd_waiting_fw_stop_req(VOID* bufp, USHORT cpri_no)
{
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsnd_waiting_fw_stop_req] ENTER" );
	
	/*更新中リンク情報のリンク断CPRI#ビットOFF */
	gt_dlw_dlm_tbl.link_inf &= ~(CMD_ON << (cpri_no -1));
	
	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsnd_waiting_fw_stop_req] RETURN" );
	
	return ;
}

/* @} */

