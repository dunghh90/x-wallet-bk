/*!
 * @skip  $ld:$
 * @file  f_frwre_nop.c
 * @brief No process
 * @date  2015/08/04 TDIPS)sasaki create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */


 /*!
 * @addtogroup RRH_PF_FRWRE
 * @{
 */

#include "f_frwre_inc.h"

/*!
 *  @brief  No process
 *  @note   This function is processed as follows.\n
 *          - No process.\n
 *  @return VOID
 *  @retval none
 *  @date  2015/08/04 TDIPS)sasaki create\n
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frwre_nop( VOID* pRcvMsg )
{
	cmn_com_assert_th_get(f_frwrew_assert_p,  D_RRH_LOG_AST_LV_WARNING, "Non process",((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo);
	return;
}

/* @} */
