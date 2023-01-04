/*!
 * @skip  $ld:$
 * @file  f_fmng_flashWriteReFwInfoCmp.c
 * @brief Reception of RE File Info Save Response
 * @date 2015/08/14 TDIPS)sasaki create
 * @date 2015/08/21 TDIPS)sasaki コメント修正
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	Reception of RE File Info Save Response or RE-FLASH R/W T.O.
* @note 	
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2015/08/14 TDIPS)sasaki create
* @date 	2015/08/20 TDIPS)sasaki 状態番号のみ初期化するよう変更
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @warning	N/A
* @FeatureID	RRH-007-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_flashSaveReFwInfoCmp(VOID *pRcvMsg)
{	
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_flashSaveReFwInfoCmp] ENTER" );

	f_fmngw_thrdState = D_FMNG_THRDSTA_RUN;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_flashSaveReFwInfoCmp] RETURN to IDLE" );
	return;
}

/* @} */  
