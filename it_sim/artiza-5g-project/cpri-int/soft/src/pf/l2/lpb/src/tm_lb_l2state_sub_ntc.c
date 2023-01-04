/*!
 * @skip    $Id$
 * @file    tm_lb_l2state_sub_ntc.c
 * @brief   Send LAPB link state
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 * 
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"			/* TRIF common head file		       	*/
#include "l2_com_inc.h"
#include "rrhApi_L2.h"
#include "tm_lb_def.h"
#include "tm_lb_typ.h"
#include "tm_lb_ext.h"
#include "m_lb_def.h"
#include "m_lb_typ.h"
#include "m_lb_ext.h"
#include "f_rrh_reg_cpri.h"


/********************************************************************************************************************/
/*!
 *  @brief  Send LAPB link state
 *  @note   Send LAPB link state to subscription process
 *  @param  link_num   [in]   CPRIリンク番号
 *  @param  lnkState   [in]   link state
 *  @param  astlog     [in]   呼び出し関数名(__func__)
 *  @retval -
 *  @date   2013/11/15 FFCS)hongj Create zynq
 *  @date   2015/07/28 TDI)ikeda  FHMでの17リンク対応
 */
 /********************************************************************************************************************/
void 	tm_lb_l2state_sub_ntc(USHORT link_num, INT lnkState, const CHAR *astlog)
{
#if 0	
//	int index;
#endif

	T_API_L2_LINKSTATE_CHG_NTC apiNtc;
	UINT state;
	UINT  count;

	if(astlog)
	cm_Assert( CMD_ASL_DBGLOW, (UINT)((link_num<<16)+lnkState), astlog);
#if 0	
//	index = lbw_lnkStatLog[link_num].index % 128;
//	strcpy(lbw_lnkStatLog[link_num].tbl[index].func,astlog);
#endif
	if(lnkState == D_L2_LINKSTATE_ON)
	{
		/* link on */
		state = D_API_L2_STACHG_EST;
	}
	else if(lnkState == D_L2_LINKSTATE_OFF)
	{
		/* link off */
		state = D_API_L2_STACHG_REL;
	}
	else
	{
		/* link abnormal */
		state =  D_API_L2_STACHG_ABN;
	}
	/* send l2 link state notice */
	memset(&apiNtc, 0, sizeof(apiNtc));

	for(count = 0; count < sizeof(lbw_L2StsNotiDstrbtTbl[0]) / sizeof(UINT) ; count++)
	{
		if(lbw_L2StsNotiDstrbtTbl[link_num][count] == 0) continue;
		
		apiNtc.header.uiEventNo		= D_API_MSGID_L2_LINKSTATE_NTC;	/**< Event	ID		   */
		apiNtc.header.uiSignalkind	= 0;								/**< Signal Kind	   */
		apiNtc.header.uiDstPQueueID	= lbw_L2StsNotiDstrbtTbl[link_num][count];				/**< Destination P QID */
		apiNtc.header.uiDstTQueueID	= 0;								/**< Destination T QID */
		apiNtc.header.uiSrcPQueueID	= D_RRH_PROCQUE_L2;								/**< Source P QID	   */
		apiNtc.header.uiSrcTQueueID	= D_L2_MSGQ_MAIN;								/**< Source T QID	   */
		apiNtc.header.uiLength		= sizeof(apiNtc);					/**< Length 	 */ 	
		apiNtc.data.link_num = link_num;
		apiNtc.data.state = state;
		
		/* call send message func */
		l2_com_comMsgSnd(
				apiNtc.header.uiDstPQueueID,						/* Thread queue ID		   */
				D_API_MSGID_L2_LINKSTATE_NTC,
				(VOID *)&apiNtc,					/* The message to transmit */
				sizeof(apiNtc)						/* The size of message	   */
			);
	}

    return;
}

void tm_lb_l2state_log_begin(USHORT linkno, int event)
{
#if 0
//	int index;
//	T_RRH_SYSTEM_TIME	datetime;									/*!< 時刻情報				*/
//	void *lbw_p;
//	
//	index = lbw_lnkStatLog[linkno].index % 128;
//
//	cm_RtcGet( &datetime );
//	memcpy(&lbw_lnkStatLog[linkno].tbl[index].datetime,&datetime,sizeof(datetime));
//
//	lbw_p = (void*)&lbw_mngtbl[linkno].lap_dt.status;
//	memcpy((void*)lbw_lnkStatLog[linkno].tbl[index].lpbdat_prev ,lbw_p,48);
//	lbw_p = (void*)&lbw_mngtbl[linkno].cmd_rsp_flg;
//	memcpy((void*)lbw_lnkStatLog[linkno].tbl[index].lpbmng_prev ,lbw_p,48);
//
//	lbw_lnkStatLog[linkno].tbl[index].linkstate_prev = lbw_mngtbl[linkno].lap_dt.status;
//	lbw_lnkStatLog[linkno].tbl[index].linkevent = (USHORT)event;
//	
//	lbw_lnkStatLog[linkno].tbl[index].setflg = D_RRH_ON;
//
#endif
	return;
}

void tm_lb_l2state_log_end(USHORT linkno)
{
#if 0
//	int index;
//	void *lbw_p;
//	
//	/* save log*/
//	index = lbw_lnkStatLog[linkno].index % 128;
//	if(lbw_mngtbl[linkno].lap_dt.status  == lbw_lnkStatLog[linkno].tbl[index].linkstate_prev )
//	{
//		lbw_lnkStatLog[linkno].tbl[index].setflg = D_RRH_OFF;
//		lbw_lnkStatLog[linkno].tbl[index].linkstate_prev = 0;
//
//		return;
//	}
//
//	lbw_lnkStatLog[linkno].tbl[index].setflg = D_RRH_ON;
//	lbw_p = (void*)&lbw_mngtbl[linkno].lap_dt.status;
//	memcpy((void*)lbw_lnkStatLog[linkno].tbl[index].lpbdat_now ,lbw_p,48);
//	lbw_p = (void*)&lbw_mngtbl[linkno].cmd_rsp_flg;
//	memcpy((void*)lbw_lnkStatLog[linkno].tbl[index].lpbmng_now ,lbw_p,48);
//	lbw_lnkStatLog[linkno].tbl[index].linkstate_now = lbw_mngtbl[linkno].lap_dt.status;
//
//	lbw_lnkStatLog[linkno].index++;
//	lbw_lnkStatLog[linkno].index = lbw_lnkStatLog[linkno].index % 128;
//
#endif
	return;
}
/* @} */

