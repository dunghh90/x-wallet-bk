/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_nego_AutoNegoStopInd.c
 *  @brief  Auto Nego Stop Notification function
 *  @date 2015/04/22 FFCS)fuxg create
 *  @date   2015/8/4 TDIPS)ikeda rev.27800 2.4Gビットレート対応
 *  @date   2015/8/4 TDIPS)ikeda rev.27819 オートネゴ修正
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_nego_inc.h"

/**
* @brief 	Auto Nego Stop Indication function
* @note  	Auto Nego Stop Indication function.\n
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2015/04/22 FFCS)fuxg create
* @warning	N/A
* @FeatureID	RRH-012-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_nego_AutoNegoStopInd(UINT * buff_adr)
{
	INT			ret;
	INT			errcd;
	CHAR *		bitratename[] = { "2.4G","4.9G","9.8G"};

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "EvtNo:0x%x", ((T_RRH_HEAD*)buff_adr)->uiEventNo);

	/* count clean */
	f_nego_4s_count = 0;
	f_nego_hfn_count = 0;

	/* log */
	BPF_COM_PLOG_TRACE_CPRI(E_TRC_LV_INFO, "Auto Nego Timer Stop!Bit Rate = %s(%d)", 
								bitratename[f_nego_bitrate],
								f_nego_bitrate);

	/* stop timer */
	ret = BPF_RU_HRTM_CANCEL(E_RRH_TIMID_AUTO_NEGO, &errcd);
	if(ret != BPF_RU_HRTM_COMPLETE)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, 
							"Timer Register NG!ret = %d, errcd = %d", 
							ret, errcd);
	}
	
	f_nego_thrdState = D_NEGO_THRDSTA_IDLE;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "EvtNo:0x%x", ((T_RRH_HEAD*)buff_adr)->uiEventNo);
	
	return;
}
/* @} */

