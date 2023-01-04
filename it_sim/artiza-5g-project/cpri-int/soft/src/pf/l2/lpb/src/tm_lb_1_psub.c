/*!
 * @skip    $Id$
 * @file    tm_lb_1_psub.c
 * @brief   NRエラー処理関数(tm_lb_NrError)
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.22 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"
#include "tm_lb_header.h"
#include "l2_com_inc.h"

/*!
 *  @brief   NRエラー処理関数(tm_lb_NrError)
 *  @note    解放CPRIリンク処理,FRMR送出
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.22 FFCS)hongj modify for zynq.
 */
void tm_lb_NrError(struct LBT_MNGTBL * s_lblap_p)
{

	s_lblap_p->ctl_sv = LBD_ERRORZ;
													/*---------------------------*/
													/*  I フレーム 強制廃棄処理  */
													/*---------------------------*/
	lblap_disciq(s_lblap_p);
													/*-----------------------*/
													/*	  Stop All Timer	 */
													/*-----------------------*/
	tm_lb_StpAllTmr(s_lblap_p);
													/* ----------------------- */
													/*  ﾔﾙﾋﾍ Counter Clear    */
													/* ----------------------- */
	s_lblap_p->lap_dt.retry_cnt = CMD_NUM0;
													/*-----------------------*/
													/*  FRMR ERROR 処理      */
													/*-----------------------*/
	lblap_frmrer(s_lblap_p);

    lblap_statis(s_lblap_p->link_num, LBD_L2_RELIND_U);
	return;
}

/* @} */


