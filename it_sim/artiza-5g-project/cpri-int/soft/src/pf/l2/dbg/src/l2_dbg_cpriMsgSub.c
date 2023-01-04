/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_cpriMsgSub.c
 *  @brief  Add new subscriber
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
#include "rrhApi_L2.h"
#include "tm_lb_def.h"
#include "tm_lb_typ.h"
#include "tm_lb_ext.h"
#include "m_lb_def.h"
#include "m_lb_typ.h"
#include "m_lb_ext.h"


/********************************************************************************************************************/
/*!
 *  @brief  Add new subscriber
 *  @note   Add new item into Cpri message subscription table
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 *  @date   2015/8/10 TDIPS)ikeda rev.27999 LPB 17ƒŠƒ“ƒNC³
 */
/********************************************************************************************************************/
VOID l2_dbg_cpriMsgSub(UINT *inf_p)
{
	T_API_CPRIMSG_SUB_REQ	*cprisub_p;
	UINT count;
	UINT count_search;
	USHORT linkno;

	cprisub_p = (T_API_CPRIMSG_SUB_REQ *)inf_p;
	linkno = cprisub_p->link_num;
	if (cprisub_p->data.msgCount <= 0)
	{
		cm_Assert( CMD_ASL_DBGLOW, 0,
			(CHAR*)"[l2_main_cpriMsgSub] message count NG" );
		return;
	}
	
	if (lbw_L3DstrbtTbl_cur[linkno] >= D_L2_L3MSGNUM)
	{
		cm_Assert( CMD_ASL_DBGLOW, lbw_L3DstrbtTbl_cur[linkno],
			(CHAR*)"[l2_main_cpriMsgSub] The number of subscription is maximum." );
		return;
	}
	/* Add new item into Cpri message subscription table */
	for (count = 0; count < cprisub_p->data.msgCount; count++)
	{
		/* search if same item exists */
		for (count_search = 0; count_search < lbw_L3DstrbtTbl_cur[linkno]; count_search++)
		{
			if ((lbw_L3DstrbtTbl[linkno][count_search][0] == cprisub_p->data.msgs[count])
			         && (lbw_L3DstrbtTbl[linkno][count_search][1] == cprisub_p->header.uiSrcPQueueID))
			{
				break;
			}
		}
		if (count_search < lbw_L3DstrbtTbl_cur[linkno])
		{
			/* there is same item exists */
			continue;
		}
		lbw_L3DstrbtTbl[linkno][lbw_L3DstrbtTbl_cur[linkno]][0] = cprisub_p->data.msgs[count];
		lbw_L3DstrbtTbl[linkno][lbw_L3DstrbtTbl_cur[linkno]][1] = cprisub_p->header.uiSrcPQueueID;
		lbw_L3DstrbtTbl_cur[linkno]++;
		
		if(lbw_L3DstrbtTbl_cur[linkno] >= D_L2_L3MSGNUM)
		{
			cm_Assert( CMD_ASL_DBGLOW, lbw_L3DstrbtTbl_cur[linkno],
				(CHAR*)"[l2_main_cpriMsgSub] The number of subscription is maximum 2." );
			break;
		}	
	}
	
	return;
}
/* @} */

