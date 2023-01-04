/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_Init.c
 *  @brief  Polling task initialization.
 *  @date   2008/07/23 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_POLLING
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file           			*/
#include "tm_pl_header.h"			/* POLLING task header					*/
#include "l2_com_inc.h"			
/********************************************************************************************************************/
/*!
 *  @brief  POLLING task initialization.
 *  @note  Set timer registers' values.
 *             Send init rsp
 *  @param  *inf_p [in] Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned.
 *  @retval -
 *  @date   2008/07/24 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
VOID tm_pl_Init(UINT *inf_p)
{
	CMT_TSKIF_HEAD *head_p;
	INT			ret_buf;

	plw_assert_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"pol thread");

    ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_HEAD), (VOID **)&head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, CMD_NUM0, "tm_pl_Init:cm_BReq NG");
		return;
	}

	head_p->uiLength = sizeof(CMT_TSKIF_HEAD);
	head_p->uiEventNo = CMD_TSKIF_INIRSP;
	head_p->uiSrcTQueueID = CMD_TSKID_POL;
	head_p->uiDstTQueueID = CMD_TSKID_INI;
	/*Send response to init task*/
	ret_buf = cm_Enter(CMD_TSKID_INI, CMD_QRB_NORMAL, CMD_TSKIF_INIRSP, head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
	}

	/* Update task state from init to use	*/	
	plw_stateno = PLD_ST_USE ;
}	

/* @} */

