/*!
 * @skip  $ld:$
 * @file  f_fmng_re_SaveFwDataReq.c
 * @brief Accept RE File Save Request
 * @date  2015/08/11 TDIPS)sasaki create
 * @date  2015/08/19 TDIPS)sasaki 開始タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
 * @date  2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
 * @date  2015/08/20 TDIPS)sasaki 面番号、オフセット設定値変更(RE-FLASHアクセス用BPFのI/Fに合わせる)
 * @date  2015/08/21 TDIPS)sasaki コメント修正
 * @date  2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Accept RE File Save Request
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki 開始タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/20 TDIPS)sasaki 面番号、オフセット設定値変更(RE-FLASHアクセス用BPFのI/Fに合わせる)
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
* @warning	N/A
* @FeatureID	RRH-007-000

* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_SaveFwDataReq( VOID *pRcvMsg )
{
	T_SYS_RCVDATA_MKHEAD_S3G*	p_rcvdata = (T_SYS_RCVDATA_MKHEAD_S3G *)(f_cmw_rcvfirm_tbl + D_SYS_NUM32);
	T_SYS_FLSERASE_REQ			flashEraseReq;
	T_RRH_FW_INFO*				p_fwinfo;
	UINT						a_face;
	
	T_SYS_FWINFO_SAVE_REQ		flashFwInfoSaveReq;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_SaveFwDataReq] ENTER" );				

	if(f_fmngw_fw_type == D_API_FILE_FW_TYPE_FHM)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "Processing FHM File RETURN");
		return;
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RE file UPDATE Start");

	memset(&flashEraseReq, 0, sizeof(flashEraseReq));
	f_fmngw_fw_type = ((T_API_FILE_SAVE_FW_REQ *)pRcvMsg)->data.fw_type;

	a_face = ((T_API_FILE_SAVE_FW_REQ *)pRcvMsg)->data.save_pos - 1;
	p_fwinfo = &((T_API_FILE_SAVE_FW_REQ *)pRcvMsg)->data.fw_info;

	/* Save Response QId  */
	f_fmngw_respQid = ((T_API_FILE_SAVE_FW_REQ *)pRcvMsg)->header.uiSrcPQueueID;
	/* Save Response Kind  */
	f_fmngw_rsp_kind = D_FMNG_RSP_SAVE;

	memset(&flashFwInfoSaveReq, 0, sizeof(flashFwInfoSaveReq));

	/****************************************************************************/
	/* Send FW Info Save Request to FRW thread									*/
	/****************************************************************************/
	flashFwInfoSaveReq.thdIf.uiEventNo = D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ;
	flashFwInfoSaveReq.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
	flashFwInfoSaveReq.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQ;
	flashFwInfoSaveReq.thdIf.uiLength = sizeof(flashFwInfoSaveReq);
	flashFwInfoSaveReq.savepos = ((T_API_FILE_SAVE_FW_REQ *)pRcvMsg)->data.save_pos;

	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQ,
					(CHAR*)&flashFwInfoSaveReq,
					sizeof(flashFwInfoSaveReq), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_re_thrdState, 
					D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ );

	/* save running history */
    f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&flashFwInfoSaveReq);


	/****************************************************************************/
	/* Save WRITE Request to FRWRE thread										*/
	/****************************************************************************/
	f_fmngw_flashWaitWrtReq_re.thdIf.uiEventNo = D_SYS_MSGID_FLASH_RE_WRITE_REQ;
	f_fmngw_flashWaitWrtReq_re.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
	f_fmngw_flashWaitWrtReq_re.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQRE;
	f_fmngw_flashWaitWrtReq_re.thdIf.uiLength = sizeof(f_fmngw_flashWaitWrtReq_re);
	f_fmngw_flashWaitWrtReq_re.writeFileNum = 1;
	f_fmngw_flashWaitWrtReq_re.writeFlashRom[0].face = a_face;
	f_fmngw_flashWaitWrtReq_re.writeFlashRom[0].offset = 0;
	f_fmngw_flashWaitWrtReq_re.writeFlashRom[0].data = (UCHAR *)p_rcvdata;
	f_fmngw_flashWaitWrtReq_re.writeFlashRom[0].size = p_fwinfo->file_size;
	f_fmngw_flashWaitWrtReq_re.writeFlashRom[0].fileName[0] = '\0';

	/****************************************************************************/
	/* Send ERASE Request to FRWRE thread										*/	
	/****************************************************************************/
	flashEraseReq.thdIf.uiEventNo = D_SYS_MSGID_FLASH_RE_ERASE_REQ;
	flashEraseReq.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
	flashEraseReq.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQRE;
	flashEraseReq.thdIf.uiLength = sizeof(flashEraseReq);
	flashEraseReq.clearFileNum = 1;
	flashEraseReq.clearFlashRom[0].face = a_face; 
	flashEraseReq.clearFlashRom[0].offset = 0;
	flashEraseReq.clearFlashRom[0].size = p_fwinfo->file_size;

	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQRE,
					(CHAR*)&flashEraseReq,
					sizeof(flashEraseReq), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_re_thrdState, 
					D_SYS_MSGID_FLASH_RE_ERASE_REQ );

	/****************************************************************************/
	/* Save Processing RE File Info												*/
	/****************************************************************************/
	f_fmngw_save_fw_reqdata = ((T_API_FILE_SAVE_FW_REQ *)pRcvMsg)->data;

	/* Start RE-FLASH R/W timer	*/
	f_fmng_com_startTimer(E_RRH_TIMID_RE_FLSRWSV, D_FMNG_TIMEVAL_RE_FLSRW, D_SYS_MSGID_RE_FLASH_TIMEOUT_NTC);

	/* Set received api kind	*/
	f_fmngw_re_apikind = D_FMNG_API_ERA;

	/* Change thread state into RE-FLASH WRITE Waiting	*/
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_FLSHRWWAIT;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RE file UPDATE End");
	
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_SaveFwDataReq] RETURN to FLSHRW WAIT" );
	return;
}

/* @} */  
