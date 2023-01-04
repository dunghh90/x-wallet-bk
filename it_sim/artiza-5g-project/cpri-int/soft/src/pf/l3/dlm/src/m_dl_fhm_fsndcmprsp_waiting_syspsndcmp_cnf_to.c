/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_cnf_to.c
 * @brief  Event:システムパラメータ送信完了確認タイマタイムアウト通知, 対RE状態:ファイルデータ送信完了報告応答(シスパラ)待ち
 * @date   2015/08/22 FPT)Yen Create
 * @date   2015/09/26 TDIPS)sasaki Update MKレビュー指摘No.200対応

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
 *  @brief  Event:システムパラメータ送信完了確認タイマタイムアウト通知, 対RE状態:ファイルデータ送信完了報告応答(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01699
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/09/26 TDIPS)sasaki M-RRU-ZSYS-01699 Update MKレビュー指摘No.200対応
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.257対応
 */
VOID m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_cnf_to(VOID* bufp, USHORT cpri_no)
{
	UINT		lwSysType;
//	T_RRH_LAYER3		ltLayer3;
	UCHAR*				dlw_rcvsyspa_tbl[CMD_MAX_SYSNUM] =
										{ dlw_rcvsyspa_tbl_3g, dlw_rcvsyspa_tbl_s3g };

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_cnf_to] ENTER" );

	lwSysType = (( CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;

	/* Check システムパラメータデータ送信再送カウンタ */
	/* システムパラメータデータ送信再送カウンタ = システムパラメータデータ送信再送回数  */
	if( gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].filedata_trans_tc >= DLD_FILEDATATRANS_SYSPARA_MAX )
	{
		/* Change 対RE状態 => アイドル */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;
		/* REシステムパラメータ更新応答(NG) */
		m_dl_rl_sysp_dl_rsp(cpri_no, CMD_NG, lwSysType);
	}
	else
	{
		/* システムパラメータデータ送信再送カウンタをインクリメントインクリメント */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].filedata_trans_tc++;
//		/* Layer3状態取得 */
//		f_cmn_com_layer3_get(lwSysType, &ltLayer3);
//		/* Check REC接続状態 */
//		/* REC接続状態 = 未接続 */
//		if( DLD_L3_STA_REC_IDLE == ltLayer3.layer3_rec )
//		{
//			/* 送信用システムパラメータに暫定値を設定 */
//			m_dl_fhm_sysp_filedatasend_req(cpri_no, lwSysType, (INT*)&cmw_sys_mng_tbl[lwSysType]);
//		}
//		/* REC接続状態 = 接続済 */
//		else
//		{
//			/* 送信用システムパラメータにRECからの転送データを設定 */
//			/* ファイルデータ送信 */
//			m_dl_fhm_sysp_filedatasend_req(cpri_no, lwSysType, (INT*)dlw_rcvsyspa_tbl[lwSysType]);
//		}

		/* ファイルデータ送信(システムパラメータ) */
		m_dl_fhm_sysp_filedatasend_req(cpri_no, lwSysType, (INT*)dlw_rcvsyspa_tbl[lwSysType]);
		/* タイマ起動 システムパラメータ送信完了確認タイマ */
		m_dl_fhm_sysp_filedatasendfin_timersta(cpri_no, lwSysType);
	}
	return ;
}

/* @} */

