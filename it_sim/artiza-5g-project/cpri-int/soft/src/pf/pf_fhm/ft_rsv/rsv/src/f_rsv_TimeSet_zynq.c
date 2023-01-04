/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_rsv_TimeSet_zynq.c
 *  @brief  API Set datetime Request process function
 *  @date   2013/11/08 FFCS)Xut create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */
#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/

/**
* @brief API Set datetime Request process function
* @note API Set datetime Request process .\n
* @param timeinf_p [in] the buffer address pointer of received message
* @return None
* @date   2013/11/08 FFCS)Xut create
* @warning	N/A
* @FeatureID	PF-Svp-008-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_TimeSet(T_API_SVP_SETTIME_REQ*	timeinf_p  )
{
	T_RRH_SYSTEM_TIME	datetime;
	INT					ret;
	
	/* Get Date time */
	memcpy((VOID*)&datetime, (VOID*)&(timeinf_p->datetime), sizeof(T_RRH_SYSTEM_TIME)); 
	datetime.ucount = D_SYS_NUM0;
	/*********************/
	/* RTC共通関数コール */
	/*********************/
	ret = BPF_HM_DEVC_SETTIME_BCD(datetime);
	if(ret != BPF_HM_DEVC_COMPLETE)
	f_com_assert(ret, "set datetime error");
	else
	f_com_assert(ret, "set datetime OK");
		
	return;
}

/* @} */

