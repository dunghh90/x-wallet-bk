/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_rsv_cm_GetAlmInf.c
 *  @brief  convert card alarm factor to alarm code
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/05/18 FFCS)Wangjuan modify for M-S3G-eNBPF-02009
 							when several alarm code exist, all need report.
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date 2012/06/20 FFCS)niemsh modify for 15.GRRE
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */

/****************************************************************************/
/* wb_CN[h                                                       */
/****************************************************************************/
#include <signal.h>
#include "f_rsv_header.h"			

/*****************************************************************************/
/**
 *  @brief  TRA common function - Get current factor of hardware alarm
 *  @note  Get current factor of hardware alarm
 *          according to alarm type(Soft alarm is excluded) \n
 *  @param  almType [in] alarm type
 *  
 *  @return NONE 
 *  @retval NONE
 *  @date   2012/06/26		FFCS)Xut	new create
 *  @warning	N/A
 *  @FeatureID	PF-Svp-005-004-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/
/*****************************************************************************/
USHORT f_rsv_cm_GetHardAlmFact(UINT almType)
{

	USHORT usFactor ;

	switch(almType)
	{
		/* PLL Alarm */
		case D_SYS_HWALMTYPE_PLL:
			usFactor = (tra_sv_newsta[D_SYS_NUM3] & D_RSV_PLLALM_BIT);
			break;
		/* CPRI alarm */
		case D_SYS_HWALMTYPE_CPRI:
			usFactor = (tra_sv_newsta[D_SYS_NUM3] & D_RSV_CPRIALM_BIT);
			break;
		/* APD COM alarm */
		case D_SYS_HWALMTYPE_APDCOM:
			usFactor = (tra_sv_newsta[D_SYS_NUM3] & D_RSV_COM2ALM_BIT);
			break;
		default:
			usFactor = D_SYS_ALLF_SHORT;
			break;
	}

	return usFactor;
}

/* @{ */
/**
* @brief Get alarm Information
* @note Get alarm Information.\n
* @param a_almfactor [in] card alarm factor
* @param a_alminf [out] alarm information
* @return none
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2012/07/20 FFCS)niemsh modify for 15.GRRE
* @warning	N/A
* @FeatureID	PF-Svp-005-004-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_cm_GetAlmInf( USHORT a_almfactor, USHORT* a_alminf )
{
	if(a_alminf == NULL)
	{
		return;
	}

	/* init */
	*a_alminf = D_SYS_NUM0;
	/* check if soft alarm exist or not */
	if((a_almfactor & D_RSV_SOFTALM_BIT) != D_SYS_NUM0)
	{
		*a_alminf = tra_sv_newsta[D_SYS_NUM12];
	}

	/* Move the pointer to get the next Alarm Info */
	a_alminf++;

	/* init */
	*a_alminf = D_SYS_NUM0;
	/* check if PLL alarm exist or not */
	if((a_almfactor & D_RSV_MAJALM_PLL_BIT) != D_SYS_NUM0)
	{
		/* read minor alarm factor of PLL alarm */
		*a_alminf = f_rsv_cm_GetHardAlmFact(D_SYS_HWALMTYPE_PLL);
	}

	/* Move the pointer to get the next Alarm Info */
	a_alminf++;

	/* init */
	*a_alminf = D_SYS_NUM0;

	/* check if CPRI alarm exist or not */
	if((a_almfactor & D_RSV_MAJALM_CPRI_BIT) != D_SYS_NUM0)
	{
		/* read minor alarm factor of PLL alarm */
		*a_alminf = f_rsv_cm_GetHardAlmFact(D_SYS_HWALMTYPE_CPRI);
	}

	/* Move the pointer to get the next Alarm Info */
	a_alminf++;

	/* init */
	*a_alminf = D_SYS_NUM0;

	/* check if APD COM alarm exist or not */
	if((a_almfactor & D_RSV_MAJALM_APDCOM_BIT) != D_SYS_NUM0)
	{
		/* read minor alarm factor of APD COM alarm */
		*a_alminf = f_rsv_cm_GetHardAlmFact(D_SYS_HWALMTYPE_APDCOM);
	}

	return ;
}
/* @} */

/* @} */

