/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_monStop.c
 *  @brief  stop the monitor maker MT 
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


 /*!
 * @addtogroup RRH_L2_DBG
 * @{
 */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "l2_com_inc.h"
#include "rrhApi_Dbg.h"

/********************************************************************************************************************/
/*!
 *  @brief  stop Maker MT.
 *  @note   stop Maker MT for this task.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
VOID l2_dbg_monStop(UINT *inf_p)
{
	T_API_DBG_CPRIMON_STOP_IND *stop_p;		/* Maker MT stop		*/
	UINT                          linkno;
	
	stop_p = (T_API_DBG_CPRIMON_STOP_IND *)inf_p;
	linkno = stop_p->data.linkno;

	/* Stop simulator mode	of this link	*/
	l2_dbgw_mkmtmode[linkno].monitor = CMD_OFF;
	l2_dbgw_mkmtmode[linkno].rspQID_moni= 0;

	return;
}
/* @} */
