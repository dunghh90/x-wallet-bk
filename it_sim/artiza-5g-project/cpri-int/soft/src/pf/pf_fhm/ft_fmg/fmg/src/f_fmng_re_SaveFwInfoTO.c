/*!
 * @skip  $ld:$
 * @file  f_fmng_re_SaveFwInfoTO.c
 * @brief Reception of RE-FLASH access timer out
 * @date 2015/08/14 TDIPS)sasaki create
 * @date 2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 * @date 2015/08/19 TDIPS)sasaki MKレビュー指摘事項No.64対応
 * @date 2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
 * @date 2015/08/21 TDIPS)sasaki コメント修正
 * @date 2015/08/31 TDIPS)sasaki CT検出問題(中止応答送信のログが記録されない)の対処
 * @date 2015/09/01 TDIPS)sasaki CT検出問題(中止応答のNGコード不正)の対処
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Reception of RE-FLASH access timer out at FW Info Saving
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/14 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
* @date 	2015/08/19 TDIPS)sasaki MKレビュー指摘事項No.64対応
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/31 TDIPS)sasaki CT検出問題(中止応答送信のログが記録されない)の対処
* @date 	2015/09/01 TDIPS)sasaki CT検出問題(中止応答のNGコード不正)の対処
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_SaveFwInfoTO( VOID *pRcvMsg )
{
	T_API_FILE_FIRM_CANCEL_RSP	flashCancelRsp;
	
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_SaveFwInfoTO] ENTER" );

	switch(f_fmngw_rsp_kind)
	{
		case D_FMNG_RSP_SAVE:
			f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR, "FW Info Save Timeout");
			break;
		case D_FMNG_RSP_CANCEL:
			memset(&flashCancelRsp, 0, sizeof(flashCancelRsp));
			flashCancelRsp.header.uiEventNo     = D_API_MSGID_FILE_FIRM_CANCEL_RSP;
			flashCancelRsp.header.uiSignalkind  = 0;
			flashCancelRsp.header.uiDstPQueueID = f_fmngw_respQid;
			flashCancelRsp.header.uiDstTQueueID = 0;
			flashCancelRsp.header.uiSrcPQueueID = D_RRH_PROCID_Q_PF;
			flashCancelRsp.header.uiSrcTQueueID = 0;
			flashCancelRsp.header.uiLength = sizeof(flashCancelRsp);
			flashCancelRsp.data.result = D_SYS_NG;

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
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_IDLE;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_SaveFwInfoTO] RETURN to IDLE" );
	return;
}

/* @} */  
