/*!
 * @skip   $Id$
 * @file   m_dl_rec_fsnd_waiting_syspsndcmp_ntc.c
 * @brief  Event:ファイルデータ送信完了報告通知, 対RE状態:ファイルデータ送信(シスパラ)待ち
 * @date   2015/08/31 FPT)Yen Create
 * @date   2015/09/28 TDIPS)sasaki Update
 * @date   2015/10/15 TDIPS)sasaki Update

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
 *  @brief Event:ファイルデータ送信完了報告通知, 対RE状態:ファイルデータ送信(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01783
 *  @date   2015/08/31 FPT)Yen Create
 *  @date   2015/09/28 TDIPS)sasaki Update MKレビュー指摘No.246対応
 *  @date   2015/10/15 TDIPS)sasaki M-RRU-ZSYS-01783 Update IT2問処No.88対応
 */
VOID m_dl_rec_fsnd_waiting_syspsndcmp_ntc(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILSNDFINNTC*	ltFilSndFinNtc_p = NULL;/* CPRI信号データ部	*/
	UINT							lwSysType;
	UINT							llResult;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rec_fsnd_waiting_syspsndcmp_ntc] ENTER" );

	/* CPRI信号データ部取得 */
	ltFilSndFinNtc_p = (CMT_TSKIF_CPRIRCV_FILSNDFINNTC*)bufp;

	lwSysType = ltFilSndFinNtc_p->cpridat_filsndfinntc.signal_kind & CMD_SYS_CHK_CPRID;

	/* forward msg to re */
	((CMT_TSKIF_CPRIRCV_FILSNDFINNTC*)bufp)->cprircv_inf.link_num = cpri_no;
	/* (0x4001)RE Forwarding Notice(ファイルデータ送信完了報告通知) */
	llResult = l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, bufp, sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINNTC));
	if(D_RRH_OK != llResult)
	{
		cm_Assert( CMD_ASL_USELOW, llResult,"[m_dl_rec_fsnd_waiting_syspsndcmp_ntc] File data send finish NG " );
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}
	/* ファイルデータ送信完了報告応答結果を初期化 */
	gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].filedata_trans_rslt = DLD_RESLT_IDLE;
	/* Change 対RE状態 => ファイルデータ送信完了報告応答(シスパラ)待ち */
	gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_FILE_COMP_SYSPRM_WAIT;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rec_fsnd_waiting_syspsndcmp_ntc] RETURN" );
	return ;
}


/* @} */

