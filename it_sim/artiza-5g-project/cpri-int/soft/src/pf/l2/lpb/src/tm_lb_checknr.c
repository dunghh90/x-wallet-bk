/*!
 * @skip    $Id$
 * @file    tm_lb_checknr.c
 * @brief   チェックNR処理関数(tm_lb_checknr)
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
 *  @brief   チェックNR処理関数(tm_lb_checknr)
 *  @note    According to NR value, set T1 timer status
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_checknr(struct LBT_MNGTBL * s_lblap_p)
{
	UINT		Result;                                     /*!< */
	INT         errcd;
													/*-----------------------*/
													/* NR = VS  ?            */
													/*-----------------------*/
	if(s_lblap_p->nr_sav == s_lblap_p->lap_dt.vs)
	{
													/*-----------------------*/
													/* YES   (NR = VS)       */
													/*  T1タイマ停止処理     */
													/*-----------------------*/
		if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
		{
			Result =	cm_TStop(s_lblap_p->t1_id, &errcd);
			if(Result	!= CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
			}
													/*-----------------------*/
													/*      Flag Clear       */
													/*-----------------------*/
			s_lblap_p->lap_dt.t1_flg = CMD_OFF;
		}
			
	}
	else
	{
													/*-----------------------*/
													/*  NR != VA ?           */
													/*-----------------------*/
		if(s_lblap_p->nr_sav != s_lblap_p->lap_dt.va)
		{
													/*-----------------------*/
													/* YES   (NR != VA)      */
													/* T1 Timer Restart      */
													/*-----------------------*/
			if(s_lblap_p->lap_dt.t1_flg == CMD_ON)
			{
				Result =	cm_TStop(s_lblap_p->t1_id, &errcd);
				if(Result	!= CMD_RES_OK)
				{
					cm_Assert( CMD_ASL_DBGLOW, Result, "T1 Timer Stop NG" );
				}
				Result = cm_TStat(s_lblap_p->t1_id, 
						s_lblap_p->lap_dt.t1, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
				if(Result != CMD_RES_OK)
				{
					cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t1_id, "T1 Timer Start NG" );
				}
			}
			else
			{
				Result = cm_TStat(s_lblap_p->t1_id, 
						s_lblap_p->lap_dt.t1, CMD_TIMMD_SINGLE,CMD_TSKIF_TIMOUTNTC,CMD_TSKID_LPB,&errcd);
				if(Result != CMD_RES_OK)
				{
					cm_Assert( CMD_ASL_DBGLOW, s_lblap_p->t1_id, "T1 Timer Start NG" );
				}
			}
													/*-----------------------*/
													/*      T1 Flag ON       */
													/*-----------------------*/
			s_lblap_p->lap_dt.t1_flg = CMD_ON;
		}

	}

	return;
}

/* @} */


