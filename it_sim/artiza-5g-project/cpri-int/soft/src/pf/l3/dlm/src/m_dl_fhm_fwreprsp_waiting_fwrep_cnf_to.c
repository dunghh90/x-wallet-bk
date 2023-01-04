/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to.c
 * @brief  Event: ファームウェアファイル報告確認タイマタイムアウト通知 RE status 3 DLD_RESTA_FWREP_WAIT
 * @date   2015/08/22 FPT)Khiemnv1 Create
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
 *  @brief  Event: ファームウェアファイル報告確認タイマタイムアウト通知 RE status 3 DLD_RESTA_FWREP_WAIT
 *  @note   -------7.5.11--------------
			--1
			ref: m_dl_fl_fwrep_cnf_to.c
			--1.1
			"リトライ可能=>3
			else=>0"
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @Bug_No M-RRU-ZSYS-01816
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/08 FPT)Duong update (MKレビュー指摘No.243対応)
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 *  @date   2015/10/20 TDIPS)sasaki M-RRU-ZSYS-01816 Update IT2問処No.118対処
 */
VOID m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to] ENTER" );

	//input
	USHORT lw_CPRI_no = cpri_no;

	//msg (0x4001)RE Forwarding Notice(ファームウェアファイル報告要求)
	CMT_TSKIF_CPRIRCV_FIRMFILEREQ lt_FW_file_report_verify;
	memset(&lt_FW_file_report_verify, 0, sizeof(lt_FW_file_report_verify));
	//REFW
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(cpri_no);
	USHORT lw_resend_times = 0;

	USHORT			lwSysType;

	lwSysType = (( CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;
	
	if(CMD_SYS_3G == lwSysType)
	{
		m_dl_fhm_fwreprsp_waiting_fwrep_cnf_to_3g(cpri_no);
		return;
	}
	/*再送回数＜再送回数最大値
	 *resend times < max resend times
	 */
	lw_resend_times = get_resend_counter(lw_REFW_index,
			DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ);
	if (lw_resend_times < DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ_MAX)
	{
		/*再送回数[ファームウェアファイル報告要求]#nを加算
		 *inc resend counter
		 */
		set_resend_counter(lw_REFW_index,
			DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ,
			lw_resend_times + 1);

		/*ファームウェアファイル報告確認タイマ#n(※1)開始
		 *start FW file report verify timer
		 */
		m_dl_StartTimer(CMD_TIMID_FIRMFILE_REP, lw_CPRI_no);

		/*(0x4001)RE Forwarding Notice(ファームウェアファイル報告要求)
		 *FW file report request
		 * (LTE)(0x2111)ファームウェアファイル報告要求
		 * CPRI信号受信通知(ファームウェアファイル報告要求)
		 * CMT_TSKIF_CPRIRCV_FIRMFILEREQ
		 */
		lt_FW_file_report_verify.cprircv_inf.link_num = lw_CPRI_no;
		lt_FW_file_report_verify.cpridat_firmfilereq.signal_kind = CMD_CPRID_FIRMFILEREQ + CMD_NUM1;
		lt_FW_file_report_verify.head.uiSignalkind = CMD_CPRID_FIRMFILEREQ + CMD_NUM1;
		RE_forwarding_notice(MSG_FW_FILE_REPORT_REQ, &lt_FW_file_report_verify);

		/* ファームウェアファイル報告応答結果を初期化 */
		gt_dlw_refw_tbl[cpri_no-1].fw_rep_rslt = DLD_RESLT_IDLE;
		return;
	}

	/*当該対RE-FW情報テーブルのファームウェアファイル報告結果にタイムアウトを設定
	*in RE FW info table's FW file report result
	*set time out
	*/
	set_report_result(lw_REFW_index, REPORT_RESULT_FW_FILE, REPORT_RESULT_TIMEOUT);

	/*当該対RE-FW情報テーブルの使用中タイマを初期化
	*init RE FW table's using timer
	*/
	set_REFW_using_timer(lw_REFW_index, 0);

	/*再送回数[ファームウェアファイル報告要求]#n初期化
	*init send counter(FW file report request)
	*/
	set_resend_counter(lw_REFW_index,
		DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ,
		0);

	/*更新開始要因#nを初期化
	*init update start factor
	*/
	set_update_start_factor(lw_REFW_index, DLD_UPDATE_FACTOR_NONE);

	/*更新中リンク情報のT.O.発生CPRI#ビットOFF
	*updating link info's T.O. occur bit OFF
	*/
	set_updating_link_info(lw_CPRI_no, 0);

	/*7-5-10 REファイル更新なし(パラメータ:NG)
	*do 7.5.10 RE file not update
	*/
	m_dl_REFileNotUpdate(0, lw_CPRI_no, CMD_NG, CMD_OFF);

	return;
}

/* @} */
