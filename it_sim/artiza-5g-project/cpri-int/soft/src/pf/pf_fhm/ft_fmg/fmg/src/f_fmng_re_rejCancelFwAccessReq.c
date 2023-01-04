/*!
 * @skip  $ld:$
 * @file  f_fmng_re_rejCancelFwAccessReq.c
 * @brief Reject RE-FLASH READ/WRITE Cancel
 * @date 2015/08/11 TDIPS)sasaki create
 * @date 2015/08/21 TDIPS)sasaki コメント修正
 * @date 2015/08/31 TDIPS)sasaki CT検出問題(中止応答送信のログが記録されない)の対処
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Reject RE-FLASH READ/WRITE Cancel
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/31 TDIPS)sasaki CT検出問題(中止応答送信のログが記録されない)の対処
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_rejCancelFwAccessReq( VOID *pRcvMsg )
{
	T_API_FILE_FIRM_CANCEL_RSP	flashCancelRsp;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_rejCancelFwAccessReq] ENTER");

	if(f_fmngw_fw_type == D_API_FILE_FW_TYPE_FHM)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "Processing FHM File RETURN");
		return;
	}

	memset(&flashCancelRsp, 0, sizeof(flashCancelRsp));

	/****************************************************************************/
	/* Send FLASH-ACCESS Cancel Response										*/
	/****************************************************************************/
	flashCancelRsp.header.uiEventNo     = D_API_MSGID_FILE_FIRM_CANCEL_RSP;
	flashCancelRsp.header.uiSignalkind  = 0;
	flashCancelRsp.header.uiDstPQueueID = ((T_API_FILE_FIRM_CANCEL_IND *)pRcvMsg)->header.uiSrcPQueueID;
	flashCancelRsp.header.uiDstTQueueID = ((T_API_FILE_FIRM_CANCEL_IND *)pRcvMsg)->header.uiSrcTQueueID;
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

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_rejCancelFwAccessReq] RETURN" );
	return;
}

/* @} */  
