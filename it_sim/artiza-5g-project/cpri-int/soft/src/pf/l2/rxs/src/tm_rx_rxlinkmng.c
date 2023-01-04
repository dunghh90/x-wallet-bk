/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_RxLinkMng.c
 *  @brief  data receiving links management
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
#include "m_cm_header.h"			/* TRIF common head file	        		*/
#include "l2_com_inc.h"
#include "tm_rx_header.h"			/* RXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  data receiving links management.
 *  @note   Manage RX links, in order to be read
 *  @param  linkno       [in]  CPRIリンク番号
 *  @param  *frame_cnt_p [in]  address of frame_cnt
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/22 FFCS)Zouw Create 
 *  @date   2015/07/30 TDI)ikeda FHMでの17link対応
 */
/********************************************************************************************************************/
VOID tm_rx_RxLinkMng(USHORT linkno, USHORT *frame_cnt_p)
{
	USHORT 		frm_perlink;		       /* The amount of frames of each link	*/

	frm_perlink = CMD_NUM0;
	
	/* If  this link not in simulator mode */
	if(l2_dbgw_mkmtmode[linkno].simulator == CMD_OFF)	
	{	
		/* Get the amount of frames for this link */
		frm_perlink =  (USHORT)tm_rx_cm_GetFrmNum(linkno);

		if(frm_perlink)
		{
			*frame_cnt_p = frm_perlink;                         /* Set the amount of frames */
		}
		else
		{
			*frame_cnt_p = CMD_NUM0;			/* This link has no frames	*/
		}
	}
	else 		/* Else this link has no frames or this link is in simulator mode	*/
	{
		*frame_cnt_p = CMD_NUM0;			/* This link has no frames	*/
	}

}

/* @} */

