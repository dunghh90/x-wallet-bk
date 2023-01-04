/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_rsp.c
 * @brief  Event:ファイルデータ送信完了報告応答, 対RE状態:ファイルデータ送信完了報告応答(シスパラ)待ち.
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
 *  @brief  Event:ファイルデータ送信完了報告応答, 対RE状態:ファイルデータ送信完了報告応答(シスパラ)待ち.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01699
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/09/26 TDIPS)sasaki M-RRU-ZSYS-01699 Update MKレビュー指摘No.200対応
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.257対応
 *  @date   2015/10/17 TDI)satou FHM技説-QA-013
 */
VOID m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_rsp(VOID* bufp, USHORT cpri_no)
{
	INT			ldwerrcd;
	UINT		lwResult;
	UINT		lwSysType;
	UINT		lwTimerId;
//	T_RRH_LAYER3		ltLayer3;
	UCHAR*				dlw_rcvsyspa_tbl[CMD_MAX_SYSNUM] =
										{ dlw_rcvsyspa_tbl_3g, dlw_rcvsyspa_tbl_s3g };

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fhm_fsndcmprsp_waiting_syspsndcmp_rsp] ENTER" );

	lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;
	/* 応答結果 */
	lwResult = ((CMT_TSKIF_CPRIRCV_FILSNDFINRES *)bufp)->cpridat_filsndfinres.result;

	/* タイマ停止 システムパラメータ送信完了確認タイマ */
	lwTimerId = CMD_TIMID_SYSPRM_SNDFIN + cpri_no +  D_RRH_CPRINO_RE_MAX * lwSysType - CMD_NUM1;
	cm_TStop( lwTimerId, &ldwerrcd );

	/* 応答OK */
	if( CMD_OK == lwResult )
	{
	    if ((CMD_SYS_S3G == lwSysType) && (CMD_SYS_S3G != ((CMT_TSKIF_CPRISND_FILSNDFINRES_S3G *)bufp)->cpridat_filsndfinres.primary_sys))
	    {
            /* S3Gのファイルデータ送信完了報告応答の場合、優先システムがS3Gでない場合、NGを送信し、REリセットを送信させる */
            m_dl_rl_sysp_dl_rsp(cpri_no, CMD_NG_PRIMARY, lwSysType);
	    }
	    else
	    {
	        /* REシステムパラメータ更新応答(OK) */
            m_dl_rl_sysp_dl_rsp(cpri_no, CMD_OK, lwSysType);
	    }
		/* Change 対RE状態 => アイドル */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;
		return;
	}

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
//			m_dl_fhm_sysp_filedatasend_req(cpri_no, lwSysType, (INT*)dlw_rcvsyspa_tbl[lwSysType]);
//		}

		/* ファイルデータ送信(システムパラメータ) */
		m_dl_fhm_sysp_filedatasend_req(cpri_no, lwSysType, (INT*)dlw_rcvsyspa_tbl[lwSysType]);
		/* タイマ起動 システムパラメータ送信完了確認タイマ */
		m_dl_fhm_sysp_filedatasendfin_timersta(cpri_no, lwSysType);
	}

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fhm_finfrsp_waiting_finf_rsp] RETURN" );

	return ;
}

/* @} */

