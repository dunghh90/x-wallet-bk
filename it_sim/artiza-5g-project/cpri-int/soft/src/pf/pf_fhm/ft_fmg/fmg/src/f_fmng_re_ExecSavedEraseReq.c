/*!
 * @skip  $ld:$
 * @file  f_fmng_re_ExecSavedEraseReq.c
 * @brief Send Saved ERASE Request to FRWRE thread
 * @date 2015/08/11 TDIPS)sasaki create
 * @date 2015/08/19 TDIPS)sasaki タイマ再起動処理を削除
 * @date 2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
 * @date 2015/08/21 TDIPS)sasaki コメント修正
 * @date 2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Send Saved ERASE Request to FRWRE thread
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/19 TDIPS)sasaki タイマ再起動処理を削除
* @date 	2015/08/20 TDIPS)sasaki API種別をREファイル用の変数に保存するよう変更
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @date 	2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_ExecSavedEraseReq( VOID *pRcvMsg )
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_ExecSavedEraseReq] ENTER" );				

	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQRE,
					(CHAR*)&f_fmngw_flashWaitEraReq_re,
					sizeof(f_fmngw_flashWaitEraReq_re), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_re_thrdState, 
					D_SYS_MSGID_FLASH_RE_ERASE_REQ );

	/* Set received api kind	*/
	f_fmngw_re_apikind = D_FMNG_API_ERA;

	/* Change thread state into RE-FLASH READ/WRITE		*/
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_FLSHRW;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_ExecSavedEraseReq] RETURN to FLSHRW" );
	return;
}

/* @} */  
