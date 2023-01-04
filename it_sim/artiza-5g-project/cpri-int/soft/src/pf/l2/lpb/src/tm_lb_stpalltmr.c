/*!
 * @skip    $Id$
 * @file    tm_lb_stpalltmr.c
 * @brief   全タイム停止処理関数(tm_lb_StpAllTmr)
 * @date    2008.07.22 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"
#include "tm_lb_header.h"



/*!
 *  @brief   全タイム停止処理関数(tm_lb_StpAllTmr)
 *  @note    This function call perform stop all timer
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_StpAllTmr(struct LBT_MNGTBL * s_lblap_p)
{
	UINT	Result;                                         /*!< */
	INT     errcd;

													/*-----------------------*/
													/*   Stop T1   			 */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
	{
		Result =	cm_TStop(s_lblap_p->t1_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
		}
		s_lblap_p->lap_dt.t1_flg = CMD_OFF;
	}
													/*-----------------------*/
													/*   Stop T2  			 */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t2_cnt != CMD_NUM0)
	{
		Result =	cm_TStop(s_lblap_p->t2_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T2 Timer Stop NG" );
		}
		s_lblap_p->lap_dt.t2_cnt = CMD_NUM0;
	}
													/*-----------------------*/
													/*   Stop T3  			 */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t3_flg == CMD_ON)
	{
		Result =	cm_TStop(s_lblap_p->t3_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t3_id, "T3 Timer Stop NG" );
		}
		s_lblap_p->lap_dt.t3_flg = CMD_OFF;
	}
													/*-----------------------*/
													/*   Stop T4			 */
													/*-----------------------*/
	if(s_lblap_p->lap_dt.t4_flg == CMD_ON)
	{
		Result =	cm_TStop(s_lblap_p->t4_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "T4 Timer Stop NG" );
		}
		s_lblap_p->lap_dt.t4_flg = CMD_OFF;
	}
													/*----------------------------------*/
													/*  Stop Layer 2 busy tiemr flag	*/
													/*----------------------------------*/
	if(s_lblap_p->lap_dt.l2bsytmrflg == CMD_ON)
	{
		Result = cm_TStop(s_lblap_p->l2bsydettmr_id, &errcd);
		if(Result	!= CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, Result, "L2 busy Timer Stop NG" );
		}
		s_lblap_p->lap_dt.l2bsytmrflg = CMD_OFF;
	}
	return;
}

/* @} */


