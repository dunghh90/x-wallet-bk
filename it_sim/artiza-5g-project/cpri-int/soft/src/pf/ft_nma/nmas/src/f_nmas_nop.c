/*!
 * @skip  $ld:$
 * @file  f_nmas_nop.c
 * @brief No process
 * @date  2019/02/21 KCN)kamimoto create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


 /*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */

#include "f_nmas_inc.h"

/*!
 *  @brief  No process
 *  @note   This function is processed as follows.\n
 *          - No process.\n
 *  @return VOID
 *  @retval none
 *  @date  2019/02/21 KCN)kamimoto  Create\n
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_nmas_nop( VOID* pRcvMsg )
{
	cmn_com_assert_th_get(f_nmasw_assert_p,  D_RRH_LOG_AST_LV_WARNING, "Non process",((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo);
	return;
}

/* @} */
