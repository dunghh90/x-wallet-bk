/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_CpriLinkPrepareReq.c
 *  @brief  REC CPRI Link Preparation Request handler function
 *  @date   2015/08/03 TDIPS)ikeda create
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
* @brief  REC CPRI Link Preparation Request
* @note   \n
* @param  buff_adr [in] the buffer address pointer of received message
* @return None
* @date   2015/08/03 TDIPS)ikeda create
* @date   2015/11/17 TDI)satou ハソ-QA-047
*/
VOID m_cr_CpriLinkPrepareReq( UINT * buff_adr )
{
	/*call API */
	INT ret;
	USHORT linkno;

	cm_Assert(CMD_ASL_DBGLOW,0,"perfrom cpri startup");

	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++)
	{
		/* make subscription of lapb state request */
		ret = rrhApi_L2_Mpr_SubscribeLinkState(linkno, D_RRH_PROCQUE_L3,0,D_RRH_NULL);
		if(ret != E_API_RCD_OK)
		{
			cm_Assert(	CMD_ASL_USELOW,
						ret, "rrhApi_L2_Mpr_SubscribeLinkState");
		}

		/* make subscription of cpri state request */
		ret = rrhApi_Cpri_Mpr_SubscribeLinkState(linkno, D_RRH_PROCQUE_L3,0,D_RRH_NULL);
		if(ret != E_API_RCD_OK)
		{
			cm_Assert(	CMD_ASL_USELOW,
						ret, "rrhApi_Cpri_Mpr_SubscribeLinkState");
		}
	}

	ret = rrhApi_Cpri_Mnt_StartupLink(D_RRH_CPRINO_REC, (UINT)E_RRH_SFPLOG_TRIGGER_CARD_START, D_RRH_PROCQUE_L3,0,D_RRH_NULL);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
					ret, "rrhApi_Cpri_Mnt_StartupLinkNG");
	}

	/* set task state to RECStateE待ち */
	crw_taskstate = CRD_TASKSTA_REC_ACTIVE;
}

/* @} */
