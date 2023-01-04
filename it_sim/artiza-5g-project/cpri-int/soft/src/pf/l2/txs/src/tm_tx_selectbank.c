/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_tx_SelectBank.c
 *  @brief  Bank select process
 *  @date   2008/07/17 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/


/*! @addtogroup RRH_L2_TXS
 * @{ */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRIF common head file			*/
#include "tm_tx_header.h"			/* TXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  Bank select process.
 *  @note   Select a bank no, according to input CPRI link no.
 *  @param  link_idx [in]  index of CPRI link no
 *  @param  *bank_idx_p [out]  index of bank no
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/17 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
VOID tm_tx_SelectBank(USHORT link_idx, UINT *bank_idx_p)
{
	/* Set output bank index as current to-be-selected bank index	*/
	*bank_idx_p = txw_curbankidx[link_idx];

	/* Update current to-be-selected bank index		*/
	txw_curbankidx[link_idx] = (USHORT)((txw_curbankidx[link_idx] + CMD_NUM1) % TXD_BD_MDEIF_BANK);
}

/* @} */

