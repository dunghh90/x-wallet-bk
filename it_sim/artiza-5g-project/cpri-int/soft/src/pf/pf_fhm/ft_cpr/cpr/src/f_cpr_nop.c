/*!
 * @skip  $ld:$
 * @file  f_cpr_nop.c
 * @brief CPRI nop function.
 * @date 2013/11/14 FFCS)niemsh create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"
    
/*!
 * @addtogroup RRH_PF_CPR
 * @{
 */
/**
* @brief	CPRI nop function
* @note 	only access \n
* @param 	pRcvMsg [in] the buffer address pointer of received message
* @retval 	None
* @date 	2013/11/14 FFCS)niemsh create
* @warning	N/A
* @FeatureID	PF_Cpri-001-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/
VOID f_cpr_nop(UINT *pRcvMsg)
{
	f_com_assert(0,"f_cpr_nop");
}
/* @} */  

