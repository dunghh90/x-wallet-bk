/*!
 * @skip    $Id$
 * @file    m_lb_RunHisTmrInfGet.c
 * @brief   Get LAPB management information related timer(LAPB走行履歴)
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

#include "m_lb_header.h"
/*!
 *  @brief   Get LAPB management information related timer(LAPB走行履歴)
 *  @note    According to timer id, get LAPB management information
 *  @param   link_sta_p			[in]  link_sta_p
 *  @param   in_cpri_no_p		[in]  in_cpri_no_p
 *  @param   inter_event_no_p	[in]  inter_event_no_p
 *  @param   a_source			[in]  a_source
 *  @param   a_inf_p            [in]  receive message
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2015.08.11 TDIPS)ikeda タイムアウト17リンク対応
 */
void	tm_lb_RunHisTmrInfGet(UCHAR *link_sta_p, UCHAR *in_cpri_no_p, 
											UCHAR *inter_event_no_p, UINT  a_source,UINT *  a_inf_p)
{

	struct LBT_MNGTBL* a_mngtbl;

	a_mngtbl = get_mngtbl(a_inf_p);

	*in_cpri_no_p = (UCHAR)a_mngtbl->link_num;

	*link_sta_p = a_mngtbl->lap_dt.status;

	if (a_source >= E_RRH_TIMID_LPB_T1 && a_source < E_RRH_TIMID_L1BSYDET_MAX)
	{
		 if (a_source < E_RRH_TIMID_LPB_T1_MAX)
		 {
			*inter_event_no_p = LBD_T1TIMEOUT;
		 }
		 else if (a_source < E_RRH_TIMID_LPB_T2_MAX)
		 {
			*inter_event_no_p = LBD_T2TIMEOUT;
		 }
		 else if (a_source < E_RRH_TIMID_LPB_T3_MAX)
		 {
			*inter_event_no_p = LBD_T3TIMEOUT;
		 }
		 else if (a_source < E_RRH_TIMID_LPB_T4_MAX)
		 {
			*inter_event_no_p = LBD_T4TIMEOUT;
		 }
		 else if (a_source < E_RRH_TIMID_L2BSYDET_MAX)
		 {
			*inter_event_no_p = LBD_L2BSYDETTIMEOUT;
		 }
		 else
		 {
			*inter_event_no_p = LBD_L1BSYDETTIMEOUT;
		 }
	}

	return;
}

/* @} */


