/*!
 * @skip   $Id$
 * @file   m_dl_rec_finfrsp_waiting_refw_finf_rsp.c
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
 *	対RE状態遷移: ファイル情報報告応答(FW)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 */

VOID m_dl_rec_finfrsp_waiting_refw_finf_rsp(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILEINFRES* ltCpriRcvRes;
	
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	
	ltCpriRcvRes = (CMT_TSKIF_CPRIRCV_FILEINFRES*) bufp;
	
	if (CMD_NML == ltCpriRcvRes->cpridat_fileinfres.result)
	{
		m_dl_FileInfRes_OK(cpri_no);
	}
	/* NG result process */
	else
	{
		m_dl_FileInfRes_NG(bufp, cpri_no);
	}
	
#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;
}
/** @} */
