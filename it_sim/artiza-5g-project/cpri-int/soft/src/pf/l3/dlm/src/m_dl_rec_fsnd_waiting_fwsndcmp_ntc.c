/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsnd_waiting_fwsndcmp_ntc.c
 * @brief  the processing for "ファイルデータ送信完了報告通知" receiving.
 * @date   2015/08/20 FPT)VietCV Create
 * @date   2015/09/30 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */

/*!
 *  @brief  the processing for "ファイルデータ送信完了報告通知" receiving
 *  対REC状態:ファイルデータ送信(FW)待ち
 *	対RE状態:ファイルデータ送信中(FW)
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01816
 *  @Bug_No M-RRU-ZSYS-01844
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/09/30 TDIPS)sasaki Update MKレビュー指摘No.246対応
 *  @date   2015/10/20 TDIPS)sasaki M-RRU-ZSYS-01816 Update IT2問処No.118対処
 */
VOID m_dl_rec_fsnd_waiting_fwsndcmp_ntc(VOID* bufp, USHORT cpri_no)
{
	INT							ldwerrcd;
	USHORT						lwTimerId;
	UINT						ldwResult;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	
	/* forward msg to re */
	((CMT_TSKIF_CPRIRCV_FILSNDFINNTC*)bufp)->cprircv_inf.link_num = cpri_no;
	/* (0x4001)RE Forwarding Notice(ファイルデータ送信完了報告通知) */
	l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, bufp, sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINNTC));
	
	/* ファイルデータ送信完了報告応答結果を初期化 */
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_IDLE;
	/* change status of re status */
	gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_FILE_COMP_WAIT;
	
	/* ファームウェア送信完了確認タイマ#n(※1)開始 */
	lwTimerId = (cpri_no - 1) + CMD_TIMID_FIRMFILE_SNDFIN;
	
	ldwResult = cm_TStat( lwTimerId, CMD_TIMVAL_FIRMFILE_SNDFIN, CMD_TIMMD_SINGLE, CMD_TSKIF_TIMOUTNTC,CMD_TSKID_DLM,(INT*)&ldwerrcd);
	if( ldwResult != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_USELOW, ldwResult, "ファームウェア送信完了確認タイマ#n(※1)開始 NG" );
	}
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
}
/** @} */
