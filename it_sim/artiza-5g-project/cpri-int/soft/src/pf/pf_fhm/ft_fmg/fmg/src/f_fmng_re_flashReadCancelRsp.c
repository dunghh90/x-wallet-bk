/*!
 * @skip  $ld:$
 * @file  f_fmng_re_flashReadCancelRsp.c
 * @brief Reception of RE-FLASH READ response (cancel) from FRWRE thread
 * @date  2015/08/11 TDIPS)sasaki create
 * @date  2015/08/19 TDIPS)sasaki 停止タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
 * @date  2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
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
* @brief	Reception of RE-FLASH READ response (cancel) from FRWRE thread
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki 停止タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/31 TDIPS)sasaki CT検出問題(中止応答送信のログが記録されない)の対処
* @warning	N/A
* @FeatureID	RRH-007-000yy
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_flashReadCancelRsp( VOID *pRcvMsg )
{
	T_API_FILE_FIRM_CANCEL_RSP	flashCancelRsp;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_flashReadCancelRsp] ENTER" );

	memset(&flashCancelRsp, 0, sizeof(flashCancelRsp));

	/* Stop RE-FLASH R/W timer	*/
	f_fmng_com_stopTimer(E_RRH_TIMID_RE_FLSRWSV);

	/****************************************************************************/
	/* Send FLASH-ACCESS Cancel Response										*/
	/****************************************************************************/
	flashCancelRsp.header.uiEventNo     = D_API_MSGID_FILE_FIRM_CANCEL_RSP;
	flashCancelRsp.header.uiSignalkind  = 0;
	flashCancelRsp.header.uiDstPQueueID = f_fmngw_respQid;
	flashCancelRsp.header.uiDstTQueueID = 0;
	flashCancelRsp.header.uiSrcPQueueID = D_RRH_PROCID_Q_PF;
	flashCancelRsp.header.uiSrcTQueueID = 0;
	flashCancelRsp.header.uiLength = sizeof(flashCancelRsp);
	flashCancelRsp.data.result = D_SYS_OK;

	f_com_sendMsgToAplFHM(
		flashCancelRsp.header.uiDstPQueueID,
		&flashCancelRsp,
		sizeof(flashCancelRsp),
		D_API_MSGID_FILE_FIRM_CANCEL_RSP,
		D_SYS_THDQID_PF_FRMG
    );

	/* save running history */
    f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&flashCancelRsp);

	memset((UCHAR *)&f_fmngw_flashWaitWrtReq_re, 0, sizeof(f_fmngw_flashWaitWrtReq_re));
	memset((UCHAR *)&f_fmngw_flashWaitEraReq_re, 0, sizeof(f_fmngw_flashWaitEraReq_re));

	/* Change thread state into IDLE	*/
	f_fmngw_read_cancel_re = D_SYS_OFF;
	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_re_apikind = D_FMNG_API_NONE;
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_IDLE;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_flashReadCancelRsp] RETURN to IDLE" );
	return;
}

/* @} */  
