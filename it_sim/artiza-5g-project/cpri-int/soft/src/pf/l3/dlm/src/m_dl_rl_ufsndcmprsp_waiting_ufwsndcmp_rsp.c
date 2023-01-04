/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufsndcmprsp_waiting_ufwsndcmp_rsp.c
 * @brief  the processing for "運用中ファイルデータ送信完了報告応答" receiving.
 * @date   2015/08/25 FPT)VietCV Create

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
 *  @brief  the processing for "運用中ファイルデータ送信完了報告応答" receiving.
 *  REC主導: 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 * 	@date   2015/08/25 FPT)VietCV create
 * 	@date   2015/09/01 FPT)DuongCD update
 */
 
VOID m_dl_rl_ufsndcmprsp_waiting_ufwsndcmp_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsndcmprsp_waiting_ufwsndcmp_rsp] ENTER" );
	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);
	
	/* call C_function */
	/* 立ち上げ時REファイル更新終了処理 */
	m_dl_reFileUpdateTermination();
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsndcmprsp_waiting_ufwsndcmp_rsp] RETURN" );
	/* C_function end*/
	return;
}
/** @} */
