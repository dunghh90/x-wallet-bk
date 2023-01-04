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
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2014
 */
/******************************************************************************************************************************/

#include	"f_hdl_inc.h"

/** @addtogroup RRH_HDL
 *  @{
 */

/**
 * @brief thread create manage table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_HDL_CRT_EVENT_CTBL f_hdlw_thdCreMng[D_RRH_HDLTHDNO_MAX];

/**
 * @brief thread state management table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_hdlw_thdState;
 	
/** @} */
