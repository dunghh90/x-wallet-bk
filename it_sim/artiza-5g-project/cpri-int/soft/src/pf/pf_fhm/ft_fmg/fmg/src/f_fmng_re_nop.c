/*!
 * @skip  $ld:$
 * @file  f_fmng_re_nop.c
 * @brief 
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
* @brief	No Operation
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

VOID f_fmng_re_nop( VOID *pRcvMsg )
{
	cmn_com_assert_th_get(
		f_fmngw_assert_p,
		D_RRH_LOG_AST_LV_INFO,
		"f_fmngr_re_matrix: non process 0x%x,0x%x,0x%x",
		f_fmngw_re_thrdState,
		f_fmngw_re_preThrdState,
		((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo
	);

	return;
}

/* @} */  
