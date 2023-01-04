/*!
 * @skip  $ld:$
 * @file  f_fmng_nop.c
 * @brief 
 * @date 2013/11/14 FFCS)linb create
 * @date 2015/08/21 TDIPS)sasaki コメント修正
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_fmng_inc.h"
    
/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */
/**
* @brief	FMNG nop function
* @note 	only access \n
* @param	pRcvMsg [in] the buffer address pointer of received message
* @@retval	None
* @date 	2013/11/15 FFCS)linb create
* @date 	2015/08/21 TDIPS)sasaki コメント修正
* @warning	N/A
* @FeatureID	PF_File-003-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_fmng_nop(VOID *pRcvMsg)
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "non process 0x%x,0x%x,0x%x",
				f_fmngw_thrdState,f_fmngw_preThrdState ,((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo);
	return;
}

/* @} */  
