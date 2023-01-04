/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufwreprsp_waiting_ufinf_ntc_ng.c
 * @brief  the processing for "運用中ファイル情報報告通知" receiving.
 * @date   2015/10/29 TDIPS)sasaki Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "運用中ファイル情報報告通知" receiving at the 運用中ファームウェア報告応答集約中 REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/10/29 TDIPS)sasaki Create N/E切替問題対応
 */

VOID m_dl_rl_ufwreprsp_waiting_ufinf_ntc_ng(VOID* bufp, USHORT cpri_no)
{
	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_ufwreprsp_waiting_ufinf_ntc_ng] ENTER");

	/* (LTE)(0x2521)運用中ファイル情報報告応答(51:処理NG(RE)) */
	m_dl_cm_CRUSFileInfRspSnd(CMD_HDL_NG);

	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_ufwreprsp_waiting_ufinf_ntc_ng] RETURN");
	return ;
}

/* @} */

