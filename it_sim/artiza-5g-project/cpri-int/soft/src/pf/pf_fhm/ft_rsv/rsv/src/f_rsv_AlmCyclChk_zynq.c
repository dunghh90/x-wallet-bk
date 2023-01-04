/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   f_rsv_AlmCyclChk_zynq.c
 *  @brief  Cycling Alarm detect
 *  @date   2013/11/21FFCS)Xut create for Zynq
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2012-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_PF_RSV
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル	*/


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief   Alarm Cycling Check process
 *  @note    Alarm Cycling Check process
 *
 *  @return   RETURN CODE
 *  @retval   None
 *  @date   2013/11/21FFCS)Xut create for Zynq
 * @warning	N/A
 * @FeatureID	PF-Svp-001-007-001
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 *
 */
/********************************************************************************************************************/
VOID f_rsv_AlmCyclChk( )
{
	UINT	loop;
	UINT	regDat;
	UINT	hfcSync;
	UINT	cpriSta;
	UINT	cpriTime;
	UINT	pllCount;
	
	for(loop = D_SYS_NUM0; loop < D_RSV_ALM_NUM; loop++)
	{
		/* Accumulate cycle count */
		(*(f_rsvr_almInfo[loop].cyclCntAddr))++;
		/* If the cycle is not reached, no process */
		if(*(f_rsvr_almInfo[loop].cyclCntAddr) < f_rsvr_almInfo[loop].chkCycle)
		{
			continue;
		}
		/* Read register data */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, 
					f_rsvr_almInfo[loop].chkRegAddr, &regDat);
		
		if(f_rsvr_almInfo[loop].almCode == D_RRH_ALMCD_PLLNG)
		{
			/* Read CPRI STATUS register */
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,
				D_RRH_REG_CPRI_CPST,&hfcSync);
			/* Read EXT-PLL register */
			/* If HFNSYNC is SYNC and PLL Unlock */
			if(((hfcSync & 0x00000001) != D_SYS_NUM0) && 
				((regDat & D_RSV_ALMBIT_PLLNG) != D_SYS_NUM0))
			{
				/* Increase the alarm occuring count  */
				(*(f_rsvr_almInfo[loop].cntAddr) )++;
			}
			else
			{
				/* Clear the count */
				*(f_rsvr_almInfo[loop].cntAddr) = D_SYS_NUM0;
			}
		}
		/* ★TODO ハソ改版されるまでの暫定対処 */
		else if(f_rsvr_almInfo[loop].almCode == D_RRH_ALMCD_FPGANG2)
		{
			/* Check the bit value */
			if((regDat & f_rsvr_almInfo[loop].chkBitmap) == D_SYS_NUM0)
			{
				/* Increase the alarm occuring count  */
				(*(f_rsvr_almInfo[loop].cntAddr) )++;
			}
			else
			{
				/* Clear the count */
				*(f_rsvr_almInfo[loop].cntAddr) = D_SYS_NUM0;
			}
		}
		else
		{
			/* Check the bit value */
			if((regDat & f_rsvr_almInfo[loop].chkBitmap) != D_SYS_NUM0)
			{
				/* Increase the alarm occuring count  */
				(*(f_rsvr_almInfo[loop].cntAddr) )++;
			}
			else
			{
				/* Clear the count */
				*(f_rsvr_almInfo[loop].cntAddr) = D_SYS_NUM0;
			}
		}
		/* If the occuring time reach the threshold */
		if(*(f_rsvr_almInfo[loop].cntAddr) >= f_rsvr_almInfo[loop].count)
		{
			if(f_rsvr_almInfo[loop].almCode == D_RRH_ALMCD_PLLNG)
			{
				/* Get CPRI state */
				BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, 
						D_RRH_REG_CPRI_CPSTAT, &cpriSta);
				/* If State E down, don't trigger PLL UNLOCK alarm */
				if(cpriSta < 4)
				{
					/* Clear the count */
					*(f_rsvr_almInfo[loop].cntAddr) = D_SYS_NUM0;
					continue;
				}
				/* Get CPRI Stable time */
				BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, 
						D_RRH_REG_CPRI_CPSTAB, &cpriTime);
				/* Get PLL Unlock alarm triggering count */
				pllCount = cpriTime*10/100 + f_rsvr_almInfo[loop].count;
				if(*(f_rsvr_almInfo[loop].cntAddr) < pllCount)
				{
					continue;
				}
			}

			/* Trigger alarm report process */
			f_com_almReportFHM(D_SYS_THDID_PF_RSV, f_rsvr_almInfo[loop].almCode);
			/* Clear the count */
			*(f_rsvr_almInfo[loop].cntAddr) = D_SYS_NUM0;
			/* Set assert log */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL,"ALM detected : Code = 0x%08x,RegData = 0x%08x",
				f_rsvr_almInfo[loop].almCode,regDat
			);
		}
		/* Clear the cycle count when reached */
		*(f_rsvr_almInfo[loop].cyclCntAddr) = D_SYS_NUM0;
	}

	return;
}

/* @} */

/* @} */
