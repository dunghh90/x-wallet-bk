/*!
 * @skip   $Id$
 * @file   m_dl_rec_ufinfrsp_waiting_refw_stop_req.c
 * @brief  the processing for "REファイル更新中止要求" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/13 TDIPS)sasaki Update

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
 *  @brief  the processing for "REファイル更新中止要求" receiving at the 運用中ファイル情報報告応答(FW)待ち RE state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/28 FPT)DuongCD Create
 *  @date   2015/09/29 FPT)DuongCD IT update
 *  @date   2015/10/13 TDIPS)sasaki Update MKレビュー指摘No.286対応
 */

VOID m_dl_rec_ufinfrsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no)
{
	USHORT						lwTimerId;
	INT							ldwerrcd;
	UINT						ldwResult;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufinfrsp_waiting_refw_stop_req] ENTER" );
	
	/* ファームウェアファイル情報報告確認(更新用)タイマ#n停止 */
	lwTimerId = CMD_TIMID_UFIRMFILE_INF + cpri_no -1;
	
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル情報報告確認(更新用)タイマ Stop NG" );
	}
	
	/* 使用中タイマを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].use_timer = DLD_RERETIM_NONE;
	
	/* 当該対RE-FW情報初期化 */
	gt_dlw_refw_tbl[cpri_no -1].re_sta = DLD_RESTA_IDLE;

	gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_NONE;
	gt_dlw_refw_tbl[cpri_no-1].fhmrefile_inf = 0xFFFF;
	gt_dlw_refw_tbl[cpri_no-1].fw_rep_rslt = DLD_RESLT_IDLE;
	gt_dlw_refw_tbl[cpri_no-1].fw_rep_tc = 0;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufinfrsp_waiting_refw_stop_req] RETURN" );
	return ;
}

/* @} */

