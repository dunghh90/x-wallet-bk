/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fwreprsp_waiting_fwrep_rsp.c
 * @brief  Event: ファームウェアファイル報告応答(L3信号：FHM←RE) RE state 3 DLD_RESTA_FWREP_WAIT
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
#include "m_rc_header.h"
/*!
 *  @brief  Event: ファームウェアファイル報告応答(L3信号：FHM←RE) RE state 3 DLD_RESTA_FWREP_WAIT
 *  @note   7.5.2
			--1
			ref: m_dl_fl_fwrep_rsp.c
			--1.2
			"応答NGで
			リトライ可能=>3
			リトライ不可=>0
			
			応答OKで
			ファイル読み込み未実行=>5 [DLD_RESTA_RE_FILEREAD]
			ファイル読み込み済み(ファイル情報報告通知送信)=>4 [DLD_RESTA_FILEINF_WAIT]
			
			FHM保持FWバージョンと一致=>0"
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.243対応
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 */
VOID m_dl_fhm_fwreprsp_waiting_fwrep_rsp(VOID* bufp, USHORT cpri_no)
{
	USHORT	lw_result_nofile[CMD_MAX_SYSNUM] = {CMD_FRM_NOFILE, CMD_FRM_NOFILE_S3G};
	USHORT	lw_systype;
	USHORT	lw_result;
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_fwreprsp_waiting_fwrep_rsp] ENTER" );

	lw_systype = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.signal_kind & CMD_SYS_CHK_CPRID;
	lw_result = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.result;

	/* 応答結果が「ファームウェアなし」ならば応答内容を無効値に更新 */
	if(lw_result == lw_result_nofile[lw_systype])
	{
		((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.makername = 0;
		((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.mjr_ver = 0;
		((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.mir_ver = 0;
		lw_result = CMD_OK;
	}
	/*	3Gの場合	*/
	if( CMD_SYS_3G == lw_systype )
	{
		if (lw_result != CMD_OK)
		{
			m_dl_FWFileReportRespondNG_3g(bufp, cpri_no);
		}
		else
		{
			m_dl_FWFileReportRspRcv_3g(bufp, cpri_no);
		}
	}
	/*	LTEの場合	*/
	else
	{
		if (lw_result != CMD_OK)
		{
			m_dl_FWFileReportRespondNG(bufp, cpri_no);
		}
		else
		{
			m_dl_FWFileReportRspRcv(bufp, cpri_no);
		}
	}
}
/** @} */
