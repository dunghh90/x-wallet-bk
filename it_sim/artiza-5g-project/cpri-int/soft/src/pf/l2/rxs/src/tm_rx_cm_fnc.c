/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_cm_fnc.c
 *  @brief  Get the amount of frames for the link
 *  @date   2008/07/22 FFCS)Zouw Create 
 *  @date   2012/07/11 FFCS)niemsh modify for 1.5GVA  
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_RXS
 * @{ */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file	       		*/
#include "tm_rx_header.h"			/* RXS task header					*/


/********************************************************************************************************************/
/*!
 *  @brief  Count amount of bit 1.
 *  @note   Count the amount of bit 1 in the input value
 *  @param   rxstareg  [in]  RX status register value
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval onebitnum: The amount of bit 1 in the input value
 *  @date   2008/07/22 FFCS)Zouw Create 
 *  @date   2012/07/11 FFCS)niemsh modify for 1.5GVA  
 */
/********************************************************************************************************************/
UINT tm_rx_cm_OneBitNum(UINT rxstareg)
{
	UINT 		bank_idx;						/* Bank index			*/
	UINT 		bit_mask;						/* Mask bit				*/
	UINT 		onebitnum;						/* Amount of bit 1		*/

	bank_idx = CMD_NUM0;
	onebitnum = CMD_NUM0;
	bit_mask = RXD_MSK_ONE;							/* Set mask as 1		*/

	bit_mask <<= CMD_NUM16;							/* Set mask as 1		*/

	for (bank_idx = CMD_NUM0 ;bank_idx < RXD_BD_MDEIF_BANK; bank_idx ++)
	{
		if((bit_mask & rxstareg) != RXD_MSK_ZERO)		/* If this bit equals 1	*/
		{
			onebitnum ++;				/* Increment amount of bit 1	*/
		}
		
		bit_mask <<= CMD_NUM1;					/* Shift mask bit left by 1		*/
	}

	return onebitnum;
}


/********************************************************************************************************************/
/*!
 *  @brief  Get frame amount.
 *  @note   Get the amount of received frame, according to CPRI link index
 *  @param  linkno [in] CPRIリンク番号
 *  @return Result code is returned
 *  @retval Frame amount	
 *  @date   2008/07/22 FFCS)Zouw Create 
 *  @date   2012/07/11 FFCS)niemsh modify for 1.5GVA  
 *  @date   2015/07/30 TDI)ikeda FHMでの17link対応
 */
/********************************************************************************************************************/
UINT tm_rx_cm_GetFrmNum(USHORT linkno)
{
	UINT 		rxstareg_val;		/* LAPB RX status register value		*/
	
	/* Read this link's RX status register		*/
	BPF_HM_DEVC_REG_READ(0,
		M_RRH_REG_CM_LRXST(linkno),&rxstareg_val);

	return tm_rx_cm_OneBitNum(rxstareg_val);	/* Count amount of frames	*/
}

/* @} */

