/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsndcmprsp_waiting_fwsndcmp_rsp.c
 * @brief  the processing for "ファイルデータ送信完了報告応答" receiving.
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
 *  @brief  the processing for "ファイルデータ送信完了報告応答" receiving
 *  対REC状態:ファイルデータ送信完了報告応答(シスパラ)集約中
 *	対RE状態:ファイルデータ送信完了報告応答(FW)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 *	@date   2015/09/16 FPT)VietCV update
 */
 
VOID m_dl_rec_fsndcmprsp_waiting_fwsndcmp_rsp(VOID* bufp, USHORT cpri_no)
{
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	if (CMD_NML == ((CMT_TSKIF_CPRISND_FILSNDFINRES*)bufp)->cpridat_filsndfinres.result)
	{
		m_dl_fileDataTransRes_OK(cpri_no);
	}
	else
	{
		m_dl_fileDataTransRes_NG(cpri_no);
	}
	
	/* call C_function */
	/* 立ち上げ時REファイル更新終了処理 */
	m_dl_reFileUpdateTermination();
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return;
}
/** @} */
