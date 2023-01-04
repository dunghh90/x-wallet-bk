/*!
 * @skip    $Id$
 * @file    tm_lb_main.c
 * @brief   LAPB制御メイン処理(tm_lb_Main)
 * @date    2008.07.22 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/

/** @addtogroup RRH_L2_LPB
 * @{ */

#include "m_cm_header.h"
#include "tm_lb_header.h"

#include "m_lb_header.h"


/*!
 *  @brief   LAPB制御メイン処理(tm_lb_Main)
 *  @note    Call analyse function to analyse event code, then do proper processing.
 *  @param   a_source		[in]  a_source
 *  @param   a_inf_p		[in]  a_inf_p
 *  @param   a_event		[in]  a_event
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_Main(UINT a_source,UINT a_event,UINT * a_inf_p)
{
	UINT	Result;                                         /*!< */
											/* タスク起動ログ書込み処理 	 */
											/* LAPB走行履歴書込み	*/
	tm_lb_RunHisSet(LBD_RUNHIS_FLG_RCV, a_source, a_event, a_inf_p);
														/*------------------------------*/
														/*	Set keep buffer flag off	*/
														/*------------------------------*/
	lbw_keepbufflg = CMD_OFF;
											/*--------------------------------------*/
											/*起動要因番号の値によって				*/
											/*タスクイベントテーブルに値を設定する。*/
											/*--------------------------------------*/
	Result = tm_lb_Anlz(a_event);

	if(Result == CMD_OK)
	{
		lbr_0_m0[lbw_tskstt][lbw_tskevent](a_source,a_event,a_inf_p);
	}
	else
	{
		cm_Assert(CMD_ASL_USELOW, a_event, "tm_lb_Main CPRI_Sig_ABNORMAL");
	}

													/*All buffer will be freed buffer at here*/
	if((lbw_keepbufflg == CMD_OFF) && (a_inf_p != NULL))
	{
		Result = cm_BFree((void *)a_inf_p);
		if(Result != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "tm_lb_Main",
						"Buffer FreeNG_1", Result, (UINT)a_inf_p, CMD_NUM0);
		}
	}	
}

/* @} */


