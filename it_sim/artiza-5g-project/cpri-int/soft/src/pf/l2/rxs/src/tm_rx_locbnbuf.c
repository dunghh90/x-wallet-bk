/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_LocBnBuf.c
 *  @brief  Locate bank buffer
 *  @date   2008/07/22 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
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
 *  @brief  Locate bank buffer.
 *  @note   Locate bank buffer address, according to bank index
 *  @param  linkno   [in]  CPRIリンク番号
 *  @param  bank_idx [in]  Bank no index	
 *  @param  **bnbuf_p [out] Double pointer of bank buffer
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/22 FFCS)Zouw Create 
 *  @date   2015/07/30 TDI)ikeda FHMでの17link対応
 */
/********************************************************************************************************************/
VOID tm_rx_LocBnBuf(USHORT linkno, USHORT bank_idx, UINT *bnbuf_p) 
{
	UINT 		linkbuf_base;		/* Base addr of each link's buffer	*/

	linkbuf_base = M_RXD_RXBUF_BASE(linkno);

	/* Set output bank buffer address		*/
	*bnbuf_p = (linkbuf_base + RXD_BNBUF_SIZE * bank_idx);
}

/* @} */

