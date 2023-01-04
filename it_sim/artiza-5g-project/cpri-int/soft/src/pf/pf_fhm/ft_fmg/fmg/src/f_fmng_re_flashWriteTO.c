/*!
 * @skip  $ld:$
 * @file  f_fmng_re_flashWriteTO.c
 * @brief Reception of RE-FLASH access timer out at Writing
 * @date  2015/08/14 TDIPS)sasaki create
 * @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 * @date  2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
 * @date  2015/08/21 TDIPS)sasaki コメント修正
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Reception of RE-FLASH access timer out at Writing
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/14 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_flashWriteTO( VOID *pRcvMsg )
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_flashWriteTO] ENTER" );

	f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR, "FW Info Save NG");

	memset((UCHAR *)&f_fmngw_flashWaitWrtReq_re, 0, sizeof(f_fmngw_flashWaitWrtReq_re));
	memset((UCHAR *)&f_fmngw_flashWaitEraReq_re, 0, sizeof(f_fmngw_flashWaitEraReq_re));
	memset(&f_fmngw_save_fw_reqdata, 0, sizeof(f_fmngw_save_fw_reqdata));

	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_rsp_kind = D_FMNG_RSP_NONE;
	f_fmngw_re_apikind = D_FMNG_API_NONE;
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_IDLE;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_flashWriteTO] RETURN to IDLE" );
	return;
}

/* @} */  
