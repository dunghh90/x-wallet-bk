/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fwload_waiting_refw_stop_req.c
 * @brief  Event: REファイル更新中止要求(L3/DLM←RE/REC) RE status 2 DLD_RESTA_RE_FILEREAD
 * @date   2015/08/22 FPT)Khiemnv1 Create
 * @date   2015/11/19 TDIPS)sasaki Update

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
 *  @brief  Event: REファイル更新中止要求(L3/DLM←RE/REC) RE status 2 DLD_RESTA_RE_FILEREAD
 *  @note   7.5.17
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/11/19 TDIPS)sasaki Update REC CNT-N/E切替問題対応
 */

VOID m_dl_fhm_fwload_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no)
{
	E_RRHAPI_RCODE	leRtn;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fhm_fwload_waiting_refw_stop_req] ENTER");
	m_dl_RELinkDisconect(bufp, cpri_no, ((CMT_TSKIF_REFILNEWSTP *)bufp)->systerm);

	/* REファイルデータ読込中止API発行 */
	leRtn = rrhApi_File_Mnt_CancelUpdFirm(D_RRH_PROCQUE_L3, 0, 0);
	if(leRtn != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_USELOW, leRtn, "rrhApi_File_Mnt_CancelUpdFirm NG");
	}

	/* DL中REファイル読込状態を未実行に設定 */
	gt_dlw_dlm_tbl.dlrefile_read_sta = DLD_READ_NONE;
	/* DL中ファイル情報初期化 */
	gt_dlw_dlm_tbl.dlrefile_inf = 0xFFFF;
}
/* @} */
