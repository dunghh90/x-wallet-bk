/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_finf_rsp.c
 * @brief  the processing for "ファイル情報報告応答" receiving.
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
 *  @brief  the processing for "ファイル情報報告応答" receiving
 *  対REC状態遷移:ファイルデータ送信(FW)待ち state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 */

VOID m_dl_rl_fsnd_waiting_finf_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsnd_waiting_finf_rsp] ENTER" );
	m_dl_rec_main(bufp, cpri_no);
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsnd_waiting_finf_rsp] RETURN" );
}
/** @} */
