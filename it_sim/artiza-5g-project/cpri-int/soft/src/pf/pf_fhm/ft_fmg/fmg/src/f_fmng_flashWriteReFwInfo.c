/*!
 * @skip  $ld:$
 * @file  f_fmng_flashWriteReFwInfo.c
 * @brief Send RE File Info Update/Delete messege 
 * @date 2015/08/14 TDIPS)sasaki create
 * @date 2015/08/19 TDIPS)sasaki タイマ開始処理を削除
 * @date 2015/08/20 TDIPS)sasaki API種別保存処理を削除(RE-FLASH R/Wの延長で呼ばれる関数の為)
 * @date 2015/08/20 TDIPS)sasaki 状態遷移先をFW情報保存中に変更
 * @date 2015/08/21 TDIPS)sasaki RE-FLASH WRITE結果によりFWファイル情報更新/削除を切り替えるよう変更
 * @date 2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
 * @date 2015/08/28 TDIPS)sasaki CT検出問題(FWファイル保存要求送信のログが記録されない)の対処
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"

/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Accept RE File Info Update Request for f_fmngr_matrix
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/14 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki タイマ開始処理を削除
* @date 	2015/08/20 TDIPS)sasaki API種別保存処理を削除(RE-FLASH R/Wの延長で呼ばれる関数の為)
* @date 	2015/08/20 TDIPS)sasaki 状態遷移先をFW情報保存中に変更
* @date 	2015/08/21 TDIPS)sasaki RE-FLASH WRITE結果によりFWファイル情報更新/削除を切り替えるよう変更
* @date 	2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
* @date 	2015/08/28 TDIPS)sasaki CT検出問題(FWファイル保存要求送信のログが記録されない)の対処
* @date 	2015/08/28 TDIPS)sasaki return時のログ出力内容の誤りを修正
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_flashUpdReFwInfo( VOID *pRcvMsg )
{
	T_SYS_FWINFO_SAVE_REQ		flashFwInfoSaveReq;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_flashUpdReFwInfo] ENTER" );				

	memset(&flashFwInfoSaveReq, 0, sizeof(flashFwInfoSaveReq));

	/****************************************************************************/
	/* Send FW Info Save Request to FRW thread									*/
	/****************************************************************************/
	flashFwInfoSaveReq.thdIf.uiEventNo = D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ;
	flashFwInfoSaveReq.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
	flashFwInfoSaveReq.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQ;
	flashFwInfoSaveReq.thdIf.uiLength = sizeof(flashFwInfoSaveReq);
	flashFwInfoSaveReq.savepos = f_fmngw_save_fw_reqdata.save_pos;

	/****************************************************************************/
	/* RE-FLASH WRITEがOKならFWファイル情報更新									*/
	/* RE-FLASH WRITEがNGならFWファイル情報削除									*/
	/****************************************************************************/
	if(((T_SYS_LMCWRT_RSP *)pRcvMsg)->result == D_SYS_OK)
	{
		flashFwInfoSaveReq.fwinfo = f_fmngw_save_fw_reqdata.fw_info;
	}

	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQ,
					(CHAR*)&flashFwInfoSaveReq,
					sizeof(flashFwInfoSaveReq), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_thrdState, 
					D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ );

	/* save running history */
    f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&flashFwInfoSaveReq);

	/* Change thread state into FW Info Saving	*/
	f_fmngw_thrdState = D_FMNG_THRDSTA_FWINFO_SAVE;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashUpdReFwInfo:state->%d] RETURN", f_fmngw_thrdState);
	return;
}

/* @} */  

/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Accept RE File Delete Request for f_fmngr_matrix
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki タイマ開始処理を削除
* @date 	2015/08/20 TDIPS)sasaki API種別保存処理を削除(RE-FLASH R/Wの延長で呼ばれる関数の為)
* @date 	2015/08/20 TDIPS)sasaki 状態遷移先をFW情報保存中に変更
* @date 	2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
* @date 	2015/08/28 TDIPS)sasaki CT検出問題(FWファイル保存要求送信のログが記録されない)の対処
* @date 	2015/08/28 TDIPS)sasaki return時のログ出力内容の誤りを修正
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_flashDelReFwInfo( VOID *pRcvMsg )
{
	T_SYS_FWINFO_SAVE_REQ		flashFwInfoSaveReq;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_flashDelReFwInfo] ENTER" );				

	memset(&flashFwInfoSaveReq, 0, sizeof(flashFwInfoSaveReq));

	/****************************************************************************/
	/* Send FW Info Save Request to FRW thread (don't edit fwinfo)				*/	
	/****************************************************************************/
	flashFwInfoSaveReq.thdIf.uiEventNo = D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ;
	flashFwInfoSaveReq.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
	flashFwInfoSaveReq.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQ;
	flashFwInfoSaveReq.thdIf.uiLength = sizeof(flashFwInfoSaveReq);
	flashFwInfoSaveReq.savepos = f_fmngw_save_fw_reqdata.save_pos;

	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQ,
					(CHAR*)&flashFwInfoSaveReq,
					sizeof(flashFwInfoSaveReq), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_thrdState, 
					D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ );

	/* save running history */
    f_fmng_cm_RunHisSet(D_FMNG_RUNHIS_FLG_SND, (UCHAR *)&flashFwInfoSaveReq);

	/* Change thread state into FW Info Saving	*/
	f_fmngw_thrdState = D_FMNG_THRDSTA_FWINFO_SAVE;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashDelReFwInfo:state->%d] RETURN", f_fmngw_thrdState);
	return;
}

/* @} */  
