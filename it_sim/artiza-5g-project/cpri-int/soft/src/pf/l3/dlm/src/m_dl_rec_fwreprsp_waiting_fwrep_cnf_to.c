/*!
 * @skip   $Id$
 * @file   m_dl_rec_fwreprsp_waiting_fwrep_cnf_to.c
 * @brief  the processing for "ファームウェアファイル報告確認タイマ" receiving.
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
 *  @brief  the processing for "ファームウェアファイル報告確認タイマ" receiving at the ファームウェアファイル報告応答待ち RE state
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01840
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/09/22 TDIPS)Lay M-RRU-ZSYS-01840 Add: gt_dlw_refw_tbl[cpri_no -1].re_sta = DLD_RESTA_IDLE;
 */
VOID m_dl_rec_fwreprsp_waiting_fwrep_cnf_to(VOID* bufp, USHORT cpri_no)
{	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_fwreprsp_waiting_fwrep_cnf_to] ENTER" );
	
	/*******************************************/
	/* 7-1-5ファームウェアファイル報告確認T.O. */
	/*******************************************/
	
	/* 当該対RE-FW情報テーブルのファームウェアファイル報告結果にタイムアウトを設定 */
	gt_dlw_refw_tbl[cpri_no -1].fw_rep_rslt = DLD_RESLT_TO;
	
	/* 当該対RE-FW情報テーブルの使用中タイマを初期化 */
	gt_dlw_refw_tbl[cpri_no -1].use_timer = DLD_RERETIM_NONE;
	
	/* 更新開始要因#nを初期化 */
	gt_dlw_refw_tbl[cpri_no -1].start_factor = DLD_UPDATE_FACTOR_NONE;
	
	/* アイドル */
	gt_dlw_refw_tbl[cpri_no -1].re_sta = DLD_RESTA_IDLE;
	
	/* 更新中リンク情報のT.O.発生CPRI#ビットOFF */
	gt_dlw_dlm_tbl.link_inf &= ~(CMD_ON << (cpri_no -1));
	
	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,	"[m_dl_rec_fwreprsp_waiting_fwrep_cnf_to] RETURN" );
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	
	printf( "[%d]%s fw_rep_rslt[%d], use_timer[%d], start_factor[%d], re_sta[%d] \n",__LINE__,__FUNCTION__,
			gt_dlw_refw_tbl[cpri_no -1].fw_rep_rslt,
			gt_dlw_refw_tbl[cpri_no -1].use_timer,
			gt_dlw_refw_tbl[cpri_no -1].start_factor,
			gt_dlw_refw_tbl[cpri_no -1].re_sta);
	
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return ;

}

/* @} */

