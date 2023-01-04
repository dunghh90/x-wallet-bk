/*!
 * @skip   $Id$
 * @file   m_dl_fhm_finfrsp_waiting_fwinf_rsp.c
 * @brief  Event: ファイル情報報告応答(L3信号：FHM←RE) RE status 5 DLD_RESTA_FILEINF_WAIT
 * @date   2015/08/22 FPT)Khiemnv1 Create

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
 *  @brief  Event: ファイル情報報告応答(L3信号：FHM←RE) RE status 5 DLD_RESTA_FILEINF_WAIT
 *  @note   -------7.5.6--------------------------
			--(1)
			"対RE状態遷移(FHM自律)へ入力する
			=>2"
			call m_dl_fhm_main
			  >>m_dl_fhm_finfrsp_waiting_fwinf_rsp
			....>>7.5.6
			--(1.2)
			"応答NGで
			リトライ可能=>4
			リトライ不可=>0
			
			応答OK(ファイルデータ送信開始)=>6 [DLD_RESTA_FILE_TRANS]
			
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 */

VOID m_dl_fhm_finfrsp_waiting_fwinf_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_finfrsp_waiting_fwinf_rsp] ENTER" );

	//input
	//msg CPRI信号受信通知(ファイル情報報告応答) 0x2220
	USHORT lw_result = 0;
	CMT_TSKIF_CPRIRCV_FILEINFRES* lt_file_info_report_respond_p = 
		(CMT_TSKIF_CPRIRCV_FILEINFRES*)bufp;

	//REFW
	USHORT lw_REFW_index = 0;
	USHORT lw_can_retry = 0;

	//DLM
	USHORT lw_downloading_RE_file_read_status = 0;

	lw_REFW_index = convert_CPRI_no_REFW_index(cpri_no);

	lw_result = lt_file_info_report_respond_p->cpridat_fileinfres.result;

	if (lw_result != CMD_OK)
	{
		lw_can_retry = can_retry(lw_REFW_index,
			DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE);

		//call 7.5.15
		m_dl_FileInfoReportRespondNG(bufp, cpri_no);

		if (lw_can_retry)
		{
			set_RE_status(lw_REFW_index, DLD_RESTA_FILEINF_WAIT);
		}
		else
		{
			set_RE_status(lw_REFW_index, DLD_RESTA_IDLE);
		}
	}
	else
	{
		lw_downloading_RE_file_read_status = get_DownLoading_RE_file_read_status();

		//call 7.5.6
		m_dl_REFileInfReportRspRcv(bufp, cpri_no);

	}
}
/** @} */
