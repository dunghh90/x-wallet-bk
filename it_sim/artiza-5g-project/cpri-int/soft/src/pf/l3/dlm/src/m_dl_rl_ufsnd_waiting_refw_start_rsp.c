/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufsnd_waiting_refw_start_rsp.c
 * @brief  the processing for "運用中REファイル更新開始応答(L3/DLM←RE/REC)" receiving.
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
 *  @brief  the processing for "運用中REファイル更新開始応答(L3/DLM←RE/REC)" receiving.
 *  REC主導:運用中ファイルデータ送信(FW)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 * 	@date   2015/08/25 FPT)VietCV create
 */
 
VOID m_dl_rl_ufsnd_waiting_refw_start_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsnd_waiting_refw_start_rsp] ENTER" );
	m_dl_rec_main(bufp, cpri_no);
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsnd_waiting_refw_start_rsp] RETURN" );
}
/** @} */
