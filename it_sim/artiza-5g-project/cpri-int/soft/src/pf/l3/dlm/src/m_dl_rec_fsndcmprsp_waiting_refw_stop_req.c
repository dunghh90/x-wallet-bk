/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsndcmprsp_waiting_refw_stop_req.c
 * @brief  the processing for "REファイル更新中止要求" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/13 TDIPS)sasaki Update

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
 *  @brief  the processing for "REファイル更新中止要求" receiving at the ファイル情報報告応答(FW)待ち RE state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/28 FPT)DuongCD Create
 *  @date   2015/10/13 TDIPS)sasaki Update MKレビュー指摘No.289対応
 */

VOID m_dl_rec_fsndcmprsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no)
{
	USHORT						lwTimerId;
	INT							ldwerrcd;
	UINT						ldwResult;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fsndcmprsp_waiting_refw_stop_req] ENTER" );
	
	/* ファームウェア送信完了確認タイマ#n(※1)停止 */
	lwTimerId = CMD_TIMID_FIRMFILE_SNDFIN + cpri_no -1;
	
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェア送信完了確認タイマ Stop NG" );
	}
	
	/* 使用中タイマを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].use_timer = DLD_RERETIM_NONE;
	
	/* 当該対RE-FW情報初期化 */
	gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_IDLE;

	gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_NONE;
	gt_dlw_refw_tbl[cpri_no-1].fhmrefile_inf = 0xFFFF;
	gt_dlw_refw_tbl[cpri_no-1].trx_div_no = 0;
	gt_dlw_refw_tbl[cpri_no-1].trx_data_size = 0;
	gt_dlw_refw_tbl[cpri_no-1].fw_rep_rslt = DLD_RESLT_IDLE;
	gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_rslt = DLD_RESLT_IDLE;
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_IDLE;
	gt_dlw_refw_tbl[cpri_no-1].fw_rep_tc = 0;
	gt_dlw_refw_tbl[cpri_no-1].fileinf_rep_tc = 0;
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_tc = 0;

	/* call C_function */
	/* 立ち上げ時REファイル更新終了処理 */
	m_dl_reFileUpdateTermination();
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fsndcmprsp_waiting_refw_stop_req] RETURN" );
	return ;
}

/* @} */

