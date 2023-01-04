/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_ls_init.c
 *  @brief   Send message to Init task, change task state to Running state	
 *  @date   2008/07/28  FFCS)zhengmh Create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_LNS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "tm_ls_header.h"			/* LNS task header					*/

/********************************************************************************************************************/
/**
 *  @brief     Send message to Init task, change task state to Running state
 *  @note     1.Send message to Init task
 		    2.Change task state to Running state
 *  @param  a_inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/


VOID tm_ls_Init(UINT * a_inf_p)
{
	CMT_TSKIF_HEAD *head_p;
	INT			ret_buf;

    ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_HEAD), (VOID **)&head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, CMD_NUM0, "tm_ls_Init:cm_BReq NG");
		return;
	}

	head_p->uiLength = sizeof(CMT_TSKIF_HEAD);
	head_p->uiEventNo = CMD_TSKIF_INIRSP;
	head_p->uiSrcTQueueID = CMD_TSKID_LNS;
	head_p->uiDstTQueueID = CMD_TSKID_INI;
	/*Send message to Init task*/
	ret_buf = cm_Enter(CMD_TSKID_INI,CMD_QRB_NORMAL,CMD_TSKIF_INIRSP,head_p);	
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
	}

	/*Change task state to Running state*/
	lsw_tststat = LSD_STA_RUN;
}

/* @} */
