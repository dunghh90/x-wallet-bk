/*!
 * @skip  $ld:$
 * @file  f_fmng_re_CancelSaveFwInfo.c
 * @brief Accept RE-FLASH READ/WRITE Cancel
 * @date  2015/08/14 TDIPS)sasaki create
 * @date  2015/08/20 TDIPS)sasaki 面番号、オフセット設定値変更(RE-FLASHアクセス用BPFのI/Fに合わせる)
 * @date  2015/08/21 TDIPS)sasaki コメント修正
 * @date  2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
 * @date  2015/08/31 TDIPS)sasaki 処理中要求が無い場合のアサートログを追加
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Accept RE-FLASH READ/WRITE Cancel at FW Info Saving
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/14 TDIPS)sasaki create
* @date 	2015/08/20 TDIPS)sasaki 面番号、オフセット設定値変更(RE-FLASHアクセス用BPFのI/Fに合わせる)
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
* @date 	2015/08/31 TDIPS)sasaki 処理中要求が無い場合のアサートログを追加
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_CancelSaveFwInfo( VOID *pRcvMsg )
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_CancelSaveFwInfo] ENTER" );				

	if(f_fmngw_fw_type == D_API_FILE_FW_TYPE_FHM)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "Processing FHM File RETURN");
		return;
	}

	/* Save Response QId  */
	f_fmngw_respQid = ((T_API_FILE_FIRM_CANCEL_IND *)pRcvMsg)->header.uiSrcPQueueID;

	switch(f_fmngw_rsp_kind)
	{
		case D_FMNG_RSP_SAVE:
			/****************************************************************************/
			/* Save ERASE Request to FRWRE thread										*/
			/****************************************************************************/
			f_fmngw_flashWaitEraReq_re.thdIf.uiEventNo = D_SYS_MSGID_FLASH_RE_ERASE_REQ;
			f_fmngw_flashWaitEraReq_re.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
			f_fmngw_flashWaitEraReq_re.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQRE;
			f_fmngw_flashWaitEraReq_re.thdIf.uiLength = sizeof(f_fmngw_flashWaitEraReq_re);
			f_fmngw_flashWaitEraReq_re.clearFileNum = 1;
			f_fmngw_flashWaitEraReq_re.clearFlashRom[0].face = f_fmngw_save_fw_reqdata.save_pos - 1;
			f_fmngw_flashWaitEraReq_re.clearFlashRom[0].offset = 0;
			f_fmngw_flashWaitEraReq_re.clearFlashRom[0].size = f_fmngw_save_fw_reqdata.fw_info.file_size;
			f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_FLSHRWCANCEL;
			break;
		case D_FMNG_RSP_CANCEL:
			break;
		default:
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "No request processing" );
			break;
	}

	/* Save Response Kind  */
	f_fmngw_rsp_kind = D_FMNG_RSP_CANCEL;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_CancelSaveFwInfo] RETURN" );
	return;
}

/* @} */  
