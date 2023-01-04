/*!
 * @skip  $ld:$
 * @file  f_fmng_flashWriteTimout.c
 * @brief Reception of Flash writing timer out
 * @date  2013/11/15 FFCS)linb create
 * @date  2015/08/14 TDIPS)sasaki update
 * @date  2015/08/19 TDIPS)sasaki MKレビュー指摘事項No.62対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"


/*!
 *  @brief  Reception of Flash writing timer out
 *  @note   This function is processed as follows.\n
 *          - Send API Load Update Response(result = NG).\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create\n
 *  @date  2015/08/14 TDIPS)sasaki update
 *  @date  2015/08/19 TDIPS)sasaki MKレビュー指摘事項No.62対応
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_fmng_flashWriteTimout(void *pRcvMsg)
{
	/* assert */
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "flash write T.O 0x%x,0x%x",
				f_fmngw_thrdState,f_fmngw_preThrdState);

	/* Send API Load Update Response (result = NG)   */
	f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR,"flash write T.O");

	/* Change thread state into running */
	f_fmngw_fw_type = D_API_FILE_FW_TYPE_NONE;
	f_fmngw_thrdState = D_FMNG_THRDSTA_RUN;
        
	return ;    
}
/* @} */

