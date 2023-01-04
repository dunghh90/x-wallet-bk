/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_monStart.c
 *  @brief  start the monitor of maker MT 
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
#include "l2_main_inc.h"
#include "rrhApi_Dbg.h"

/********************************************************************************************************************/
/*!
 *  @brief  start Maker MT.
 *  @note   start Maker MT for this task.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
VOID l2_dbg_monStart(UINT *inf_p)
{
	T_API_DBG_CPRIMON_START_IND *start_p;	/* Pointer of Maker MT start req IF	*/
	UINT                           linkno;

	start_p = (T_API_DBG_CPRIMON_START_IND *)inf_p;
	linkno = start_p->data.linkno;

	/* Start monitor of this link	*/
	l2_dbgw_mkmtmode[linkno].monitor = CMD_ON;
	l2_dbgw_mkmtmode[linkno].rspQID_moni= start_p->header.uiSrcPQueueID;

	return;
}


/* @} */

