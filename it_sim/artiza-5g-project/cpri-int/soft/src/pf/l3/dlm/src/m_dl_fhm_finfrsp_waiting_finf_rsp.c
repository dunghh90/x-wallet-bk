/*!
 * @skip   $Id$
 * @file   m_dl_fhm_finfrsp_waiting_finf_rsp.c
 * @brief  Event:ファイル情報報告応答(L3信号：FHM←RE), 対RE状態:ファイル情報報告応答(シスパラ)待ち
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
 *  @brief  Event:ファイル情報報告応答(L3信号：FHM←RE), 対RE状態:ファイル情報報告応答(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01699
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/09/26 TDIPS)sasaki M-RRU-ZSYS-01699 Update MKレビュー指摘No.200対応
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.257対応
 *  @date   2021/03/03 M&C) Update src based on No.27 - Return if 3G
 */
VOID m_dl_fhm_finfrsp_waiting_finf_rsp(VOID* bufp, USHORT cpri_no)
{
	INT			ldwerrcd;
	UINT		lwResult;
	UINT		lwSysType;
	UINT		lwTimerId;
//	T_RRH_LAYER3		ltLayer3;
	UCHAR*				dlw_rcvsyspa_tbl[CMD_MAX_SYSNUM] =
							{ dlw_rcvsyspa_tbl_3g, dlw_rcvsyspa_tbl_s3g };

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
#endif

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_fhm_finfrsp_waiting_finf_rsp] ENTER" );

	lwSysType = (( CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;

	if (CMD_SYS_3G == lwSysType)
	{
		return;
	}

	/* 応答結果 */
	lwResult = ((CMT_TSKIF_CPRIRCV_FILEINFRES *)bufp)->cpridat_fileinfres.result;

	/* タイマ停止 システムパラメータ情報報告確認タイマ */
	lwTimerId = CMD_TIMID_SYSPRM_INF + cpri_no +  D_RRH_CPRINO_RE_MAX * lwSysType - CMD_NUM1;
	cm_TStop( lwTimerId, &ldwerrcd );

	/* 応答OK */
	if( CMD_OK == lwResult )
	{
		/* ファイルデータ送信準備 */
//		/* Layer3状態取得 */
//		f_cmn_com_layer3_get(lwSysType, &ltLayer3);
//		/* Check REC接続状態 */
//		/* REC接続状態 = 未接続 */
//		if( DLD_L3_STA_REC_IDLE == ltLayer3.layer3_rec )
//		{
//			/* 送信用システムパラメータに暫定値を設定 */
//			/* ファイルデータ送信 */
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
		/* システムパラメータデータ送信再送カウンタ初期化(=0回) */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].filedata_trans_tc = CMD_NUM0;

		/* タイマ起動 システムパラメータ送信完了確認タイマ */
		m_dl_fhm_sysp_filedatasendfin_timersta(cpri_no, lwSysType);
		/* Change 対RE状態 => ファイルデータ送信完了報告応答(シスパラ)待ち */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_FILE_COMP_SYSPRM_WAIT;

		return;
	}

	/* Check ファイル情報報告(システムパラメータ)通知再送カウンタ */
	/* ファイル情報報告(システムパラメータ)通知再送カウンタ = ファイル情報報告(システムパラメータ)通知再送回数 */
	if( gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].fileinf_rep_tc >= DLD_FILEINFREP_SYSPARA_MAX )
	{
		/* Change 対RE状態 => アイドル */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;
		/* REシステムパラメータ更新応答(NG) */
		m_dl_rl_sysp_dl_rsp(cpri_no, CMD_NG, lwSysType);
	}
	else
	{
		/* ファイル情報報告(システムパラメータ)通知再送カウンタをインクリメント */
		gt_dlw_refw_all_tbl[lwSysType][cpri_no - CMD_NUM1].fileinf_rep_tc++;
		/* ファイル情報報告通知(シスパラ)を対象配下REへ送信 */
		m_dl_fhm_sysp_fileinfo_req( cpri_no, lwSysType );
		/* タイマ起動 システムパラメータ情報報告確認タイマ */
		m_dl_fhm_sysp_fileinfo_timersta( cpri_no, lwSysType );
	}

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	return ;
}

/* @} */

