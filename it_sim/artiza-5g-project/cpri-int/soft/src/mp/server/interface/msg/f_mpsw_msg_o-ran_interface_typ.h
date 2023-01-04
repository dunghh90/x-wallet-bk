/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_INTERFACE.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_INTERFACE_TYP_H
#define	F_MPSW_MSG_O_RAN_INTERFACE_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_interface
 *  @{
 */

/**
 * @brief	(0x81170211)o-ran-interface EDIT要求
 * @note	o-ran-interface EDIT要求
 * 
 */
typedef struct{
	UINT				plane;								/**< Plane種別  */
	UINT				port;								/**< Port番号  */
	USHORT				marking[2];							/**< PCP  */
	USHORT				vlan_id;							/**< vlan-id  */
}T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ;

/**
 * @brief	(0x81170211)MSI_O_RAN_INTERFACE_EDIT_REQ
 * @note	o-ran-interface EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_INTERFACE_EDIT_REQ;

/**
 * @brief	(0x81170212)MSI_O_RAN_INTERFACE_EDIT_CFM
 * @note	o-ran-interface EDIT要求(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_INTERFACE_EDIT_CFM;

/**
 * @brief	(0x81170213)o-ran-interface EDIT要求(NG応答)
 * @note	o-ran-interface EDIT要求(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REJ;

/**
 * @brief	(0x81170213)MSI_O_RAN_INTERFACE_EDIT_REJ
 * @note	o-ran-interface EDIT要求(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REJ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_INTERFACE_EDIT_REJ;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_INTERFACE_TYP_H */
