
/*!
 * @skip    $Id$
 * @file    tm_lb_1_RelNG.c
 * @brief   Send response(NG) of releasing lapb link
 * @date    2013.11.21 FFCS)hongj create for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"
#include "tm_lb_header.h"


/*!
 *  @brief   Send response of releasing lapb link
 *  @note    Send response of releasing lapb link
 *  @param   s_lblap_p		[in]  単一のリンク管理テーブル
 *  @param   a_inf_p        [in]  a_inf_p
 *  @return  None
 *  @date    2013.11.21 FFCS)hongj create for zynq.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void	tm_lb_1_RelNG(struct LBT_MNGTBL * s_lblap_p, UINT * a_inf_p)
{
    tm_lb_rell2link_rsp(s_lblap_p->link_num, CMD_NG, "NG");
	return;
}
