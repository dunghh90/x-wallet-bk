/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_lr_init.c
 *  @brief   Send message to Init task, change task state to Running state
 *  @date   2008/07/28  FFCS)zhengmh Create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_LNR
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "tm_lr_header.h"			/* LNR task header					*/

/********************************************************************************************************************/
/**
 *  @brief     Send message to Init task, change task state to Running state
 *  @note     1.Send message to Init task
		    2.change task state to Running state
 *  @param  a_inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/

VOID tm_lr_Init(UINT * a_inf_p)
{
	CMT_TSKIF_HEAD *head_p;
	INT			ret_buf;
	USHORT loopnum;			/* use as loop counter */
	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_HEAD), (VOID **)&head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "tm_lr_Init:cm_BReq NG");
		return;
	}

	head_p->uiLength = sizeof(CMT_TSKIF_HEAD);
	head_p->uiEventNo = CMD_TSKIF_INIRSP;
	head_p->uiSrcTQueueID = CMD_TSKID_LNR;
	head_p->uiDstTQueueID = CMD_TSKID_INI;
	/*Send message to Init task*/
	ret_buf = cm_Enter(CMD_TSKID_INI,CMD_QRB_NORMAL,CMD_TSKIF_INIRSP,head_p);	
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
	}
	
	for (loopnum =CMD_NUM0; loopnum < LRD_SRVCOUNT; loopnum++)
	{
	lrw_srvsocket[loopnum] = CMD_NUM_MINUS1;
	}
	/*Change task state to running state*/
	lrw_tststat = LRD_STA_RUN;
	
	return;
}
/* @} */
