/*!
 * @skip  $ld:$
 * @file  f_frwsla_nop.c
 * @brief No process
 * @date  2014/12/12 FFCS)linb create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014-2015
 */


 /*!
 * @addtogroup RRH_PF_FRWSLA
 * @{
 */

#include "f_frwsla_inc.h"

/*!
 *  @brief  No process
 *  @note   This function is processed as follows.\n
 *          - No process.\n
 *  @return VOID
 *  @retval none
 *  @date  2014/12/12 FFCS)linb create\n
 *  @warning	N/A
 *  @FeatureID	RRH-011-012
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_frwsla_nop( VOID* pRcvMsg )
{
	cmn_com_assert_th_get(f_frwslaw_assert_p,  D_RRH_LOG_AST_LV_WARNING, "Non process",((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo);
	return;
}

/* @} */
