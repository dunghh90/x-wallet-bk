/*!
 * @skip  $ld:$
 * @file  f_fmng_re_DiscardSavedWriteReq.c
 * @brief Discard Saved WRITE request
 * @date 2015/08/11 TDIPS)sasaki create
 * @date 2015/08/21 TDIPS)sasaki コメント修正
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Discard Saved WRITE request
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/11 TDIPS)sasaki create
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_re_DiscardSavedWriteReq( VOID *pRcvMsg )
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_re_DiscardSavedWriteReq] ENTER");

	if(f_fmngw_fw_type == D_API_FILE_FW_TYPE_FHM)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "Processing FHM File RETURN");
		return;
	}

	/* Save Response QId  */
	f_fmngw_respQid = ((T_API_FILE_FIRM_CANCEL_IND *)pRcvMsg)->header.uiSrcPQueueID;
	/* Save Response Kind  */
	f_fmngw_rsp_kind = D_FMNG_RSP_CANCEL;

	/****************************************************************************/
	/* Discard Saved WRITE Request to FRWRE thread								*/
	/****************************************************************************/
	memset(&f_fmngw_flashWaitWrtReq_re, 0, sizeof(f_fmngw_flashWaitWrtReq_re));

	/* Change thread state into RE-FLASH WRITE	*/
	f_fmngw_re_thrdState = D_FMNG_THRDSTA_RE_FLSHRW;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_re_DiscardSavedWriteReq] RETURN to FLSHRW" );
	return;
}

/* @} */  
