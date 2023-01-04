/*!
 * @skip   $Id$
 * @file   m_dl_rl_finfrsp_waiting_fwsnd_cnf_to.c
 * @brief  the processing for "ファームウェア送信確認タイマタイムアウト通知" receiving.
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
 *  @brief  the processing for "ファームウェア送信確認タイマタイムアウト通知" receiving at the ファイルデータ転送先決定待ち REC state
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/18 FPT)DuongCD Create
 */

VOID m_dl_rl_finfrsp_waiting_fwsnd_cnf_to(VOID* bufp, USHORT cpri_no)
{	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_finfrsp_waiting_fwsnd_cnf_to] ENTER" );
	
	/* 対REC情報テーブルのファイルデータ送信完了報告結果にタイムアウトを設定 */
	gt_dlw_rec_tbl.filedata_trans_rslt = DLD_RESLT_TO;
	
	/* 対REC情報テーブルの使用中タイマを初期化 */
	gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_NONE;
	
	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,	"[m_dl_rl_finfrsp_waiting_fwsnd_cnf_to] RETURN" );
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s filedata_trans_rslt[%d], use_timer[%d]\n",__LINE__,__FUNCTION__, gt_dlw_rec_tbl.filedata_trans_rslt, gt_dlw_rec_tbl.use_timer);
	
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return ;
}

/* @} */
