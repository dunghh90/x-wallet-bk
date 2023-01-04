/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_DHCP.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_DHCP_TYP_H
#define	F_MPSW_MSG_O_RAN_DHCP_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_dhcp
 *  @{
 */

/**
 * @brief	(0x81100111)MSI_O_RAN_DHCP_GET_REQ
 * @note	o-ran-dhcp GET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_DHCP_GET_REQ;

/**
 * @brief	(0x81100112)o-ran-dhcp GET応答(OK) -> dhcpv4
 * @note	
 * 
 */
typedef struct{
	UINT				dhcp_server_id;						/**< dhcp-server-identifier  */
	UINT				default_gateway;					/**< default-gateway  */
	UINT				netconf_client_id;					/**< netconf-client-id  */
}T_MPSW_DHCPV4;

/**
 * @brief	(0x81100112)o-ran-dhcp GET応答(OK) -> m-plane-dhcp
 * @note	
 * 
 */
typedef struct{
	USHORT				private_enterprise_number;			/**< private-enterprise-number  */
}T_MPSW_M_PLANE_DHCP;

/**
 * @brief	(0x81100112)o-ran-dhcp GET応答(OK)
 * @note	o-ran-dhcp GET応答(OK)
 * 
 */
typedef struct{
	UINT				interface;							/**< interface  */
	T_MPSW_DHCPV4		dhcpv4;								/**< dhcpv4  */
	T_MPSW_M_PLANE_DHCP	m_plane_dhcp;						/**< m-plane-dhcp  */
}T_MPSW_MSI_O_RAN_DHCP_GET_CFM;

/**
 * @brief	(0x81100112)MSI_O_RAN_DHCP_GET_CFM
 * @note	o-ran-dhcp GET応答(OK)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_DHCP_GET_CFM	data;					/**< Message data  */
}T_MSG_MSI_O_RAN_DHCP_GET_CFM;

/**
 * @brief	(0x81100113)o-ran-dhcp GET応答(NG)
 * @note	o-ran-dhcp GET応答(NG)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_DHCP_GET_REJ;

/**
 * @brief	(0x81100113)MSI_O_RAN_DHCP_GET_REJ
 * @note	o-ran-dhcp GET応答(NG)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_DHCP_GET_REJ	data;					/**< Message data  */
}T_MSG_MSI_O_RAN_DHCP_GET_REJ;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_DHCP_TYP_H */
