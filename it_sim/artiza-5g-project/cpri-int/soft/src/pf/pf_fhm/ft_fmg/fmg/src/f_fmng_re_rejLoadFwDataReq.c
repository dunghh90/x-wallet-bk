/*!
 * @skip  $ld:$
 * @file  f_fmng_re_rejLoadFwDataReq.c
 * @brief Reject RE File Read Request
 * @date 2015/08/11 TDIPS)sasaki create
 * @date 2015/08/21 TDIPS)sasaki コメント修正
 * @date 2015/08/31 TDIPS)sasaki CT検出問題(REファイルデータ読込応答送信ログが記録されない)の対処
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Reject RE File Read Request
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/31 TDIPS)sasaki CT検出問題(REファイルデータ読込応答送信ログが記録されない)の対処
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_rejLoadFwDataReq( VOID *pRcvMsg )
{
	T_API_FILE_LOAD_FW_DATA_RSP	flashReadRsp;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_rejLoadFwDataReq] ENTER" );

	memset(&flashReadRsp, 0, sizeof(flashReadRsp));

	/****************************************************************************/
	/* Send FW File Read Response												*/
	/****************************************************************************/
    flashReadRsp.header.uiEventNo     = D_API_MSGID_FILE_FIRM_READ_RSP;
    flashReadRsp.header.uiSignalkind  = 0;
    flashReadRsp.header.uiDstPQueueID = ((T_API_FILE_LOAD_FW_DATA_REQ *)pRcvMsg)->header.uiSrcPQueueID;
    flashReadRsp.header.uiDstTQueueID = ((T_API_FILE_LOAD_FW_DATA_REQ *)pRcvMsg)->header.uiSrcTQueueID;
    flashReadRsp.header.uiSrcPQueueID = D_RRH_PROCID_Q_PF;
    flashReadRsp.header.uiSrcTQueueID = 0;
    flashReadRsp.header.uiLength = sizeof(flashReadRsp);
    flashReadRsp.data.result = D_SYS_NG;

    f_com_sendMsgToAplFHM(
		flashReadRsp.header.uiDstPQueueID,
		&flashReadRsp,
		sizeof(flashReadRsp),
		D_API_MSGID_FILE_FIRM_READ_RSP,
		D_SYS_THDQID_PF_FRMG
    );

	/* save running history */
    f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&flashReadRsp);

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_rejLoadFwDataReq] RETURN" );
	return;
}

/* @} */  
