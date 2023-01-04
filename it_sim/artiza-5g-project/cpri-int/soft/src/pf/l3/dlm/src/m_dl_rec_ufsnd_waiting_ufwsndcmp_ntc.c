/*!
 * @skip   $Id$
 * @file   m_dl_rec_ufsnd_waiting_ufwsndcmp_ntc.c
 * @brief  the processing for "運用中ファイルデータ送信完了報告通知" receiving.
 * @date   2015/08/20 FPT)VietCV Create
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"												/* MDIF common head file			*/
#include "m_dl_header.h"												/* download management task head file */


/*!
 *  @brief  the processing for "運用中ファイルデータ送信完了報告通知" receiving
 *	対RE状態:運用中ファイルデータ送信中(FW)
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01816
 *  @date   2015/08/25 FPT)DuongCD Create
 *  @date   2015/10/02 TDIPS)sasaki Update MKレビュー指摘No.246対応
 *  @date   2015/10/20 TDIPS)sasaki M-RRU-ZSYS-01816 Update IT2問処No.118対処
 */
VOID m_dl_rec_ufsnd_waiting_ufwsndcmp_ntc(VOID* bufp, USHORT cpri_no)
{
	INT							ldwerrcd;
	USHORT						lwTimerId;
	UINT						ldwResult;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufsnd_waiting_ufwsndcmp_ntc] ENTER" );
	
	/* forward msg to re */
	((CMT_TSKIF_CPRIRCV_UFILSNDFINNTC*)bufp)->cprircv_inf.link_num = cpri_no;
	
	/* (0x4001)RE Forwarding Notice(運用中ファイルデータ送信完了報告通知) */
	l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, bufp, sizeof(CMT_TSKIF_CPRISND_UFILSNDFINNTC));
	
	/* ファイルデータ送信完了報告応答結果を初期化 */
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_IDLE;

	/* change status of re status */
	gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_USE_FILE_COMP_WAIT;
	
	/* ファームウェア送信完了確認タイマ(更新用)#n(※1)開始 */
	lwTimerId =CMD_TIMID_UFIRMFILE_SNDFIN + (cpri_no - 1);
	
	ldwResult = cm_TStat( lwTimerId, CMD_TIMVAL_UFIRMFILE_SNDFIN, CMD_TIMMD_SINGLE, CMD_TSKIF_TIMOUTNTC,CMD_TSKID_DLM,(INT*)&ldwerrcd);
	if( ldwResult != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_USELOW, ldwResult, "ファームウェア送信完了確認タイマ#n(※1)開始 NG" );
	}
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rec_ufsnd_waiting_ufwsndcmp_ntc] RETURN" );
	return;
}
/** @} */
