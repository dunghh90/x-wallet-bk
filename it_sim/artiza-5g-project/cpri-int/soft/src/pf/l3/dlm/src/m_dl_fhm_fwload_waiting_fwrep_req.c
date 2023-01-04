/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fwload_waiting_fwrep_req.c
 * @brief  the processing for ファームウェアファイル報告要求 receiving.
 * @date   2015/11/18 TDIPS)sasaki Create

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
 *  @brief  the processing for ファームウェアファイル報告要求, status: REファイルデータ読込中.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/11/18 TDIPS)sasaki Create REC CNT-N/E切替問題対応
 */

VOID    m_dl_fhm_fwload_waiting_fwrep_req(VOID* bufp, USHORT cpri_no)
{
	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fhm_fwload_waiting_fwrep_req] ENTER");

	/* REファイルデータ読込中止API発行 */
	rrhApi_File_Mnt_CancelUpdFirm(D_RRH_PROCQUE_L3, 0, 0);
	
	/* RECスレッドへREファイル更新応答(NG)送信 */
	m_dl_UpdateExpandREDowloadingFile(bufp, cpri_no);

	/* 当該対RE-FW情報初期化 */
	REFW_init(&gt_dlw_refw_all_tbl[CMD_SYS_S3G][cpri_no-1]);
}

/* @} */