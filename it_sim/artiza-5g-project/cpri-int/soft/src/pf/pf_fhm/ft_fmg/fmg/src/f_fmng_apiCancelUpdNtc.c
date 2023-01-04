/*!
 * @skip  $ld:$
 * @file  f_fmng_apiCancelUpdNtc.c
 * @brief Reception of API get file version request
 * @date  2014/01/11 FFCS)linb create
 * @date  2015/06/17 ALP)murakami TDD-ZYNQ対応
 * @date  2015/08/14 TDIPS)sasaki update
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"


/*!
 *  @brief  Reception of API Cancel Update Indication
 *  @note   This function is processed as follows.
 *          - Cancel the current update process
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create
 *  @date  2015/06/12 ALP)murakami TDD-ZYNQ対応
 *  @date  2015/08/14 TDIPS)sasaki update
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_apiCancelUpdNtc(void *pRcvMsg)
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_apiCancelUpdReq] ENTER");

	if(f_fmngw_fw_type == D_API_FILE_FW_TYPE_RE)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "Processing RE File");
		return;
	}

	/* if thread state is RW waiting, reset the waiting writing request */	
	if(f_fmngw_thrdState == D_FMNG_THRDSTA_FLSHRWWAIT)
	{
		memset((CHAR*)&f_fmngw_flashWaitWrtReq, 0, sizeof(f_fmngw_flashWaitWrtReq));
		memset((CHAR*)&f_fmngw_flashWaitWrtReq_slave, 0, sizeof(f_fmngw_flashWaitWrtReq_slave));
	}

	/* change thread state to cancel firm updating */
	f_fmngw_thrdState = D_FMNG_THRDSTA_FLSHRWCANCEL;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_apiCancelUpdReq] RETURN to CANCEL");
	return ;
}
/* @} */

