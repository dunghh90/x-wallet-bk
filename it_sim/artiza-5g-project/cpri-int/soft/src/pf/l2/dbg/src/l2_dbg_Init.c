/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_Init.c
 *  @brief  L2 debug thread initialization.
 *  @date   2013/12/03 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_DBG
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file           			*/
#include "l2_com_inc.h"
#include "l2_dbg_inc.h"
/********************************************************************************************************************/
/*!
 *  @brief  L2 debug thread initialization.
 *  @note   Send init rsp
 *  @param  *inf_p [in] Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned.
 *  @retval -
 *  @date   2013/12/03 FFCS)hongj Create 
 */
/********************************************************************************************************************/
#if 0
VOID l2_dbg_Init(UINT *inf_p)
{
	INT	ret_buf;
	CMT_TSKIF_HEAD *head_p;

	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_HEAD), (VOID **)&head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "l2_dbg_Init:cm_BReq NG");
		return;
	}

	head_p->uiLength = sizeof(CMT_TSKIF_HEAD);
	head_p->uiEventNo = CMD_TSKIF_INIRSP;
	head_p->uiSrcTQueueID = CMD_TSKID_DBG;
	head_p->uiDstTQueueID = CMD_TSKID_INI;

	/*Send response to init task*/
	cm_Enter(CMD_TSKID_INI, CMD_QRB_NORMAL, CMD_TSKIF_INIRSP, head_p);

	/* Update task state from init to use	*/
   	l2_dbgw_stateno = L2_DBG_ST_USE;

	return;
}	
#endif
/*! @} */
