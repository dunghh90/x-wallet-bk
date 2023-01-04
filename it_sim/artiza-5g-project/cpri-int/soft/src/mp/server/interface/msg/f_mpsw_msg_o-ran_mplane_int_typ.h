/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also FBIF_llsDU_cm_Server_MSI_O_RAN_MPLANE_INT.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MPSW_MSG_O_RAN_MPLANE_INT_TYP_H
#define	F_MPSW_MSG_O_RAN_MPLANE_INT_TYP_H

#include	"f_mpsw_msg_MSI.h"

/** @addtogroup Message0x_o_ran_mplane_int
 *  @{
 */

/**
 * @brief	(0x811A0111)MSI_O_RAN_MPLANE_INT_GET_REQ
 * @note	o-ran-mplane-int GET要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_MPLANE_INT_GET_REQ;

/**
 * @brief	(0x811A0112)o-ran-mplane-int GET応答(OK応答)
 * @note	o-ran-mplane-int GET応答(OK応答)
 * 
 */
typedef struct{
	UINT				mplane_ipv4;						/**< mplane-ipv4  */
	USHORT				port;								/**< port  */
}T_MPSW_MSI_O_RAN_MPLANE_INT_GET_CFM;

/**
 * @brief	(0x811A0112)MSI_O_RAN_MPLANE_INT_GET_CFM
 * @note	o-ran-mplane-int GET応答(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_MPLANE_INT_GET_CFM	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_MPLANE_INT_GET_CFM;

/**
 * @brief	(0x811A0113)o-ran-mplane-int GET応答(NG応答)
 * @note	o-ran-mplane-int GET応答(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_MPLANE_INT_GET_REJ;

/**
 * @brief	(0x811A0113)MSI_O_RAN_MPLANE_INT_GET_REJ
 * @note	o-ran-mplane-int GET応答(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_MPLANE_INT_GET_REJ	data;				/**< Message data  */
}T_MSG_MSI_O_RAN_MPLANE_INT_GET_REJ;

/**
 * @brief	(0x811A0211)o-ran-mplane-int EDIT要求 -> m-plane-ssh-port
 * @note	
 * 
 */
typedef struct{
	UINT				call_home_ssh_port;					/**< call-home-ssh-port  */
	UINT				server_ssh_port;					/**< server-ssh-port  */
}T_MPSW_MPLANE_SSH_PORT;

/**
 * @brief	(0x811A0211)o-ran-mplane-int EDIT要求 -> searchable-mplane-access-vlans-info
 * @note	
 * 
 */
typedef struct{
	USHORT				lowest_vlan_id;						/**< lowest-vlan-id  */
	USHORT				highest_vlan_id;					/**< highest-vlan-id  */
	USHORT				num_of_vlans;						/**< searchable-access-vlans有効数  */
	USHORT				searchable_vlan_id[4094];			/**< searchable-access-vlans  */
}T_MPSW_ACCESS_VLAN_INFO;

/**
 * @brief	(0x811A0211)o-ran-mplane-int EDIT要求
 * @note	o-ran-mplane-int EDIT要求
 * 
 */
typedef struct{
	T_MPSW_MPLANE_SSH_PORT	mplane_ssh_port;				/**< m-plane-ssh-port  */
	T_MPSW_ACCESS_VLAN_INFO	access_vlan_info;				/**< searchable-mplane-access-vlans-info  */
}T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REQ;

/**
 * @brief	(0x811A0211)MSI_O_RAN_MPLANE_INT_EDIT_REQ
 * @note	o-ran-mplane-int EDIT要求(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REQ	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REQ;

/**
 * @brief	(0x811A0212)MSI_O_RAN_MPLANE_INT_EDIT_CFM
 * @note	o-ran-mplane-int EDIT応答(OK応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
}T_MSG_MSI_O_RAN_MPLANE_INT_EDIT_CFM;

/**
 * @brief	(0x811A0213)o-ran-mplane-int EDIT応答(NG応答)
 * @note	o-ran-mplane-int EDIT応答(NG応答)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGERROR	msgErr;								/**< エラー情報  */
}T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REJ;

/**
 * @brief	(0x811A0213)MSI_O_RAN_MPLANE_INT_EDIT_REJ
 * @note	o-ran-mplane-int EDIT応答(NG応答)(header + data)
 * 
 */
typedef struct{
	T_MPSW_SRV_MSGHEAD			msgHead;					/**< Message header  */
	T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REJ	data;			/**< Message data  */
}T_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REJ;


/** @} */

#endif	/* F_MPSW_MSG_O_RAN_MPLANE_INT_TYP_H */
