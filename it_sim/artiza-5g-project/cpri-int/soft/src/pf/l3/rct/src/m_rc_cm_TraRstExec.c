/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_cm_TraRstExec.c
 *  @brief  TRA reset execution
 *  @date   2008/07/25 FFCS)linlj create
 *  @date     2012/07/19 FFCS)niemsh modify for 1.5GVA
 *  @date     2013/11/05 FFCS)Xut modify for ZYNQ.
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    TRA reset execution
 *  @note     TRA reset execution
 *  @param    rstkind [in] reset kind - local/remote COM/reset NoCOM/AUTO/ALL RE
 *  @param    rstFact [in] reset factor
 *  @return   -
 *  @retval   -
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2012/07/19 FFCS)niemsh modify for 1.5GVA
 *  @date     2013/11/05 FFCS)Xut modify for ZYNQ.
 */
/********************************************************************************************************************/

VOID m_rc_cm_TraRstExec(UINT rstkind, UINT rstFact)
{			
	UINT   		rtn  = E_API_RCD_OK;			/* 戻り値						*/

	/****************/
	/* ?$(D*^(B?Z?b?g???? */
	/****************/
	rtn = rrhApi_Svp_Mnt_ResetCard(D_RRH_PROCQUE_L3, CMD_NUM0, D_RRH_NULL, rstkind, rstFact);

	if(rtn != E_API_RCD_OK)
	{
		/* アサート情報出力 */
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0, 
					"rrhApi_Svp_Mnt_ResetCard NG");
	}
	
	return ;
}

/* @} */

/* @} */

