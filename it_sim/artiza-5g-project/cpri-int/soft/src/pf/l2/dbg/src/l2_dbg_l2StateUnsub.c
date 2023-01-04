/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_l2StateUnsub.c
 *  @brief  Remove subscriber
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
#include "m_cm_header.h"			/* TRIF common head file		       	*/
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
 *  @brief  Remove subscriber
 *  @note   Remove subscriber from L2 state subscription table
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 *  @date   2015/8/10 TDIPS)ikeda rev.27999 LPB 17ƒŠƒ“ƒNC³
 */
/********************************************************************************************************************/
VOID l2_dbg_l2StateUnsub(UINT *inf_p)
{
    T_API_L2_LINKSTATE_UNSUB_IND *l2stateunsub_p;
	UINT count;
	USHORT linkno;

	l2stateunsub_p = (T_API_L2_LINKSTATE_UNSUB_IND *)inf_p;
	linkno = l2stateunsub_p->link_num;
	/* Remove subscriber*/
	for (count = 0; count < sizeof(lbw_L2StsNotiDstrbtTbl[linkno]) / sizeof(UINT); count++)
	{
		if (lbw_L2StsNotiDstrbtTbl[linkno][count] == l2stateunsub_p->header.uiSrcPQueueID)
		{
			lbw_L2StsNotiDstrbtTbl[linkno][count] = 0;
			break;
		}
	}

	return;
}


/* @} */

