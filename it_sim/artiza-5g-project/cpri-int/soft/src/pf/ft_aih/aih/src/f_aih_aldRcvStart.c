/**
 *  @skip	$Id:$
 *  @file	f_aih_aldSendend.c
 *  @brief  5GDU-LLS ALD receive start function
 *  @date   2019/08/01 FJT)Takahashi create
 *  ALL Rights Reserved, Copyright FUJITSU Limited 2019-
 */

/** @addtogroup RRH_PF_AIH
* @{ */

#include "f_aih_inc.h"			/* ALD(AISG)タスクヘッダファイル		*/

/**
* @brief 5GDU-LLS ALD receive start function
* @note  ALD(AISG) interface function.\n
* @param  *pRcvMsg   [in]    received message buffer
* @return VOID
* @retval none
* @date 2019/08/01 FJT)Takahashi create
* @warning	N/A
* @FeatureID	5GDU-M-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_aih_aldRcvStart( VOID *pRcvMsg )
{
	/* Update task state to ALD receive state */
	f_aih_aldCtrlLogp->rcvstartFlg = E_AIH_FLG_RCVON;
	
	(void)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_HDL_READ_WRITE, D_AISG_RADR_IRQ1MSK, D_AISG_RBIT_IRQ1MSK_RXING );

	return ;
}

/* @} */

