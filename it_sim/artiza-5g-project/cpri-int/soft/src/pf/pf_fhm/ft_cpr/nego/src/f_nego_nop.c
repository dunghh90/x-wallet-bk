/*!
 * @skip  $ld:$
 * @file  f_nego_nop.c
 * @brief PF_nego nop function.
 * @date 2015/04/22 FFCS)fuxg create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_sys_inc.h"
#include "f_nego_inc.h"
    
/*!
 * @addtogroup RRH_PF_NEGO
 * @{
 */
/**
* @brief	PF_nego nop function
* @note 	only access \n
* @param 	pRcvMsg [in] the buffer address pointer of received message
* @@retval 	None
* @date 	2015/04/22 FFCS)fuxg create
* @warning	N/A
* @FeatureID	RRH-012-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/
VOID f_nego_nop(UINT *pRcvMsg)
{
	/* Assert log */
	BPF_COM_PLOG_TRACE_CPRI( E_TRC_LV_INFO, 
						"PF NEGO NOP!status = %d, event = 0x%x", 
						f_nego_thrdState,
						f_nego_extEventNo);
}

/* @} */  

