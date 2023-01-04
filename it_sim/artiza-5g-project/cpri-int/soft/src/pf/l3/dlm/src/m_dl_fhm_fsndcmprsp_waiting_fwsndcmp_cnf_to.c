/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fsndcmprsp_waiting_fwsndcmp_cnf_to.c
 * @brief  Event: ファームウェア送信完了確認タイマタイムアウト通知 RE status 7 DLD_RESTA_FILE_COMP_WAIT
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
/*!
 *  @brief  Event: ファームウェア送信完了確認タイマタイムアウト通知 RE status 7 DLD_RESTA_FILE_COMP_WAIT
 *  @note   -------7.5.13------------------
			--1
			ref: m_dl_fl_fwsndcmp_cnf_to.c
			--1.1
			"リトライ可能=>6
			else=>0"
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/11 FPT)Duong fix bug IT1
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 */

VOID m_dl_fhm_fsndcmprsp_waiting_fwsndcmp_cnf_to(VOID* bufp, USHORT cpri_no)
{

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_fsndcmprsp_waiting_fwsndcmp_cnf_to] ENTER" );

	//input
	USHORT lw_CPRI_no = cpri_no;

	//REFW
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);
	USHORT lw_resend_times = 0;
	USHORT lw_is_last_part;

	/*再送回数＜再送回数最大値
	 *resend times < max resend times
	 */
	lw_resend_times = get_resend_counter(lw_REFW_index,
		DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE);
	if (lw_resend_times < DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE_MAX)
	{
		/*再送回数[ファイルデータ送信完了報告通知]#nを加算
		 *inc counter: file data send complete report notice
		 */
		set_resend_counter(lw_REFW_index,
			DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE,
			lw_resend_times + 1);

		/*対RE-FW情報テーブルの下記情報を初期化
		 *・送信済み分割番号#n
		 *・送信済みデータサイズ#n
		 *init RE FW info table's info
		 *+ sent partial no
		 *+ sent data size
		 */
		set_transfer_complete_part_no(lw_REFW_index, 0);
		set_transfer_complete_data_size(lw_REFW_index, 0);

		/*7-5-8 ファイルデータ送信(LTE)
		 *do 7.5.8 file data send
		 */
		m_dl_SendFileDataStart(lw_CPRI_no);
		//REFW status change to file data sending
		set_RE_status(lw_REFW_index, DLD_RESTA_FILE_TRANS);

		lw_is_last_part = m_dl_SendFileData(lw_CPRI_no);
		if(lw_is_last_part)
		{	/* ファイルデータ送信間隔タイマは、以下の関数内で停止している */
			m_dl_SendFileDataEnd(lw_CPRI_no);
		}

		return;
	}

	/*当該対RE-FW情報テーブルのファイルデータ送信完了報告結果にタイムアウトを設定
	 *set timeout: RE FW info table's file data send complete report result
	 *set time out
	 */
	set_report_result(lw_REFW_index,
		REPORT_RESULT_FILE_DATA_SEND_COMPLETE,
		REPORT_RESULT_TIMEOUT);

	/*当該対RE-FW情報テーブルの使用中タイマを初期化
	 *init RE FW table's using timer
	 */
	set_REFW_using_timer(lw_REFW_index, DLD_RERETIM_NONE);

	/*再送回数[ファイルデータ送信完了報告通知]#n初期化
	 *init counter: file data send complete report notice
	 */
	set_resend_counter(lw_REFW_index,
		DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE,
		0);

	/*更新開始要因を初期化
	 *init update start factor
	 */
	set_update_start_factor(lw_REFW_index, DLD_UPDATE_FACTOR_NONE);

	/*更新中リンク情報のT.O.発生CPRI#ビットOFF
	 *updating link info's T.O. occurence bit OFF
	 */
	set_updating_link_info(lw_CPRI_no, 0);

	/*7-5-10 REファイル更新なし(パラメータ:NG)
	 *do 7.5.10 RE file not update
	 */
	m_dl_REFileNotUpdate(0, lw_CPRI_no, CMD_NG, CMD_OFF);

	//REFW status change to idle
	set_RE_status(lw_REFW_index, DLD_RESTA_IDLE);
}
/* @} */
