/*!
 * @skip  $ld:$
 * @file  f_fmng_re_flashSaveReFwInfoRsp.c
 * @brief 
 * @date  2015/08/14 TDIPS)sasaki create
 * @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 * @date  2015/08/19 TDIPS)sasaki 停止タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
 * @date  2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
 * @date  2015/08/21 TDIPS)sasaki REファイル情報の保存結果とRE-FLASH WRITE/ERASE応答結果を併せてDLMへの応答結果に設定するよう変更
 * @date  2015/08/21 TDIPS)sasaki コメント修正
 * @date  2015/08/31 TDIPS)sasaki CT検出問題(中止応答送信のログが記録されない)の対処
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"

/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Reception of RE File Info WRITE Response
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/14 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
* @date 	2015/08/19 TDIPS)sasaki 停止タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/21 TDIPS)sasaki 保存されているREファイル情報保存結果とRE-FLASH WRITE/ERASE応答結果を併せてDLMへの応答結果を設定するよう変更
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/31 TDIPS)sasaki CT検出問題(中止応答送信のログが記録されない)の対処
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_flashSaveReFwInfoRsp(VOID *pRcvMsg)
{
	T_API_FILE_FIRM_CANCEL_RSP	flashCancelRsp;
	UINT						result;
	
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_flashSaveReFwInfoRsp] ENTER" );

	result = ((T_SYS_FWINFO_SAVE_RSP *)pRcvMsg)->result;
	if(f_fmngw_re_fileUpdResult != D_SYS_OK)
	{
		result = D_SYS_NG;
	}

	/* Stop RE-FLASH R/W timer	*/
	f_fmng_com_stopTimer(E_RRH_TIMID_RE_FLSRWSV);

	switch(f_fmngw_rsp_kind)
	{
		case D_FMNG_RSP_SAVE:
			if(result != D_SYS_OK)
			{
				f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR, "FW Info Save NG");
			}
			else
			{
				f_fmng_com_sndLoadUpdRsp(result, "FW Info Save OK");
			}
			break;
		case D_FMNG_RSP_CANCEL:
			memset(&flashCancelRsp, 0, sizeof(flashCancelRsp));
			flashCancelRsp.header.uiEventNo     = D_API_MSGID_FILE_FIRM_CANCEL_RSP;
			flashCancelRsp.header.uiSignalkind  = 0;
			flashCancelRsp.header.uiDstPQueueID = ((T_API_FILE_FIRM_CANCEL_IND *)pRcvMsg)->header.uiSrcPQueueID;
			flashCancelRsp.header.uiDstTQueueID = ((T_API_FILE_FIRM_CANCEL_IND *)pRcvMsg)->header.uiSrcTQueueID;
			flashCancelRsp.header.uiSrcPQueueID = D_RRH_PROCID_Q_PF;
			flashCancelRsp.header.uiSrcTQueueID = 0;
			flashCancelRsp.header.uiLength = sizeof(flashCancelRsp);
			flashCancelRsp.data.result = result;

			f_com_sendMsgToAplFHM(
				flashCancelRsp.header.uiDstPQueueID,
				&flashCancelRsp,
				sizeof(flashCancelRsp),
				D_API_MSGID_FILE_FIRM_CANCEL_RSP,
				D_SYS_THDQID_PF_FRMG
		    );

			/* save running history */
			f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&flashCancelRsp);
			break;
		default:
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "No request processing" );
			break;
	}

	memset((UCHAR *)&f_fmngw_flashWaitWrtReq_re, 0, sizeof(f_fmngw_flashWaitWrtReq_re));
	memset((UCHAR *)&f_fmngw_flashWaitEraReq_re, 0, sizeof(f_fmngw_flashWaitEraReq_re));
	memset(&f_fmngw_save_fw_reqdata, 0, sizeof(f_fmngw_save_fw_reqdata));

	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_rsp_kind = D_FMNG_RSP_NONE;
	f_fmngw_re_apikind = D_FMNG_API_NONE;
	f_fmngw_re_fileUpdResult = D_SYS_OK;
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_IDLE;
	
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_flashSaveReFwInfoRsp] RETURN to IDLE" );
	return;
}

/* @} */  
