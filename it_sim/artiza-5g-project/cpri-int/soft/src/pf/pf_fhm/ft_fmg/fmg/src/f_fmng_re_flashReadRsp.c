/*!
 * @skip  $ld:$
 * @file  f_fmng_re_flashReadRsp.c
 * @brief Reception of RE-FLASH READ response (complete) from FRWRE thread
 * @date  2015/08/11 TDIPS)sasaki create
 * @date  2015/08/19 TDIPS)sasaki 停止タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
 * @date  2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
 * @date  2015/08/21 TDIPS)sasaki API発行元への応答結果にFRWREスレッドからの応答結果を設定するよう変更
 * @date  2015/08/21 TDIPS)sasaki コメント修正
 * @date  2015/08/31 TDIPS)sasaki CT検出問題(REファイルデータ読込応答送信ログが記録されない)の対処
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Reception of RE-FLASH READ response (complete) from FRWRE thread
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki 停止タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/21 TDIPS)sasaki API発行元への応答結果にFRWREスレッドからの応答結果を設定するよう変更
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/31 TDIPS)sasaki CT検出問題(REファイルデータ読込応答送信ログが記録されない)の対処
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_flashReadRsp( VOID *pRcvMsg )
{
	T_API_FILE_LOAD_FW_DATA_RSP	flashReadRsp;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_flashReadRsp] ENTER" );

	memset(&flashReadRsp, 0, sizeof(flashReadRsp));

	/* Stop RE-FLASH R/W timer	*/
	f_fmng_com_stopTimer(E_RRH_TIMID_RE_FLSRWSV);

	/****************************************************************************/
	/* Send FW File Read Response												*/
	/****************************************************************************/
    flashReadRsp.header.uiEventNo     = D_API_MSGID_FILE_FIRM_READ_RSP;
    flashReadRsp.header.uiSignalkind  = 0;
    flashReadRsp.header.uiDstPQueueID = f_fmngw_respQid;
    flashReadRsp.header.uiDstTQueueID = 0;
    flashReadRsp.header.uiSrcPQueueID = D_RRH_PROCID_Q_PF;
    flashReadRsp.header.uiSrcTQueueID = 0;
    flashReadRsp.header.uiLength = sizeof(flashReadRsp);
	flashReadRsp.data.result = ((T_SYS_FLASH_READ_RSP *)pRcvMsg)->result;

    f_com_sendMsgToAplFHM(
		flashReadRsp.header.uiDstPQueueID,
		&flashReadRsp,
		sizeof(flashReadRsp),
		D_API_MSGID_FILE_FIRM_READ_RSP,
		D_SYS_THDQID_PF_FRMG
    );

	/* save running history */
    f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&flashReadRsp);

	memset((UCHAR *)&f_fmngw_flashWaitWrtReq_re, 0, sizeof(f_fmngw_flashWaitWrtReq_re));
	memset((UCHAR *)&f_fmngw_flashWaitEraReq_re, 0, sizeof(f_fmngw_flashWaitEraReq_re));

	/* Change thread state into IDLE	*/
	f_fmngw_read_cancel_re = D_SYS_OFF;
	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_re_apikind = D_FMNG_API_NONE;
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_IDLE;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_flashReadRsp] RETURN to IDLE" );
	return;
}

/* @} */  
