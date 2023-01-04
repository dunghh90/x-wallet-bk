/*!
 * @skip   $Id$
 * @file   m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_rsp.c
 * @brief  the processing for "運用中ファイルデータ送信完了報告応答(L3信号：FHM←RE)" receiving.
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
 *  @brief  the processing for "運用中ファイルデータ送信完了報告応答(L3信号：FHM←RE)" receiving
 *	対RE状態:運用中ファイルデータ送信完了報告応答(FW)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 */

VOID m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_rsp] ENTER" );
	if (CMD_NML == ((CMT_TSKIF_CPRISND_FILSNDFINRES*)bufp)->cpridat_filsndfinres.result)
	{
		m_dl_fileDataTransRes_OK(cpri_no);
	}
	else
	{
		m_dl_fileDataTransRes_NG(cpri_no);
	}
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufsndcmprsp_waiting_ufwsndcmp_rsp] RETURN" );
	return;
}
/** @} */
