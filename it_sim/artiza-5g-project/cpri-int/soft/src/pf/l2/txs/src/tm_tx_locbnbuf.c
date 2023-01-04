/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_tx_LocBnBuf.c
 *  @brief  Locate bank buffer
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
#include "l2_com_inc.h"
#include "f_rrh_reg_cpri.h"

#include "m_cm_header.h"			/* TRIF common head file	       		*/
#include "tm_tx_header.h"			/* TXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  Locate bank buffer.
 *  @note   Locate address of bank buffer, through link index,bank index.
 *  @param  linkno   [in] CPRIリンク番号
 *  @param  bank_idx [in] Bank no
 *  @param  bnbuf_p  [out] Double pointer of bank buffer
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/17 FFCS)Zouw Create 
 *  @date   2015/07/30 TDI)ikeda FHMでの17link対応
 */
/********************************************************************************************************************/
VOID tm_tx_LocBnBuf(USHORT linkno, USHORT bank_idx, UINT *bnbuf_p) 
{
	UINT 	linkbuf_base;	/* Base addr of each link's buffer		*/

	linkbuf_base = M_TXD_TXBUF_BASE(linkno);
	/* Set output bank buffer address		*/
	*bnbuf_p = (linkbuf_base + TXD_BNBUF_SIZE * bank_idx);

	return;
}
/* @} */

