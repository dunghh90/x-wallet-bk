/*!
 * @skip   $Id$
 * @file   m_dl_rec_fwreprsp_waiting_refw_stop_ind.c
 * @brief  the processing for "REファイル更新中止指示" receiving.
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
 *  @brief  the processing for "REファイル更新中止指示" receiving at the ファームウェアファイル報告応答待ち RE state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */

VOID m_dl_rec_fwreprsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no)
{
	USHORT  							lwTimerId;
	INT									ldwerrcd;
	UINT								ldwResult;
	
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fwreprsp_waiting_refw_stop_ind] ENTER" );
	
	/* ファームウェアファイル報告確認タイマ */
	lwTimerId = CMD_TIMID_FIRMFILE_REP + (cpri_no - 1);
	
	ldwResult = cm_TStop(lwTimerId,&ldwerrcd);
	/* Stop timer NG */
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "ファームウェアファイル報告確認タイマ Stop NG" );
	}
	
	/* タイマ管理テーブルのタイマ起動情報をタイマ未起動に設定 */
	gt_dlw_refw_tbl[cpri_no -1].use_timer = DLD_RERETIM_NONE;
	
	/* アイドル */
	gt_dlw_refw_tbl[cpri_no -1].re_sta = DLD_RESTA_IDLE;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fwreprsp_waiting_refw_stop_ind] RETURN" );
	return ;
}

/* @} */

