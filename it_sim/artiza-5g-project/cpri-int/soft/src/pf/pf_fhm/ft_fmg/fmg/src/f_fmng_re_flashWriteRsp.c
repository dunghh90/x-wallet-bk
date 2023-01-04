/*!
 * @skip  $ld:$
 * @file  f_fmng_re_flashWriteRsp.c
 * @brief Reception of RE-FLASH WRITE response from FRWRE thread
 * @date 2015/08/11 TDIPS)sasaki create
 * @date 2015/08/19 TDIPS)sasaki タイマ停止処理を削除
 * @date 2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
 * @date 2015/08/21 TDIPS)sasaki RE-FLASH WRITE結果の保存処理を追加
 * @date 2015/08/24 TDIPS)sasaki コメント追加
 * @date 2015/08/31 TDIPS)sasaki return時のアサートログ内容修正
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Reception of RE-FLASH WRITE response from FRWRE thread
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki タイマ停止処理を削除
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/21 TDIPS)sasaki RE-FLASH WRITE結果の保存処理を追加
* @date 	2015/08/24 TDIPS)sasaki コメント追加
* @date 	2015/08/31 TDIPS)sasaki return時のアサートログ内容修正
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_flashWriteRsp( VOID *pRcvMsg )
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_flashWriteRsp] ENTER" );
	
	f_fmng_flashUpdReFwInfo(pRcvMsg);

	memset((UCHAR *)&f_fmngw_flashWaitWrtReq_re, 0, sizeof(f_fmngw_flashWaitWrtReq_re));
	memset((UCHAR *)&f_fmngw_flashWaitEraReq_re, 0, sizeof(f_fmngw_flashWaitEraReq_re));
	
	/* Change thread state into FW Info Saving	*/
	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_re_apikind = D_FMNG_API_NONE;
	f_fmngw_re_fileUpdResult = ((T_SYS_LMCWRT_RSP *)pRcvMsg)->result;
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_FWINFO_SAVE;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_flashWriteRsp] RETURN to FWINFO_SAVE" );
	return;
}

/* @} */  
