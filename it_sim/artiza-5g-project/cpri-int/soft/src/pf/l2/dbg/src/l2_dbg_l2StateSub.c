/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_l2StateSub.c
 *  @brief  Add new subscriber
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


 /*!
 * @addtogroup RRH_L2_DBGs
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
 *  @note   Add new item into L2 state subscription table
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 *  @date   2015/7/28 TDIPS)ikeda rev.27478 CPRIƒŠƒ“ƒN”Ô†•¡”‘Î‰ž
 *  @date   2015/8/10 TDIPS)ikeda rev.27999 LPB 17ƒŠƒ“ƒNC³
 */
/********************************************************************************************************************/
VOID l2_dbg_l2StateSub(UINT *inf_p)
{
	T_API_L2_LINKSTATE_SUB_REQ	 *l2statesub_p;
	T_API_L2_LINKSTATE_CHG_NTC	 apiNtc;
	UINT						 state;
	UINT count;
	INT						blank;
	USHORT						a_link_num;

	l2statesub_p = (T_API_L2_LINKSTATE_SUB_REQ *)inf_p;
	a_link_num = l2statesub_p->link_num;

	/* Remove subscriber*/
	blank = -1;
	for (count = 0; count < sizeof(lbw_L2StsNotiDstrbtTbl[a_link_num]) / sizeof(UINT); count++)
	{
		if (lbw_L2StsNotiDstrbtTbl[a_link_num][count] == l2statesub_p->header.uiSrcPQueueID)
			break;
		if((lbw_L2StsNotiDstrbtTbl[a_link_num][ count] == 0) && (blank < 0))
			blank = count;

	}

	if(count >= sizeof(lbw_L2StsNotiDstrbtTbl[a_link_num]) / sizeof(UINT))
	{
		if(blank >= 0)
		lbw_L2StsNotiDstrbtTbl[a_link_num][ blank] = l2statesub_p->header.uiSrcPQueueID;
		else 
		{
			cm_Assert(CMD_ASL_USELOW, l2statesub_p->header.uiSrcPQueueID, (CHAR*)"too many subscribe L2 state");
			return;
		}
	}

	if(lbw_mngtbl[a_link_num].lap_dt.status == LBD_L2_INFSTS)
		state = D_API_L2_STACHG_EST;
	else
		state = D_API_L2_STACHG_REL;
	
	/* send l2 link state notice */
	memset(&apiNtc, 0, sizeof(apiNtc));
	apiNtc.header.uiEventNo		= D_API_MSGID_L2_LINKSTATE_NTC;			/**< Event	ID		   */
	apiNtc.header.uiSignalkind	= 0;									/**< Signal Kind	   */
	apiNtc.header.uiDstPQueueID	= l2statesub_p->header.uiSrcPQueueID;	/**< Destination P QID */
	apiNtc.header.uiDstTQueueID	= 0;									/**< Destination T QID */
	apiNtc.header.uiSrcPQueueID	= D_RRH_PROCQUE_L2;						/**< Source P QID	   */
	apiNtc.header.uiSrcTQueueID	= CMD_TSKID_DBG;						/**< Source T QID	   */
	apiNtc.header.uiLength		= sizeof(apiNtc);						/**< Length 		   */
	apiNtc.data.link_num = a_link_num;
	apiNtc.data.state = state;

	/* call MW func */
	l2_com_comMsgSnd(
			apiNtc.header.uiDstPQueueID,		/* Thread queue ID		   */
			D_API_MSGID_L2_LINKSTATE_NTC,		/* message ID		  */
			(VOID *)&apiNtc,					/* The message to transmit */
			sizeof(apiNtc)						/* The size of message	   */
		);

	return;
}


/* @} */

