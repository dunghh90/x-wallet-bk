/*!
 * @skip  $ld:$
 * @file  f_nmas_common.c
 * @brief Process of Frw common function
 * @date  2019/02/21 KCN)kamimoto  create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

 /*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */

#include "f_nmas_inc.h"


/*!
 *  @brief  the function for flash erasing.
 *  @note   This function is processed as follows.
 *          1) start up the common function of flash erasing,if flash is faild,abort
 *  @param  a_flashadr      [in]  the start of address to be flashed
 *  @param  a_dvlpdatasize  [in]  the size to be flashed
 *  @return Void.
 *  @retval -
 *  @date   2019/02/21 KCN)kamimoto Create.
 *  @warning	N/A
 *  @FeatureID	PF_File-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
UINT f_nmas_cm_EraseFlash(UINT flshOffset, UINT size)
{
	UINT rtn = BPF_HM_DEVC_COMPLETE;

	cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_nmas_cm_EraseFlash] ENTER" );

	cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO, "NMAS flash erase: offset=%08x, size=%08x", flshOffset, size);
	
	rtn = BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE(flshOffset,size);
	if(rtn != BPF_HM_DEVC_COMPLETE)
	{
		cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_ERROR , "NMAS flash erase NG:");
		return D_RRH_NG;
	}
	cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_INFO  , "NMAS flash erase OK");
	cmn_com_assert_th_get(f_nmasw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_nmas_cm_EraseFlash] RETURN" );
	return D_RRH_OK;
}
/* @} */


