/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fsndcmprsp_waiting_fsndcmp_rsp.c
 * @brief  Event:ファイルデータ送信完了報告応答, 対RE状態:ファイルデータ送信完了報告応答(FW)待ち.
 * @date   2015/08/22 FPT)Yen Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */

/*!
 *  @brief  Event:ファイルデータ送信完了報告応答, 対RE状態:ファイルデータ送信完了報告応答(FW)待ち.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/09/24 FPT)Yen Update comment
 */

VOID m_dl_fhm_fsndcmprsp_waiting_fsndcmp_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fhm_fsndcmprsp_waiting_fsndcmp_rsp] ENTER" );

	m_dl_REFileDataSndCompleteReportRspRcv(bufp, cpri_no);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fhm_fsndcmprsp_waiting_fsndcmp_rsp] RETURN" );

	return ;
}

/* @} */

