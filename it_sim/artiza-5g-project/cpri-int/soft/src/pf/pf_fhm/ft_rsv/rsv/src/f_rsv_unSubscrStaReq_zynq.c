/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   f_rsv_unSubscrStaReq_zynq.c
 *  @brief  API unsubscribe State Indication function
 *  @date   2013/11/21FFCS)Xut create
 * 
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */

#include "f_rsv_header.h"			/* RE�Ď��^�X�N�w�b�_�t�@�C��			*/


/**
* @brief API unsubscribe State Indication
* @note Clear the response QID.\n
* @param msgInd_p [in] received message
* @return None
* @date   2013/11/21FFCS)Xut create
* @warning	N/A
* @FeatureID	PF-Svp-005-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/
VOID f_rsv_unSubscrStaReq(T_API_SVP_UNSUBSCARDSTA_IND* msgInd_p)
{
	UINT 	num;
	
	for(num = 0; num < sizeof(f_rsvw_cardStaChgNtc_QID) / sizeof(UINT);num++)
	{
		if(f_rsvw_cardStaChgNtc_QID[ num] == msgInd_p->header.uiSrcPQueueID)
		{
			/* Clear the requested Response QID */
			f_rsvw_cardStaChgNtc_QID[ num] = D_SYS_NUM0;
			break;
		}
	}

	return;
}

/* @} */
