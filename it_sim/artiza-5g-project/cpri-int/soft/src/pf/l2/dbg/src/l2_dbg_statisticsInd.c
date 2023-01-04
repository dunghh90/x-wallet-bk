/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_statisticsInd.c
 *  @brief  Increase index l3 statistics information table
 *  @date   2013/11/25 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


 /*!
 * @addtogroup RRH_L2_MAIN
 * @{
 */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "l2_com_inc.h"
#include "rrhApi_L2.h"


/********************************************************************************************************************/
/*!
 *  @brief  Increase index l3 statistics information table
 *  @note   Increase index l3 statistics information table
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/25 FFCS)hongj Create for zynq
 *  @date   2015/8/6 TDIPS)ikeda rev.27892 L2 LPB レビュー反映
 */
/********************************************************************************************************************/
VOID l2_dbg_statisticsInd(UINT *inf_p)
{
	T_API_CPRIMSG_UNSUB_IND* apiInd;
	T_RRH_L2_STATISTICS_L3TABLE * l2_comw_stati_L3Tbl_workp;
	apiInd = (T_API_CPRIMSG_UNSUB_IND*)inf_p;

	l2_comw_stati_L3Tbl_workp = l2_comw_stati_L3Tbl+((D_RRH_CPRINO_NUM*apiInd->system_type)+(apiInd->link_num));
	/* L3はヘルスチェック断後に切り替える */
	l2_comw_stati_L3Tbl_workp->mng.index = (l2_comw_stati_L3Tbl_workp->mng.index + 1) % D_RRH_L2_STATI_L3BLK_NUM;
	memset(&l2_comw_stati_L3Tbl_workp->blks[l2_comw_stati_L3Tbl_workp->mng.index], 0, sizeof(T_RRH_L2_STATISTICS_L3BLK));

	return;
}


/* @} */

