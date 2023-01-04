/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_CpriStartReNotice.c
 *  @brief  REC CPRI Link Preparation Request handler function
 *  @date   2015/07/22 TDIPS)ikeda create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"
#include "m_cr_header.h"
#include "rrhApi_L2.h"

/**
* @brief RE CPRI Link Start Notice
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2015/07/22 TDIPS)ikeda create
* @date   2015/8/3 TDIPS)ikeda rev.27759 対RE実装
* @date   2015/10/19 TDI)satou ハソ-QA-051 二次リセット
* @date   2015/11/17 TDI)satou ハソ-QA-047
*/
VOID m_cr_CpriStartReNotice( UINT * buff_adr )
{
	CMT_TSKIF_RE_CPRILNKSTART_NTC*	a_msg;		/* RE CPRIスタート	*/
	INT ret;
	UINT a_bit;

	a_msg = (CMT_TSKIF_RE_CPRILNKSTART_NTC*)buff_adr;

	ret = rrhApi_Cpri_Mnt_StartupLink(a_msg->link_num, a_msg->sfpLogTrigger, D_RRH_PROCQUE_L3,0,D_RRH_NULL);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
					ret, "rrhApi_Cpri_Mnt_StartupLinkNG");
	}

    /* 2次リセット対象の場合、REプロセスに要求を送信し、対象からクリア */
	a_bit = BIT0 << (a_msg->link_num - 1);
	if (0 != (crw_poweron_reset_target & a_bit))
	{
        m_cr_Cpri2ndRstReq(a_msg->link_num);
        crw_poweron_reset_target &= ~a_bit;
    }
}

/* @} */
