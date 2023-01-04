/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	ROM Table
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also TableSpecifications(hlgp)_ramm.xls
 * 
 *  @date 2013/05/27 ALPHA)fujiiy create(B25対応)
 *  @date 2013/11/10 ALPHA)samejima modify for docomo zynq
 *  @date 2015/09/16 ALPHA)kamada modify for RRE 3.5G TDD
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/******************************************************************************************************************************/

#include	"f_hdl_inc.h"

/** @addtogroup RRH_HDL
 *  @{
 */
T_HDL_CRT_EVENT_CTBL	f_hdlw_thdCreMng[D_RRH_HDLTHDNO_MAX];	/**< thread create manage table 		*/
USHORT					f_hdlw_thdState;						/**< thread state management table		*/
/** @} */

/** @} */

