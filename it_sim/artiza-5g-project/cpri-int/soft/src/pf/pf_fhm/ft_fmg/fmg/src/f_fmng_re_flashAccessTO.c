/*!
 * @skip  $ld:$
 * @file  f_fmng_re_flashAccessTO.c
 * @brief Reception of RE-FLASH access timer out at READ/WRITE
 * @date  2015/08/14 TDIPS)sasaki create
 * @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 * @date  2015/08/19 TDIPS)sasaki MKレビュー指摘事項No.63対応
 * @date  2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
 * @date  2015/08/21 TDIPS)sasaki コメント修正
 * @date  2015/08/31 TDIPS)sasaki CT検出問題(REファイルデータ読込応答送信ログが記録されない)の水平展開
 * @date  2015/09/01 TDIPS)sasaki CT検出問題(REファイルデータ読込応答のNGコード不正)の対処
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Reception of RE-FLASH access timer out at READ/WRITE
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/14 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
* @date 	2015/08/19 TDIPS)sasaki MKレビュー指摘事項No.63対応
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/31 TDIPS)sasaki CT検出問題(REファイルデータ読込応答送信ログが記録されない)の水平展開
* @date 	2015/09/01 TDIPS)sasaki CT検出問題(REファイルデータ読込応答のNGコード不正)の対処
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_flashAccessTO( VOID *pRcvMsg )
{
	T_API_FILE_LOAD_FW_DATA_RSP	flashReadRsp;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_flashAccessTO] ENTER" );

	switch(f_fmngw_re_apikind)
	{
		case D_FMNG_API_ERA:
		case D_FMNG_API_WRT:
			f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR, "FLASH WRITE NG");
			break;
		case D_FMNG_API_RD:
			/****************************************************************************/
			/* Send FW File Read Response												*/
			/****************************************************************************/
			memset(&flashReadRsp, 0, sizeof(flashReadRsp));

			flashReadRsp.header.uiEventNo     = D_API_MSGID_FILE_FIRM_READ_RSP;
			flashReadRsp.header.uiSignalkind  = 0;
			flashReadRsp.header.uiDstPQueueID = f_fmngw_respQid;
			flashReadRsp.header.uiDstTQueueID = 0;
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
			break;
		default:
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "RE-FLASH not processing" );
			break;
	}

	memset((UCHAR *)&f_fmngw_flashWaitWrtReq_re, 0, sizeof(f_fmngw_flashWaitWrtReq_re));
	memset((UCHAR *)&f_fmngw_flashWaitEraReq_re, 0, sizeof(f_fmngw_flashWaitEraReq_re));
	memset(&f_fmngw_save_fw_reqdata, 0, sizeof(f_fmngw_save_fw_reqdata));

	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_rsp_kind = D_FMNG_RSP_NONE;
	f_fmngw_re_apikind = D_FMNG_API_NONE;
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_IDLE;
	
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_flashAccessTO] RETURN to IDLE" );
	return;
}

/* @} */  
