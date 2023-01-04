/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	RAM Table
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also TableSpecifications(rfcp)_ram.xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013
 */
/******************************************************************************************************************************/

#include	"f_rfcp_inc.h"

/** @addtogroup RRH_PF_RFP
 *  @{
 */
T_RFCP_INFO					f_rfcpw_thdInfo;										/**< rfcp thrad information table		*/
T_RFCP_CAR_MNG_INFO			f_rfcpw_carInfo_mem;									/**< carrier info table(memory)			*/

T_RFCP_CAR_MNG_INFO			*f_rfcpw_carInfo;										/**< carrier info table					*/

UINT						f_upcnf_state[D_SYS_ANT_MAX][D_SYS_CAR_MAX][D_RFCP_UPCNF_TXRX_MAX];		/**< U-Plane Conf. State Data */
UINT						f_upcnf_latest_msi_reqid[D_SYS_ANT_MAX][D_SYS_CAR_MAX][D_RFCP_UPCNF_TXRX_MAX];		/**< U-Plane Conf. latest edit MSI Reqid */
/** @} */

