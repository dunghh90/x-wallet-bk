/*!
 * @skip  $ld:$
 * @file  f_fmng_re_LoadFwDataReq.c
 * @brief Accept RE File Read Request
 * @date  2015/08/11 TDIPS)sasaki create
 * @date  2015/08/19 TDIPS)sasaki 開始タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
 * @date  2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
 * @date  2015/08/20 TDIPS)sasaki 読込中止フラグOFF処理を追加
 * @date  2015/08/20 TDIPS)sasaki REファイルデータ読込応答(NG)送信処理を追加
 * @date  2015/08/20 TDIPS)sasaki FWファイル更新中にREファイルデータ読込要求を受けた場合の復帰処理を追加
 * @date  2015/08/20 TDIPS)sasaki 面番号、オフセット設定値変更(RE-FLASHアクセス用BPFのI/Fに合わせる)
 * @date  2015/08/21 TDIPS)sasaki コメント修正
 * @date  2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
 * @date  2015/09/01 TDIPS)sasaki CT検出問題(NG応答送信のログが記録されない)の対処
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Accept RE File Read Request
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki 開始タイマのIDを変更(RE-FLASH READ/WRITEタイマ)
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/20 TDIPS)sasaki 読込中止フラグOFF処理を追加
* @date 	2015/08/20 TDIPS)sasaki REファイルデータ読込応答(NG)送信処理を追加
* @date 	2015/08/20 TDIPS)sasaki FWファイル更新中にREファイルデータ読込要求を受けた場合の復帰処理を追加
* @date 	2015/08/20 TDIPS)sasaki 面番号、オフセット設定値変更(RE-FLASHアクセス用BPFのI/Fに合わせる)
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
* @date 	2015/09/01 TDIPS)sasaki CT検出問題(NG応答送信のログが記録されない)の対処
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_LoadFwDataReq( VOID *pRcvMsg )
{
	T_API_FILE_LOAD_FW_DATA_RSP	flashReadRsp;
	T_SYS_FLASH_READ_REQ		flashReadReq;
	T_RRH_FW_INFO				fwInfo;
	INT							ret = D_SYS_NUM0;
	UINT						a_face;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_LoadFwDataReq] ENTER" );				

	if(f_fmngw_fw_type != D_API_FILE_FW_TYPE_NONE)
	{
		/************************************************************************/
		/* REC主導ダウンロード中にRE増設処理は走らない為、FWファイル更新中に	*/
		/* DLMスレッドがREファイルデータ読込要求を送信することは有り得ない		*/
		/************************************************************************/
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "Processing any FW File RETURN");
		return;
	}

	memset(&flashReadRsp, 0, sizeof(flashReadRsp));
	memset(&flashReadReq, 0, sizeof(flashReadReq));

	a_face = ((T_API_FILE_LOAD_FW_DATA_REQ *)pRcvMsg)->data.save_pos - 1;

	/* Save Response QId  */
	f_fmngw_respQid = ((T_API_FILE_LOAD_FW_DATA_REQ *)pRcvMsg)->header.uiSrcPQueueID;

	/* 読込位置のREファイル情報を共有メモリから取得		*/
	ret = f_cmn_com_fw_info_re_get(a_face, &fwInfo);
	if( ret != D_RRH_OK )
	{
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

		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_cmn_com_fw_info_re_get(%d) NG", a_face);
		return;
	}

	/****************************************************************************/
	/* Send READ Request to FRWRE thread										*/	
	/****************************************************************************/
	flashReadReq.thdIf.uiEventNo = D_SYS_MSGID_FLASH_RE_READ_REQ;
	flashReadReq.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
	flashReadReq.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQRE;
	flashReadReq.thdIf.uiLength = sizeof(flashReadReq);
	flashReadReq.face = a_face; 
	flashReadReq.offset = 0;
	flashReadReq.size = fwInfo.file_size;

	/****************************************************************************/
	/* FRWREからのREAD応答(正常に全データREAD完了)とDLMからのキャンセル要求が	*/
	/* すれ違うと読込中止フラグがONのままとなる為、ここでOFFにしておく。		*/
	/****************************************************************************/
	f_fmngw_read_cancel_re = D_SYS_OFF;

	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQRE,
					(CHAR*)&flashReadReq,
					sizeof(flashReadReq),
					D_SYS_THDQID_PF_FRMG,
					f_fmngw_re_thrdState,
					D_SYS_MSGID_FLASH_RE_READ_REQ );

	/* Start RE-FLASH R/W timer	*/
	f_fmng_com_startTimer(E_RRH_TIMID_RE_FLSRWSV, D_FMNG_TIMEVAL_RE_FLSRW, D_SYS_MSGID_RE_FLASH_TIMEOUT_NTC);

	/* Set received api kind	*/
	f_fmngw_re_apikind = D_FMNG_API_RD;

	/* Change thread state into RE-FLASH READ/WRITE		*/
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_FLSHRW;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_LoadFwDataReq] RETURN to FLSHRW" );
	return;
}

/* @} */  
