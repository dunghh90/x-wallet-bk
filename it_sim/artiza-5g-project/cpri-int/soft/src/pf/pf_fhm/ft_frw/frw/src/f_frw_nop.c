/*!
 * @skip  $ld:$
 * @file  f_frw_nop.c
 * @brief No process
 * @date  2013/11/13 FFCS)linb create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */


 /*!
 * @addtogroup RRH_PF_FRW
 * @{
 */

#include "f_frw_inc.h"

/*!
 *  @brief  No process
 *  @note   This function is processed as follows.\n
 *          - No process.\n
 *  @return VOID
 *  @retval none
 *  @date  2013/11/13 FFCS)linb create\n
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frw_nop( VOID* pRcvMsg )
{
	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_WARNING, "Non process",((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo);
	return;
}

/* @} */
