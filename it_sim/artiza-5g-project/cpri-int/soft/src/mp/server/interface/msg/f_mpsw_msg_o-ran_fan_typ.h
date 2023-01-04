/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_FAN.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2019
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_FAN_TYP_H
#define	F_MPSW_MSG_O_RAN_FAN_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_fan
 *  @{
 */

/**
 * @brief	(0x81130111)MSI_O_RAN_FAN_GET_REQ
 * @note	MSI_O_RAN_FAN_GET_REQ(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_FAN_GET_REQ;

/**
 * @brief	(0x81130112)MSI_O_RAN_FAN_GET_CFM -> Fan情報
 * @note	0：FAN#1 1：FAN#2 2：FAN#3
 * 
 */
typedef struct{
	UINT				locationNo;							/**< fan-location  */
	UINT				fanStatus;							/**< present-and-operating  */
	UINT				vendorCode;							/**< verndor-code  */
	UINT				speedPercent;						/**< fan-speed  */
	UINT				targetSpeed;						/**< target-speed  */
}T_MPSW_FANINFO;

/**
 * @brief	(0x81130112)MSI_O_RAN_FAN_GET_CFM
 * @note	
 * 
 */
typedef struct{
	UINT				fanInfoNum;							/**< Fan情報数  */
	T_MPSW_FANINFO		fanInfo[3];							/**< Fan情報  */
}T_MPSW_MSI_O_RAN_FAN_GET_CFM;

/**
 * @brief	(0x81130112)MSI_O_RAN_FAN_GET_CFM
 * @note	MSI_O_RAN_FAN_GET_CFM(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_FAN_GET_CFM	data;					/**< Message data  */
}T_MSG_MSI_O_RAN_FAN_GET_CFM;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_FAN_TYP_H */
