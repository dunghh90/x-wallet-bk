/*!
 * @skip   $Id$
 * @file   m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_cnf_to.c
 * @brief  the processing for ファームウェア送信完了確認(更新用)T.O.
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
 *  @brief  the processing for ファームウェア送信完了確認(更新用)T.O.
 *	対RE状態:運用中ファイルデータ送信完了報告応答(FW)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/09/01 FPT)DuongCD update
 *  @date   2015/09/16 FPT)VietCV update
 */
 
 
VOID m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_cnf_to(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_cnf_to] ENTER" );
	/* 当該対RE-FW情報テーブルの
	ファイルデータ送信完了報告結果にTOを設定 */
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_TO;
	
	/* 当該対RE-FW情報テーブルの
	使用中タイマを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].use_timer = DLD_RERETIM_NONE;
	
	/* 更新開始要因#nを初期化 */
	gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_NONE;
	
	/* call C_function */
	/* 立ち上げ時REファイル更新終了処理 */
	m_dl_reFileUpdateTermination();
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_cnf_to] RETURN" );
	return;
}
/** @} */
