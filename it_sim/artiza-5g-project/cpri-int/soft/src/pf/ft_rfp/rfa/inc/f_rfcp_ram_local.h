/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	External Definition
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also ...
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013
 */
/******************************************************************************************************************************/

#ifndef	F_RFCP_RAM_LOCAL_H
#define	F_RFCP_RAM_LOCAL_H

/** @addtogroup RRH_PF_RFP
 *  @{
 */
#include "f_rfcp_inc.h"

extern	T_RFCP_INFO					f_rfcpw_thdInfo __attribute__ ((aligned (32)));	/**< rfcp thrad information table		*/
extern	T_RFCP_CAR_MNG_INFO			f_rfcpw_carInfo_mem;							/**< carrier info table(memory)			*/

extern	T_RFCP_CAR_MNG_INFO			*f_rfcpw_carInfo;								/**< carrier info table					*/

extern	UINT						f_upcnf_state[D_SYS_ANT_MAX][D_SYS_CAR_MAX][D_RFCP_UPCNF_TXRX_MAX];		/**< U-Plane Conf. State Data */
extern  UINT				        f_upcnf_latest_msi_reqid[D_SYS_ANT_MAX][D_SYS_CAR_MAX][D_RFCP_UPCNF_TXRX_MAX];		/**< U-Plane Conf. latest edit MSI Reqid */

#endif	/* F_RFCP_RAM_LOCAL_H */
/** @} */
