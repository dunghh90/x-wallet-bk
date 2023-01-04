/*!
 * @skip   $Id$
 * @file   m_dl_fhm_finfrsp_waiting_fwinf_cnf_to.c
 * @brief  Event: ファームウェアファイル情報報告確認タイマタイムアウト通知 RE status 5 DLD_RESTA_FILEINF_WAIT
 * @date   2015/08/22 FPT)Khiemnv1 Create
 * @date   2015/10/19 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "m_rc_header.h"
/*!
 *  @brief  Event: ファームウェアファイル情報報告確認タイマタイムアウト通知 RE status 5 DLD_RESTA_FILEINF_WAIT
 *  @note   -------7.5.12---------------
			--1
			ref: m_dl_fl_fwinf_cnf_to.c
			--1.1
			"リトライ可能=>4
			else=>0"
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 */

VOID m_dl_fhm_finfrsp_waiting_fwinf_cnf_to(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_finfrsp_waiting_fwinf_cnf_to] ENTER" );

	//input
	USHORT lw_CPRI_no = cpri_no;
	//input msg ファームウェアファイル情報報告確認T.O.

	//REFW
	USHORT lw_resend_times = 0;
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);

	/*再送回数＜再送回数最大値
	 *resend times < max resend times
	 */
	lw_resend_times = get_resend_counter(lw_REFW_index,
		DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE);
	if (lw_resend_times < DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE_MAX)
	{
		/*再送回数ファイル情報報告通知]#nを加算
		 *inc resend times (file info report notice)
		 */
		set_resend_counter(lw_REFW_index,
			DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE,
			lw_resend_times + 1);

		/*do 7-5-5 ファイル情報報告通知(LTE)
		 *7.5.5 file info report notice
		 */
		m_dl_FileInfoReportNotice(lw_CPRI_no);

		return;
	}

	/*対RE-FW情報テーブルのファイル情報報告結果にタイムアウトを設定
	 *set timeout: REFW info table's file info report result
	 */
	set_report_result(lw_REFW_index,
		REPORT_RESULT_FILE_INFO,
		REPORT_RESULT_TIMEOUT);

	/*対RE-FW情報テーブルの使用中タイマを初期化
	 *init REFW info table's using timer
	 */
	set_REFW_using_timer(lw_REFW_index, 0);

	/*再送回数[ファイル情報報告通知]#n初期化
	 *init counter
	 */
	set_resend_counter(lw_REFW_index,
		DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE,
		0);

	/*更新開始要因#nを初期化
	 *init update start factor
	 */
	set_update_start_factor(lw_REFW_index, DLD_UPDATE_FACTOR_NONE);

	/*更新中リンク情報のT.O.発生CPRI#ビットOFF
	 *updating link info's T.O. occurence CPRI bit OFF
	 */
	 set_updating_link_info(lw_CPRI_no, 0);

	/*7-5-10 REファイル更新なし(パラメータ:NG)
	 *7.5.10 RE file update not
	 */
	m_dl_REFileNotUpdate(0, lw_CPRI_no, CMD_NG, CMD_OFF);
}
/* @} */
