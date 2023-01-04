/*!
 * @skip  $ld:$
 * @file  f_mpsw_srv_types.h
 * @brief Public definition
 * @date  2018/10/24 Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup MPSW_SRV_PUBLIC
 * @{
 */
#ifndef F_MPSW_SRV_TYPES_H
#define F_MPSW_SRV_TYPES_H

/*!
 * @brief	M-PlaneSW Server default Port Number
 * @note	M-PlaneSW Server default Port Number is ..
 * 
 */
#define	NSI_PORT_NO				65001
#define	MSI_PORT_NO				65002
#define	MDI_PORT_NO				65003
#define	MSI_PORT_NO_READY_IND	65004
/** @} */

/*!
 * @brief	M-PlaneSW Server default IP Address
 * @note	M-PlaneSW Server default IP Address is ..
 * 
 */
#define	D_MPSW_SRV_IPADDR	"127.0.0.1"
/** @} */


typedef enum {
	E_MPSW_SRV_LOG_BUG = 1,
	E_MPSW_SRV_LOG_CRITICAL = 3,
	E_MPSW_SRV_LOG_ABN = 5,
	E_MPSW_SRV_LOG_INFO = 6,
	E_MPSW_SRV_LOG_DEBUG = 7,
	E_MPSW_SRV_LOG_ALL = 0xFF
}E_MPSW_SRV_LOGLVL;

#endif
/** @} */
