/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_fwinf_cnf_to.c
 * @brief  the processing for "ファームウェアファイル情報報告確認T.O." receiving.
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
 *  @brief  the processing for "ファームウェアファイル情報報告確認T.O." receiving
 *  REC主導:ファイルデータ送信(FW)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/25 FPT)VietCV Create
 */
 
 
VOID m_dl_rl_fsnd_waiting_fwinf_cnf_to(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_fwinf_cnf_to] ENTER" );
	m_dl_rec_main(bufp, cpri_no);
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_fwinf_cnf_to] RETURN" );
}
/** @} */
