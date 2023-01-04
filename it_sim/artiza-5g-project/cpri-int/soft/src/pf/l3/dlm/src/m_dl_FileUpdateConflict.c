#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */


/*!
 *  @brief  増設REダウンロード中の運用中ファイル更新(LTE)
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/10/14 TDIPS)sasaki Update MKレビュー指摘No.296対応
 */
//ref: VOID m_dl_CRUSFirmFileReqRcv(VOID *a_rcvmsg_p)
VOID m_dl_UpdateExpandREDowloadingFile(VOID* bufp, USHORT cpri_no)
{
	//input
	USHORT lw_CPRI_no = cpri_no;
	//+ CPRI信号受信通知(運用中ファームウェアファイル報告要求)
	//  CMD_CPRID_UFIRMFILEREQ	0x2410
	//CMT_TSKIF_CPRIRCV_UFIRMFILEREQ* lt_using_FW_file_report_request_p = (CMT_TSKIF_CPRIRCV_UFIRMFILEREQ*)bufp;


	//REFW
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);
	//USHORT lw_using_timer = 0;
	USHORT lw_update_start_factor = 0;

	//msg REファイル更新応答(CMT_TSKIF_REFILNEWRES)
	CMT_TSKIF_REFILNEWRES lt_RE_file_update_respond;
	memset(&lt_RE_file_update_respond, 0, sizeof(lt_RE_file_update_respond));

	/*+ 更新開始要因#nがRE起動(REC接続済)(=2)以外*/
	lw_update_start_factor = get_update_start_factor(lw_REFW_index);
	/*  if (RE not start) check next RE*/
	/*  if (RE started) check this RE*/
	if (lw_update_start_factor != DLD_UPDATE_FACTOR_RESTART_REC_CON)
	{
		return;
	}
//TBD
//	/*+ 対RE-FW情報の使用中タイマチェック*/
//	/*  check timer*/
//	lw_using_timer = get_REFW_using_timer(lw_REFW_index);
//
//	/*+ ファームウェアファイル報告確認(更新用)タイマ*/
//	/*  ファームウェアファイル 報告確認(更新用) タイマ#n停止*/
//	/*+ ファームウェアファイル情報報告確認(更新用)タイマ*/
//	/*  タイマ#n停止*/
//	/*+ ファームウェア送信完了確認(更新用)タイマ*/
//	/*  タイマ#n停止*/
//
//	/*+ case report timer*/
//	/*+ case report info timer*/
//	/*+ case send finish timer*/
//	switch(lw_using_timer) {
//			case DLD_RERETIM_REPRT_UPDATE:
//				m_dl_StopTimer(DLD_RERETIM_REPRT_UPDATE, lw_CPRI_no);
//				break;
//			case DLD_RERETIM_INFREPRT_UPDATE:
//				m_dl_StopTimer(DLD_RERETIM_INFREPRT_UPDATE, lw_CPRI_no);
//				break;
//			case DLD_RERETIM_SNDCMP_UPDATE:
//				m_dl_StopTimer(DLD_RERETIM_SNDCMP_UPDATE, lw_CPRI_no);
//				break;
//			default:
//				break;
//	}

	/*+ REファイル更新応答(結果(NG),更新なし)
	*  respond to RE
	*/
	lt_RE_file_update_respond.head.uiEventNo = CMD_TSKIF_REFIRM_DL_RSP;
	lt_RE_file_update_respond.cpri_no = lw_CPRI_no;
	lt_RE_file_update_respond.result_code = CMD_NG;
	lt_RE_file_update_respond.dummy = ((CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;
	send_msg_to_RE(MSG_RE_FILE_UPDATE_RESPOND, &lt_RE_file_update_respond);

	/*+ 対RE-FW情報#n初期化*/
	/*  init RE-FW info*/
	init_REFW(lw_REFW_index);
	set_RE_status(lw_REFW_index, DLD_RESTA_IDLE);

	return; 
}
