/*!
 * @skip   $Id$
 * @file   m_dl_ReFileUpdate3g.c
 * @brief  
 * @date   2015/10/08 FPT)Duong Create
 * @date   2015/10/19 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update

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
 *  @brief  Send (0x4001)RE Forwarding Notice(ファームウェアファイル報告要求(3G))
 *  @note   
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01816
 *  @date   2015/10/08 FPT)Duong Create (MKレビュー指摘No.243対応)
 *  @date   2015/10/17 FPT)Yen Update (memset lt_FW_file_report_req)
 *  @date   2015/10/20 TDIPS)sasaki M-RRU-ZSYS-01816 Update IT2問処No.118対処
 */
VOID m_dl_FWFileInfoRequestSnd_3g(USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FIRMFILEREQ 		lt_FW_file_report_req;
	INT									llerrcd;
	USHORT								lwTimerId;
	UINT								llResult;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileInfoRequestSnd_3g] ENTER" );

	/*(0x4001)RE Forwarding Notice(ファームウェアファイル報告要求)
	 * ファームウェアファイル報告要求(CMT_CPRIF_FIRMFILEREQ)
	 * 0x2110
	 * CMT_CPRIF_FIRMFILEREQ
	 * CPRI信号受信通知(ファームウェアファイル報告要求)
	 */
	memset(&lt_FW_file_report_req, 0, sizeof(lt_FW_file_report_req));
	lt_FW_file_report_req.head.uiSignalkind = CMD_CPRID_FIRMFILEREQ;
	lt_FW_file_report_req.cprircv_inf.link_num = cpri_no;
	lt_FW_file_report_req.cpridat_firmfilereq.signal_kind = CMD_CPRID_FIRMFILEREQ;
	RE_forwarding_notice(MSG_FW_FILE_REPORT_REQ, &lt_FW_file_report_req);

	/* ファームウェアファイル報告確認タイマ(3G)#n(※1)開始 */
	lwTimerId =  CMD_TIMID_FIRMFILE_REP_3G + (cpri_no - 1);
	
	llResult = cm_TStat( lwTimerId, CMD_TIMVAL_FIRMFILE_REP, CMD_TIMMD_SINGLE,
						CMD_TSKIF_TIMOUTNTC, CMD_TSKID_DLM, (INT*)&llerrcd);
	if( llResult != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_USELOW, llResult, "ファームウェアファイル報告確認タイマ#n(※1)開始 NG" );
	}

	/* ファームウェアファイル報告応答結果を初期化 */
	gt_dlw_refw_3g_tbl[cpri_no-1].fw_rep_rslt = DLD_RESLT_IDLE;

	/*対RE状態#n(※1)をファームウェアファイル報告応答待ちへ遷移
	*change RE status to FW file report respond wait state
	*/
	gt_dlw_refw_3g_tbl[cpri_no - CMD_NUM1].re_sta = DLD_RESTA_FWREP_WAIT;
	
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileInfoRequestSnd_3g] RETURN" );
}
/*!
 *  @brief 
 *  @note   7-5-2 ファームウェアファイル報告応答受信 (3G)
 *  @param  bufp [in]
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @Bug_No M-RRU-ZSYS-01795
 *  @date   2015/10/08 FPT)Duong create (MKレビュー指摘No.243対応)
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 *  @date   2015/10/21 FPT)Yen M-RRU-ZSYS-01795 update (ファームウェアファイル報告確認タイマ#n(3G)(※1)停止, 対RE状態#n(※1)をアイドルへ遷移)
 */
VOID m_dl_FWFileReportRspRcv_3g(VOID* bufp, USHORT cpri_no)
{
	USHORT lw_result = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.result;
	USHORT				lwTimerId;
	INT					ll_error_code;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileReportRspRcv_3g] ENTER" );

	/* ファームウェアファイル報告確認タイマ#n(3G)(※1)停止 */
	lwTimerId =  CMD_TIMID_FIRMFILE_REP_3G + (cpri_no - 1);
	cm_TStop(lwTimerId, &ll_error_code);

	/* 対RE状態#n(※1)をアイドルへ遷移 */
	gt_dlw_refw_3g_tbl[cpri_no - CMD_NUM1].re_sta = DLD_RESTA_IDLE;

	/* 再送回数[ファームウェアファイル報告要求]#n初期化 */
	gt_dlw_refw_3g_tbl[cpri_no - CMD_NUM1].fw_rep_tc = CMD_NUM0;

	/* 7-5-10 REファイル更新なし(パラメータ:OK) */
	m_dl_REFileNotUpdate_3g(cpri_no, CMD_OK, (lw_result == CMD_FRM_NOFILE ? CMD_OFF : CMD_ON));

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileReportRspRcv_3g] RETURN" );
}
/*!
 *  @brief  Receive ファームウェアファイル報告応答(L3信号：FHM←RE) (NG)
 *  @note   7-5-14 ファームウェアファイル報告応答NG (3G)
 *  @param  bufp [in]
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/10/08 FPT)Duong create (MKレビュー指摘No.243対応)
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 */
VOID m_dl_FWFileReportRespondNG_3g(VOID* bufp, USHORT cpri_no)
{
	USHORT				lwTimerId;
	INT					ll_error_code;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileReportRespondNG_3g] ENTER" );

	/* ファームウェアファイル報告確認タイマ#n(3G)(※1)停止 */
	lwTimerId =  CMD_TIMID_FIRMFILE_REP_3G + (cpri_no - 1);
	cm_TStop(lwTimerId, &ll_error_code);

	/* 再送回数＜再送回数最大値 */
	if( gt_dlw_refw_3g_tbl[cpri_no - CMD_NUM1].fw_rep_tc < DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ_MAX )
	{
		/* 再送回数[ファームウェアファイル報告要求]#nを加算 */
		gt_dlw_refw_3g_tbl[cpri_no - CMD_NUM1].fw_rep_tc++;
		/* ファームウェアファイル報告確認タイマ#n(※1)開始 */
		/* (0x4001)RE Forwarding Notice(ファームウェアファイル報告要求(3G)) */
		m_dl_FWFileInfoRequestSnd_3g(cpri_no);

		return;
	}

	/* 再送回数[ファームウェアファイル報告要求]#n初期化 */
	gt_dlw_refw_3g_tbl[cpri_no - CMD_NUM1].fw_rep_tc = CMD_NUM0;

	/* 7-5-10 REファイル更新なし(パラメータ:NG) */
	m_dl_REFileNotUpdate_3g(cpri_no, CMD_NG, CMD_OFF);

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileReportRespondNG_3g] RETURN" );
}
/*!
 *  @brief  Receive ファームウェアファイル報告確認タイマタイムアウト通知(3G)
 *  @note   Receive ファームウェアファイル報告確認タイマタイムアウト通知(3G)
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/10/08 FPT)Duong create (MKレビュー指摘No.243対応)
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 */
VOID m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to_3g(USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to_3g] ENTER" );

	/* 再送回数＜再送回数最大値 */
	if( gt_dlw_refw_3g_tbl[cpri_no - CMD_NUM1].fw_rep_tc < DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ_MAX )
	{
		/* 再送回数[ファームウェアファイル報告要求]#nを加算 */
		gt_dlw_refw_3g_tbl[cpri_no - CMD_NUM1].fw_rep_tc++;
		/* ファームウェアファイル報告確認タイマ#n(※1)開始 */
		/* (0x4001)RE Forwarding Notice(ファームウェアファイル報告要求(3G)) */
		m_dl_FWFileInfoRequestSnd_3g(cpri_no);

		return;
	}

	/* 再送回数[ファームウェアファイル報告要求]#n初期化 */
	gt_dlw_refw_3g_tbl[cpri_no - CMD_NUM1].fw_rep_tc = CMD_NUM0;

	/* 7-5-10 REファイル更新なし(パラメータ:NG) */
	m_dl_REFileNotUpdate_3g(cpri_no, CMD_NG, CMD_OFF);

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to_3g] RETURN" );
}
/*!
 *  @brief  REファイル更新なし
 *  @note   Send ファイルデータ送信完了報告通知(更新なし) -> re/rec
 			Send REファイル更新応答(結果(OK or NG),更新なし) -> re/rec
 *  @param  cpri_no [in]  the CPRI no
 *  @param  aw_res [in]  response result
 *  @param  sndcmpntc_flg [in]  ファイルデータ送信完了報告通知送信要否
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/10/08 FPT)Duong create (MKレビュー指摘No.243対応)
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 */
VOID m_dl_REFileNotUpdate_3g(USHORT cpri_no, USHORT aw_res, USHORT sndcmpntc_flg)
{
	CMT_TSKIF_CPRIRCV_FILSNDFINNTC 		ltFilSndFinNtc;
	CMT_TSKIF_REFILNEWRES 				ltReFilNewRes;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_REFileNotUpdate_3g] ENTER" );

	if(sndcmpntc_flg == CMD_ON)
	{
		/* ファイルデータ送信完了報告通知 */
		memset(&ltFilSndFinNtc, 0, sizeof(ltFilSndFinNtc));

		ltFilSndFinNtc.cprircv_inf.link_num             = cpri_no;
		ltFilSndFinNtc.head.uiSignalkind                = CMD_CPRID_FILSNDFINNTC;
		ltFilSndFinNtc.cpridat_filsndfinntc.signal_kind = CMD_CPRID_FILSNDFINNTC;
		/* 更新なし */
		ltFilSndFinNtc.cpridat_filsndfinntc.tgtdat      = CMD_FRM_NONEW;

		/* (0x4001)RE Forwarding Notice(ファイルデータ送信完了報告通知(更新なし)) */
		RE_forwarding_notice(MSG_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE, &ltFilSndFinNtc);
	}

	/* msg REファイル更新応答 */
	memset(&ltReFilNewRes, 0, sizeof(ltReFilNewRes));
	/* REファイル更新応答(結果(OK or NG),更新なし) */
	ltReFilNewRes.head.uiEventNo 	= CMD_TSKIF_REFIRM_DL_RSP;
	ltReFilNewRes.cpri_no 			= cpri_no;
	ltReFilNewRes.result_code 		= aw_res;
	ltReFilNewRes.reset_jdg 		= CMD_OFF;
	ltReFilNewRes.dummy				= CMD_SYS_3G;				/* 3G systerm */
	RE_forwarding_notice(MSG_RE_FILE_UPDATE_RESPOND, &ltReFilNewRes);

	/* 対RE状態#n(※1)をアイドルへ遷移 */
	gt_dlw_refw_3g_tbl[cpri_no - 1].re_sta = DLD_RESTA_IDLE;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_REFileNotUpdate_3g] RETURN" );
}


/* @} */
